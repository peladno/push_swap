# Benchmark Mode (`--bench`) / ベンチマークモード(`--bench`)

> **Decided in**: Kickoff Topic 8 (role split); plan drafted 2026-06-11 (So took over the full benchmark from Javier)
> **Drafter**: So
> **Depends on**: `operations.md` (the 11 wrappers), `adaptive-dispatcher.md` (strategy selection), `compute_disorder.c` (Phase 8 metric, done)
> **References**: subject VI.5 (rules + `--bench` output), VI.5.1 (output examples)

## English

### Scope

- Defines the `t_bench` struct, op counting, and the `--bench` stderr output.
- Op counting threads a `t_bench *` through every sort tier down to the 11 op wrappers — **global variables are forbidden by subject VI.5**, so a passed pointer is the only option.
- Output goes to **stderr**, only when `--bench` is set; the op stream stays on stdout.

NOT covered here:
- The sort algorithms themselves (their plans) — this only adds the `t_bench *` parameter to their signatures.
- `compute_disorder` (already implemented; `--bench` just formats its result as a percentage).

### Why a passed pointer (not a global)

Subject VI.5 states **"global variables are forbidden."** That rules out a `g_bench` counter. The only漏れ-free place to count an op is the wrapper that emits it (the single choke point), so the counter struct must reach the wrappers — by parameter. This is the resolution of the "Phase 8 op-counting" item deferred in `operations.md`.

So's earlier instinct ("count in the sort loop") is right in spirit but leaks: ops are emitted at scattered, sometimes nested call sites (e.g. Phase 4's `rotate_b_to_top` calls `rb`/`rrb` deep inside a helper). Counting at the wrapper catches every emission with no duplication.

### Subject output format (VI.5.1)

```text
[bench] disorder: 49.93%
[bench] strategy:  Adaptive / O(n√n)
[bench] total_ops: 7997
[bench] sa: 0  sb: 0  ss: 0  pa: 500  pb: 500
[bench] ra: 4840  rb: 1098  rr: 0  rra: 0  rrb: 1059  rrr: 0
```

- `disorder`: percentage, 2 decimals (`compute_disorder(a) * 100`).
- `strategy`: name + complexity class string.
- `total_ops`: sum of all op counts.
- Per-op counts: all 11, split across two lines as shown.
- All lines `[bench] ` prefixed, all to **stderr**.

### Interface

```c
/* New struct, in push_swap.h */
typedef struct s_bench
{
    int     count[11];     /* indexed by t_op_id below */
    char    *strategy;     /* e.g. "Adaptive / O(n√n)" — set by the dispatcher */
    double  disorder;      /* compute_disorder(a) result, set before sorting */
    int     enabled;       /* 1 if --bench was passed, else 0 */
}   t_bench;

/* Op identity for indexing count[] */
typedef enum e_op_id
{
    OP_SA, OP_SB, OP_SS,
    OP_PA, OP_PB,
    OP_RA, OP_RB, OP_RR,
    OP_RRA, OP_RRB, OP_RRR
}   t_op_id;

void  bench_init(t_bench *bench, int enabled, double disorder);
void  bench_print(t_bench *bench);   /* prints to stderr iff bench->enabled */
```

The 11 wrappers gain a `t_bench *bench` parameter:

```c
t_operation  sa(t_stack *a, t_bench *bench);
t_operation  pa(t_stack *a, t_stack *b, t_bench *bench);
/* ...all 11... */
```

Each wrapper increments its slot when it emits (on `OP_DONE`):

```c
t_operation  sa(t_stack *a, t_bench *bench)
{
    if (swap(a) == OP_NOT_DONE)
        return (OP_NOT_DONE);
    ft_putstr_fd("sa\n", 1);
    bench->count[OP_SA]++;
    return (OP_DONE);
}
```

### Open design decisions (recorded for review)

**D1 — `count[11]` array vs 11 named fields.**
- Array + `t_op_id` enum (above): one `bench->count[OP_SA]++` line per wrapper, easy to sum in a loop for `total_ops`. **Recommended.**
- 11 named `int sa; int sb; ...`: more readable at the print site, but `total_ops` becomes an 11-term addition and there's no clean loop.
- Leaning array for the loopable total.

**D2 — count always, or only when `--bench`?**
- Always count (every wrapper does `bench->count[...]++` regardless), print only if `enabled`. Simpler, branch-free in the hot path; the increment is trivial. **Recommended (case X).**
- Count only if `enabled`: saves a handful of int increments but adds a branch to every op. Not worth it.

**D3 — does the `bench` pointer have to be non-NULL?**
- If main always constructs a `t_bench` (even without `--bench`, with `enabled = 0`), wrappers can assume non-NULL and unconditionally increment. Cleanest.
- Alternative: pass `NULL` when not benching and guard every wrapper with `if (bench) ...`. Adds a branch + a NULL risk to all 11 wrappers. **Avoid** — always construct the struct.

### Ripple into other phases

Adding the `t_bench *` parameter touches every sort tier's signature and their internal op calls:

| File | Owner | Change |
|---|---|---|
| `src/operations/*.c` (11 wrappers) | So | add `t_bench *bench` param + count line |
| `radix_sort.c` (Phase 6) | So | add `t_bench *` param, pass to its `pb`/`ra`/`pa` calls |
| `insertion_sort.c` (Phase 4) | **Javier** | add `t_bench *` param, pass to every op call (incl. inside `rotate_b_to_top`) |
| `chunk_sort.c` (Phase 5) | **Javier** | same, once it's complete |
| `adaptive_sort.c` (Phase 7) | both | add `t_bench *` param, set `bench->strategy`, pass down |
| `main.c` | So | construct `t_bench`, parse `--bench`, call `bench_print` |

**Coordination needed with Javier**: his Phase 4 (and Phase 5) op calls must take the new parameter. Since So is now owning benchmark, So can do the mechanical parameter-threading on Javier's files during integration — but flag it so it's not a surprise.

### `--bench` flag parsing

`parse_args` currently handles `--simple/--medium/--complex/--adaptive` (one strategy flag, ≥2 rejected). `--bench` is **orthogonal** — it can coexist with a strategy flag (subject example: `./push_swap --bench --adaptive $ARG`). So:
- `--bench` is a separate boolean, not part of the strategy-flag dedup count.
- Parser sets a `benched` flag (or out-param) when it sees `--bench`, independent of the strategy selector.

### Implementation outline

1. Define `t_bench` + `t_op_id` in `push_swap.h`.
2. `bench_init`: zero the counts, store `enabled` + `disorder`, NULL strategy.
3. Thread `t_bench *` into the 11 wrappers; add the count line to each.
4. Thread it through `radix_sort` (So) — and, at integration, the other tiers.
5. Dispatcher sets `bench->strategy` to the right string.
6. `bench_print`: if `enabled`, format the 5 `[bench]` lines to stderr (disorder ×100, 2 decimals; sum count[] for total_ops).
7. `main`: parse `--bench`, `compute_disorder` before sorting, `bench_init`, sort, `bench_print`.

### Strategy strings

| t_strategy | string |
|---|---|
| forced `--simple` | `Simple / O(n^2)` |
| forced `--medium` | `Medium / O(n√n)` |
| forced `--complex` | `Complex / O(n log n)` |
| adaptive → low | `Adaptive / O(n^2)` |
| adaptive → medium | `Adaptive / O(n√n)` |
| adaptive → high | `Adaptive / O(n log n)` |

### Edge cases

- No `--bench`: counts still accumulate (case X), nothing printed.
- `argc == 1` / empty: returns before any sort; no bench output.
- Already-sorted input: `total_ops` may be 0; still print the summary if `--bench`.
- `disorder` for n < 2: `compute_disorder` returns 0.0 → prints `0.00%`.

### Testing

- Reproduce subject VI.5.1 examples: `./push_swap --bench --adaptive 4 67 3 87 23 2> bench.txt | ./checker_linux ...` → `OK`, and `bench.txt` matches the format.
- `total_ops` equals `wc -l` of the stdout op stream.
- Per-op counts sum to `total_ops`.
- `--bench` absent → no stderr output, op stream unchanged.
- 500-element run → disorder ~50%, counts in the expected band.

### Complexity note

- Counting is O(1) per op (one array increment), so it doesn't change any sort's complexity class.
- `bench_print` is O(1) (fixed 11 counts).

---

## 日本語

### Scope

- `t_bench` 構造体、op カウント、`--bench` の stderr 出力を定義。
- op カウントは `t_bench *` を全 sort tier から 11 個の op wrapper まで貫通させる ——**subject VI.5 でグローバル変数が禁止**されているため、ポインタ引数渡しが唯一の手段。
- 出力は **stderr**、`--bench` 指定時のみ;op ストリームは stdout のまま。

ここでカバーしないもの:
- sort アルゴリズム本体(各プラン)—— ここでは `t_bench *` 引数をシグネチャに足すだけ。
- `compute_disorder`(実装済み;`--bench` は結果を % 整形するだけ)。

### なぜポインタ引数(グローバルでなく)

subject VI.5 に **「グローバル変数は禁止」** と明記。`g_bench` カウンタは不可。op を漏れなく数えられる唯一の場所は **op を発行する wrapper(単一の関所)** なので、カウンタ構造体を wrapper まで届ける必要 —— 引数で。これが `operations.md` で保留した「Phase 8 op-counting」の決着。

So の当初の直感(「sort のループで数える」)は方向性は正しいが漏れる:op は散在した、時にネストした呼び出し箇所で発行される(例:Phase 4 の `rotate_b_to_top` が helper の奥で `rb`/`rrb` を呼ぶ)。wrapper で数えれば全発行を重複なく捕捉できる。

### subject 出力フォーマット(VI.5.1)

```text
[bench] disorder: 49.93%
[bench] strategy:  Adaptive / O(n√n)
[bench] total_ops: 7997
[bench] sa: 0  sb: 0  ss: 0  pa: 500  pb: 500
[bench] ra: 4840  rb: 1098  rr: 0  rra: 0  rrb: 1059  rrr: 0
```

- `disorder`:% 表記、小数 2 桁(`compute_disorder(a) * 100`)。
- `strategy`:名前 + 計算量クラス文字列。
- `total_ops`:全 op カウントの合計。
- 各 op カウント:11 種すべて、上記の通り 2 行に分割。
- 全行 `[bench] ` プレフィックス、全て **stderr** へ。

### Interface

```c
/* 新規構造体、push_swap.h に */
typedef struct s_bench
{
    int     count[11];     /* 下記 t_op_id でインデックス */
    char    *strategy;     /* 例:"Adaptive / O(n√n)" —— dispatcher が設定 */
    double  disorder;      /* compute_disorder(a) の結果、ソート前に設定 */
    int     enabled;       /* --bench があれば 1、なければ 0 */
}   t_bench;

/* count[] インデックス用の op 識別子 */
typedef enum e_op_id
{
    OP_SA, OP_SB, OP_SS,
    OP_PA, OP_PB,
    OP_RA, OP_RB, OP_RR,
    OP_RRA, OP_RRB, OP_RRR
}   t_op_id;

void  bench_init(t_bench *bench, int enabled, double disorder);
void  bench_print(t_bench *bench);   /* bench->enabled の時のみ stderr へ */
```

11 wrapper は `t_bench *bench` 引数を得る:

```c
t_operation  sa(t_stack *a, t_bench *bench);
t_operation  pa(t_stack *a, t_stack *b, t_bench *bench);
/* ...全 11...*/
```

各 wrapper は発行時(`OP_DONE`)に自分のスロットを increment:

```c
t_operation  sa(t_stack *a, t_bench *bench)
{
    if (swap(a) == OP_NOT_DONE)
        return (OP_NOT_DONE);
    ft_putstr_fd("sa\n", 1);
    bench->count[OP_SA]++;
    return (OP_DONE);
}
```

### 未確定の設計判断(レビュー用に記録)

**D1 —— `count[11]` 配列 vs 11 個の名前付きフィールド。**
- 配列 + `t_op_id` enum(上記):wrapper ごとに `bench->count[OP_SA]++` の 1 行、`total_ops` をループで合計しやすい。**推奨**。
- 11 個の名前付き `int sa; int sb; ...`:print 箇所では読みやすいが、`total_ops` が 11 項の加算になりループにできない。
- ループ可能な合計のため配列寄り。

**D2 —— 常にカウントか、`--bench` 時のみか?**
- 常にカウント(全 wrapper が `enabled` に関係なく `bench->count[...]++`)、出力だけ `enabled` で分岐。シンプル、hot path に分岐なし;increment は些末。**推奨(案 X)**。
- `enabled` 時のみカウント:int increment を数回節約するが全 op に分岐追加。割に合わない。

**D3 —— `bench` ポインタは非 NULL 前提でよいか?**
- main が常に `t_bench` を構築(`--bench` なしでも `enabled = 0` で)すれば、wrapper は非 NULL 前提で無条件 increment できる。最もクリーン。
- 代替:bench しない時 `NULL` を渡し、全 wrapper で `if (bench) ...` ガード。分岐 + NULL リスクを 11 wrapper に追加。**避ける** —— 常に構造体を構築する。

### 他 Phase への波及

`t_bench *` 引数追加は全 sort tier のシグネチャと内部 op 呼び出しに波及:

| ファイル | 担当 | 変更 |
|---|---|---|
| `src/operations/*.c`(11 wrapper)| So | `t_bench *bench` 引数 + count 行追加 |
| `radix_sort.c`(Phase 6)| So | `t_bench *` 引数追加、`pb`/`ra`/`pa` 呼び出しに渡す |
| `insertion_sort.c`(Phase 4)| **Javier** | `t_bench *` 引数追加、全 op 呼び出しに渡す(`rotate_b_to_top` 内含む)|
| `chunk_sort.c`(Phase 5)| **Javier** | 同様、完成後 |
| `adaptive_sort.c`(Phase 7)| 両者 | `t_bench *` 引数追加、`bench->strategy` 設定、下に渡す |
| `main.c` | So | `t_bench` 構築、`--bench` parse、`bench_print` 呼び出し |

**Javier との調整が必要**:彼の Phase 4(と Phase 5)の op 呼び出しは新引数を取る必要。So が benchmark を持つので、統合時に So が Javier のファイルへの機械的な引数貫通を実施できる —— が、驚かせないよう事前に共有する。

### `--bench` フラグの parse

`parse_args` は現在 `--simple/--medium/--complex/--adaptive`(strategy フラグ 1 個、≥2 で拒否)を扱う。`--bench` は **直交** —— strategy フラグと共存できる(subject 例:`./push_swap --bench --adaptive $ARG`)。よって:
- `--bench` は別の boolean、strategy フラグの重複カウントには含めない。
- parser は `--bench` を見たら `benched` フラグ(or out-param)を立てる、strategy セレクタとは独立に。

### Implementation outline

1. `push_swap.h` に `t_bench` + `t_op_id` を定義。
2. `bench_init`:count をゼロ化、`enabled` + `disorder` を保存、strategy を NULL。
3. `t_bench *` を 11 wrapper に貫通;各に count 行を追加。
4. `radix_sort`(So)に貫通 —— 統合時に他 tier も。
5. dispatcher が `bench->strategy` を正しい文字列に設定。
6. `bench_print`:`enabled` なら 5 行の `[bench]` を stderr に整形(disorder ×100、2 桁;count[] を合計して total_ops)。
7. `main`:`--bench` parse、ソート前に `compute_disorder`、`bench_init`、ソート、`bench_print`。

### strategy 文字列

| t_strategy | 文字列 |
|---|---|
| 強制 `--simple` | `Simple / O(n^2)` |
| 強制 `--medium` | `Medium / O(n√n)` |
| 強制 `--complex` | `Complex / O(n log n)` |
| adaptive → low | `Adaptive / O(n^2)` |
| adaptive → medium | `Adaptive / O(n√n)` |
| adaptive → high | `Adaptive / O(n log n)` |

### Edge cases

- `--bench` なし:count は蓄積される(案 X)が何も出力しない。
- `argc == 1` / 空:ソート前に return;bench 出力なし。
- 既ソート入力:`total_ops` が 0 かも;`--bench` なら summary は出す。
- n < 2 の `disorder`:`compute_disorder` が 0.0 → `0.00%` を出力。

### Testing

- subject VI.5.1 例を再現:`./push_swap --bench --adaptive 4 67 3 87 23 2> bench.txt | ./checker_linux ...` → `OK`、`bench.txt` がフォーマット一致。
- `total_ops` が stdout op ストリームの `wc -l` と一致。
- 各 op カウントの合計が `total_ops` と一致。
- `--bench` なし → stderr 出力なし、op ストリーム不変。
- 500 要素 → disorder 約 50%、count が期待範囲。

### 計算量ノート

- カウントは op あたり O(1)(配列 increment 1 回)、どの sort の計算量クラスも変えない。
- `bench_print` は O(1)(固定 11 カウント)。

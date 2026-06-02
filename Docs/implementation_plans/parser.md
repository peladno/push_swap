# Parser / パーサー

> **Decided in**: subject-driven (Phase 1)
> **Drafter**: Both (Phase 1, parallel-draft workflow)
> **Depends on**: `data-structure.md`, `coordinate-compression.md`
> **References**: subject VI.5 (Arguments, External Functions, Error handling)
>
> **M4 revisions (2026-05-29)**: Argv handling reworked. Subject only requires the multi-argument form, but evaluator scripts commonly pass quoted strings (`"3 1 2"`). The plan now treats each `argv[i]` independently via `ft_split`, which uniformly handles **all** of: multi-args, single quoted string, multiple quoted strings, and unquoted shell expansion. The argv boundary carries no semantic weight.

## English

### Phase 1 integration overview

Phase 1 is "stack/parser base (incl. coordinate compression)". Three plans cover it:

1. **Stack primitives** — `t_node` / `t_stack` types + lifecycle helpers (`data-structure.md`).
2. **Parser** — argv → validated integers → stack `a` (this plan).
3. **Coordinate compression** — rank-based index assignment (`coordinate-compression.md`).

The parser is the orchestrator: it uses the stack primitives to build `a`, then calls `coord_compress(stack_a)` before returning to `main()`. After Phase 1 completes, `main()` has a fully populated, index-assigned stack `a` and an empty stack `b`, ready for dispatch.

### Scope (parser-specific)

- Parse `argv` into integers and build stack `a`.
- Validate: integer format, `int` range, no duplicates (across **all** tokens from all argv).
- Identify the strategy selector flag (`--simple`, `--medium`, `--complex`, `--adaptive`).
- Error reporting: print `"Error\n"` to **stderr** and exit non-zero on any failure.

NOT covered here:
- Stack type definitions and helpers (see `data-structure.md`).
- Coordinate compression details (see `coordinate-compression.md`).
- The 11 push_swap operations (Phase 2).
- Strategy dispatch and `main()` orchestration (`adaptive-dispatcher.md`).

### Interface

```c
typedef enum e_strategy
{
    STRATEGY_ADAPTIVE,   /* default when no flag is given */
    STRATEGY_SIMPLE,
    STRATEGY_MEDIUM,
    STRATEGY_COMPLEX
}   t_strategy;

/* Parses argv, validates, builds stack_a, runs coordinate compression.
 * Writes the selected variant to *out_strategy.
 * Returns 0 on success; non-zero on any validation or allocation failure.
 * On failure, caller is responsible for printing "Error\n" and exiting. */
int parse_args(int argc, char **argv,
               t_stack *stack_a, t_strategy *out_strategy);
```

### Supported input forms

The parser treats each `argv[i]` independently with `ft_split`, so the **argv boundary carries no semantic meaning**. The following all produce the same stack state:

```
./push_swap 1 2 4 5 9 0 3                # 7 separate args
./push_swap "1 2 4 5 9 0 3"              # 1 quoted string
./push_swap "1 2 4 5" " 9 0 3"           # 2 quoted strings (leading space OK)
./push_swap "1 2" "4 5" "9" "0 3"        # mix of single/multi-int quoted strings
ARG="1 2 4 5 9 0 3"; ./push_swap $ARG    # unquoted shell expansion
```

→ All result in `argv[1..n]` flowing through `ft_split` and being collected into a single ordered list of integers, with the **first integer ending up at the stack's top**.

### Implementation outline

Three logical phases:

**Phase A — argv handling**

1. If `argc == 1`: nothing to parse. Return success with empty stack and default strategy. `main()` then prints nothing and exits cleanly (subject VI.5: "If no parameters are specified, the program must not display anything and give the prompt back").
2. Scan argv tokens for a strategy flag (`--simple` / `--medium` / `--complex` / `--adaptive`). Set `*out_strategy`. Default to `STRATEGY_ADAPTIVE`. Remember which `argv` indices were consumed as flags.
3. For each remaining `argv[i]`, call `ft_split(argv[i], ' ')` to break it into individual numeric tokens.
   - A single integer arg (`"42"`) yields 1 token.
   - A multi-integer quoted arg (`"3 1 2"`) yields multiple tokens.
   - An empty string (`""`) or whitespace-only (`"   "`) yields 0 tokens → **Error** (caller passed something non-meaningful).
4. Concatenate all token lists in argv order to form a single ordered list of strings.

**Phase B — per-token validation**

For each token in the accumulated list:

1. Optional leading `-` allowed for negatives.
2. All remaining characters must be ASCII digits (`'0'`–`'9'`).
3. The resulting value must fit in `int` range (`INT_MIN` … `INT_MAX`).
4. On any failure: clean up resources allocated so far, return non-zero.

**Phase C — stack construction + compression**

1. For each validated value in order, push to **bottom** of `stack_a` so the **first token ends up at the top** (subject VI.5: "the first argument is the top of the stack").
2. Detect duplicates **across all tokens from all argv** (not just per argv).
3. If a duplicate is found: clean up, return non-zero.
4. Call `coord_compress(stack_a)` to set `t_node->index` for every node.

### Edge cases

| Input | Expected behavior |
| --- | --- |
| `./push_swap` | No output, exit 0 |
| `./push_swap ""` | `"Error\n"` (empty token list from this arg) |
| `./push_swap "   "` | `"Error\n"` (whitespace-only yields 0 tokens) |
| `./push_swap "" "1 2 3"` | `"Error\n"` (first arg yields 0 tokens) |
| `./push_swap 1 2 one` | `"Error\n"` (non-integer token) |
| `./push_swap "1 abc 3"` | `"Error\n"` (non-integer token within quoted string) |
| `./push_swap 9999999999` | `"Error\n"` (out of `int` range) |
| `./push_swap 1 2 1` | `"Error\n"` (duplicate across args) |
| `./push_swap "1 2" "2 3"` | `"Error\n"` (duplicate across quoted args) |
| `./push_swap 1 2 3` (already sorted) | No output, exit 0 (dispatcher emits 0 ops at disorder 0) |
| `./push_swap "3 1 2"` | Equivalent to `./push_swap 3 1 2` |
| `./push_swap "1 2" "3"` | Equivalent to `./push_swap 1 2 3` |
| `./push_swap --complex 5 4 3 2 1` | Strategy = COMPLEX, sort proceeds |
| `./push_swap --foo 1 2` (unknown flag) | Recommend: treat as integer token → fails digit check → `"Error\n"` |
| `./push_swap --simple --medium 1 2` (two flags) | Recommend: `"Error\n"` (subject doesn't specify; defensive choice) |
| `./push_swap --simple` (flag only, no integers) | Same as `argc == 1` after flag consumption: no output, exit 0 |

### Complexity argument

- Token splitting: O(L) where L is total input string length.
- Per-token validation: O(L).
- Stack construction: O(n).
- Duplicate detection: O(n²) naïve walk over all tokens, or O(n log n) with sort.
- Coordinate compression: O(n²) naïve (per its own plan).
- Memory: O(n) for stack + O(n) temp arrays for split results and duplicate check.
- **Push_swap operations generated: 0** — this entire stage does not affect the subject's complexity measurement (VI.3.1).

### Testing

- `./push_swap` → no output, exit 0.
- `./push_swap 1` → no output, exit 0 (single element trivially sorted).
- `./push_swap 1 2 3` → no output, exit 0.
- `./push_swap "3 1 2"` → identical output to multi-arg form.
- `./push_swap "1 2" "3"` → identical output to `./push_swap 1 2 3`.
- `./push_swap "1 2 4 5" " 9 0 3"` → 7 elements pushed, first integer (1) at top.
- `./push_swap 3 1 2` → some operations, `./checker_linux 3 1 2` reports `OK`.
- `./push_swap 1 2 one` → `"Error\n"` to stderr, exit non-zero.
- `./push_swap "1 2 a"` → `"Error\n"` (non-integer in quoted form).
- `./push_swap 1 2 1` → `"Error\n"` (duplicate).
- `./push_swap "1 2" "2 3"` → `"Error\n"` (duplicate across args).
- `./push_swap 9999999999` → `"Error\n"` (out of `int` range).
- `./push_swap ""` → `"Error\n"`.
- `./push_swap "   "` → `"Error\n"`.
- `./push_swap --complex $(shuf -i 0-99 -n 100)` → exit 0, checker OK.
- Memory: run under `valgrind` or `MallocStackLogging`; all heap allocations must be freed on both success and error paths.

---

## 日本語

### Phase 1 の統合的な見取り図

Phase 1 は「stack / parser 基盤(座標圧縮含む)」。3 つのプランがカバーする:

1. **スタック primitives** — `t_node` / `t_stack` 型とライフサイクル helper(`data-structure.md`)。
2. **Parser** — argv → 検証済み整数 → スタック `a`(本プラン)。
3. **座標圧縮** — rank ベースの index 割り当て(`coordinate-compression.md`)。

Parser は orchestrator の役割:スタック primitives を使って `a` を構築し、`main()` に return する前に `coord_compress(stack_a)` を呼ぶ。Phase 1 完了後、`main()` は要素が積まれて index 割り当て済みの `a` と、空の `b` を持ち、dispatch 可能な状態になる。

### Scope(parser 固有)

- `argv` を整数列にパースし、スタック `a` を構築する。
- バリデーション:整数フォーマット、`int` 範囲、**全 argv の全 token 横断**で重複なし。
- 戦略セレクタフラグ(`--simple` / `--medium` / `--complex` / `--adaptive`)を識別する。
- エラー時:`"Error\n"` を **stderr** に出力、non-zero で exit する。

ここでカバーしないもの:
- スタック型定義と helper(`data-structure.md` 参照)。
- 座標圧縮の詳細(`coordinate-compression.md` 参照)。
- 11 個の push_swap 命令(Phase 2)。
- 戦略 dispatch と `main()` の調整(`adaptive-dispatcher.md`)。

### Interface

```c
typedef enum e_strategy
{
    STRATEGY_ADAPTIVE,   /* フラグなし時のデフォルト */
    STRATEGY_SIMPLE,
    STRATEGY_MEDIUM,
    STRATEGY_COMPLEX
}   t_strategy;

/* argv をパース、バリデーション、stack_a 構築、座標圧縮を行う。
 * 選ばれた variant を *out_strategy に書き込む。
 * 成功時 0、validation or allocation 失敗時 non-zero を返す。
 * 失敗時は呼び出し側で "Error\n" を出して exit する。 */
int parse_args(int argc, char **argv,
               t_stack *stack_a, t_strategy *out_strategy);
```

### サポートする入力形式

各 `argv[i]` を独立に `ft_split` で処理するので、**argv の境目に意味はない**。以下はすべて同じスタック状態を生む:

```
./push_swap 1 2 4 5 9 0 3                # 7 個の引数(個別)
./push_swap "1 2 4 5 9 0 3"              # 1 つのクォート文字列
./push_swap "1 2 4 5" " 9 0 3"           # 2 つのクォート文字列(先頭空白 OK)
./push_swap "1 2" "4 5" "9" "0 3"        # 単一/複数整数のクォートが混在
ARG="1 2 4 5 9 0 3"; ./push_swap $ARG    # クォートなしの shell 展開
```

→ いずれも `argv[1..n]` を `ft_split` → 1 列の整数リストに集約 → **最初の整数がスタックの top** となる。

### Implementation outline

3 段階:

**Phase A — argv handling**

1. `argc == 1` ならパース対象なし。空スタックとデフォルト戦略で成功 return。`main()` は何も出力せず exit する(subject VI.5:「パラメータ未指定なら何も表示せずプロンプトに戻る」)。
2. argv トークンから戦略フラグ(`--simple` / `--medium` / `--complex` / `--adaptive`)を探す。`*out_strategy` を設定。なければ `STRATEGY_ADAPTIVE`。フラグとして消費した index を覚えておく。
3. フラグ以外の各 `argv[i]` について `ft_split(argv[i], ' ')` を呼んで個別の数値トークン列に分解。
   - 単一整数引数(`"42"`)→ 1 token。
   - 複数整数を含むクォート引数(`"3 1 2"`)→ 複数 token。
   - 空文字列(`""`)や空白のみ(`"   "`)→ 0 token → **Error**(意味のないものを渡された)。
4. 全 argv 分の token リストを argv 順に連結し、1 列の文字列リストを得る。

**Phase B — トークン単位のバリデーション**

集約された token リスト の各トークンについて:

1. 先頭の `-`(負数用)は可。
2. 残りの文字は ASCII 数字(`'0'`–`'9'`)のみ。
3. 結果値が `int` 範囲(`INT_MIN` … `INT_MAX`)に収まること。
4. いずれかが失敗:確保済みリソースを片付け、non-zero を return。

**Phase C — スタック構築 + 圧縮**

1. 各検証済み値を順に **bottom に push** することで、**最初のトークンがスタックの top** に来るようにする(subject VI.5:「最初の引数がスタックのトップ」)。
2. **全 argv の全 token 横断**で重複検出(個別 argv 内だけではない)。
3. 重複検出時:片付け、non-zero return。
4. `coord_compress(stack_a)` を呼んで全ノードに `t_node->index` を設定。

### Edge cases

| 入力 | 期待動作 |
| --- | --- |
| `./push_swap` | 出力なし、exit 0 |
| `./push_swap ""` | `"Error\n"`(この arg から 0 token) |
| `./push_swap "   "` | `"Error\n"`(空白のみ → 0 token) |
| `./push_swap "" "1 2 3"` | `"Error\n"`(1 つ目の arg が 0 token) |
| `./push_swap 1 2 one` | `"Error\n"`(非整数) |
| `./push_swap "1 abc 3"` | `"Error\n"`(クォート内に非整数) |
| `./push_swap 9999999999` | `"Error\n"`(`int` 範囲外) |
| `./push_swap 1 2 1` | `"Error\n"`(重複) |
| `./push_swap "1 2" "2 3"` | `"Error\n"`(クォート横断で重複) |
| `./push_swap 1 2 3`(既にソート済み) | 出力なし、exit 0(disorder 0 でディスパッチャが操作を 0 個出す) |
| `./push_swap "3 1 2"` | `./push_swap 3 1 2` と同等 |
| `./push_swap "1 2" "3"` | `./push_swap 1 2 3` と同等 |
| `./push_swap --complex 5 4 3 2 1` | 戦略 = COMPLEX、ソート実行 |
| `./push_swap --foo 1 2`(未知フラグ) | 推奨:整数トークン扱い → 桁検証で失敗 → `"Error\n"` |
| `./push_swap --simple --medium 1 2`(フラグ 2 つ) | 推奨:`"Error\n"`(subject 未指定、防衛的選択) |
| `./push_swap --simple`(フラグのみ、整数なし) | フラグ消費後 `argc == 1` と同等:出力なし、exit 0 |

### Complexity argument

- トークン分割:O(L)、L は入力文字列の合計長。
- トークン単位バリデーション:O(L)。
- スタック構築:O(n)。
- 重複検出:naive で O(n²)、sort 利用なら O(n log n)。
- 座標圧縮:naive で O(n²)(個別プラン参照)。
- メモリ:スタック用 O(n) + split 結果と重複検出用の一時配列 O(n)。
- **生成される push_swap 操作:0** —— この段階全体が subject の計算量測定(VI.3.1)に影響しない。

### Testing

- `./push_swap` → 出力なし、exit 0。
- `./push_swap 1` → 出力なし、exit 0(単一要素は自明にソート済み)。
- `./push_swap 1 2 3` → 出力なし、exit 0。
- `./push_swap "3 1 2"` → 複数引数形式と同じ操作出力。
- `./push_swap "1 2" "3"` → `./push_swap 1 2 3` と同じ。
- `./push_swap "1 2 4 5" " 9 0 3"` → 7 要素 push、最初の整数(1)が top。
- `./push_swap 3 1 2` → 何らかの操作出力、`./checker_linux 3 1 2` が `OK` を返す。
- `./push_swap 1 2 one` → stderr に `"Error\n"`、non-zero exit。
- `./push_swap "1 2 a"` → `"Error\n"`(クォート内に非整数)。
- `./push_swap 1 2 1` → `"Error\n"`(重複)。
- `./push_swap "1 2" "2 3"` → `"Error\n"`(クォート横断で重複)。
- `./push_swap 9999999999` → `"Error\n"`(`int` 範囲外)。
- `./push_swap ""` → `"Error\n"`。
- `./push_swap "   "` → `"Error\n"`。
- `./push_swap --complex $(shuf -i 0-99 -n 100)` → exit 0、checker OK。
- メモリ:`valgrind` または `MallocStackLogging` 下で実行。成功・エラー両方の経路で全 heap 確保が解放されること。

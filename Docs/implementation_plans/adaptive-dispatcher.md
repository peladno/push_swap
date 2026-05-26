# Adaptive Dispatcher / Adaptive ディスパッチャ

> **Decided in**: Kickoff Topic 1 + Topic 6 (2026-05-26)
> **Drafter**: Both (Phase 7)
> **Depends on**: `data-structure.md`, `coordinate-compression.md`, `simple-insertion-sort.md`, `medium-chunk-sort.md`, `complex-radix-lsd.md`
> **References**: `learning_log/algorithms-adaptive.md`, subject VI.3.2, subject VI.3.3 #4

## English

### Scope

- Computes input `disorder` once before any moves (subject VI.3.2 requirement).
- Dispatches to the appropriate tier algorithm based on disorder.
- Used by default (no flag) and explicitly by `--adaptive`.

NOT covered: the implementations of each tier algorithm (those have their own plan files).

### Interface

```c
/* Returns the disorder ratio mistakes/total_pairs in [0, 1].
 * Caller must ensure stack_a->size >= 2 (else this is undefined). */
double  compute_disorder(t_stack *a);

/* Computes disorder and dispatches to the right tier algorithm.
 * Records the chosen strategy in bench->strategy. */
void    adaptive_sort(t_stack *a, t_stack *b, t_bench *bench);
```

### Implementation outline

**`compute_disorder`** — translates subject VI.3.2 pseudo-code to a linked-list walk:

```text
mistakes = 0
total_pairs = 0
for node_i = a.top; node_i != NULL; node_i = node_i.next:
    for node_j = node_i.next; node_j != NULL; node_j = node_j.next:
        total_pairs += 1
        if node_i.index > node_j.index:
            mistakes += 1
return mistakes / total_pairs   # as double, OR integer-compare-form (see below)
```

**`adaptive_sort`** — three-way dispatch:

```text
d = compute_disorder(a)
if d < 0.2:
    bench.strategy = "Adaptive / O(n^2)"
    insertion_sort(a, b, bench)
elif d < 0.5:
    bench.strategy = "Adaptive / O(n√n)"
    chunk_sort(a, b, bench)
else:
    bench.strategy = "Adaptive / O(n log n)"
    radix_sort_lsd(a, b, bench)
```

### Float vs integer threshold comparison

The thresholds 0.2 and 0.5 are subject-fixed. The C implementation can compare in either way:

| Style | Form |
| --- | --- |
| Float (literal) | `if (mistakes / (double)total_pairs < 0.2)` |
| Integer | `if (mistakes * 5 < total_pairs)` (equivalent to `< 0.2`) and `if (mistakes * 2 < total_pairs)` (equivalent to `< 0.5`) |

Integer form avoids floating-point and is Norm-friendly (no `double` variable). **Recommended unless there's a reason otherwise.** Document the choice in the README's complexity argument section.

### Edge cases

- `n == 0` or `n == 1`: `total_pairs == 0`. Skip disorder entirely; nothing to sort. Caller should return early or this function should detect and return immediately.
- `disorder == 0.0` (already sorted): falls into low-disorder regime → Insertion Sort, which handles sorted input in near-O(n) ops (no actual moves emitted if perfectly sorted).
- Boundary values: subject uses strict `<` for the lower bound and `≥` (implicit) for the upper. So `disorder == 0.2` → medium regime; `disorder == 0.5` → high regime. The integer-comparison form preserves this:
  - `mistakes * 5 < total_pairs` ↔ `disorder < 0.2` (strict)
  - `mistakes * 2 < total_pairs` ↔ `disorder < 0.5` (strict)

### Complexity argument (for README / defense)

- `compute_disorder`: **O(n²) C-side**, generates **0 push_swap operations**.
- Dispatch (if/else): **O(1)**.
- Selected algorithm:
  - Low-disorder → Insertion Sort: **O(n²)** push_swap ops (often closer to O(n) when input is nearly sorted)
  - Medium-disorder → Chunk Sort: **O(n√n)** push_swap ops
  - High-disorder → Radix LSD: **O(n log n)** push_swap ops
- **Total push_swap operations = the dispatched algorithm's cost** (dispatcher adds zero).

**Threshold rationale (for README)**:

- `< 0.2`: ~80% of pairs already correctly ordered → input is nearly sorted → Insertion Sort approaches O(n) total operations.
- `0.2 ≤ d < 0.5`: moderate disorder → Chunk Sort's value-range partitioning is the sweet spot without paying for Radix's bit machinery.
- `≥ 0.5`: essentially random → only O(n log n) clears the n=500 benchmark thresholds.

### Testing

| Input | Expected regime | Strategy reported |
| --- | --- | --- |
| Already sorted, n=100 | low (d=0.0) | `Adaptive / O(n^2)` |
| Nearly sorted, n=100 | low | `Adaptive / O(n^2)` |
| Moderately shuffled, n=100 | medium | `Adaptive / O(n√n)` |
| Random, n=500 | high | `Adaptive / O(n log n)` |
| Reverse sorted, n=500 | high (d=1.0) | `Adaptive / O(n log n)` |

- Verify final output is sorted via `./checker_linux $ARG`.
- Confirm `--bench` output reports the correct strategy string.
- Build a small test script that generates inputs at known disorder levels and confirms the right regime is hit.

---

## 日本語

### Scope

- 移動前に `disorder` を 1 度測定する(subject VI.3.2 要件)。
- disorder に応じて適切な tier algorithm にディスパッチ。
- デフォルト(フラグなし)および `--adaptive` で明示的に使用される。

カバーしないもの:各 tier algorithm の実装(個別のプランファイル参照)。

### Interface

```c
/* mistakes / total_pairs を [0, 1] の値で返す。
 * 呼び出し側は stack_a->size >= 2 を保証する必要(さもなくば未定義)。 */
double  compute_disorder(t_stack *a);

/* disorder を計算して適切な tier algorithm にディスパッチ。
 * 選ばれた strategy を bench->strategy に記録する。 */
void    adaptive_sort(t_stack *a, t_stack *b, t_bench *bench);
```

### Implementation outline

**`compute_disorder`** —— subject VI.3.2 の pseudo-code を連結リスト走査に翻訳:

```text
mistakes = 0
total_pairs = 0
for node_i = a.top; node_i != NULL; node_i = node_i.next:
    for node_j = node_i.next; node_j != NULL; node_j = node_j.next:
        total_pairs += 1
        if node_i.index > node_j.index:
            mistakes += 1
return mistakes / total_pairs   # double で返す、または整数比較形式(下記)
```

**`adaptive_sort`** —— 3 way ディスパッチ:

```text
d = compute_disorder(a)
if d < 0.2:
    bench.strategy = "Adaptive / O(n^2)"
    insertion_sort(a, b, bench)
elif d < 0.5:
    bench.strategy = "Adaptive / O(n√n)"
    chunk_sort(a, b, bench)
else:
    bench.strategy = "Adaptive / O(n log n)"
    radix_sort_lsd(a, b, bench)
```

### しきい値比較を float でやるか整数でやるか

しきい値 0.2 と 0.5 は subject 固定。C 実装はどちらでも可:

| スタイル | 形式 |
| --- | --- |
| Float(リテラル) | `if (mistakes / (double)total_pairs < 0.2)` |
| 整数 | `if (mistakes * 5 < total_pairs)`(`< 0.2` と等価)、`if (mistakes * 2 < total_pairs)`(`< 0.5` と等価) |

整数形式は浮動小数点を避け、Norm 適合(`double` 変数を 1 行使わずに済む)。**特別な理由がなければ整数形式を推奨**。選択した方を README の計算量議論セクションに documented にする。

### Edge cases

- `n == 0` または `n == 1`:`total_pairs == 0`。disorder 計算をスキップ、ソートするものなし。呼び出し側で早期 return するか、この関数が検出して即 return する。
- `disorder == 0.0`(既にソート済み):低 disorder レジーム → Insertion Sort へ。完全ソート済みなら移動が出力されない(near-O(n) 操作)。
- 境界値:subject は下限に厳密な `<`、上限に `≥`(暗黙)を使う。よって `disorder == 0.2` → 中レジーム、`disorder == 0.5` → 高レジーム。整数比較形式でもこれが保存される:
  - `mistakes * 5 < total_pairs` ⇔ `disorder < 0.2`(厳密)
  - `mistakes * 2 < total_pairs` ⇔ `disorder < 0.5`(厳密)

### Complexity argument(README / ディフェンス用)

- `compute_disorder`:**C 側 O(n²)**、**push_swap 操作生成は 0**。
- ディスパッチ(if/else):**O(1)**。
- 選ばれた algorithm:
  - 低 disorder → Insertion Sort:**O(n²)** push_swap 操作(ほぼソート済みなら O(n) に近づく)
  - 中 disorder → Chunk Sort:**O(n√n)** push_swap 操作
  - 高 disorder → Radix LSD:**O(n log n)** push_swap 操作
- **合計 push_swap 操作 = ディスパッチされた algorithm のコスト**(ディスパッチャ自体は何も加算しない)。

**しきい値の根拠(README に書く)**:

- `< 0.2`:約 80% のペアが既に正しい順 → 入力はほぼソート済み → Insertion Sort の合計操作数が O(n) に近づく。
- `0.2 ≤ d < 0.5`:中程度の乱れ → Chunk Sort の値域分割が sweet spot。Radix の bit 機構を払うほどではない。
- `≥ 0.5`:ほぼランダム → O(n log n) でないと n=500 ベンチを通せない。

### Testing

| 入力 | 期待 regime | 報告される strategy |
| --- | --- | --- |
| 既にソート済み、n=100 | 低(d=0.0) | `Adaptive / O(n^2)` |
| ほぼソート済み、n=100 | 低 | `Adaptive / O(n^2)` |
| 中程度に混ぜた、n=100 | 中 | `Adaptive / O(n√n)` |
| ランダム、n=500 | 高 | `Adaptive / O(n log n)` |
| 逆順、n=500 | 高(d=1.0) | `Adaptive / O(n log n)` |

- `./checker_linux $ARG` で最終出力がソート済みか検証。
- `--bench` 出力で正しい strategy 文字列が出ているか確認。
- 既知の disorder レベルで入力を生成し、正しい regime に入っているか確認する小さなテストスクリプトを書く。

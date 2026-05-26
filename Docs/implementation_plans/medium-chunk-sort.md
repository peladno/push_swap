# Medium Tier: Chunk Sort with In-Chunk Sort / Medium tier: Chunk Sort + in-chunk sort

> **Decided in**: Kickoff Topic 1 (2026-05-26)
> **Drafter**: Javier (Phase 5)
> **Depends on**: `data-structure.md`, `coordinate-compression.md`
> **References**: `learning_log/algorithms-medium-o-nsqrtn.md`

## English

### Scope

- Sorts stack `a` ascending via Chunk Sort with insertion-style in-chunk sorting.
- Used by `--medium` flag **and** Adaptive medium-disorder regime.
- Operates on `t_node->index` (values are in `[0, n)` thanks to coordinate compression, making chunk boundaries trivial).

NOT covered: dispatching from Adaptive (see `adaptive-dispatcher.md`).

### Interface

```c
void chunk_sort(t_stack *a, t_stack *b, t_bench *bench);
```

- Inputs: `a` (compressed, size n), `b` (empty).
- Outputs: `a` sorted ascending top→bot, `b` empty.

### Implementation outline

**Chunk size**: `c = ⌈√n⌉`. Number of chunks: `k = ⌈n / c⌉`.

(A small constant adjustment to `c` based on `n` is permitted but should be documented.)

**Push phase** (low-value chunks first):

For chunk index `i` from `0` to `k - 1`:
  Walk through `a` and for each top element:
    - If `top->index` falls in chunk `i` (i.e., `i*c ≤ index < (i+1)*c`):
      - **In-chunk insertion**: rotate `b` (`rb` / `rrb`, shorter direction) so the **largest index in b less than `top->index`** (or `max(b)` if none) is at b's top.
      - `pb`.
    - Else: `ra`.

**Finalize**:

- Rotate `b` so `max(b->index)` is at b's top (canonical descending).
- `pa × n`. `a` is sorted.

**Pseudo-code**:

```text
c = ceil(sqrt(n))
for chunk_i in 0 .. ceil(n/c) - 1:
    chunk_low  = chunk_i * c
    chunk_high = chunk_low + c
    while there are still elements in `a` belonging to chunk_i:
        if a.top.index in [chunk_low, chunk_high):
            x_below = (largest in b with index < a.top.index) or max(b)
            rotate b shortest direction so x_below at top
            pb
        else:
            ra

# Finalize
rotate b shortest direction so max(b) at top
while b not empty:
    pa
```

> "While there are still elements" can be tracked with a counter set when entering the chunk (count how many of `a`'s nodes are in this chunk before processing).

### Edge cases

- `n == 0` or `n == 1`: caller handles.
- `n` not divisible by `c`: the last chunk is smaller; the loop handles this naturally because the chunk-range check excludes nonexistent indices.
- In-chunk rotation depth is bounded by `c` (≤ √n) — this is what keeps the total at O(n√n) instead of O(n²).
- Already sorted: chunk sort is not strongly adaptive; expect similar op counts to random input.

### Complexity argument (for README / defense)

- Number of chunks: `k ≈ √n`.
- Per chunk:
  - Walk over `a`'s elements: at most `n` operations (each is `ra` or `pb`).
  - For each `pb`, in-chunk insertion rotation depth ≤ √n (only chunk-i elements in b).
  - So per chunk: O(n + √n × √n) = O(n + n) = **O(n)**.
- Push phase total: `√n × O(n)` = **O(n√n)**.
- Finalize rotation: ≤ n, dominated by previous step.
- `pa` phase: O(n).
- **Total: O(n√n) push_swap operations.**

> The "in-chunk rotation depth ≤ √n" claim is the load-bearing one. It holds because by the time we're inserting chunk-i elements, b's "chunk-i region" has at most c = √n elements. We only rotate among that region (cyclically over all of b, but never more than √n positions).

### Testing

- n=100 random: expect ~700–1,500 ops.
- n=500 random: expect ~5,000–8,000 ops (good band; aiming below 8,000).
- Always verify with `./checker_linux $ARG`.
- `--bench` should report `strategy: Medium / O(n√n)` (or `Adaptive / O(n√n)` when dispatched).
- Try varying `c` (e.g., `√n`, `√n + 2`, etc.) and benchmark to find the sweet spot for our implementation's constant factor.

---

## 日本語

### Scope

- Chunk Sort + insertion 流 in-chunk sort で `a` を昇順ソート。
- `--medium` フラグ **および** Adaptive 中 disorder レジームから使う。
- `t_node->index` で動作(座標圧縮で値が `[0, n)` に収まっているので、chunk 境界が簡潔)。

カバーしないもの:Adaptive からのディスパッチ(`adaptive-dispatcher.md` 参照)。

### Interface

```c
void chunk_sort(t_stack *a, t_stack *b, t_bench *bench);
```

- 入力:`a`(圧縮済み、サイズ n)、`b`(空)。
- 出力:`a` が昇順(top→bot)、`b` が空。

### Implementation outline

**チャンクサイズ**:`c = ⌈√n⌉`。チャンク数:`k = ⌈n / c⌉`。

(n に応じて `c` を少し調整するのは可。ただし根拠を documented にする。)

**Push 段階**(低値チャンクから順に):

`chunk_i` を `0` から `k - 1` まで:
  `a` を走査して各 top 要素について:
    - `top->index` が chunk `i` の範囲(`i*c ≤ index < (i+1)*c`)に含まれる場合:
      - **In-chunk 挿入**:`b` を回転(`rb`/`rrb` の短い方向)して、**`b` の中で `top->index` より小さい最大 index**(`b` にそれより小さいものがなければ `max(b)`)を b の top に。
      - `pb`。
    - そうでなければ:`ra`。

**仕上げ**:

- `b` を回転して `max(b->index)` を b の top に(canonical 降順)。
- `pa × n`。`a` がソートされる。

**Pseudo-code**:

```text
c = ceil(sqrt(n))
for chunk_i in 0 .. ceil(n/c) - 1:
    chunk_low  = chunk_i * c
    chunk_high = chunk_low + c
    while a に chunk_i 範囲の要素が残っている限り:
        if a.top.index in [chunk_low, chunk_high):
            x_below = (b 内で index < a.top.index の最大) または max(b)
            短い方向で b を回転して x_below を top に
            pb
        else:
            ra

# 仕上げ
短い方向で b を回転して max(b) を top に
while b not empty:
    pa
```

> 「残っている限り」はチャンク突入時にカウンタをセットして追跡できる(処理前に `a` 内の chunk_i 範囲要素数を数えておく)。

### Edge cases

- `n == 0` または `n == 1`:呼び出し側で処理。
- `n` が `c` で割り切れない:最後の chunk が小さい。範囲 check が自動的に存在しない index を除外するので、ループは自然に処理。
- In-chunk 回転深さは `c`(≤ √n)で bounded —— これが合計を O(n²) ではなく O(n√n) に保つ根拠。
- 既にソート済み:Chunk Sort は強く適応的ではないので、ランダム入力と同程度の操作数を予想。

### Complexity argument(README / ディフェンス用)

- チャンク数:`k ≈ √n`。
- チャンクあたり:
  - `a` を走査:最大 `n` 操作(各々 `ra` または `pb`)。
  - 各 `pb` について、in-chunk 挿入の回転深さ ≤ √n(b 内の chunk_i 要素のみ対象)。
  - したがってチャンクあたり:O(n + √n × √n) = O(n + n) = **O(n)**。
- Push 段階合計:`√n × O(n)` = **O(n√n)**。
- 仕上げ回転:≤ n、前段で吸収。
- `pa` 段階:O(n)。
- **合計:O(n√n) push_swap 操作**。

> 「in-chunk 回転深さ ≤ √n」が load-bearing な主張。chunk_i の要素を挿入する時点で、b の chunk_i 領域は最大 `c = √n` 要素しか含まないため、その範囲だけで回転すれば足りる(b 全体を巡回するが、その範囲を超えて回ることはない)。

### Testing

- n=100 random:約 700〜1,500 ops。
- n=500 random:約 5,000〜8,000 ops(good 圏、8,000 を下回ることを狙う)。
- `./checker_linux $ARG` で必ず検証。
- `--bench` で `strategy: Medium / O(n√n)`(または dispatch 時は `Adaptive / O(n√n)`)が表示されること。
- `c` を変えて(例:`√n`、`√n + 2` など)ベンチを取り、定数倍の sweet spot を探る。

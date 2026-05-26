# Simple Tier: Insertion Sort / Simple tier: Insertion Sort

> **Decided in**: Kickoff Topic 1 (2026-05-26)
> **Drafter**: Javier (Phase 4)
> **Depends on**: `data-structure.md`, `coordinate-compression.md`
> **References**: `learning_log/algorithms-simple-o-n2.md`

## English

### Scope

- Sorts stack `a` ascending via Insertion Sort.
- Used by `--simple` flag **and** Adaptive low-disorder regime (no algorithm differentiation needed because Insertion already handles low disorder well).
- Operates on `t_node->index`.

NOT covered: dispatching from Adaptive (see `adaptive-dispatcher.md`), benchmarking (`bench` counter — see `benchmark-mode.md` if/when added).

### Interface

```c
void insertion_sort(t_stack *a, t_stack *b, t_bench *bench);
```

- Inputs: `a` (compressed, size n ≥ 2), `b` (empty).
- Outputs: `a` sorted ascending top→bot, `b` empty.
- Side effect: emits push_swap operations and updates `bench` counters.

### Implementation outline

Build `b` as a **circular-descending accumulator** (exactly one wrap point allowed). Then canonicalize and pop all back.

**Insertion rule** (the heart of Insertion Sort in push_swap):

> Before each `pb`, rotate `b` so that the **largest element in b smaller than `x`** (or `max(b)` if no element is smaller than `x`) sits at b's top. Then `pb` places `x` directly above its predecessor in descending order.

**Steps**:

1. Push the first element to `b` (no rotation, b is empty).
2. For each subsequent element popped from `a`:
   2a. Determine `x_below` per the insertion rule.
   2b. Rotate `b` using whichever of `rb` / `rrb` reaches `x_below` in fewer ops.
   2c. `pb`.
3. After `a` is empty, rotate `b` so that `max(b)` (by `index`) is at `b->top` (canonical descending).
4. `pa × n`. `a` is now ascending.

**Pseudo-code**:

```text
for x in a (top to bot, popped one by one):
    if b is empty:
        pb
        continue
    x_below = (largest in b with index < x.index) or max(b)
    rotate b shortest direction so x_below at top
    pb

# Finalize
rotate b shortest direction so max(b) at top
while b not empty:
    pa
```

### Edge cases

- `n == 0` or `n == 1`: caller handles (this function expects `n ≥ 2`).
- Already sorted: at each push, `x_below` is `max(b)` which lands at top after at most one rotation step → roughly O(n) total operations. **This is the killer feature for the adaptive low-disorder regime.**
- Reverse sorted: worst case for op count, but still O(n²) bounded.
- Determining "shortest rotation direction": track positions during insertion, choose `rb` if `position < size/2` else `rrb`.

### Complexity argument (for README / defense)

- For each of n pushes: insertion-point search and rotation cost up to **O(b's current size) = O(n)**.
- Push phase total: **O(n²)**.
- Canonicalize: O(n) worst case.
- `pa` phase: O(n).
- **Total: O(n²) push_swap operations.**

**Best case (sorted or near-sorted input)**: each push needs ≈ 0 rotations because `x_below` is already at top. Total operations approach **O(n)**. This is why Insertion fits the Adaptive low-disorder regime.

### Testing

- n=100 random: expect ~2,500–5,000 ops.
- n=100 already sorted: expect close to ~200 ops (n push + n pa + minimal finalize).
- n=500 (forced `--simple`): expect tens of thousands; benchmark will not pass — that's OK, `--simple` is required by subject but not expected to clear n=500 thresholds.
- Always verify with `./checker_linux $ARG`.
- `--bench` should report `strategy: Simple / O(n²)` (or `Adaptive / O(n²)` when dispatched).

---

## 日本語

### Scope

- Insertion Sort で `a` を昇順にソートする。
- `--simple` フラグ **および** Adaptive の低 disorder レジームから使う(Insertion は低 disorder に強いので algorithm 差別化は不要)。
- `t_node->index` で動作する。

カバーしないもの:Adaptive からのディスパッチ(`adaptive-dispatcher.md` 参照)、bench カウンタ詳細。

### Interface

```c
void insertion_sort(t_stack *a, t_stack *b, t_bench *bench);
```

- 入力:`a`(圧縮済み、サイズ n ≥ 2)、`b`(空)。
- 出力:`a` が昇順(top→bot)、`b` が空。
- 副作用:push_swap 命令を出力、`bench` カウンタを更新。

### Implementation outline

`b` を **円環的降順アキュムレータ**(ラップ点が厳密に 1 つ)として構築する。最後に canonical 化して全部 pop で戻す。

**挿入ルール**(push_swap における Insertion Sort の核心):

> 各 `pb` の前に、**`b` の中で `x` より小さい最大の要素**(`b` に `x` より小さいものがなければ `max(b)`)を b の top に持ってくるよう回転する。`pb` で `x` がその要素のすぐ上に乗り、降順関係が保たれる。

**手順**:

1. 最初の要素を `b` に push(`b` が空なので回転不要)。
2. `a` から pop した各要素について:
   2a. 上記ルールで `x_below` を特定。
   2b. `rb` か `rrb` のうち手数の少ない方向で `b` を回転して `x_below` を top に。
   2c. `pb`。
3. `a` が空になったら、`b` を回転して `max(b)`(`index` 基準)を `b->top` に持ってくる(canonical 降順)。
4. `pa × n`。これで `a` が昇順になる。

**Pseudo-code**:

```text
for x in a (top から bot へ、1 つずつ pop):
    if b が空:
        pb
        continue
    x_below = (b 内で index < x.index の最大) または max(b)
    短い方向で b を回転して x_below を top に
    pb

# 仕上げ
短い方向で b を回転して max(b) を top に
while b not empty:
    pa
```

### Edge cases

- `n == 0` または `n == 1`:呼び出し側で処理(この関数は `n ≥ 2` を前提)。
- 既にソート済み:各 push で `x_below` が `max(b)` で、最大 1 ステップの回転で top に到達 → 合計ほぼ O(n) 操作。**これが Adaptive 低 disorder レジームでの killer feature**。
- 逆順ソート済み:操作数のワーストケースだが、それでも O(n²) 上界内。
- 「短い方向の回転」の決定:挿入位置を走査中に追跡し、`position < size/2` なら `rb`、そうでなければ `rrb`。

### Complexity argument(README / ディフェンス用)

- n 回の push それぞれで、挿入位置探索と回転コストが最大 **O(b の現在サイズ)= O(n)**。
- Push 全体:**O(n²)**。
- Canonicalize:最大 O(n)。
- `pa` 段階:O(n)。
- **合計:O(n²) push_swap 操作**。

**ベストケース(ソート済みまたはほぼソート済み入力)**:各 push の回転が約 0 回で済む(`x_below` が既に top にあるため)。合計操作数は **O(n)** に近づく。これが Insertion を Adaptive 低 disorder レジームに採用する根拠。

### Testing

- n=100 random:約 2,500〜5,000 ops。
- n=100 既にソート済み:約 200 ops(n 回 push + n 回 pa + 最小限の仕上げ)。
- n=500(`--simple` 強制):数万 ops。ベンチを通らないが OK ——`--simple` は subject 要件で、n=500 ラインを越えることは想定されていない。
- `./checker_linux $ARG` で必ず検証。
- `--bench` で `strategy: Simple / O(n²)`(または dispatch 時は `Adaptive / O(n²)`)が表示されること。

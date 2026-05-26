# Complex Tier: Radix Sort LSD (Binary) / Complex tier: Radix Sort LSD(2 進)

> **Decided in**: Kickoff Topic 1 (2026-05-26)
> **Drafter**: So (Phase 6)
> **Depends on**: `data-structure.md`, `coordinate-compression.md`
> **References**: `learning_log/algorithms-complex-o-nlogn.md`, `learning_log/radix_sort.md`(Javier)

## English

### Scope

- Sorts stack `a` ascending via binary radix LSD (Least Significant Digit).
- Used by `--complex` flag **and** Adaptive high-disorder regime.
- Operates on `t_node->index` (coordinate-compressed values in `[0, n)`).
- Assumes coordinate compression already done by the parser (Phase 1).

NOT covered: coordinate compression itself (see `coordinate-compression.md`), dispatching from Adaptive (see `adaptive-dispatcher.md`).

### Interface

```c
void radix_sort_lsd(t_stack *a, t_stack *b, t_bench *bench);
```

- Inputs: `a` (compressed, size n), `b` (empty).
- Outputs: `a` sorted ascending top→bot, `b` empty.
- Side effect: emits push_swap operations, updates `bench` counters.

### Implementation outline

**Determine bit count**: `max_bits = ⌈log₂(n)⌉`. This is the number of bits needed to represent any index in `[0, n)`.

**Main loop**: for each bit position `i` from `0` (LSD) to `max_bits - 1`:

1. **Distribute pass** — walk over `a`'s current top n times:
   - If `(a->top->index >> i) & 1 == 1`: `ra` (1-bit stays in a, moves to back).
   - Else: `pb` (0-bit goes to b).
2. **Re-merge pass** — while `b->size > 0`: `pa`.

After all bits processed, `a` is sorted ascending.

**Pseudo-code**:

```text
max_bits = ceil(log2(n))   # bits needed to represent n - 1
for i in 0 .. max_bits - 1:
    # distribute: 0-bits to b, 1-bits stay in a
    for _ in 0 .. n - 1:
        if ((a.top.index >> i) & 1) == 1:
            ra
        else:
            pb
    # merge b back on top of a (1-bits below, 0-bits on top → sorted by bit i)
    while b not empty:
        pa
# after max_bits rounds, a is fully sorted
```

**Computing `max_bits`**:

```c
int  count_bits(int n);  // returns ceil(log2(n)) for n >= 2, 0 for n <= 1
```

A simple implementation walks bit positions while `(1 << bits) < n`.

### Edge cases

- `n == 0` or `n == 1`: caller handles; this function expects `n ≥ 2`.
- Already sorted input: same operation count (Radix is non-adaptive — every bit round runs).
- Reverse sorted input: same operation count.
- `max_bits` calculation: be careful with `n = 2` (needs 1 bit, not 2) — `ceil(log2(2)) = 1`.

### Complexity argument (for README / defense)

This is the defendable argument required by subject VI.3.3:

> **Coordinate compression fixes the value range to `[0, n)`**, so the required bit count is `⌈log₂(n)⌉`. Each bit round performs distribute (`pb`/`ra` over n elements) + merge (`pa` × number of 0-bits), totaling **≤ 2n push_swap operations per round**. With `⌈log₂(n)⌉` rounds, the total is **O(n log n) push_swap operations**.

- Per bit round: ~2n ops.
- Number of rounds: `⌈log₂(n)⌉`.
- **Total: O(n log n) push_swap operations.**
- C-side complexity: same — no auxiliary data structures, just integer bit shifts.
- Memory: no additional allocation beyond `a` and `b`.

### Testing

- n=100 random: expect ~600–700 ops.
- n=500 random: expect ~4,500–6,000 ops (target: excellent band <5,500).
- n=500 sorted: same count (non-adaptive).
- Verify with `./checker_linux $ARG`.
- `--bench` should report `strategy: Complex / O(n log n)` (or `Adaptive / O(n log n)` when dispatched).
- Test boundary `n` values: 2, 4, 8 (powers of 2) and 3, 7 (non-powers, to check `max_bits` calculation).

---

## 日本語

### Scope

- 2 進 Radix Sort LSD(Least Significant Digit)で `a` を昇順ソート。
- `--complex` フラグ **および** Adaptive 高 disorder レジームから使う。
- `t_node->index`(座標圧縮済みで `[0, n)`)で動作。
- 座標圧縮は parser(Phase 1)で完了している前提。

カバーしないもの:座標圧縮自体(`coordinate-compression.md` 参照)、Adaptive からのディスパッチ(`adaptive-dispatcher.md` 参照)。

### Interface

```c
void radix_sort_lsd(t_stack *a, t_stack *b, t_bench *bench);
```

- 入力:`a`(圧縮済み、サイズ n)、`b`(空)。
- 出力:`a` が昇順(top→bot)、`b` が空。
- 副作用:push_swap 命令を出力、`bench` カウンタを更新。

### Implementation outline

**bit 数の決定**:`max_bits = ⌈log₂(n)⌉`。`[0, n)` の任意 index を表現するのに必要な bit 数。

**メインループ**:bit 位置 `i` を `0`(LSD)から `max_bits - 1` まで:

1. **分配段階** —— `a` の top を n 回見る:
   - `(a->top->index >> i) & 1 == 1` なら:`ra`(1-bit は a の末尾へ)。
   - そうでなければ:`pb`(0-bit は b へ)。
2. **再マージ段階** —— `while b->size > 0`:`pa`。

全 bit を処理し終えると `a` が昇順にソート済み。

**Pseudo-code**:

```text
max_bits = ceil(log2(n))   # n - 1 を表現するのに必要な bit 数
for i in 0 .. max_bits - 1:
    # 分配:0-bit を b へ、1-bit は a に残る
    for _ in 0 .. n - 1:
        if ((a.top.index >> i) & 1) == 1:
            ra
        else:
            pb
    # b を a の上に戻す(1-bit が下、0-bit が上 → bit i でソート済み)
    while b not empty:
        pa
# max_bits round 後、a は完全にソート済み
```

**`max_bits` の計算**:

```c
int  count_bits(int n);  // n >= 2 で ceil(log2(n)) を、n <= 1 で 0 を返す
```

単純な実装:`(1 << bits) < n` の間 `bits` を増やす。

### Edge cases

- `n == 0` または `n == 1`:呼び出し側で処理(この関数は `n ≥ 2` を前提)。
- 既にソート済み入力:操作数同じ(Radix は非適応 —— 全 bit round を回す)。
- 逆順入力:操作数同じ。
- `max_bits` 計算:`n = 2` に注意(2 bit ではなく 1 bit。`ceil(log2(2)) = 1`)。

### Complexity argument(README / ディフェンス用)

subject VI.3.3 が要求する defendable な議論:

> **座標圧縮で値域を `[0, n)` に固定**するため、必要 bit 数は `⌈log₂(n)⌉`。各 bit round で 分配(`pb`/`ra` を n 要素分)+ マージ(`pa` を 0-bit の数だけ)が走り、**1 round あたり最大 2n push_swap 操作**。`⌈log₂(n)⌉` round で合計は **O(n log n) push_swap 操作**。

- bit round あたり:約 2n 操作。
- round 数:`⌈log₂(n)⌉`。
- **合計:O(n log n) push_swap 操作**。
- C 側計算量:同じ —— 補助データ構造なし、整数 bit shift のみ。
- メモリ:`a` と `b` 以外の追加確保なし。

### Testing

- n=100 random:約 600〜700 ops。
- n=500 random:約 4,500〜6,000 ops(目標:excellent 圏 <5,500)。
- n=500 sorted:同じ操作数(非適応)。
- `./checker_linux $ARG` で必ず検証。
- `--bench` で `strategy: Complex / O(n log n)`(または dispatch 時は `Adaptive / O(n log n)`)が表示されること。
- 境界 `n` 値テスト:2、4、8(2 のべき乗)と 3、7(べき乗でない、`max_bits` 計算の確認)。

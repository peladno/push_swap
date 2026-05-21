# Simple O(n²) Algorithms / Simple O(n²) のアルゴリズム

## English

This entry covers four O(n²) sorting algorithms adapted to push_swap. They share the same complexity class but differ in operation count and in how they behave on partially sorted input.

### Common idea (the family)

Each algorithm performs roughly `n × n` work: for each of `n` elements, about `n` operations are needed to lock it into its final place. So the total grows as `n²`.

In push_swap operation counts: thousands at n=100, tens of thousands at n=500. **The subject benchmarks (100 < 2000, 500 < 12000) are not meant to be passed by Simple alone.** Simple exists to satisfy subject VI.3.3 #1 and to serve as the low-disorder branch of the adaptive strategy.

### ① Selection Sort

**Idea**: Find the smallest element in the unsorted side and move it out. Repeat.

1. Find the minimum in `a`.
2. Rotate `a` (`ra` or `rra`) until that minimum is at the top.
3. `pb` to move it to `b`.
4. Repeat 1–3 while `a` has elements.
5. `pa` × n to move everything back to `a`.

`b` ends up with **max at top, min at bottom (descending)**. When `pa` pops in order, the max comes off first and is pushed to the bottom of the final `a`; the min comes off last and ends at the top of `a` → `a` is ascending.

### ② Min Extraction

**Idea**: Same as Selection Sort with one optimization — in step 2, choose whichever of `ra` / `rra` is shorter.

#### Difference from Selection Sort

Step 2: bring the minimum to the top with **whichever direction takes fewer operations**.

This roughly halves the average rotation cost from `n/2` per iteration to `n/4`.

### ③ Insertion Sort

**Idea**: Keep `b` as a "sorted accumulator". For each element popped from `a`, place it at its correct position in `b` by rotating `b` first.

1. Look at `a`'s top (= the element `x` to insert).
2. Rotate `b` (`rb` / `rrb`) so that **the largest element in `b` smaller than `x`** sits at `b`'s top. (If no element in `b` is smaller than `x`, rotate so `max(b)` is at the top.)
3. `pb` to send `x`; it lands directly above its "predecessor" in descending order.
4. Repeat 1–3 until `a` is empty.
5. Rotate `b` (`rb` / `rrb`) so its max is at the top (canonical descending). Then `pa` × n.

Why `b` is **descending** (top = max): so that `pa pa pa ...` at the end pops max first into `a`'s top (pushing previously-popped items down), and the last `pa` (which pops `b`'s bot = min) ends at the top of `a` → `a` is ascending.

**Strong on partially sorted input**: when many elements are already near their final position, almost no rotation is needed. This is the killer feature for the adaptive low-disorder regime.

### ④ Bubble Sort

**Idea**: Compare adjacent pairs and swap if reversed; repeat full passes until one pass produces zero swaps.

1. Look at `a`'s top two elements; if reversed, `sa`.
2. `ra` to expose the next pair at the top.
3. After a full pass over `n` elements, check whether any `sa` happened. If yes, repeat from 1.
4. When a full pass uses zero `sa`, sorted.

In push_swap, every pair check requires a rotation to advance, which inflates the operation count. **Slowest of the four.**

### Comparison table

| Algorithm | Concept | Ops at n=100 (approx) | Ops at n=500 (approx) | Implementation | Behavior on low disorder | Pick? |
| --- | --- | ---: | ---: | --- | --- | :---: |
| Selection Sort (naive) | Find min, rotate, push | ~5,100 | ~125,500 | Easy | Same as worst case | △ |
| Min Extraction | + shorter rotation direction | ~2,600 | ~62,600 | Easy + one `if` | Same as worst case | ◎ |
| Insertion Sort | Sorted accumulator in `b` | ~2,500–5,000 | ~62,500+ | Medium (circular-descending logic) | **Very fast** | ◯ (great for adaptive) |
| Bubble Sort | Adjacent compare + swap | ~10,000 | ~250,000 | Easy | Poor | ✗ |

### How to decide (questions to bring to the pair meeting)

- Want **lowest ops under `--simple`**? → **Min Extraction**.
- Want the algorithm that **boosts adaptive on low-disorder input**? → **Insertion Sort**.
- Want the algorithm that's **easiest to defend verbally**? → **Selection Sort** (clearest but largest op count).
- **Avoid Bubble Sort** — strictly dominated by Selection in op count, no compensating advantage.

> The rationale we pick must be documented in the project README and defendable in peer-eval. "It was in the skeleton" is not a defense.

---

## 日本語

このエントリは push_swap 向けに適応された O(n²) 系のソートアルゴリズム 4 つをまとめる。同じ計算量クラスに属するが、生成される操作数と部分整列入力での挙動が異なる。

### 共通の発想(ファミリー)

各アルゴリズムは「n 要素それぞれに対して n に比例する作業」を繰り返す → 合計は `n²` のオーダー。

push_swap 操作数の感覚:n=100 で数千、n=500 で数万〜十数万。**subject のベンチライン(100 < 2000、500 < 12000)を Simple 単体で通すことは想定されていない**。Simple は VI.3.3 #1 の必須要件として、また adaptive の低 disorder レジームで呼ぶ用途として実装する。

### ① Selection Sort(選択ソート)

**発想**: 未整列の中から最小値を取り出して整列済みに移す、を繰り返す。

1. `a` の中で最小値を探す。
2. その最小値が `a` の先頭に来るように `ra` か `rra` で回転。
3. `pb` で `b` に送る。
4. `a` に要素が残っている限り 1〜3 を繰り返す。
5. 最後に `pa` を n 回繰り返して `a` に戻す。

`b` は最終的に **top=max、bot=min(降順)**になる。`pa` 連打で `b` の top(最大)から順に取り出され、最後に取り出されるのが `b` の bot(最小)→ `a` の top=最小 → `a` は昇順。

### ② Min Extraction(最小抽出 = 選択ソートの最適化)

**発想**: Selection Sort と同じだが、操作 2 で `ra` と `rra` のうち**手数が少ない方向**を選ぶ最適化。

#### Selection Sort との違い

操作 2: 最小値を先頭に持ってくる回転を、`ra` と `rra` の**少ない方の手数**で実行する。

これで平均回転コストが `n/2` から `n/4` におよそ半減する。

### ③ Insertion Sort(挿入ソート)

**発想**: `b` を「整列済みアキュムレータ」として保持し、`a` から取り出した要素を `b` の正しい位置に挿入していく。

1. `a` の先頭を見る(= 挿入する要素 `x`)。
2. `b` 内で **`x` より小さい最大の要素**(無ければ `max(b)`)が `b` の top に来るように `rb` / `rrb` で回転。
3. `pb` で送る → `x` がその要素の上に乗り、降順関係が保たれる(円環的降順)。
4. `a` が空になるまで 1〜3 を繰り返す。
5. `rb` / `rrb` で `b` の max を top に揃える(canonical 降順)→ `pa` を n 回。

`b` を **降順**(top=max)で保持する理由:最後に `pa` 連打すると `b` の top(=最大)から順に `a` に積まれ、最後に積まれるのが `b` の bot(=最小)。`a` の top=最小 → `a` は昇順になる。

**部分整列入力に強い**: 多くの要素が既にあるべき位置近くにあれば、回転がほとんど不要になる。これが adaptive の低 disorder レジームで活きる決め手。

### ④ Bubble Sort(バブルソート)

**発想**: 隣接ペアを比較して逆順なら入れ替え、を全体にわたって繰り返す。1 周通して入れ替えが 1 回も発生しなければ完了。

1. `a` の先頭 2 要素を見て、逆順なら `sa`。
2. `ra` で次のペアを先頭に持ってくる。
3. 全 n 要素を 1 周したら、また 1 から繰り返す。
4. 1 周まるごと `sa` が 0 回なら終了。

push_swap では各ペアチェックごとに回転が必須なため、**Simple の中で最も操作数が多い**。

### 比較表

| アルゴリズム | 発想 | n=100 概算 | n=500 概算 | 実装難度 | 低 disorder 時 | 採用度 |
| --- | --- | ---: | ---: | --- | --- | :---: |
| Selection Sort(naive) | min を探して回転して push | 約 5,100 | 約 125,500 | 易 | 最悪と同じ | △ |
| Min Extraction | + 回転方向の最適化 | 約 2,600 | 約 62,600 | 易 + `if` 1 つ | 最悪と同じ | ◎ |
| Insertion Sort | `b` に整列済みアキュムレータ | 約 2,500〜5,000 | 約 62,500+ | 中(円環的降順の管理) | **非常に速い** | ◯(adaptive 向き) |
| Bubble Sort | 隣接比較 + swap | 約 10,000 | 約 250,000 | 易 | 悪い | ✗ |

### 選び方の指針(相方との議論材料)

- `--simple` 強制時の**操作数を最小化**したい → **Min Extraction**
- **adaptive の低 disorder レジーム**で活きるものが欲しい → **Insertion Sort**
- **ディフェンスでの説明しやすさ**を最重視 → **Selection Sort**(ただし操作数で劣る)
- Bubble Sort は Selection に対して操作数で厳密に劣り、補う長所も無いため**避ける**

> 採用根拠は **README に文書化**し、ピア評価で説明できる状態にする。「スケルトンに入っていたから」は不可。

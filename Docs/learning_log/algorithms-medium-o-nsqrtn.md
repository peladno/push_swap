# Medium O(n√n) Algorithms / Medium O(n√n) のアルゴリズム

## English

The Medium tier is essentially a single family: **Chunk Sort**. The subject lists four candidates (Chunk-based, Block-based partitioning, Bucket Sort with √n buckets, Range-based), but in push_swap they all converge to the same approach.

### Common idea

**Divide the value range into √n chunks, then process the chunks one at a time.** Each chunk processing requires one pass over `a` (`O(n)`). With √n chunks total:

```
√n chunks × O(n) per chunk = O(n√n)
```

### Why coordinate compression matters here

If values are first compressed to `0..n-1`, chunk boundaries become trivial (`floor(value / chunk_size)`). Even when we don't use Radix for the Complex tier, **coordinate compression alone is worth doing for the sake of Medium**.

### Operation count feel

| n | √n | Theoretical O(n√n) | Typical implementations |
| ---: | ---: | ---: | --- |
| 100 | 10 | ~1,000 | ~700–1,500 |
| 500 | 22 | ~11,000 | ~5,000–14,000 |
| 1000 | 32 | ~32,000 | ~15,000–40,000 |

The n=500 benchmark thresholds (<12,000 / <8,000 / <5,500) live right inside this range. **Whether we land at "barely pass" or "excellent" depends on implementation quality**, not on choosing a different algorithm.

### ① Chunk Sort — the main approach

#### Basic algorithm

1. Compress values to `0..n-1`.
2. Pick a chunk size `c ≈ √n`.
3. Process chunks **from lowest to highest values**:
   - Iterate through `a`. If the top is in the current chunk, `pb`. Otherwise `ra`.
4. After all chunks are pushed, `b` is roughly descending (highest chunk on top, lowest on bottom).
5. `pa × n` to send everything back, with adjustments if `b` isn't perfectly sorted.

#### Small trace (n=4, chunk_size=2)

Input: `a = [2, 0, 3, 1]` (top→bot). Chunk 0 = {0, 1}, chunk 1 = {2, 3}.

**Pass 1 — chunk 0:**

| op | a after | b after |
| --- | --- | --- |
| `ra` (2 not in chunk 0) | `[0, 3, 1, 2]` | `[]` |
| `pb` (0 in chunk 0) | `[3, 1, 2]` | `[0]` |
| `ra` (3 not in chunk 0) | `[1, 2, 3]` | `[0]` |
| `pb` (1 in chunk 0) | `[2, 3]` | `[1, 0]` |

**Pass 2 — chunk 1:**

| op | a after | b after |
| --- | --- | --- |
| `pb` | `[3]` | `[2, 1, 0]` |
| `pb` | `[]` | `[3, 2, 1, 0]` |

**Final `pa × 4`** → `a = [0, 1, 2, 3]` ✓ (10 ops total)

This particular input gives a perfectly descending `b` after the push phase. In general it won't — that's what `in-chunk sort` addresses.

### The four names from the subject

| Name | General concept | Why it lands at O(n√n) | In push_swap |
| --- | --- | --- | --- |
| **Chunk-based sorting** | The push_swap label. Process chunks of values. | √n chunks × O(n) per chunk | Main approach |
| **Block-based partitioning** | General technique: split data into blocks, process per block. Used in external sort, BlockQuicksort, cache optimization. | Same `k × (n/k)²` math when blocks are value-based | Equivalent to chunk sort |
| **Bucket Sort with √n buckets** | Classical algorithm: distribute into `k` buckets, sort each (insertion), concatenate. | `O(n + n²/k)`; `k = √n` minimizes → `O(n√n)` | Equivalent to chunk sort |
| **Range-based sorting** | Umbrella term for non-comparison sorts that use value ranges (Counting, Radix, Bucket, …) | Algorithm-dependent | Chunk sort is one instance |

#### Why they collapse in push_swap

- **No random-position access**: only the top of each stack is reachable, so position-based blocks (Block-based) don't help. Value-based remains the only useful axis.
- **No separate per-bucket arrays**: "sort each bucket independently and concatenate" reduces to "interleave with the same stack `b`".
- **Same complexity math**: all four hit O(n√n) because `k buckets × (n/k)² per bucket = n²/k`, minimized at `k = √n`.

The takeaway: the four names are different *thinking lenses* over the same push_swap mechanic. Useful for defense ("we considered all four; they reduce to chunk sort in this operation model") but not for picking a different algorithm.

### Why **`k = √n`** is the optimal bucket count (worth memorizing)

| k (buckets) | Distribution | Per-bucket sort | Total |
| ---: | --- | --- | --- |
| 1 (no buckets) | O(n) | O(n²) | **O(n²)** |
| **√n** | O(n) | O(n × √n) | **O(n√n)** ← optimal |
| n (one per element) | O(n) | O(n × 1) | (degenerates; loses the partitioning meaning) |

`n²/k` is minimized when distribution cost and per-bucket sort cost balance. Setting `k = √n` makes the two terms equal.

### In-chunk sort — keeping `b` ordered as each element arrives

The basic chunk sort (above) doesn't sort *within* a chunk. To squeeze better operation counts, we sort each new element into `b` at the right position when we push it.

#### The challenge

`pb` only places at the top of `b`. To put a smaller value below an existing larger one in the same chunk, we need rotation.

#### The rule (identical to Insertion Sort)

> Before `pb`, rotate `b` (`rb`/`rrb`) so that **the largest element in `b` smaller than `x`** (or `max(b)` if no such element) sits at `b`'s top. Then `pb`.

This keeps `b` in a **circular descending** form (one wrap point), exactly like Insertion Sort.

#### Why it stays O(n√n)

The key constraint: since `x` belongs to chunk `c`, the position it needs to slot into is among **chunk `c`'s elements in `b`**. There are at most √n of them. So the rotation depth per push is bounded by √n.

```
n pushes × O(√n) rotations per push = O(n√n) ✓
```

Compare to pure Insertion Sort, where the rotation depth is bounded by `n`, giving O(n²).

#### Why cross-chunk order is preserved despite rotating all of `b`

`rb` / `rrb` rotates **all of `b`** — but it's a *cyclic* shift. The relative circular order between elements never changes; only the "top pointer" moves. Once we canonicalize at the end (`max` to top), `pa × n` gives a sorted `a`.

#### Alternative: the `rb`-trick (simpler but approximate)

```
for each element x of chunk c:
    pb
    if x is in the lower half of chunk c:
        rb     # send x toward the bottom of b
```

- ✅ Implementation reduces to one `if`
- ❌ `b` isn't perfectly sorted, so the final `pa` phase needs corrections
- ❌ Operation count is harder to predict

The insertion-style approach (above) is easier to reason about and easier to defend.

### Comparison table (variants of Chunk Sort)

| Variant | n=100 (approx) | n=500 (approx) | Implementation | Picked? |
| --- | ---: | ---: | --- | :---: |
| Basic chunk sort | ~1,500 | ~14,000 | Medium | △ (n=500 borderline) |
| + direction-aware `ra`/`rra` | ~1,000 | ~10,000 | Medium | ◯ |
| + in-chunk sort (insertion-style) | ~700 | ~7,000 | Medium–Hard | ◎ |
| Bucket / Range variants | similar | similar | Medium | ◯ |

The choice is less *which algorithm* and more *how much optimization to invest*.

### How to decide (questions to bring to the pair meeting)

1. **Medium = Chunk Sort, agreed?** The other three subject names reduce to it in push_swap.
2. **Chunk size policy** — fixed `√n`, or adaptive to `n`?
3. **Optimization depth** — basic only, direction-aware, or with in-chunk sort?
4. **Coordinate compression** — needed for Medium even if Complex doesn't use Radix?

The README must justify the choice. Implementation effort vs benchmark headroom is the main trade-off to articulate.

---

## 日本語

Medium tier は実質 **Chunk Sort 一族**。subject には 4 つの候補(Chunk-based、Block-based partitioning、Bucket Sort with √n buckets、Range-based)が列挙されているが、push_swap ではすべて同じアプローチに収束する。

### 共通の発想

**値の範囲を √n 個のチャンクに分割し、チャンク単位で処理する**。各チャンクの処理は a を 1 周(`O(n)`)。√n 個のチャンク × O(n) = **O(n√n)**。

### 座標圧縮との相性

値を `0..n-1` に圧縮しておくと、チャンク境界が `floor(値 / chunk_size)` で素直に計算できる。Complex で Radix を選ばなくても、**Medium だけのために座標圧縮を入れる価値がある**。

### 操作数の感覚

| n | √n | O(n√n) 理論ベース | 実装でよくある実測 |
| ---: | ---: | ---: | --- |
| 100 | 10 | 約 1,000 | 約 700〜1,500 |
| 500 | 22 | 約 11,000 | 約 5,000〜14,000 |
| 1000 | 32 | 約 32,000 | 約 15,000〜40,000 |

n=500 のベンチライン(<12,000 / <8,000 / <5,500)はちょうどこのレンジに収まる。**「合格ラインギリギリ」になるか「excellent」になるかは、別アルゴリズム選択ではなく実装の質で決まる**。

### ① Chunk Sort — 主役

#### 基本アルゴリズム

1. 値を `0..n-1` に圧縮
2. チャンクサイズ `c ≈ √n` を決める
3. **低い値のチャンクから順に**処理:
   - a を 1 周。top が現在のチャンクに属すれば `pb`、そうでなければ `ra`
4. 全チャンク処理後、b はおおむね降順(高チャンクが top、低チャンクが bot)
5. `pa × n` で a に戻す(必要なら微調整)

#### 小さなトレース(n=4, chunk_size=2)

入力:`a = [2, 0, 3, 1]`(top→bot)。チャンク 0 = {0, 1}、チャンク 1 = {2, 3}

**Pass 1 — チャンク 0:**

| op | a の状態 | b の状態 |
| --- | --- | --- |
| `ra`(2 はチャンク 1) | `[0, 3, 1, 2]` | `[]` |
| `pb`(0 をチャンク 0 として) | `[3, 1, 2]` | `[0]` |
| `ra`(3 はチャンク 1) | `[1, 2, 3]` | `[0]` |
| `pb`(1 をチャンク 0 として) | `[2, 3]` | `[1, 0]` |

**Pass 2 — チャンク 1:**

| op | a の状態 | b の状態 |
| --- | --- | --- |
| `pb` | `[3]` | `[2, 1, 0]` |
| `pb` | `[]` | `[3, 2, 1, 0]` |

**最後の `pa × 4`** → `a = [0, 1, 2, 3]` ✓(合計 10 操作)

この入力は push 順が偶然うまく行って b が完全降順になったケース。**一般の入力ではそうならない** —— それを補うのが in-chunk sort。

### subject の 4 つの名前

| 名称 | 一般概念 | O(n√n) になる根拠 | push_swap での扱い |
| --- | --- | --- | --- |
| **Chunk-based sorting** | push_swap の用語。値の chunk で処理 | √n チャンク × O(n)/チャンク | 本命 |
| **Block-based partitioning** | データを「block」に分割して処理する一般技法。外部ソート、BlockQuicksort、キャッシュ最適化 | block が値ベースなら `k × (n/k)²` で同じ算術 | Chunk Sort と等価 |
| **Bucket Sort with √n buckets** | 古典算法:`k` 個のバケットに振り分け → 各 bucket を insertion sort → 連結 | `O(n + n²/k)`、`k = √n` で最小化 → O(n√n) | Chunk Sort と等価 |
| **Range-based sorting** | 値域を活用する非比較ソートの総称(Counting、Radix、Bucket など) | 算法依存 | Chunk Sort はその 1 例 |

#### push_swap で収束する理由

- **位置ベースアクセス不可**:top しか触れないので、位置ベースの block(Block-based)は意味を持たない。残るは値ベース。
- **バケットごとの独立配列がない**:「各 bucket を個別ソート → 連結」は「同じスタック b に積み重ねる」に縮退する。
- **計算量算術が同じ**:4 つすべて `k buckets × (n/k)² per bucket = n²/k` を最小化する `k = √n` で O(n√n)。

要点:4 つの名前は同じ push_swap メカニックを**異なる思考レンズ**で見ているだけ。ディフェンスで「4 つすべて検討した上で操作モデル的に同じ Chunk Sort に収束する」と答えるためには有用だが、別アルゴリズムを選ぶ材料にはならない。

### なぜ **`k = √n`** が最適バケット数か(押さえておきたい)

| k(バケット数) | 振り分けコスト | 各バケットソート合計 | 全体 |
| ---: | --- | --- | --- |
| 1(バケットなし) | O(n) | O(n²) | **O(n²)** |
| **√n** | O(n) | O(n × √n) | **O(n√n)** ← 最適 |
| n(各要素 1 バケット) | O(n) | O(n × 1) | (前提崩れる) |

`n²/k` は振り分けコストと個別ソートコストが釣り合う点で最小化される。`k = √n` で両項が均衡する。

### In-chunk sort — push 時に b を整列状態に保つ

basic Chunk Sort は chunk 内の整列をしない。操作数を減らすには、**各 push の時点で正しい位置に挿入する**。

#### 課題

`pb` は b の top にしか置けない。同じ chunk 内で既存の大きい値の下に新しい小さい値を入れたい場合、回転が必要。

#### ルール(Insertion Sort と同じ)

> `pb` の前に、b の中で **`x` より小さい最大の要素**(無ければ `max(b)`)が b の top に来るように `rb`/`rrb` で回転 → `pb`

これで b は「**円環的降順**」(ラップ点 1 つ)を保つ。

#### なぜ O(n√n) のままなのか

カギ:`x` が chunk `c` に属している以上、挿入位置は **chunk `c` の他要素**の間にある。chunk `c` の要素は最大 √n 個。よって 1 push 当たりの回転深さは √n に bounded。

```
n pushes × O(√n) 回転 / push = O(n√n) ✓
```

Pure Insertion Sort は回転深さが `n` に bounded で O(n²) になる。**chunk 構造による探索範囲の制限**が O(n√n) の根拠。

#### cross-chunk 順序が壊れない理由

`rb` / `rrb` は b **全体**を回す ——だが**巡回シフト**。要素間の循環的な相対順序は不変で、変わるのは「top ポインタの位置」だけ。最後に canonical 化(max を top に)すれば、`pa × n` で正しく取り出せる。

#### 代替:`rb`-trick(簡単だが近似的)

```
chunk c の各要素 x について:
    pb
    if x が chunk c の下半分(値が小さい):
        rb     # x を b の底側へ送る
```

- ✅ 実装が if 文 1 つ
- ❌ b が完全整列しない。最後の `pa` 段で補正が必要
- ❌ 操作数の見積もりがブレる

Insertion 流のほうが**論理を追跡しやすく、ディフェンスでも説明しやすい**。

### 比較表(Chunk Sort のバリアント)

| バリアント | n=100 概算 | n=500 概算 | 実装難度 | 採用度 |
| --- | ---: | ---: | --- | :---: |
| Basic chunk sort | 約 1,500 | 約 14,000 | 中 | △(n=500 でライン微妙) |
| + direction-aware `ra`/`rra` | 約 1,000 | 約 10,000 | 中 | ◯ |
| + in-chunk sort(insertion 流) | 約 700 | 約 7,000 | 中〜難 | ◎ |
| Bucket / Range variants | 同等 | 同等 | 中 | ◯ |

選択肢は「**どのアルゴリズム**」より「**どこまで最適化するか**」。

### 選び方の指針(相方との議論)

1. **Medium = Chunk Sort で合意できるか?** subject の他 3 つは push_swap で同じものに収束する
2. **チャンクサイズ方針** — 固定 `√n` か、`n` に応じて適応的にするか
3. **最適化レベル** — basic で出すか、direction-aware + in-chunk sort まで詰めるか
4. **座標圧縮** — Complex が Radix でなくても Medium のために入れるか

採用根拠は README に書き、ディフェンスで説明できる状態にする。実装コストとベンチ余裕のトレードオフを言語化できることが鍵。

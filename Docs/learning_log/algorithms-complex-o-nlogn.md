# Complex O(n log n) Algorithms / Complex O(n log n) のアルゴリズム

## English

The Complex tier is the only tier that can realistically hit excellent benchmarks (n=500 < 5,500 ops). The subject lists five candidates; in push_swap's operation model, **Radix Sort dominates** and the others are either complex, not competitive, or don't apply.

### Common idea

Use either **divide-and-conquer** or **digit-based bucketing** to bring the work down to `n × log n`. In push_swap:

- n=100 theoretical best: ~1,400 ops
- n=500 theoretical best: ~9,000 ops; well-tuned implementations reach **~4,500–6,000**
- The **only tier** that can land in `excellent` territory

### Conclusion preview

| Candidate | Practical in push_swap? | One-liner |
| --- | :---: | --- |
| ① Radix Sort | ◎ **default choice** | Best match for the instruction set; simple to implement |
| ② Merge Sort (2-stack) | △ | Concept is clean but the merge phase is painful |
| ③ Quick Sort (stack partitioning) | ◯ | Workable; more complex than Radix |
| ④ Heap Sort | ✗ | Stack structure doesn't accommodate a heap |
| ⑤ Binary Indexed Tree | ✗ | Not a sorting algorithm; a data structure |

---

### ① Radix Sort — the default

#### General concept

A **non-comparison** sort that buckets elements by digit (or bit) value.

- **LSD** (Least Significant Digit): process from the lowest digit upward; stable per pass
- **MSD** (Most Significant Digit): process from the highest digit; recursive partitioning

#### push_swap adaptation (LSD, binary)

Coordinate-compress values to `0..n-1`. Since the value range is exactly `n`, the bit count is `⌈log₂(n)⌉`.

For each bit:
```
for i = 0 to n - 1:                # one pass over a's current elements
    if the current bit of a's top == 0:
        pb                         # 0-bit → send to b
    else:
        ra                         # 1-bit → rotate to back of a

while b not empty:                 # bring b back
    pa
```

After `log₂(n)` passes, `a` is sorted ascending.

#### Small trace (n=4)

Input: `a = [3, 0, 2, 1]` (values 0..3, 2 bits needed).

**Bit 0 (LSD):**
| a's top | binary | bit 0 | op |
| ---: | --- | ---: | --- |
| 3 | `11` | 1 | `ra` |
| 0 | `00` | 0 | `pb` → b=[0] |
| 2 | `10` | 0 | `pb` → b=[2, 0] |
| 1 | `01` | 1 | `ra` |

`pa × 2` → `a = [0, 2, 3, 1]`

**Bit 1:**
| a's top | binary | bit 1 | op |
| ---: | --- | ---: | --- |
| 0 | `00` | 0 | `pb` → b=[0] |
| 2 | `10` | 1 | `ra` |
| 3 | `11` | 1 | `ra` |
| 1 | `01` | 0 | `pb` → b=[1, 0] |

`pa × 2` → `a = [0, 1, 2, 3]` ✓

**Total**: ~12 ops for n=4.

#### Operation count

- Per bit round: ~`2n` ops (distribute + bring back)
- `log₂(n)` rounds
- **Total**: `2n × log₂(n) = O(n log n)`

| n | log₂(n) | Theoretical | Typical implementation |
| ---: | ---: | ---: | ---: |
| 100 | 7 | ~1,400 | ~600–700 |
| 500 | 9 | ~9,000 | ~4,500–6,000 |

#### Why Radix and push_swap are a perfect match

| What Radix needs | What push_swap offers natively |
| --- | --- |
| "If bit is 0, group A; if bit is 1, group B" | `pb` (to other stack) / `ra` (to back of same stack) |
| "Process the two groups in order" | `pa` (pop the group out) |
| "Stable sort within each pass" | `ra` + `pa` preserve relative order naturally |

The instruction set looks designed for Radix.

#### LSD vs MSD

| Aspect | LSD | MSD |
| --- | --- | --- |
| Implementation | Simple (one loop) | Recursive (more complex) |
| Operation count | Predictable | Depends on balance |
| Adoption in push_swap | **Dominant** | Rare |

→ LSD is essentially the only choice.

#### The defendable complexity argument (memorize this)

This is what subject VI.3.3 requires in the README and will be asked at peer-eval:

> "**Coordinate compression fixes the value range to `[0, n)`**, so the required bit count is `⌈log₂(n)⌉`. Each bit round performs `pb` / `ra` over n elements + `pa` back over n elements = O(n) push_swap operations. With `log₂(n)` rounds, the **total is O(n log n)** push_swap operations."

Without coordinate compression, bit count = `log₂(INT_MAX)` = 31, fixed → `O(31n) = O(n)` formally, but the constant is too large to pass benchmarks. **Coordinate compression is what makes Radix viable.**

---

### ② Merge Sort (2-stack version)

#### General concept

Divide-and-conquer: split the array in half, recursively sort each half, then merge two sorted halves.

#### push_swap adaptation

- "Split" = push half of `a` to `b`
- "Sort" = recurse on each half
- "Merge" = compare tops of `a` and `b`, take the smaller, repeat

#### Why it's hard

The **merge phase is the bottleneck**. Merging two sorted stacks into one ascending order requires a third area to land the merged result — but push_swap only has two stacks. Routing around this with cyclic rotations is doable but burns operations.

#### Operation count

- Theoretically O(n log n), but with large constants
- Typically ~8,000–15,000 ops at n=500 (more than double Radix)

#### Verdict

- ✅ Concept is textbook; easy to explain
- ❌ Implementation complexity (recursion + merge)
- ❌ Operation count loses to Radix
- Little reason to pick over Radix

---

### ③ Quick Sort (stack partitioning)

#### General concept

- Pick a pivot
- Partition: elements smaller than pivot to one side, larger to the other
- Recurse on each partition

Average O(n log n), worst O(n²).

#### push_swap adaptation

- Pivot: with coordinate compression, the natural choice is the median of the current range
- Partition: walk `a` once, `pb` if smaller than pivot, `ra` if larger
- Recurse on each partition (the new `b` set and the remaining `a` set)

#### Structure

```
quicksort_a(size):
    if size <= 5: mini_sort       # base case
    pivot = median of current range
    partition_count = 0
    for i in range(size):
        if a's top < pivot:
            pb
            partition_count++
        else:
            ra
    quicksort_b(partition_count)        # recurse on b side
    quicksort_a(size - partition_count) # recurse on a side
```

#### Operation count

- Average O(n log n), comparable to Radix
- Typically ~5,000–8,000 ops at n=500

#### Verdict

- ✅ Second-best practical option
- ✅ Doesn't strictly need coordinate compression (just pivot selection)
- ❌ Pivot strategy heavily affects performance
- ❌ Recursion management adds complexity
- ❌ Worst case O(n²) is a risk
- Generally not picked unless there's a reason to avoid Radix

---

### ④ Heap Sort

#### General concept

- Build a max-heap (or min-heap) from the data
- Extract max (or min) repeatedly

#### Why it doesn't work for push_swap

A heap requires the **complete binary tree** structure with random access (or its array equivalent). push_swap has:
- No random access (only the top is reachable)
- No natural way to encode a binary tree on a stack

Simulating sift-down with rotations costs O(n) operations per operation, degrading the overall complexity to O(n²).

#### Verdict

- ❌ Cannot achieve O(n log n) in the push_swap operation model — the claimed complexity collapses
- ❌ Implementation is extremely complex
- Listed in the subject for completeness; not practically chosen

---

### ⑤ Binary Indexed Tree (BIT / Fenwick Tree)

#### Reality check

BIT is **not a sorting algorithm**. It's a data structure for fast prefix-sum queries and point updates (O(log n) each).

#### Possible role in push_swap

BIT could accelerate **C-side computation** — e.g., computing `disorder` in O(n log n), or ranking elements quickly. But the subject (VI.3.1) measures complexity in **push_swap operations generated**, not C-side work. So:

- BIT alone is not a Complex-tier algorithm
- A "BIT-augmented" version of Radix or Quick is conceivable but unusual

#### Verdict

- ❌ Cannot serve as the main Complex strategy
- Could appear as auxiliary structure, but rarely used in practice

---

### Comparison table

| Candidate | n=100 (approx) | n=500 (approx) | Implementation | Can hit excellent (<5,500)? | Picked? |
| --- | ---: | ---: | --- | :---: | :---: |
| **Radix Sort (LSD)** | ~600–700 | ~4,500–6,000 | Medium | ✅ | ◎ |
| Merge Sort | ~1,200+ | ~10,000+ | High | ✗ | △ |
| Quick Sort | ~700–1,000 | ~5,000–8,000 | Medium–High | △ (depends) | ◯ |
| Heap Sort | algorithm collapses | — | Very high | ✗ | ✗ |
| BIT (auxiliary only) | — | — | — | — | ✗ (cannot stand alone) |

---

### How to decide (questions to bring to the pair meeting)

1. **Complex = Radix LSD, agreed?** (Matches the skeleton's proposal.)
2. If Radix → **coordinate compression must be implemented** (none of the other options strictly require it).
3. Any strong reason to avoid Radix?
4. Can both partners write the **O(n log n) argument** for the README defensibly?

### The Complex answer to "Three whys"

For the kickoff's third "why" — "what input pre-processing does Complex require?":

> "Radix Sort's complexity is determined by the value range's bit count. With raw integers (INT_MIN..INT_MAX), the bit count is fixed at 32 with a large constant. **Coordinate compression fixes the value range to `[0, n)`, so the bit count becomes `log₂(n)`, making O(n log n) hold.**"

This is the defense answer verbatim.

---

## 日本語

Complex tier は excellent ベンチ(n=500 < 5,500 ops)を現実的に狙える唯一の tier。subject は 5 つの候補を挙げているが、push_swap の操作モデル上は **Radix Sort が支配的**で、他は複雑か実用性に欠けるか、そもそも単体で成立しない。

### 共通の発想

**分割統治** か **桁ベースの振り分け** で作業量を `n × log n` に抑える。push_swap では:

- n=100 理論最良:約 1,400 ops
- n=500 理論最良:約 9,000 ops。よく実装すれば **約 4,500〜6,000**
- excellent 圏に届く **唯一の tier**

### 結論先取り

| 候補 | push_swap での実用性 | 一言 |
| --- | :---: | --- |
| ① Radix Sort | ◎ **本命** | 命令セットと相性最良。実装シンプル |
| ② Merge Sort(2-stack) | △ | 概念は美しいがマージ phase が複雑 |
| ③ Quick Sort(stack partitioning) | ○ | 実装可能、Radix より複雑 |
| ④ Heap Sort | ✗ | スタック構造と heap が噛み合わない |
| ⑤ Binary Indexed Tree | ✗ | そもそも算法ではなくデータ構造 |

---

### ① Radix Sort — 本命

#### 一般概念

**比較せず、桁(または bit)で要素を振り分けて並べる**非比較ソート。

- **LSD**(Least Significant Digit):**下位桁から**処理 → 各桁で安定ソートして積み重ね
- **MSD**(Most Significant Digit):**上位桁から**処理 → 上位で分けたあと、各グループを再帰的に下位で分ける

#### push_swap 適応(LSD + 2 進)

座標圧縮で値を `0..n-1` にしておく。値域がちょうど `n` なので、必要 bit 数は `⌈log₂(n)⌉`。

各 bit について:
```
for i = 0 to n - 1:                # a の全要素を 1 周
    if a's top の現在 bit == 0:
        pb                          # 0 は b へ
    else:
        ra                          # 1 は a の末尾へ回す

while b not empty:                  # b を a に戻す
    pa
```

これを `log₂(n)` 回繰り返すと、a がソート済みになる。

#### 小さなトレース(n=4)

入力:`a = [3, 0, 2, 1]`(値 0..3、2 bit 必要)

**bit 0(LSD):**
| a's top | 2 進表記 | bit 0 | op |
| ---: | --- | ---: | --- |
| 3 | `11` | 1 | `ra` |
| 0 | `00` | 0 | `pb` → b=[0] |
| 2 | `10` | 0 | `pb` → b=[2, 0] |
| 1 | `01` | 1 | `ra` |

`pa × 2` → `a = [0, 2, 3, 1]`

**bit 1:**
| a's top | 2 進表記 | bit 1 | op |
| ---: | --- | ---: | --- |
| 0 | `00` | 0 | `pb` → b=[0] |
| 2 | `10` | 1 | `ra` |
| 3 | `11` | 1 | `ra` |
| 1 | `01` | 0 | `pb` → b=[1, 0] |

`pa × 2` → `a = [0, 1, 2, 3]` ✓

**合計**:約 12 ops(n=4)。

#### 操作数の理論

- 各 bit ラウンド ≈ `2n` ops(振り分け + pa 戻し)
- `log₂(n)` ラウンド
- **合計**:`2n × log₂(n) = O(n log n)`

| n | log₂(n) | 理論値 | 実装での実測 |
| ---: | ---: | ---: | ---: |
| 100 | 7 | 約 1,400 | 約 600〜700 |
| 500 | 9 | 約 9,000 | 約 4,500〜6,000 |

#### なぜ Radix が push_swap と相性最良か

| Radix が求めるもの | push_swap が標準で持つもの |
| --- | --- |
| 「bit が 0 なら片方、1 なら片方に分ける」 | `pb`(別スタックへ)/ `ra`(同スタックの末尾へ) |
| 「分けた 2 グループを順に処理」 | `pa`(戻す) |
| 「各 pass で安定ソート」 | `ra` + `pa` の自然な挙動で保たれる |

設計者が Radix 用に命令セットを作ったかのよう。

#### LSD vs MSD

| 観点 | LSD | MSD |
| --- | --- | --- |
| 実装 | シンプル(ループ 1 つ) | 再帰(複雑) |
| 操作数 | 予測しやすい | バランス次第 |
| push_swap での採用率 | **圧倒的に多い** | 稀 |

→ push_swap では実質 LSD 一択。

#### ディフェンスで使う計算量議論(暗記推奨)

subject VI.3.3 が README に求める計算量議論の核心。**ピア評価で必ず突かれる**:

> 「**座標圧縮で値域を `[0, n)` に固定**するため、必要 bit 数 = `⌈log₂(n)⌉`。各 bit ラウンドで `pb`/`ra` を n 回 + `pa` を n 回 = O(n) 操作。これを `log₂(n)` ラウンド繰り返すので、**合計 O(n log n)** 操作。」

座標圧縮なしだと bit 数 = `log₂(INT_MAX)` = 31 固定で、O(31n) = O(n) になるが定数が爆発してベンチを通せない。**座標圧縮が Radix を成立させる**。

---

### ② Merge Sort(2-stack 版)

#### 一般概念

**分割統治**:配列を半分に分ける → 各半分を再帰的にソート → ソート済みの 2 つをマージ。

#### push_swap 適応

- 「分割」= a の半分を b に push
- 「ソート」= 各半分を再帰的にソート
- 「マージ」= a と b の top を比較し、小さい方を取り出していく

#### なぜ難しいか

**マージ phase が悪夢**。ソート済みの 2 スタックを 1 つの昇順列にマージするには、結果を置く第三の領域が要る ——push_swap には a と b の 2 つしかない。両者を使い回しながらマージするには複雑な回転戦略が必要で、操作数が膨らむ。

#### 操作数

- 理論的に O(n log n) だが、**定数が大きい**
- n=500 で 8,000〜15,000 ops 程度(Radix の 2 倍以上)

#### 評価

- ✅ 概念は教科書通り、説明しやすい
- ❌ 実装が複雑(再帰 + マージ)
- ❌ 操作数で Radix に劣る
- Radix を選ばない理由がない限り採用しない

---

### ③ Quick Sort(stack partitioning 版)

#### 一般概念

- pivot(基準値)を選ぶ
- pivot より小さい要素と大きい要素に分ける(partition)
- 各サブグループを再帰的にソート

平均 O(n log n)、最悪 O(n²)。

#### push_swap 適応

- pivot:座標圧縮済みなら現在の値域の中央値が自然
- partition:a を 1 周しながら、pivot より小さい → `pb`、大きい → `ra`
- 各サブグループ(b 側、a の残り)を再帰的に処理

#### 構造

```
quicksort_a(size):
    if size <= 5: mini_sort       # base case
    pivot = median of current range
    partition_count = 0
    for i in range(size):
        if a's top < pivot:
            pb
            partition_count++
        else:
            ra
    quicksort_b(partition_count)        # b 側を再帰
    quicksort_a(size - partition_count) # a 側を再帰
```

#### 操作数

- 平均 O(n log n)、Radix と同オーダー
- n=500 で 5,000〜8,000 ops 程度

#### 評価

- ✅ Radix に次ぐ実用性
- ✅ 座標圧縮なしでも動く(pivot 選びだけ工夫すれば OK)
- ❌ pivot 選択戦略次第で性能が大きく変わる
- ❌ 再帰管理が必要
- ❌ 最悪 O(n²) のリスク
- Radix を避ける強い理由がない限り選ばない

---

### ④ Heap Sort

#### 一般概念

- データから max-heap(または min-heap)を構築
- top(最大値)を取り出す → heap を再構築 → 繰り返し

#### push_swap で成立しない理由

Heap は**完全二分木**としてランダムアクセス可能な配列を前提とする構造。push_swap には:
- ランダムアクセスがない(top しか触れない)
- 二分木構造を表現する自然な手段がない

sift-down を回転で模倣しようとすると 1 操作が O(n) になり、全体が O(n²) に劣化する。

#### 評価

- ❌ push_swap 操作モデルで O(n log n) を**実現できない**(主張する計算量が成立しない)
- ❌ 実装が極めて複雑
- 候補リストに名前があるだけで、事実上選ばれない

---

### ⑤ Binary Indexed Tree(BIT / Fenwick Tree)

#### そもそも

BIT は**ソートアルゴリズムではない**。「累積和の取得と要素更新を共に O(log n) で行う」**データ構造**。

#### push_swap での役割

BIT を使うと **C プログラム側で**「次にどの要素を動かすか」を高速判断できる(例:disorder を O(n log n) で計算)。**ただし subject(VI.3.1)は計算量を C 側ではなく Push_swap 操作数で測る**。よって:

- BIT 単体は Complex tier の算法ではない
- 「BIT 補助つき Radix」「BIT 補助つき Quick」のような形では使えるが、稀

#### 評価

- ❌ Complex の主軸戦略にはなれない
- 補助構造としてはあり得るが実装例は少ない

---

### 比較表

| 候補 | n=100 概算 | n=500 概算 | 実装難度 | excellent (<5,500) 狙えるか | 採用度 |
| --- | ---: | ---: | --- | :---: | :---: |
| **Radix Sort (LSD)** | 約 600〜700 | 約 4,500〜6,000 | 中 | ✅ | ◎ |
| Merge Sort | 約 1,200+ | 約 10,000+ | 高 | ✗ | △ |
| Quick Sort | 約 700〜1,000 | 約 5,000〜8,000 | 中〜高 | △(条件次第) | ○ |
| Heap Sort | 算法成立せず | — | 極高 | ✗ | ✗ |
| BIT(補助のみ) | — | — | — | — | ✗(単体不可) |

---

### 選び方の指針(相方との議論)

1. **Complex = Radix LSD で合意できるか?**(スケルトンの提案でもある)
2. Radix を採用するなら **座標圧縮の実装は必須**(他の候補では不要)
3. Radix を避ける強い理由はあるか?
4. 両者が **O(n log n) の議論**を README で説明できる状態にできるか

### 「3 つのなぜ」の Complex 答え

kickoff の 3 つ目の「なぜ」=「Complex に入力前処理がなぜ必要か」に対する回答:

> 「Radix Sort の計算量は値域の bit 数で決まる。素の整数(INT_MIN..INT_MAX)なら 32 bit 固定で定数が爆発する。**座標圧縮で値域を `[0, n)` に揃えれば bit 数 = `log₂(n)`** となり、O(n log n) が成立する。」

これがそのままディフェンス回答になる。

# ソートアルゴリズム 動作トレース集(手元用 / ディフェンス用)

各戦略が**実際にどう動くか**を、実プログラムが出力した本物の操作列でステップごとに
トレースしたもの。表記は `A[top ... bottom]  B[top ... bottom]`(左が top)。

> 生成方法:`./push_swap <flag> "<入力>"` の出力を、各操作でスタックを動かすシミュレータに
> 通したもの。数値は入力値そのまま(内部では後述の index に圧縮されているが、動きの理解は
> 値で追える)。

---

## 0. 前提

### 座標圧縮(coordinate compression)

ソート前に、各値を**順位(rank)0..n-1** に置き換える。比較は値でなく index で行う。

```
値:    42  17  99   3   8
index:  3   2   4   0   1     ← 3が最小→0、8→1、17→2、42→3、99→4
```

- 利点:キー範囲が `[0, n)` に固定 → radix が `⌈log₂ n⌉` 回のビットパスで済む / int の大きさに非依存。
- **操作は0個**(ノードの再ラベルのみ)。

### disorder 指標

スタック A の**転倒比率**。ソート前に1回測る。

```
入力 3 1 2 の転倒ペア:
  (3,1) 3>1 → 転倒
  (3,2) 3>2 → 転倒
  (1,2) 1<2 → OK
  転倒数 = 2,  C(3,2) = 3,  disorder = 2/3 = 0.667
```

値域 [0,1]:0=整列済み、1=完全逆順。adaptive はこれでレジームを選ぶ。

---

## 1. 小さい n(n ≤ 5)

サイズが小さいときは disorder より**サイズ優先**で専用処理(`dispatch` 冒頭)。整列済みは
`disorder == 0` で**即終了(0操作)**。

### sort_two(n = 2)

top > bottom なら `sa` するだけ。最大1操作。

```
  init  A[  2   1]  B[-]
  sa    A[  1   2]  B[-]
  (total 1 ops)
```

### sort_three(n = 3)

未整列の5パターンを `sa`/`ra`/`rra` だけで ≤2操作。B を使わない。

```
  init  A[  3   1   2]  B[-]
  ra    A[  1   2   3]  B[-]        ← top(3)を最後尾へ回すだけ
  (total 1 ops)
```

**注目:** B を一切使わず、最大2操作で確定。だから n=3 が ≤5 要件を楽に満たす。

### sort_small(n = 4, 5):最小2個を B へ → 残りを sort_two/three → 戻す

```
  入力 3 1 5 2 4
  init  A[  3   1   5   2   4]  B[-]
  ra    A[  1   5   2   4   3]  B[-]   ┐ 最小(1)を top に回して
  pb    A[  5   2   4   3]  B[  1]     ┘ B へ push
  ra    A[  2   4   3   5]  B[  1]     ┐ 次の最小(2)を top に回して
  pb    A[  4   3   5]  B[  2   1]     ┘ B へ push(B=[2,1] 降順)
  sa    A[  3   4   5]  B[  2   1]     ← A に残る3個(5,4,3→sort_three)を整列
  pa    A[  2   3   4   5]  B[  1]     ┐ B を戻す(2→上、1→上)
  pa    A[  1   2   3   4   5]  B[-]   ┘
  (total 7 ops)
```

**注目:**
- B は `[2, 1]`(降順=大きい方が上)になるよう「最小→次の最小」の順に push。
- 最後の `pa pa` で、2 が先・1 が後に乗るので A が `1 2 ...` の昇順になる。

---

## 2. Simple — B への挿入ソート(O(n²))

### ざっくり:カードを1枚ずつ「正しい場所」に挿していく(挿入ソート)

トランプを手で並べるときと同じ。A の山から1枚ずつ取り、もう一方の山 B の
**正しい位置に挿し込む**。B は常に「上が最大の降順」に保つ。全部 B に入れたら、
上(最大)から配り直す(`pa`)と A が昇順になる。

```
   A(バラバラ)                       B(常に降順:上=最大)
  ┌─────┐                           ┌─────┐
  │  3  │ ← 上から1枚取り出す        │  5  │ 大
  │  1  │                           │  4  │
  │  5  │   B の正しい位置を探し、    │  3  │
  │  2  │   B を回してから pb で挿す  │  2  │
  │  4  │                           │  1  │ 小
  └─────┘                           └─────┘

  全部 B に移すと B は降順 ──→ pa を5回 ──→ A は昇順
      B: 5 4 3 2 1                       A: 1 2 3 4 5
```

**「正しい位置に挿す」とは(B を降順に保つ操作):**

```
  B = [6, 2](降順)に  4  を入れたい
    4 は「2 より大きく 6 より小さい」 → 2 と 6 の間に入れたい
    └ B を回して 2 を上に出す → pb で 4 を 2 の上に載せる
       結果 … 6, 4, 2 … と降順が保たれる
```

**仕組み(コードの言い方):** A から1個ずつ B へ移し、毎回「B 内で自分より小さい最大の
要素のすぐ上」に差し込む(B を循環降順に保つ)。空になったら最大を上に回して `pa` で
全部戻す。

```
  入力 3 1 5 2 4
  init  A[  3   1   5   2   4]  B[-]
  pb    A[  1   5   2   4]  B[  3]            ← 最初の1個
  pb    A[  5   2   4]  B[  1   3]
  rrb   A[  5   2   4]  B[  3   1]            ┐ B を回して差し込み位置を調整
  pb    A[  2   4]  B[  5   3   1]            ┘
  rrb   A[  2   4]  B[  1   5   3]
  pb    A[  4]  B[  2   1   5   3]
  rrb   A[  4]  B[  3   2   1   5]
  pb    A[-]  B[  4   3   2   1   5]
  rrb   A[-]  B[  5   4   3   2   1]          ← B が降順(上=最大)に
  pa    A[  5]  B[  4   3   2   1]            ┐
  pa    A[  4   5]  B[  3   2   1]            │ 最大から順に戻す
  pa    A[  3   4   5]  B[  2   1]            │ → A は昇順
  pa    A[  2   3   4   5]  B[  1]            │
  pa    A[  1   2   3   4   5]  B[-]          ┘
  (total 14 ops)
```

**注目:**
- B を**降順**に保つのが鍵。各 `pb` の前に `rrb`/`rb` で差し込み位置を作る。
- B が降順なら `pa` 連打で A は昇順。
- 各要素で B を走査・回転 → 全体 O(n²)。低 disorder(ほぼ整列)では実移動が少なく安い。

---

## 3. Medium — チャンクソート(O(n√n))

### ざっくり:まず「大まかな帯」に分けてから運ぶ

Simple は1個ずつ B 全体を探して回す → 回転が多い。Medium は「**大まかな帯
(チャンク)に分けて運ぶ**」ことで回転を減らす。本を著者順に並べる前に、まず
「あ〜お」「か〜こ」… の棚に大まかに分けるイメージ。

```
  index 範囲 [0, n) を 幅≈√n のチャンクに分割:

   ┌── chunk0 ──┬── chunk1 ──┬── chunk2 ──┐
   │  0  1  2   │  3  4  5   │  6  7  8   │   ← index(=順位)
   └────────────┴────────────┴────────────┘
        小                          大

  処理は「下の帯から順に」:
    1) A を見て chunk0 の値(index 0,1,2)を B へ送る
    2) 次に chunk1、chunk2 … と送る
  → B は「小さい帯が上、大きい帯が下」に粗く積み上がる
  → 最後に pa 連打で A へ(A 昇順)
```

**なぜ Simple より安い?**

```
  Simple : 1個ごとに B 全体を探索+回転        → 1個あたり最大 O(n)
  Medium : 帯(幅≈√n)の中だけ気にすればよい  → 1個あたり ≈√n

  「1個あたり ≈√n」×「帯の数 ≈ n/√n」 を釣り合わせて  →  O(n√n)
  幅を √n にするのが両者のバランスが最小になる点(だから 4/15/30/45 ≈ √n)
```

**仕組み(コードの言い方):** 順位範囲を幅 ≈√n のチャンクに分割。チャンクを昇順に
処理し、各メンバーを A の top に(最短回転で)持ってきて、B が循環降順を保つ位置に
回して `pb`。最後に最大を上に回して戻す。

```
  入力 5 2 8 1 6 3   (n=6, chunk幅≈2-3)
  init  A[  5   2   8   1   6   3]  B[-]
  pb    A[  2   8   1   6   3]  B[  5]
  pb    A[  8   1   6   3]  B[  2   5]
  ra    A[  1   6   3   8]  B[  2   5]        ← A を回して次の対象を top へ
  rb    A[  1   6   3   8]  B[  5   2]        ← B を回して差し込み位置へ
  pb    A[  6   3   8]  B[  1   5   2]
  ra    A[  3   8   6]  B[  1   5   2]
  rrb   A[  3   8   6]  B[  2   1   5]
  pb    A[  8   6]  B[  3   2   1   5]
  rrb   A[  8   6]  B[  5   3   2   1]
  pb    A[  6]  B[  8   5   3   2   1]
  rb    A[  6]  B[  5   3   2   1   8]
  pb    A[-]  B[  6   5   3   2   1   8]
  rrb   A[-]  B[  8   6   5   3   2   1]      ← B が降順に
  pa    A[  8]  B[  6   5   3   2   1]        ┐
  pa    A[  6   8]  B[  5   3   2   1]        │ 戻す
  pa    A[  5   6   8]  B[  3   2   1]        │
  pa    A[  3   5   6   8]  B[  2   1]        │
  pa    A[  2   3   5   6   8]  B[  1]        │
  pa    A[  1   2   3   5   6   8]  B[-]      ┘
  (total 19 ops)
```

**注目:**
- Simple との違い:**A 側も最短方向に回して(ra/rra)**対象を効率よく top に出す。
- 回転を「上半分=正回転 / 下半分=逆回転」で短い方を選ぶ → 操作数を抑える。
- 幅≈√n が「要素ごとの回転(≈√n)」×「チャンク数(n/√n)」を釣り合わせ O(n√n)。

---

## 4. Complex — 2進 LSD 基数ソート(O(n log n))

### ざっくり:2進の桁ごとに「0の山・1の山」に分けるのを繰り返す

郵便番号で仕分けるように、**桁ごとに分けて積み直す**のを下の桁から繰り返すと、
自然に並ぶ。ここでは index を2進で見て、各ビットで「0 のグループ」「1 のグループ」に
分ける。

```
  index を2進で見る(n=4, 2ビット):

     値    index   b1 b0
      1      0      0  0
      2      1      0  1
      3      2      1  0
      4      3      1  1

  パス0 = 一番下のビット b0 で分ける:
     b0=0 のもの ┐          b0=1 のもの ┐
       index 0,2 │ → B へ      index 1,3 │ → A に残す
       (値 1,3) ┘            (値 2,4)  ┘
     → 戻すと  [1,3 | 2,4]  (b0 で安定に分かれた)

  パス1 = 次のビット b1 で分ける(同じ操作)
     → [1,2,3,4]  完成!

  パス数 = ビット数 = ⌈log₂ n⌉
```

**ポイント:** 下の桁(LSB)から順に「安定に」分けるのが肝。各パスは1ビットの仕分け
だけ(O(n))、それを log 回 → **O(n log n)**。A 内では `pb`(0をBへ)/ `ra`(1を残す)で
仕分け、パス末に `pa` で全部戻す。

```
   1パスのイメージ(A を上から1周):
     top のビット = 0  →  pb  (B へ送る)
     top のビット = 1  →  ra  (A の最後尾へ回す=残す)
   1周したら B を pa で全部 A に戻す → 次のビットへ
```

**仕組み(コードの言い方):** index を**最下位ビットから**順に見る。各ビットで A を走査:
そのビットが **0 なら `pb`(B へ)、1 なら `ra`(A に残す)**。1パス終わったら `pa` で全部
戻す。`⌈log₂ n⌉` パスで整列。

```
  入力 3 1 4 2   (index 2 0 3 1, 2ビット)
  ── パス0(bit0)── bit0: 3→0, 1→0, 4→1, 2→1
  init  A[  3   1   4   2]  B[-]
  pb    A[  1   4   2]  B[  3]      ← 3 bit0=0 → pb
  pb    A[  4   2]  B[  1   3]      ← 1 bit0=0 → pb
  ra    A[  2   4]  B[  1   3]      ← 4 bit0=1 → ra
  ra    A[  4   2]  B[  1   3]      ← 2 bit0=1 → ra
  pa    A[  1   4   2]  B[  3]      ┐ 戻す
  pa    A[  3   1   4   2]  B[-]    ┘ → bit0 で安定分割済(3,1|4,2)
  ── パス1(bit1)── bit1: 3→1, 1→0, 4→1, 2→0
  ra    A[  1   4   2   3]  B[-]    ← 3 bit1=1 → ra
  pb    A[  4   2   3]  B[  1]      ← 1 bit1=0 → pb
  ra    A[  2   3   4]  B[  1]      ← 4 bit1=1 → ra
  pb    A[  3   4]  B[  2   1]      ← 2 bit1=0 → pb
  pa    A[  2   3   4]  B[  1]      ┐ 戻す
  pa    A[  1   2   3   4]  B[-]    ┘ → 整列完了
  (total 12 ops)
```

**注目:**
- 各パス = 1ビットでの**安定分割**(0 を先に集める)。LSB から MSB へ。
- パス数 = `⌈log₂ n⌉`、各パス O(n) → 全体 **O(n log n)**。
- 大きい例:`5 2 8 1 6 3 7 4`(n=8, 3ビット)→ **3パス・36操作**でちょうど整列。
  n が増えてもパス数が log でしか増えないので、高 disorder/大 n で強い。

---

## 5. Adaptive — disorder で戦略を選ぶ

`--adaptive`(既定)では、ソート前に disorder を測り、subject 規定のレジームに対応する
tier を選ぶ(n≥6。n≤5 はサイズ優先で小n処理、整列済みは0操作)。

| disorder | レジーム | 採用 | ラベル |
|---|---|---|---|
| < 0.2 | 低 | Simple | `Adaptive / O(n²)` |
| 0.2 ≤ d < 0.5 | 中 | Medium | `Adaptive / O(n√n)` |
| ≥ 0.5 | 高 | Complex | `Adaptive / O(n log n)` |

実例(`--bench` で測定):

```
  入力 '1 2 3 4 5 7 6'       disorder:  4.76%  → Simple   (Adaptive / O(n²))      16 ops
  入力 '6 2 9 1 7 3 10 4 8 5' disorder: 42.22%  → Medium   (Adaptive / O(n√n))     30 ops
  入力 '8 7 6 5 4 3 2 1'      disorder:100.00%  → Complex  (Adaptive / O(n log n)) 36 ops
```

**注目(ディフェンスで聞かれやすい):**
- 「ほぼ整列(低 disorder)」は転倒が少ないので **O(n²) の Simple が実移動最少で一番安い** →
  だから低 disorder に Simple を割り当てる。乱れが増すほど漸近的に良い手法へ段階的に上げる。
- 閾値 0.2 / 0.5 は **subject の規定**(自分で決めた値ではない)。
- explicit フラグ(`--simple` 等)は disorder を無視してその tier を**そのまま実行**する
  (ユーザーの明示要求を尊重)。小n要件は既定(adaptive)経路で保証。

---

## 早見表

| 戦略 | 手法 | 時間(操作) | B の使い方 | 例の操作数 |
|---|---|---|---|---|
| 小n (n≤5) | sort_two/three + push2 | O(1)(n上限固定) | n=4,5 で最小2個だけ | n=5: 7 |
| Simple | B へ挿入(降順維持) | O(n²) | 全要素を B 経由 | n=5: 14 |
| Medium | チャンク(幅≈√n) | O(n√n) | 全要素を B 経由・A も最短回転 | n=6: 19 |
| Complex | 2進 LSD radix | O(n log n) | ビットごとに B へ分割 | n=8: 36 |
| Adaptive | disorder で上記を選択 | レジーム依存 | — | — |

---

# (English) Sort Algorithm Walkthroughs (reference / defense aid)

How each strategy actually behaves, traced step by step from the real program's
output. Notation: `A[top ... bottom]  B[top ... bottom]` (leftmost is the top).

> Generated by piping `./push_swap <flag> "<input>"` through a simulator that
> replays each operation on the stacks. Numbers are the input values (internally
> they are compressed to the `index` below, but the moves are easy to follow with
> the values themselves).

## 0. Prerequisites

### Coordinate compression

Before sorting, each value is replaced by its **rank (0..n-1)**. Comparisons use
the index, not the value.

```
value:  42  17  99   3   8
index:   3   2   4   0   1     ← 3 is smallest → 0, 8 → 1, 17 → 2, 42 → 3, 99 → 4
```

- Benefit: the key range is fixed to `[0, n)` → radix needs only `⌈log₂ n⌉` bit
  passes / independent of how big the raw ints are.
- Emits **zero operations** (it only relabels nodes).

### The disorder metric

The **inversion ratio** of stack A, measured once before sorting.

```
inversions of 3 1 2:
  (3,1) 3>1 → inverted
  (3,2) 3>2 → inverted
  (1,2) 1<2 → ok
  inversions = 2,  C(3,2) = 3,  disorder = 2/3 = 0.667
```

Range [0,1]: 0 = sorted, 1 = fully reversed. Adaptive picks the regime from this.

## 1. Small n (n ≤ 5)

For tiny inputs, **size takes priority over disorder** (top of `dispatch`).
Already-sorted input exits at `disorder == 0` with **zero operations**.

### sort_two (n = 2)

If top > bottom, just `sa`. At most one operation.

```
  init  A[  2   1]  B[-]
  sa    A[  1   2]  B[-]
  (total 1 ops)
```

### sort_three (n = 3)

Handles the 5 unsorted permutations with `sa`/`ra`/`rra` only, ≤ 2 ops. No B.

```
  init  A[  3   1   2]  B[-]
  ra    A[  1   2   3]  B[-]        ← just rotate the top (3) to the bottom
  (total 1 ops)
```

**Note:** never touches B, ≤ 2 ops — that is why n=3 meets the ≤ 5 budget easily.

### sort_small (n = 4, 5): push 2 smallest to B → sort_two/three → bring back

```
  input 3 1 5 2 4
  init  A[  3   1   5   2   4]  B[-]
  ra    A[  1   5   2   4   3]  B[-]   ┐ rotate the smallest (1) to top,
  pb    A[  5   2   4   3]  B[  1]     ┘ push it to B
  ra    A[  2   4   3   5]  B[  1]     ┐ rotate the next smallest (2) to top,
  pb    A[  4   3   5]  B[  2   1]     ┘ push it (B = [2,1], descending)
  sa    A[  3   4   5]  B[  2   1]     ← sort the 3 left in A (sort_three)
  pa    A[  2   3   4   5]  B[  1]     ┐ bring B back (2 first, then 1)
  pa    A[  1   2   3   4   5]  B[-]   ┘
  (total 7 ops)
```

**Note:** B becomes `[2, 1]` (descending) by pushing smallest-then-next; the final
`pa pa` then lands 2 then 1 on top, leaving A ascending.

## 2. Simple — insertion into B (O(n²))

### In a nutshell: slot each card into its right place (insertion sort)

Like sorting cards by hand: take one card at a time from pile A and **insert it at
the right spot** in pile B, keeping B in descending order (largest on top). Once
everything is in B, deal it back from the top (`pa`) and A comes out ascending.

```
   A (unsorted)                       B (always descending: top = max)
  ┌─────┐                            ┌─────┐
  │  3  │ ← take the top card        │  5  │ big
  │  1  │                            │  4  │
  │  5  │   find its slot in B,      │  3  │
  │  2  │   rotate B, then pb        │  2  │
  │  4  │                            │  1  │ small
  └─────┘                            └─────┘

  once all are in B (descending) ──→ pa ×5 ──→ A is ascending
      B: 5 4 3 2 1                        A: 1 2 3 4 5
```

**What "insert at the right spot" means (keeping B descending):**

```
  put  4  into  B = [6, 2] (descending)
    4 is "bigger than 2, smaller than 6" → it belongs between 2 and 6
    └ rotate B so 2 is on top → pb drops 4 just above 2
       result … 6, 4, 2 … descending order preserved
```

**Mechanism (in code terms):** move elements from A to B one at a time, each time
slotting just above the largest element in B that is still smaller (keeping B
cyclically descending). When A is empty, rotate the max to the top and `pa`
everything back.

## 3. Medium — chunk sort (O(n√n))

### In a nutshell: split into rough "bands" first, then carry them over

Simple searches all of B and rotates a lot for each element. Medium **splits the
values into rough bands (chunks)** to cut the rotations — like roughly shelving
books into "A–F", "G–M", … before fine-sorting.

```
  split the index range [0, n) into chunks of width ≈ √n:

   ┌── chunk0 ──┬── chunk1 ──┬── chunk2 ──┐
   │  0  1  2   │  3  4  5   │  6  7  8   │   ← index (= rank)
   └────────────┴────────────┴────────────┘
        small                        big

  process the lowest band first:
    1) scan A, send chunk0's values (index 0,1,2) to B
    2) then chunk1, chunk2 …
  → B builds up roughly: small bands on top, big bands at the bottom
  → finally pa everything back to A (ascending)
```

**Why cheaper than Simple?**

```
  Simple : search + rotate over all of B per element   → up to O(n) each
  Medium : only care within a band (width ≈ √n)        → ≈ √n each

  "≈√n per element" × "≈ n/√n bands"  →  O(n√n)
  width = √n is where the two balance out (hence 4 / 15 / 30 / 45 ≈ √n)
```

**Mechanism (in code terms):** split the rank range into chunks of width ≈ √n,
process them in increasing order, bring each member to A's top with the shorter
rotation, `pb` into the position that keeps B descending, then rotate the max up
and bring everything back.

## 4. Complex — binary LSD radix sort (O(n log n))

### In a nutshell: split into "0 pile / 1 pile" per binary digit, repeatedly

Like sorting by postal code digit by digit: **partition by one bit and stack back
up**, repeating from the lowest bit, and the data ends up sorted. Here we look at
the index in binary and split into a "0 group" and a "1 group" per bit.

```
  index in binary (n=4, 2 bits):

     value   index   b1 b0
       1       0      0  0
       2       1      0  1
       3       2      1  0
       4       3      1  1

  pass 0 = split by the lowest bit b0:
     b0=0 ┐            b0=1 ┐
       index 0,2 │ → B        index 1,3 │ → keep in A
       (val 1,3) ┘            (val 2,4) ┘
     → after bringing back:  [1,3 | 2,4]  (stably split by b0)

  pass 1 = split by b1 (same operation)
     → [1,2,3,4]  done!

  passes = number of bits = ⌈log₂ n⌉
```

**Key:** stably partitioning from the LSB upward is what makes it work. Each pass
is a single-bit bucketing (O(n)), done log times → **O(n log n)**. Inside A we
`pb` (0 → B) / `ra` (1 → keep), and `pa` everything back at the end of each pass.

```
   one pass (scan A once, top to bottom):
     current top's bit = 0  →  pb  (send to B)
     current top's bit = 1  →  ra  (rotate to A's bottom = keep)
   after one loop, pa everything back from B → next bit
```

**Mechanism (in code terms):** look at the index from the **least-significant
bit**. For each bit, scan A: `0 → pb` (to B), `1 → ra` (keep in A); at the end of
the pass, `pa` everything back. `⌈log₂ n⌉` passes sort it.

## 5. Adaptive — pick the strategy by disorder

In `--adaptive` (the default), measure disorder before sorting and pick the tier
whose required complexity matches the subject's regime (for n ≥ 6; n ≤ 5 goes to
the small-n path, sorted input to zero ops).

| disorder | regime | chosen | label |
|---|---|---|---|
| < 0.2 | low | Simple | `Adaptive / O(n²)` |
| 0.2 ≤ d < 0.5 | medium | Medium | `Adaptive / O(n√n)` |
| ≥ 0.5 | high | Complex | `Adaptive / O(n log n)` |

Real examples (measured with `--bench`):

```
  input '1 2 3 4 5 7 6'        disorder:  4.76%  → Simple   (Adaptive / O(n²))      16 ops
  input '6 2 9 1 7 3 10 4 8 5' disorder: 42.22%  → Medium   (Adaptive / O(n√n))     30 ops
  input '8 7 6 5 4 3 2 1'      disorder:100.00%  → Complex  (Adaptive / O(n log n)) 36 ops
```

**Talking points (often asked at the defense):**
- A nearly-sorted input has few inversions, so the **O(n²) Simple does the fewest
  real moves and is cheapest** there → low disorder → Simple. As disorder rises,
  an asymptotically better method pays off.
- The thresholds 0.2 / 0.5 are **fixed by the subject** (not chosen by us).
- Explicit flags (`--simple`, …) ignore disorder and run that tier as requested;
  the small-n budget is guaranteed by the default (adaptive) path.

## Quick reference

| Strategy | Method | Time (ops) | Use of B | example ops |
|---|---|---|---|---|
| small n (n≤5) | sort_two/three + push 2 | O(1) (n bounded) | only 2 smallest for n=4,5 | n=5: 7 |
| Simple | insert into B (kept descending) | O(n²) | all via B | n=5: 14 |
| Medium | chunks (width ≈ √n) | O(n√n) | all via B, shorter rotations in A | n=6: 19 |
| Complex | binary LSD radix | O(n log n) | split into B per bit | n=8: 36 |
| Adaptive | pick the above by disorder | regime-dependent | — | — |

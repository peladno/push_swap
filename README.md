*This project has been created as part of the 42 curriculum by skusakab, jperez-u.*

# push_swap

## Description

`push_swap` sorts a list of distinct integers into ascending order using **two
stacks** (A and B) and a fixed set of eleven operations (`sa`, `sb`, `ss`, `pa`,
`pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`). It prints, to standard output, a
sequence of those operations that sorts the input; the goal is to keep that
sequence short.

This implementation embeds **four sorting strategies** and, by default, selects
one **at runtime from a measured "disorder" metric** of the input:

| Strategy   | Algorithm                | Time (operations) |
| ---------- | ------------------------ | ----------------- |
| `Simple`   | insertion into B         | O(n²)             |
| `Medium`   | chunk sort               | O(n√n)            |
| `Complex`  | binary LSD radix sort    | O(n log n)        |
| `Adaptive` | picks one of the above   | by disorder       |

It also provides an optional **benchmark mode** (`--bench`) that reports the
disorder, the chosen strategy, and the operation counts on standard error.

## Instructions

### Build

Run `make` at the repository root. The project Makefile builds `libft` first,
then compiles `push_swap` with `cc -Wall -Wextra -Werror`.

```bash
make          # build ./push_swap
make clean    # remove object files
make fclean   # remove objects and the binary
make re       # fclean + build
```

### Run

Pass the integers as arguments, either as separate arguments or as a single
quoted, space-separated string:

```bash
./push_swap 3 2 1 5 4
./push_swap "3 2 1 5 4"
```

The program prints one operation per line to **stdout**. On invalid input
(non-integer, out-of-`int` range, or a duplicate) it prints `Error` followed by
a newline to **stderr** and exits. With no arguments it prints nothing.

To validate a run, pipe the operation stream into the `checker` binary provided
by 42 (`checker_linux` / `checker_OSX`, not part of this repository):

```bash
./push_swap "3 2 1 5 4" | ./checker_linux "3 2 1 5 4"   # prints OK / KO
```

### Strategy flags

By default the program runs in `Adaptive` mode. A flag forces one strategy:

```bash
./push_swap --simple   "5 3 8 1 9 2"
./push_swap --medium   "5 3 8 1 9 2"
./push_swap --complex  "5 3 8 1 9 2"
./push_swap --adaptive "5 3 8 1 9 2"   # same as the default
```

### Benchmark mode (`--bench`)

`--bench` is orthogonal to the strategy flags and may be combined with one. The
**operation stream stays on stdout**; the benchmark report is written to
**stderr**, prefixed with `[bench] `, so the two can be separated with shell
redirection (e.g. pipe the operations to a checker while saving the metrics):

```bash
./push_swap --bench --adaptive "5 3 8 1 9 2" 2> bench.txt | ./checker_linux "5 3 8 1 9 2"
cat bench.txt
[bench] disorder: 53.33%
[bench] strategy:  Adaptive / O(n log n)
[bench] total_ops:  29
[bench] sa:  0 sb:  0 ss:  0 pa:  11 pb:  11
[bench] ra:  7 rb:  0 rr:  0  rra:  0 rrb:  0 rrr:  0
```

## Algorithms — explanation and justification

### Coordinate compression (shared)

Before sorting, every value is replaced by its **rank** among all inputs
(`0 .. n-1`), preserving order. This is order-equivalent to the original values
but makes the key range exactly `[0, n)`, which (a) removes any concern about the
magnitude of the raw integers during comparisons, and (b) lets the radix
strategy use `⌈log₂ n⌉` bit-passes that depend only on `n`, not on the input
values. Coordinate compression emits **no operations** — it only relabels nodes.

### The disorder metric

`disorder` is the **inversion ratio** of stack A, measured once before any
sorting move:

```
disorder = (number of inverted pairs) / C(n, 2)
```

where a pair `(i, j)`, `i < j`, is inverted when `rank[i] > rank[j]`. It lies in
`[0, 1]`: `0` means already sorted, `1` means reverse-sorted. In `--bench` it is
printed as a percentage.

### Adaptive selection (the four regimes)

In `Adaptive` mode the program measures `disorder` and selects the tier whose
required time complexity matches the regime defined by the subject:

| Regime          | Condition                | Strategy  | Required time |
| --------------- | ------------------------ | --------- | ------------- |
| Low disorder    | `disorder < 0.2`         | `Simple`  | O(n²)         |
| Medium disorder | `0.2 ≤ disorder < 0.5`   | `Medium`  | O(n√n)        |
| High disorder   | `disorder ≥ 0.5`         | `Complex` | O(n log n)    |

**Threshold rationale.** The boundaries `0.2` and `0.5`, and the per-regime
complexity ceilings, are fixed by the subject; this project maps each regime to
an algorithm that meets its ceiling. The intuition behind the mapping: a
nearly-sorted input has very few inversions, so an insertion-style `O(n²)` method
performs only a handful of real moves and is the cheapest choice in practice —
its quadratic worst case is never approached. As disorder grows, an
asymptotically better method pays off, so the dispatcher steps up to `O(n√n)` and
then to `O(n log n)`. The boundaries `< 0.2` and `≥ 0.5` are checked with strict
/ inclusive comparisons exactly as written above (so `0.2 → Medium`,
`0.5 → Complex`).

### Simple — insertion into B (O(n²))

Push elements from A into B one at a time. For each element, scan B for the node
whose rank is the largest one still **smaller** than the element, rotate B so
that node is on top, and `pb` the element just above it; this keeps B in
(cyclic) descending order. When A is empty, rotate B so its maximum is on top and
`pa` everything back, leaving A in ascending order. Each insertion scans/rotates
`O(n)` for `n` elements → **O(n²)** operations.

*Justification:* for the low-disorder regime the input is nearly ordered, so the
per-element scan finds its place quickly and the constant factors stay small; the
simplicity beats heavier heuristics here.

### Medium — chunk sort (O(n√n))

Split the rank range `[0, n)` into chunks of width `≈ √n`. Process the chunks in
increasing order: for each chunk, repeatedly bring a member to the top of A (with
the shorter of `ra`/`rra`) and `pb` it into B at the position that keeps B in
(cyclic) descending order. After all chunks are in B, rotate B's maximum to the
top and `pa` everything back. Balancing the per-element rotations (`≈ √n`) against
the number of chunks (`n / √n`) gives **O(n√n)** operations; the chunk widths are
tuned per input-size band (4 / 15 / 30 / 45) around `√n`.

*Justification:* chunking confines moves to contiguous rank ranges, which keeps
both the push count and the rotate count low and gives good practical results in
the mid-disorder regime.

### Complex — binary LSD radix sort (O(n log n))

With ranks in `[0, n)`, sort by binary digits from the least-significant bit up.
For each of the `⌈log₂ n⌉` bits, scan A: if the current top's bit is `0`, `pb` it
to B; if it is `1`, `ra` to keep it in A. After the pass, `pa` everything back.
Each bit is one stable partition pass costing `O(n)` operations, for **O(n log n)**
total.

*Justification:* with compressed integer keys, radix maps cleanly onto the
allowed operations, scales predictably, and reliably meets the operation budgets
for large, highly-disordered inputs.

### Complexity summary (upper bounds)

| Strategy  | Time (operations) | Auxiliary space |
| --------- | ----------------- | --------------- |
| `Simple`  | O(n²)             | O(1) beyond the stacks |
| `Medium`  | O(n√n)            | O(1) beyond the stacks |
| `Complex` | O(n log n)        | O(1) beyond the stacks |

All strategies operate in place on the two doubly-linked-list stacks, which hold
`n` nodes total → **O(n)** space overall, with only a constant amount of extra
working state (indices and pointers) on top.

## Work assignments

We both work on the same points in paralel, to check different approaches:

1. **Stack primitives** - So / Javier
2. **Parser** - So / Javier
3. **Coordinate compression** - So / Javier
3. **Operations** - So / Javier

When we finished, So checked  approach of doing the logic and together we decided what to use and what not to use. Once the decision was made, So implemented it in the project. 
We divided the work in this list:

1. **Simple strategy** - Javier
2. **Medium strategy** - Javier
3. **Complex strategy** - So
4. **Adaptative** - So
5. **Bench** - So
6. **Final implementation** - So
7. **Testing** - Both

## Resources

- The `push_swap` subject and the Norm (42 coding standard).
- Radix sort: CLRS, *Introduction to Algorithms*, radix-sort sections.
- Inversions / "sortedness" as a disorder measure: standard treatments of
  inversion counting.
- Coordinate compression: standard competitive-programming write-ups on ranking
  values into `0 .. n-1`.

## AI usage

AI assistants were used as tutors and reviewers, not as code generators:

- **Claude (Claude Code)** — used by skusakab for the benchmark mode and the
  adaptive dispatcher: discussing design trade-offs (e.g. how to thread the
  operation counter, how to model the strategy label), reviewing hand-written
  code, and helping debug. The code was written and is understood by the author;
  the assistant gave hints, reviews, and explanations.
- **GPT-5 mini / GitHub Copilot** — used by jperez-u to help draft and organize
  this README and parts of the documentation, and for occasional code-completion
  suggestions that the author reviewed.

Core algorithms, the data structures, and the operation set were designed,
written, and verified by the authors.

---

# 日本語版

*This project has been created as part of the 42 curriculum by skusakab, jperez-u.*

## 概要

`push_swap` は、相異なる整数のリストを、**2 つのスタック**(A・B)と 11 個の固定
操作(`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`)だけ
で昇順にソートするプログラムです。入力をソートする操作列を標準出力に出力し、その
**操作数をできるだけ少なく**することが目標です。

本実装は **4 つのソート戦略**を内蔵し、既定では入力の **「disorder(乱れ)」指標
を実行時に測定して**戦略を選びます:

| 戦略       | アルゴリズム            | 時間計算量(操作数) |
| ---------- | ----------------------- | ------------------- |
| `Simple`   | B への挿入ソート        | O(n²)               |
| `Medium`   | チャンクソート          | O(n√n)              |
| `Complex`  | 2 進 LSD 基数ソート     | O(n log n)          |
| `Adaptive` | 上記から実行時に選択    | disorder で決定     |

さらにオプションの **ベンチマークモード**(`--bench`)があり、disorder・選んだ
戦略・操作数を標準エラーに出力します。

## 使い方

### ビルド

リポジトリ直下で `make`。Makefile はまず `libft` をビルドし、その後
`cc -Wall -Wextra -Werror` で `push_swap` をコンパイルします。

```bash
make          # ./push_swap をビルド
make clean    # オブジェクトファイル削除
make fclean   # オブジェクト + バイナリ削除
make re       # fclean + ビルド
```

### 実行

整数を引数で渡します(個別の引数でも、空白区切りの 1 つの文字列でも可):

```bash
./push_swap 3 2 1 5 4
./push_swap "3 2 1 5 4"
```

操作は 1 行 1 つで **stdout** に出力されます。不正な入力(整数でない・`int` 範囲
外・重複)のときは **stderr** に `Error` + 改行を出して終了します。引数なしのとき
は何も出力しません。

検証には、42 が提供する `checker`(`checker_linux` / `checker_OSX`。本リポジトリに
は含まれません)に操作列をパイプします:

```bash
./push_swap "3 2 1 5 4" | ./checker_linux "3 2 1 5 4"   # OK / KO を表示
```

### 戦略フラグ

既定は `Adaptive`。フラグで戦略を固定できます:

```bash
./push_swap --simple   "5 3 8 1 9 2"
./push_swap --medium   "5 3 8 1 9 2"
./push_swap --complex  "5 3 8 1 9 2"
./push_swap --adaptive "5 3 8 1 9 2"   # 既定と同じ
```

### ベンチマークモード(`--bench`)

`--bench` は戦略フラグと直交し、併用できます。**操作列は stdout のまま**で、ベン
チマーク出力は `[bench] ` 前置きで **stderr** に出ます。両者はシェルのリダイレクト
で分離できます(操作を checker に流しつつ、メトリクスを保存するなど):

```bash
./push_swap --bench --adaptive "5 3 8 1 9 2" 2> bench.txt | ./checker_linux "5 3 8 1 9 2"
cat bench.txt
[bench] disorder: 53.33%
[bench] strategy:  Adaptive / O(n log n)
[bench] total_ops:  29
[bench] sa:  0 sb:  0 ss:  0 pa:  11 pb:  11
[bench] ra:  7 rb:  0 rr:  0  rra:  0 rrb:  0 rrr:  0
```

## アルゴリズム —— 説明と正当化

### 座標圧縮(全戦略で共通)

ソート前に、各値を全入力中の**順位**(`0 .. n-1`)に置き換えます。元の値と順序は
同値ですが、キーの範囲がちょうど `[0, n)` になるため、(a) 比較時に生の整数の大きさ
を気にせずに済み、(b) 基数戦略が入力値ではなく `n` だけに依存する `⌈log₂ n⌉` 回の
ビットパスで済みます。座標圧縮は**操作を一切出力しません**(ノードの再ラベル付けの
み)。

### disorder 指標

`disorder` はスタック A の**転倒比率**で、ソート操作の前に 1 度だけ測ります:

```
disorder = (転倒ペアの個数) / C(n, 2)
```

ここで `i < j` のペア `(i, j)` は `rank[i] > rank[j]` のとき「転倒」です。値域は
`[0, 1]`:`0` は整列済み、`1` は完全な逆順。`--bench` ではパーセント表示します。

### 適応選択(4 つのレジーム)

`Adaptive` では `disorder` を測り、subject が定める各レジームの要求計算量に合う
tier を選びます:

| レジーム        | 条件                     | 戦略      | 要求時間   |
| --------------- | ------------------------ | --------- | ---------- |
| 低 disorder     | `disorder < 0.2`         | `Simple`  | O(n²)      |
| 中 disorder     | `0.2 ≤ disorder < 0.5`   | `Medium`  | O(n√n)     |
| 高 disorder     | `disorder ≥ 0.5`         | `Complex` | O(n log n) |

**しきい値の根拠。** 境界 `0.2` / `0.5` と各レジームの計算量上限は subject が定めて
おり、本実装は各レジームをその上限を満たすアルゴリズムに対応づけます。直感的には:
ほぼ整列した入力は転倒が少ないため、挿入系の `O(n²)` 手法でも実際の移動は数回で済み、
最も安上がり(最悪の二乗には届かない)。disorder が増えるほど漸近的に良い手法が効い
てくるので、`O(n√n)` → `O(n log n)` と段階的に切り替えます。境界は上表どおり厳密
/ 以上の比較で判定します(`0.2 → Medium`、`0.5 → Complex`)。

### Simple —— B への挿入ソート(O(n²))

A から B へ 1 つずつ移します。各要素について、B の中で「その要素より**小さい**順位
のうち最大」のノードを探し、それが上に来るよう B を回し、その真上に `pb` します
(これで B は循環的な降順を保ちます)。A が空になったら B の最大を上に回し、`pa` で
全部戻すと A は昇順になります。各挿入の探索/回転が `O(n)`、要素 `n` 個で **O(n²)**。

*正当化:* 低 disorder では入力がほぼ整列しているので各要素の挿入先がすぐ見つかり、
定数項も小さい。ここでは重いヒューリスティクスより単純さが勝ります。

### Medium —— チャンクソート(O(n√n))

順位の範囲 `[0, n)` を幅 `≈ √n` のチャンクに分割。チャンクを昇順に処理し、各チャンク
では、メンバーを A の上に(`ra`/`rra` の短い方で)持ってきて、B が循環降順を保つ位置
に `rb`/`rrb` で回してから `pb` します。全チャンクを B に入れたら最大を上に回し `pa`
で戻します。要素ごとの回転(`≈ √n`)とチャンク数(`n / √n`)を釣り合わせて **O(n√n)**。
チャンク幅は入力サイズ帯ごとに `√n` 近傍へ調整(4 / 15 / 30 / 45)。

*正当化:* チャンク化で移動を連続した順位範囲に閉じ込め、push 数と rotate 数の両方を
抑えられるため、中 disorder で実用的に良い結果になります。

### Complex —— 2 進 LSD 基数ソート(O(n log n))

順位が `[0, n)` にあるので、最下位ビットから 2 進桁ごとに整列します。`⌈log₂ n⌉` 個
の各ビットで A を走査し、上のビットが `0` なら `pb` で B へ、`1` なら `ra` で A に
残します。1 パス終了ごとに `pa` で全部戻します。各ビットは `O(n)` の安定分割 1 回で、
合計 **O(n log n)**。

*正当化:* 圧縮済みの整数キーでは基数ソートが許可操作にきれいに対応し、スケールが
予測でき、大きく高 disorder な入力でも操作数の上限を確実に満たします。

### 計算量まとめ(上界)

| 戦略      | 時間(操作数) | 補助空間              |
| --------- | ------------- | --------------------- |
| `Simple`  | O(n²)         | スタック以外は O(1)   |
| `Medium`  | O(n√n)        | スタック以外は O(1)   |
| `Complex` | O(n log n)    | スタック以外は O(1)   |

全戦略は 2 本の双方向連結リストスタック上でその場で動作し、スタックは合計 `n` ノード
を保持 → 全体で **O(n)** 空間。それ以外の作業用状態(添字・ポインタ)は定数個のみ。

## 参考資料

- `push_swap` の subject と Norm(42 コーディング規約)。
- 基数ソート:CLRS『アルゴリズムイントロダクション』の基数ソートの節。
- 転倒数 /「整列度」を乱れの指標とする考え方:転倒数カウントの標準的な解説。
- 座標圧縮:値を `0 .. n-1` に順位付けする競技プログラミングの標準的な解説。

## 作業分担

私たちは同じポイントを並行して進め、異なるアプローチを検証しました：

1. **スタックのプリミティブ**　ー　そう・ハビエル
2. **パーサー**　ー　そう・ハビエル
3. **座標圧縮**　ー　そう・ハビエル
3. **各種オペレーション**　ー　そう・ハビエル

完了後、So がロジックのアプローチを確認し、どれを採用するかを一緒に決定しました。  
決定後、So がプロジェクトに実装しました。  
作業は次のように分担しました：

1. **シンプル戦略** - ハビエル  
2. **ミディアム戦略** - ハビエル  
3. **複雑戦略** - そう
4. **アダプティブ戦略** - そう  
5. **ベンチ** - そう
6. **最終実装** - そう  
7. **テスト** - 両者  


## AI の利用

AI はコード生成器ではなく、チューター・レビュアーとして利用しました:

- **Claude(Claude Code)** —— skusakab がベンチマークモードと適応 dispatcher で利用。
  設計のトレードオフの議論(操作カウンタの通し方、戦略ラベルのモデル化など)、自分で
  書いたコードのレビュー、デバッグの補助。コードは著者が記述・理解しており、AI はヒン
  ト・レビュー・解説を提供。
- **GPT-5 mini / GitHub Copilot** —— jperez-u が本 README やドキュメントの草稿・構成、
  および著者が確認したコード補完の提案に利用。

中核のアルゴリズム・データ構造・操作集合は、著者自身が設計・記述・検証しました。

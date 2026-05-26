# Adaptive Algorithm / Adaptive アルゴリズム

## English

Adaptive isn't a "new algorithm" — it's a **dispatcher** that picks one of our Simple / Medium / Complex strategies based on the measured disorder of the input.

### What the subject fixes (VI.3.3 #4)

| Regime | Condition | Required complexity |
| --- | --- | --- |
| Low disorder | `disorder < 0.2` | O(n²) |
| Medium disorder | `0.2 ≤ disorder < 0.5` | O(n√n) |
| High disorder | `disorder ≥ 0.5` | O(n log n) |

**Also fixed by the subject:**
- Disorder is measured **before any moves** (VI.3.2)
- README must document: rationale for thresholds, internal techniques per regime, and a complexity argument (upper bounds for time + space in push_swap operation model)
- The disorder calculation pseudo-code (see below)

### What the subject leaves to us

- **Which internal algorithm to call in each regime** — defaults to the tier picks, but we can differ
- C implementation details of the disorder calculation (linked-list traversal, float vs integer comparison, n ≤ 1 boundary)
- README writing responsibility (who covers what)

### The disorder metric

Subject VI.3.2 provides the pseudo-code:

```text
function compute_disorder(stack a):
    mistakes = 0
    total_pairs = 0
    for i from 0 to size(a)-1:
        for j from i+1 to size(a)-1:
            total_pairs += 1
            if a[i] > a[j]:
                mistakes += 1
    return mistakes / total_pairs
```

This counts **inversions** (pairs out of order) divided by the total number of pairs `n(n-1)/2`. The result lies in `[0, 1]`:

- `0` — perfectly sorted
- `1` — perfectly reverse-sorted (every pair is out of order)
- Intermediate — partially sorted

**Cost**: O(n²) on the C side. The subject's complexity is measured in push_swap operations (which this function doesn't generate), so this doesn't affect the formal complexity claim. The wall-clock cost is real and should be noted in the README.

### Dispatch logic (the entire algorithm)

```text
disorder = compute_disorder(a)
if disorder < 0.2:
    simple_strategy()
else if disorder < 0.5:
    medium_strategy()
else:
    complex_strategy()
```

Three lines. That is the whole of Adaptive.

### Pair's current proposals (default mapping)

| Regime | Default internal call | Tier algorithm (Topic 1 proposals) |
| --- | --- | --- |
| Low (< 0.2) | Simple strategy | Min Extraction **or Insertion Sort** |
| Medium (0.2–0.5) | Medium strategy | Chunk Sort + in-chunk sort |
| High (≥ 0.5) | Complex strategy | Radix LSD (binary) |

**The only architectural nuance**: if `--simple` runs Min Extraction (lowest naive ops), Adaptive's low-disorder regime won't extract value from the partial-sorted property (Min Extraction is disorder-insensitive). Using **Insertion Sort** there would.

| Option | `--simple` runs | Adaptive low-disorder runs | Scope | Adaptive performance |
| --- | --- | --- | --- | --- |
| (i) | Insertion Sort | Insertion Sort | Smallest | ◎ |
| (ii) | Min Extraction | Insertion Sort | +1 algorithm | ◎ + best `--simple` ops |
| (iii) | Min Extraction | Min Extraction | Smallest | △ (sub-optimal) |

→ Recommended: **(i)** for scope simplicity, **(ii)** for excellent-tier benchmark hunting. Decide at kickoff.

### Complexity argument for the README (subject requirement)

#### Per-regime complexity (push_swap operations)

| Regime | Internal algorithm | Push_swap operation count |
| --- | --- | --- |
| Low | Insertion / Min Extraction | O(n²) — meets the subject's required O(n²) |
| Medium | Chunk Sort + in-chunk sort | O(n√n) — meets the subject's required O(n√n) |
| High | Radix LSD | O(n log n) — meets the subject's required O(n log n) |

#### Dispatcher overhead

- Disorder calculation: O(n²) C-side work, generates **zero push_swap operations**
- if / else branch: O(1) C-side, zero operations

→ Adaptive's push_swap operation count equals the selected regime's algorithm count. The dispatcher adds nothing to the operation total.

#### Threshold rationale (why 0.2 and 0.5 make sense)

The values are subject-fixed, but the choices are sensible because:

- `disorder < 0.2` — ~80% of pairs are already in order → input is nearly sorted → Insertion Sort approaches O(n) total operations
- `0.2 ≤ disorder < 0.5` — input is moderately disordered → Chunk Sort's value-range partitioning beats O(n²) without needing the bit machinery of Radix
- `disorder ≥ 0.5` — input is essentially random → only O(n log n) keeps us under the n=500 benchmark

### `--bench` output integration

The benchmark mode (VI.5.1 example) must display:

```
[bench] strategy:  Adaptive / O(n√n)
```

→ The dispatcher must record which regime was selected (e.g., a `t_bench->strategy` string field). Implementation detail.

### Pair-meeting decision summary (kickoff Topic 6)

The single architectural question is: **which Simple algorithm does the Adaptive low-disorder regime call?** Options (i) / (ii) / (iii) above. Everything else is either subject-fixed or implementation translation.

---

## 日本語

Adaptive は「新しいアルゴリズム」ではなく、入力の disorder を測って Simple / Medium / Complex のどれを呼ぶか決める **ディスパッチャ**。

### subject が固定するもの(VI.3.3 #4)

| レジーム | 条件 | 要求される計算量 |
| --- | --- | --- |
| Low disorder | `disorder < 0.2` | O(n²) |
| Medium disorder | `0.2 ≤ disorder < 0.5` | O(n√n) |
| High disorder | `disorder ≥ 0.5` | O(n log n) |

**他にも subject が固定**:
- disorder は**移動前に**測定する(VI.3.2)
- README に必須記載:しきい値の根拠、各レジームの内部技法、計算量議論(時間 + 空間、push_swap 操作モデル)
- disorder 計算の pseudo-code(下記)

### subject が学習者に残すもの

- **各レジームで呼ぶ内部アルゴリズム** — デフォルトは tier 選定だが、差別化も可能
- disorder 計算の C 実装詳細(連結リスト走査、float vs 整数比較、n ≤ 1 境界)
- README 執筆(役割分担)

### disorder 指標

subject VI.3.2 が pseudo-code を提供:

```text
function compute_disorder(stack a):
    mistakes = 0
    total_pairs = 0
    for i from 0 to size(a)-1:
        for j from i+1 to size(a)-1:
            total_pairs += 1
            if a[i] > a[j]:
                mistakes += 1
    return mistakes / total_pairs
```

これは **転倒数**(順序が逆になっているペアの数)を、ペア総数 `n(n-1)/2` で割った値。結果は `[0, 1]` の範囲:

- `0` — 完全にソート済み
- `1` — 完全に逆順(全ペアが逆順)
- 中間 — 部分整列

**コスト**:C 側で O(n²)。subject の計算量は push_swap 操作数で測られる(この関数は操作を生成しない)ため、formal な計算量主張には影響しない。ただし実時間コストは実在し、README で言及すべき。

### Dispatch ロジック(これが算法の全て)

```text
disorder = compute_disorder(a)
if disorder < 0.2:
    simple_strategy()
else if disorder < 0.5:
    medium_strategy()
else:
    complex_strategy()
```

3 行。Adaptive の本体はこれだけ。

### ペアの現在の提案(デフォルト対応)

| レジーム | デフォルト呼び出し | Tier アルゴリズム(Topic 1 提案) |
| --- | --- | --- |
| Low (< 0.2) | Simple strategy | Min Extraction **または Insertion Sort** |
| Medium (0.2–0.5) | Medium strategy | Chunk Sort + in-chunk sort |
| High (≥ 0.5) | Complex strategy | Radix LSD(2 進) |

**唯一のアーキテクチャ的ニュアンス**:`--simple` を Min Extraction(naive 操作数最少)にした場合、Adaptive 低 disorder レジームは低 disorder の恩恵を受けられない(Min Extraction は disorder に鈍感)。ここで **Insertion Sort** を呼べば部分整列の性質を活かせる。

| 案 | `--simple` で呼ぶもの | Adaptive 低 disorder で呼ぶもの | スコープ | Adaptive 性能 |
| --- | --- | --- | --- | --- |
| (i) | Insertion Sort | Insertion Sort | 最小 | ◎ |
| (ii) | Min Extraction | Insertion Sort | +1 algorithm | ◎ + `--simple` 操作数も最良 |
| (iii) | Min Extraction | Min Extraction | 最小 | △(最適化されない) |

→ 推奨:スコープ重視なら **(i)**、excellent ベンチ狙いなら **(ii)**。kickoff で決定。

### README の計算量議論(subject 要求)

#### 各レジームの計算量(push_swap 操作数)

| レジーム | 内部アルゴリズム | Push_swap 操作数 |
| --- | --- | --- |
| Low | Insertion / Min Extraction | O(n²) — subject 要求の O(n²) を満たす |
| Medium | Chunk Sort + in-chunk sort | O(n√n) — subject 要求の O(n√n) を満たす |
| High | Radix LSD | O(n log n) — subject 要求の O(n log n) を満たす |

#### ディスパッチャ自体のオーバーヘッド

- disorder 計算:C 側 O(n²) の作業、push_swap 操作は **0 個生成**
- if / else 分岐:O(1) の C 作業、操作生成 0

→ Adaptive の push_swap 操作数 = 選ばれたレジームのアルゴリズムと同じ。ディスパッチャ自体は操作総数に何も加算しない。

#### しきい値の根拠(なぜ 0.2 と 0.5 が妥当か)

しきい値は subject 固定だが、合理的である理由:

- `disorder < 0.2` — 全ペアの約 80% が既に正しい順 → 入力はほぼソート済み → Insertion Sort の合計操作数が O(n) に近づく
- `0.2 ≤ disorder < 0.5` — 中程度の乱れ → Chunk Sort の値域分割が O(n²) を抜けつつ、Radix の bit 機構までは不要
- `disorder ≥ 0.5` — ほぼランダム → O(n log n) でないと n=500 ベンチを通せない

### `--bench` 出力との連動

Benchmark モード(VI.5.1 の例)で表示が必要:

```
[bench] strategy:  Adaptive / O(n√n)
```

→ ディスパッチャは選択したレジームを記録する必要(例:`t_bench->strategy` の文字列フィールド)。実装詳細。

### ペアミーティングでの決定事項(kickoff Topic 6)

唯一のアーキテクチャ判断:**Adaptive 低 disorder レジームでどの Simple アルゴリズムを呼ぶか**。上記 (i)、(ii)、(iii) のどれか。それ以外は subject 固定か、実装翻訳。

# Kickoff Meeting Agenda

> Status: **agenda (pre-meeting)**. After the meeting, fill in Decisions / Open questions / Action items.

## English

### Participants
- So
- Javier

### Context
First sync meeting after the partner shared the proposed file structure (`docs:possible files` commit) and the 824-line README.
So has read the subject and reviewed the partner's plan.
Goal: align on **algorithm selection, scope, and role split** before implementation starts.

**Pre-meeting preparation by So (2026-05-20 / 2026-05-21)**: studied the Simple, Medium, and Complex tiers in depth and prepared concrete proposals for Topic 1. See `learning_log/algorithms-simple-o-n2.md`, `learning_log/algorithms-medium-o-nsqrtn.md`, and `learning_log/algorithms-complex-o-nlogn.md` for the analysis. Proposals are noted inline under Topic 1.

### Topics to discuss

#### 1. Algorithm selection per tier (nothing is locked in yet)
The skeleton proposes one specific algorithm per tier, but **the choice for each tier is still open**.

- **Simple O(n²)** — Selection Sort? Insertion Sort? Min Extraction? Bubble Sort?
  > **So's proposal**: **Min Extraction** or **Insertion Sort**. Min Extraction has the lowest naive op count (~n²/4); Insertion Sort excels on partially sorted input — relevant for Adaptive's low-disorder regime. Selection (naive) and Bubble are dominated. Final pick depends on how heavily Adaptive uses Simple.
- **Medium O(n√n)** — Chunk Sort? Block partitioning? Bucket Sort with √n buckets? Range-based?
  > **So's proposal**: **Chunk Sort with in-chunk sort**. Basic Chunk Sort risks missing the n=500 benchmark (~14,000 ops); with insertion-style in-chunk sort, ~7,000 ops (good tier) is reachable. The subject's other three names converge to Chunk Sort in push_swap's operation model.
- **Complex O(n log n)** — Radix Sort? Merge Sort (2-stack)? Quick Sort (partition)? Heap Sort? BIT?
  > Radix is the skeleton's proposal, **but not yet decided**.
  > **So's proposal**: **Radix Sort (LSD, binary)**. Best match for push_swap's instruction set (`pb` / `ra` / `pa` map directly to bit-based bucketing). Reaches the excellent benchmark band (~4,500–6,000 ops at n=500). Requires coordinate compression to fix the bit count at `log₂(n)` — settles Topic 5 if adopted. Quick Sort is a workable backup; Merge / Heap / BIT are impractical (Heap fails to achieve O(n log n) in the operation model; BIT isn't a sorting algorithm).
- **Adaptive** — given the picks above, how does the dispatcher compose them per disorder regime?

#### 2. Extras beyond subject requirements
The subject does not require these — they are optimizations.

- **mini_sort (sort_2 / sort_3 / sort_5)** — hardcoded optimal sorts for tiny inputs. Not in subject. Implement? (Often needed to hit excellent-tier benchmarks, and useful as the base case for Chunk / Quick / Merge-style strategies.) Decision affects scope and role split.
- Any other polish to flag (e.g., per-bit optimizations in Complex)?

#### 3. Role division (depends on #1 and #2)
See the draft table below — drafter assignments will be revised after the algorithm choices are confirmed.

#### 4. Data structure
The README proposes a doubly linked list with `t_node {value, index, next, prev}` and `t_stack {top, size}`. Open questions:
- Should `t_stack` also hold `tail` (to make `rotate` / `reverse rotate` O(1) internally)?
- Did we consider a circular doubly linked list? Trade-off vs. plain doubly linked + tail?

#### 5. Input pre-processing (conditional on Topic 1)
- Coordinate compression maps values to `0..n-1`. **Strictly needed only if we pick Radix** (so the bit count = `log₂(n)`).
- Merge / Quick / Heap don't strictly require compression — though indexes can still simplify code.
- Final decision depends on the Complex choice in Topic 1.

#### 6. Adaptive design (thresholds are fixed by subject VI.3.3 #4)
The subject **mandates** the regimes:
- `disorder < 0.2` → must run in **O(n²)**
- `0.2 ≤ disorder < 0.5` → must run in **O(n√n)**
- `disorder ≥ 0.5` → must run in **O(n log n)**

What's actually open for us to decide:
- **Which internal algorithm in each regime.** Default = our Topic 1 picks (Min Extraction / Chunk+in-chunk / Radix LSD). But we could differ — e.g., use **Insertion Sort** for the low-disorder regime even if `--simple` runs Min Extraction, because Insertion is much faster on partially sorted input.
- **README documentation responsibility.** Subject requires us to document: rationale for the thresholds, internal techniques per regime, and a complexity argument (upper bounds) for time and space in the Push_swap model. Who writes which part? (Tentative split in the role table below; confirm and break down further.)

> Note: The disorder calculation itself is specified by the subject VI.3.2 pseudo-code. C translation details (linked-list traversal, float vs integer comparison, n ≤ 1 boundary) are implementation choices, not meeting topics.

#### 7. Code conventions
Norm, file/function naming, where libft sits, operation wrapper signature (the README suggests `op_sa(t_stack *a, t_bench *bench, int print)`).

#### 8. Git workflow
Branch strategy (feature branches?), review process, commit message format.

#### 9. Milestones
Target dates for each phase (counting back from the campus deadline).

### Three "why?" questions we both must be able to answer (required for defense)

Tier-agnostic — the specific answers fill in once Topic 1 is settled.

- Why our chosen **data structure** (vs an array, vs other linked-list variants)?
- What **input pre-processing** does our chosen Complex algorithm require — and why?
- Why does **each of our 4 chosen algorithms** achieve its claimed complexity class **in the push_swap operation model** (not the textbook array model)?

### Proposed role split (DRAFT — pending Topics 1 & 2)

> Note: Algorithm names below reflect the partner's skeleton (selection/min_extract, chunk, radix). They are **tentative** and will be revised after the algorithm choices are confirmed.

| Phase | So (drafter) | Javier (drafter) | Co-build |
| --- | --- | --- | --- |
| 1: stack / parser base | | | ◯ |
| 2: 11 operations | | | ◯ |
| 3: mini_sort *(only if Topic 2 says yes)* | sort_5 | sort_2, sort_3 | |
| 4: Simple | (TBD) | (TBD) | |
| 5: Medium | | (TBD body) | size / policy |
| 6: Complex | (TBD core loop) | (pre-processing if any) | complexity argument |
| 7: Adaptive | | | ◯ thresholds + rationale |
| 8: Benchmark | disorder.c | benchmark.c, op counter | |
| README | algo complexity argument | data structure & overview | integration |

Rationale: drafters split, but every piece is **co-reviewed** — both must explain any line in defense (subject IX).

### Decisions
*(fill after meeting)*

### Open questions
*(fill after meeting)*

### Action items
*(fill after meeting)*
- [ ] (owner) ... — due YYYY-MM-DD

---

## 日本語

### 参加者
- So
- Javier

### 背景
相方が提案ファイル構成(`docs:possible files` コミット)と 824 行の README を共有してくれた後、初の同期ミーティング。
So は subject を読み、相方のプランをレビュー済み。
実装着手前に **アルゴリズム選定・スコープ・役割分担** を合意するのが目的。

**So の事前準備(2026-05-20 / 2026-05-21)**:Simple、Medium、Complex tier のアルゴリズムを詳しく調査し、Topic 1 に向けた具体的な提案を準備済み。分析内容は `learning_log/algorithms-simple-o-n2.md`、`learning_log/algorithms-medium-o-nsqrtn.md`、`learning_log/algorithms-complex-o-nlogn.md` を参照。提案内容は Topic 1 の各 tier に併記。

### 議題

#### 1. 各 tier のアルゴリズム選定(いずれもまだ確定していない)
スケルトンは tier ごとに 1 つの具体的アルゴリズムを提案しているが、**各 tier の選択はまだ未確定**。

- **Simple O(n²)** — Selection Sort? Insertion Sort? Min Extraction? Bubble Sort?
  > **So の提案**:**Min Extraction** または **Insertion Sort**。Min Extraction は naive な操作数が最少(~n²/4)、Insertion Sort は部分整列入力で圧倒的に速い(Adaptive の低 disorder レジームで活きる)。Selection(naive)と Bubble は他に劣り却下。最終選択は Adaptive で Simple をどう使うかによる。
- **Medium O(n√n)** — Chunk Sort? Block partitioning? √n バケットの Bucket Sort? 範囲ベース?
  > **So の提案**:**Chunk Sort + in-chunk sort**。basic な Chunk Sort のみだと n=500 のベンチライン(<12,000 ops)を割る恐れあり(~14,000 ops 想定)。insertion 流の in-chunk sort を入れれば ~7,000 ops(good tier)まで詰まる。subject の他 3 つの名前は push_swap の操作モデル上 Chunk Sort に収束する。
- **Complex O(n log n)** — Radix Sort? 2-stack の Merge Sort? Quick Sort(partition)? Heap Sort? BIT?
  > Radix はスケルトンの提案だが、**まだ採用が確定していない**。
  > **So の提案**:**Radix Sort(LSD、2 進)**。push_swap の命令セットと相性最良(`pb` / `ra` / `pa` が bit ベースの振り分けに直接対応)。n=500 で excellent 圏(約 4,500〜6,000 ops)に届く。**座標圧縮が必須**(bit 数を `log₂(n)` に固定するため)—— 採用すれば Topic 5 も同時に決まる。Quick Sort は代替案として可。Merge / Heap / BIT は非現実的(特に Heap は操作モデル上 O(n log n) を達成できない、BIT はそもそも算法ではない)。
- **Adaptive** — 上記の選定を踏まえ、disorder のレジームごとに何をどう呼ぶか

#### 2. 課題要件外の実装(extras)を入れるか
subject の要件にはない最適化。

- **mini_sort(sort_2 / sort_3 / sort_5)** — 極小入力用のハードコード最適ソート。subject 要件にはない。実装する?(excellent ベンチ達成にはしばしば必要。また Chunk / Quick / Merge 系の base case としても使える)スコープと役割分担にも影響
- その他、議論しておきたい polish はあるか(Complex の bit 単位最適化など)

#### 3. 役割分担(Topic 1, 2 が決まってから)
下のたたき台テーブル参照。下書き担当はアルゴリズム選定確定後に見直す。

#### 4. データ構造
README は双方向連結リスト(`t_node {value, index, next, prev}`, `t_stack {top, size}`)を提案。要確認:
- `t_stack` に `tail` を持たせる?(内部で `rotate`/`reverse rotate` を O(1) にするため)
- 循環双方向リストは検討した?素の双方向 + tail とのトレードオフは?

#### 5. 入力の前処理(Topic 1 に依存)
- 座標圧縮は値を `0..n-1` に写す。**Radix を選んだ場合にのみ必須**(bit 数を `log₂(n)` に固定するため)
- Merge / Quick / Heap は厳密には不要(index があるとコードが楽になる程度)
- Topic 1 の Complex 選定結果による

#### 6. Adaptive 設計(しきい値は subject VI.3.3 #4 で固定)
subject が**規定**:
- `disorder < 0.2` → **O(n²)** で動作すること
- `0.2 ≤ disorder < 0.5` → **O(n√n)** で動作すること
- `disorder ≥ 0.5` → **O(n log n)** で動作すること

学習者が決められるのは:
- **各レジームで呼ぶ内部アルゴリズム**。デフォルトは Topic 1 の選定(Min Extraction / Chunk + in-chunk / Radix LSD)。ただし別物を使う選択肢もある ——例:`--simple` フラグでは Min Extraction を使い、adaptive 低 disorder レジームでは部分整列入力に強い **Insertion Sort** を使うなど。
- **README 文書化の役割分担**。subject 要求:しきい値の根拠、各レジームの内部技法、Push_swap モデルでの時間・空間の計算量議論(上界)。誰がどの部分を書くか?(下の役割分担テーブルに暫定案あり、確認 + 細分化)

> 補足:disorder 計算自体は subject VI.3.2 の pseudo-code で specified。C 翻訳時の詳細(連結リスト走査、float vs 整数比較、n ≤ 1 境界処理)は実装時の判断事項で、ミーティング議題ではない。

#### 7. コード規約
Norm 準拠、ファイル/関数命名、libft の置き場所、operation ラッパーの引数(README は `op_sa(t_stack *a, t_bench *bench, int print)` を提案)

#### 8. Git 運用
ブランチ戦略(feature ブランチ?)、レビュー方法、コミットメッセージ形式

#### 9. マイルストーン
各 Phase の目標期日(キャンパス締切から逆算)

### 「3つのなぜ?」(両者が説明できる必要 = ディフェンス必須)

tier 非依存の形に整理。具体的な答えは Topic 1 確定後に埋まる。

- 採用する **データ構造** を選んだ理由は?(配列・他のリスト変種ではなく)
- 採用する Complex に **入力の前処理** がなぜ必要か(または不要か)?
- 採用する **4 つのアルゴリズム** それぞれが、**push_swap 操作モデル** で主張する計算量クラスを満たす理由は?(教科書の配列モデルではなく)

### 役割分担のたたき台(DRAFT — Topic 1, 2 で再調整)

> 注:下表のアルゴリズム名は相方のスケルトン(selection/min_extract, chunk, radix)を反映した **暫定** のもの。アルゴリズム選定確定後に再調整する。

| Phase | So(下書き) | Javier(下書き) | 共同 |
| --- | --- | --- | --- |
| 1: stack / parser 基盤 | | | ◯ |
| 2: 11 個の operations | | | ◯ |
| 3: mini_sort *(Topic 2 で採用と決まれば)* | sort_5 | sort_2, sort_3 | |
| 4: Simple | (TBD) | (TBD) | |
| 5: Medium | | (TBD 本体) | サイズ・方針 |
| 6: Complex | (TBD コアループ) | (前処理が必要な場合) | 計算量の議論 |
| 7: Adaptive | | | ◯ しきい値 + 根拠 |
| 8: Benchmark | disorder.c | benchmark.c, op counter | |
| README | アルゴリズム計算量議論 | データ構造・全体構成 | 統合 |

根拠: 下書きは分担するが、各ピースは **必ず相互レビュー**。両者がどの行でも説明できる状態にする(subject IX)。

### 決定事項
*(ミーティング後に記入)*

### 未解決事項
*(ミーティング後に記入)*

### 次のアクション
*(ミーティング後に記入)*
- [ ] (担当) ... 期限 YYYY-MM-DD

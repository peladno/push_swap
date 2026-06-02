# Kickoff Meeting

> Status: **partial post-meeting record (2026-05-26)** + **async update (2026-06-02)** (see bottom of file). Decided at meeting: Topics 1, 2, 3, 4, 5, plus partial Topic 8 (parallel branch strategy). Settled implicitly by Topic 1: Topic 6. Resolved async (2026-06-02): integration aspects of Topic 7 (12 sub-topics — see prep doc), Phase 2 workflow refined, parser-merge ownership assigned to So. Still pending: Topic 9 (milestones), residual Topic 8 details (commit message format, final-branch policy).

## English

### Participants
- So
- Javier

### Context
First sync meeting after the partner shared the proposed file structure (`docs:possible files` commit) and the 824-line README.
So has read the subject and reviewed the partner's plan.
Goal: align on **algorithm selection, scope, and role split** before implementation starts.

**Pre-meeting preparation by So (2026-05-20 / 2026-05-21 / 2026-05-26)**: studied the Simple, Medium, Complex, and Adaptive tiers in depth and prepared concrete proposals. See `learning_log/algorithms-simple-o-n2.md`, `algorithms-medium-o-nsqrtn.md`, `algorithms-complex-o-nlogn.md`, `algorithms-adaptive.md`. Data-structure analysis in `learning_log/linked-list-design.md`.

### Topics

#### 1. Algorithm selection per tier — DECIDED (2026-05-26)

- **Simple O(n²)** → **Insertion Sort**. Strong on partially sorted input; `--simple` and Adaptive's low-disorder regime call the same algorithm. (Reference: `learning_log/algorithms-simple-o-n2.md`)
- **Medium O(n√n)** → **Chunk Sort with in-chunk sort**. (Reference: `learning_log/algorithms-medium-o-nsqrtn.md`)
- **Complex O(n log n)** → **Radix Sort LSD (binary)**. Requires coordinate compression (see Topic 5). (Reference: `learning_log/algorithms-complex-o-nlogn.md`)
- **Adaptive** → Dispatcher calls the tier algorithm directly per regime. No algorithm differentiation needed (Simple = Insertion Sort already handles low disorder well). (Reference: `learning_log/algorithms-adaptive.md`)

#### 2. Extras beyond subject requirements — DECIDED (2026-05-26)

- **mini_sort (sort_2 / sort_3 / sort_5)** → **NOT IMPLEMENTED**. Out of subject scope; focus on the four mandatory tiers.
- No other polish flagged at this time.

#### 3. Role division — DECIDED (2026-05-26)

See the updated role table below. Phase 1 and Phase 2 follow a **parallel-draft-then-compare** workflow: both partners independently draft their own version, then compare and merge into a final version. Algorithm tiers: Simple & Medium → Javier; Complex → So.

#### 4. Data structure — DECIDED (2026-05-26)

**Option A: doubly linked list + `tail` pointer.**

- `t_node` holds `value`, `index`, `next`, `prev`
- `t_stack` holds `top`, `tail`, `size`

Justification in `learning_log/linked-list-design.md`: makes every push_swap operation O(1) without the bug-prone cycle detection of Option B (circular doubly linked). Whether `t_stack` holds `a` and `b` together or as separate instances is a follow-up implementation detail.

#### 5. Input pre-processing — DECIDED (2026-05-26)

**Approach A: apply coordinate compression at parser stage for all tiers.** All algorithms operate on `t_node->index` rather than `value`.

| Tier / use | Status |
| --- | --- |
| Complex (Radix LSD) | **Required** — fixes bit count at `⌈log₂(n)⌉`. |
| Medium (Chunk Sort) | **Useful** — chunk boundaries become uniform slices of `[0, n)`. |
| Simple (Insertion Sort) | **Order-preserving** — `index` comparisons give the same result as `value` comparisons. |
| Adaptive disorder calculation | **Order-preserving** — works the same on indices. |

Implementation belongs to Phase 1 (parser stage). Cost is one-time O(n log n) on the C side and generates **zero push_swap operations**, so it doesn't affect the subject's complexity measurement.

#### 6. Adaptive design — SETTLED BY TOPIC 1

With Simple = Insertion Sort, the dispatcher simply calls the tier algorithm in each regime. The subject-required README documentation (rationale for thresholds, internal techniques per regime, complexity argument) still needs writing — see the role table.

#### 7. Code conventions — PENDING
Norm, file/function naming, where libft sits, operation wrapper signature (the README suggests `op_sa(t_stack *a, t_bench *bench, int print)`).

#### 8. Git workflow — PARTIALLY DECIDED (2026-05-26)

**Decided**: for the parallel-draft phases (Phase 1, Phase 2), **each partner creates their own branch** and works on their version independently. Comparison and merge happen afterward.

**Pending**: branch naming convention, review process, commit message format, merge strategy for non-parallel phases, where the final-version branch lives.

#### 9. Milestones — PENDING
Target dates for each phase (counting back from the campus deadline).

### Three "why?" questions we both must be able to answer (required for defense)

- Why **doubly linked list + `tail` pointer** (vs an array, singly linked, or circular doubly)? → all 11 push_swap operations stay O(1); rotation/reverse-rotation preserve cross-element order through cyclic shifts; clearer to debug than circular (`NULL` as end marker vs cycle-detection loops). See `learning_log/linked-list-design.md`.
- What **input pre-processing** do we apply — and why? → **Coordinate compression at parser stage for all tiers** (Approach A). Required for Radix LSD (fixes bit count at `log₂(n)`); useful for Medium (uniform chunks); order-preserving so no harm for Simple.
- Why does **each chosen algorithm** achieve its claimed complexity class **in the push_swap operation model**? — concrete arguments needed for Insertion Sort, Chunk Sort + in-chunk sort, Radix LSD, and the Adaptive dispatcher.

### Role split (DECIDED 2026-05-26)

| Phase | Drafter | Workflow / Notes |
| --- | --- | --- |
| 1: stack / parser base **(incl. coordinate compression)** | both | parallel draft on separate branches → compare → final version |
| 2: 11 operations | both | parallel draft on separate branches → compare → final version |
| 4: Simple (Insertion Sort) | **Javier** | |
| 5: Medium (Chunk Sort + in-chunk sort) | **Javier** | |
| 6: Complex (Radix LSD) | **So** | coord compression handled in Phase 1 |
| 7: Adaptive (dispatcher) | both | small enough to co-build |
| 8: Benchmark | So → `disorder.c`; Javier → `benchmark.c`, op counter | |
| README | So → algo complexity argument; Javier → data structure & overview; both → integration | |

> Phase 3 (mini_sort) removed since extras are excluded (Topic 2).

Rationale: drafters split, but every piece is **co-reviewed** — both must be able to explain any line during defense (subject IX).

### Decisions (2026-05-26)

- **Topic 1 — Algorithms**: Simple = Insertion Sort; Medium = Chunk Sort + in-chunk sort; Complex = Radix Sort LSD (binary); Adaptive = direct dispatch to tier algorithms.
- **Topic 2 — Extras**: mini_sort not implemented.
- **Topic 3 — Role split**: as in the table above. Phase 1 & 2 use parallel-draft-then-compare workflow.
- **Topic 4 — Data structure**: Option A — doubly linked + `tail` pointer.
- **Topic 5 — Input pre-processing**: Approach A — apply coordinate compression at parser stage for all tiers. All algorithms operate on `t_node->index`.
- **Topic 8 (partial) — Git workflow**: parallel phases (1, 2) use one branch per partner. Other Git policy details pending.
- **Implied settlements**: Topic 6 (no algorithm differentiation needed in Adaptive).

### Open questions / remaining

- **Topic 7 — Code conventions**: Norm, naming, wrapper signature.
- **Topic 8 (details) — Git workflow**: branch naming convention, review process, commit message format, merge strategy for non-parallel phases, where the final-version branch lives.
- **Topic 9 — Milestones**: deadline + phase target dates.
- **Data-structure follow-up**: does `t_stack` hold `a` and `b` together, or as separate instances? (Implementation detail; decide before Phase 1 implementation.)
- **README writing breakdown**: how documentation is split in finer detail.

### Action items

- [ ] Both: create your own Git branch for Phase 1 work (parallel-draft begins).
- [ ] So: plan Phase 6 implementation (Radix LSD; coord compression already provided by Phase 1).
- [ ] Javier: plan Phase 4 (Insertion Sort) and Phase 5 (Chunk Sort + in-chunk sort).
- [ ] Both: schedule a follow-up meeting to resolve Topic 7, Topic 9, and Topic 8 details.

---

## 日本語

### 参加者
- So
- Javier

### 背景
相方が提案ファイル構成(`docs:possible files` コミット)と 824 行の README を共有してくれた後、初の同期ミーティング。
So は subject を読み、相方のプランをレビュー済み。
実装着手前に **アルゴリズム選定・スコープ・役割分担** を合意するのが目的。

**So の事前準備(2026-05-20 / 2026-05-21 / 2026-05-26)**:Simple、Medium、Complex、Adaptive tier すべてを詳しく調査し、Topic 1 への提案を準備済み。分析内容は `learning_log/algorithms-simple-o-n2.md`、`algorithms-medium-o-nsqrtn.md`、`algorithms-complex-o-nlogn.md`、`algorithms-adaptive.md` を参照。データ構造の分析は `learning_log/linked-list-design.md`。

### 議題

#### 1. 各 tier のアルゴリズム選定 — DECIDED(2026-05-26)

- **Simple O(n²)** → **Insertion Sort**。部分整列入力に強く、`--simple` フラグと Adaptive の低 disorder レジームで同じ algorithm を呼ぶ。(参照:`learning_log/algorithms-simple-o-n2.md`)
- **Medium O(n√n)** → **Chunk Sort + in-chunk sort**。(参照:`learning_log/algorithms-medium-o-nsqrtn.md`)
- **Complex O(n log n)** → **Radix Sort LSD(2 進)**。座標圧縮が必須(Topic 5 参照)。(参照:`learning_log/algorithms-complex-o-nlogn.md`)
- **Adaptive** → ディスパッチャは各レジームで対応する tier algorithm を直接呼ぶ。algorithm の差別化は不要(Simple = Insertion Sort が低 disorder にも適している)。(参照:`learning_log/algorithms-adaptive.md`)

#### 2. 課題要件外の実装(extras) — DECIDED(2026-05-26)

- **mini_sort(sort_2 / sort_3 / sort_5)** → **実装しない**。subject 範囲外、4 つの必須 tier に集中する。
- 他に flag したい polish は現時点でなし。

#### 3. 役割分担 — DECIDED(2026-05-26)

下の更新された役割分担テーブル参照。Phase 1 と Phase 2 は **「各自並行ドラフト → 比較 → 統合」**ワークフロー:両者が独立して自分版を書き、比較して最終版を作る。アルゴリズム tier:Simple と Medium → Javier、Complex → So。

#### 4. データ構造 — DECIDED(2026-05-26)

**Option A:双方向連結リスト + `tail` ポインタ**。

- `t_node` は `value`, `index`, `next`, `prev` を持つ
- `t_stack` は `top`, `tail`, `size` を持つ

根拠は `learning_log/linked-list-design.md` 参照:Option B(循環双方向)の cycle 検出によるバグリスクなしに、push_swap の全 11 命令を O(1) で実行できる。`t_stack` で a と b を統合するか別 instance にするかは実装時の詳細(下記の follow-up 項目)。

#### 5. 入力の前処理 — DECIDED(2026-05-26)

**Approach A:parser stage で全 tier に座標圧縮を適用**。全アルゴリズムは `value` ではなく `t_node->index` で動作する。

| Tier / 用途 | 座標圧縮の位置付け |
| --- | --- |
| Complex (Radix LSD) | **必須** —— bit 数を `⌈log₂(n)⌉` に固定するため。 |
| Medium (Chunk Sort) | **有用** —— chunk 境界が `[0, n)` の均等分割になる。 |
| Simple (Insertion Sort) | **順序保存** —— `index` 比較と `value` 比較は同じ結果。 |
| Adaptive disorder 計算 | **順序保存** —— index で計算しても結果同じ。 |

実装は Phase 1(parser stage)で行う。コストは C 側で一回限り O(n log n)、**push_swap 操作は 0 個生成**するので subject の計算量測定には影響しない。

#### 6. Adaptive 設計 — SETTLED BY TOPIC 1

Simple = Insertion Sort なので、ディスパッチャは各レジームで対応する tier algorithm を呼ぶだけ。subject 要求の README 文書化(しきい値の根拠、各レジームの内部技法、計算量議論)は別途必要 ——役割分担テーブル参照。

#### 7. コード規約 — PENDING
Norm 準拠、ファイル/関数命名、libft の置き場所、operation ラッパーの引数(README は `op_sa(t_stack *a, t_bench *bench, int print)` を提案)。

#### 8. Git 運用 — PARTIALLY DECIDED(2026-05-26)

**決定**:並行ドラフトの Phase(Phase 1、Phase 2)では **各自が自分のブランチを作成**して独立に作業する。比較と統合は後で行う。

**未決定**:ブランチ命名規則、レビュープロセス、コミットメッセージ形式、並行 Phase 以外でのマージ戦略、最終版ブランチの置き場所。

#### 9. マイルストーン — PENDING
各 Phase の目標期日(キャンパス締切から逆算)。

### 「3つのなぜ?」(両者が説明できる必要 = ディフェンス必須)

- なぜ **双方向連結リスト + `tail` ポインタ**(配列・単方向・循環双方向ではなく)? → push_swap の全 11 命令が O(1) のまま、回転/逆回転で循環シフトとして要素間の相対順序が保たれる、循環双方向よりデバッグしやすい(終端が `NULL` で明示的、サイクル検出ループ不要)。詳細は `learning_log/linked-list-design.md`。
- 採用する **入力の前処理** は何か、なぜか? → **parser stage で全 tier に座標圧縮**(Approach A)。Radix LSD で必須(bit 数を `log₂(n)` に固定)、Medium で有用(均等チャンク化)、Simple では順序保存で害なし。
- 採用する **4 つのアルゴリズム** それぞれが、**push_swap 操作モデル** で主張する計算量クラスを満たす理由は? ——Insertion Sort / Chunk Sort + in-chunk sort / Radix LSD / Adaptive dispatcher について具体的な議論が必要。

### 役割分担(DECIDED 2026-05-26)

| Phase | 下書き担当 | ワークフロー / 備考 |
| --- | --- | --- |
| 1: stack / parser 基盤 **(座標圧縮含む)** | 両者 | 別々のブランチで並行ドラフト → 比較 → 統合 |
| 2: 11 個の operations | 両者 | 別々のブランチで並行ドラフト → 比較 → 統合 |
| 4: Simple (Insertion Sort) | **Javier** | |
| 5: Medium (Chunk Sort + in-chunk sort) | **Javier** | |
| 6: Complex (Radix LSD) | **So** | 座標圧縮は Phase 1 で実装 |
| 7: Adaptive(ディスパッチャ) | 両者 | 規模が小さいので共同 |
| 8: Benchmark | So → `disorder.c`、Javier → `benchmark.c`、op counter | |
| README | So → アルゴリズム計算量議論、Javier → データ構造・全体構成、両者 → 統合 | |

> Phase 3(mini_sort)は extras 除外のため削除(Topic 2)。

根拠: 下書きは分担するが、各ピースは **必ず相互レビュー**。両者がどの行でも説明できる状態にする(subject IX)。

### 決定事項(2026-05-26)

- **Topic 1 — アルゴリズム**:Simple = Insertion Sort、Medium = Chunk Sort + in-chunk sort、Complex = Radix Sort LSD(2 進)、Adaptive = tier algorithm への直接ディスパッチ。
- **Topic 2 — Extras**:mini_sort 実装しない。
- **Topic 3 — 役割分担**:上記テーブル通り。Phase 1 & 2 は並行ドラフト → 比較 → 統合ワークフロー。
- **Topic 4 — データ構造**:Option A —— 双方向連結リスト + `tail` ポインタ。
- **Topic 5 — 入力前処理**:Approach A —— parser stage で全 tier に座標圧縮を適用。全アルゴリズムは `t_node->index` で動作。
- **Topic 8(部分)— Git 運用**:並行 Phase(1, 2)は各自 1 ブランチ。その他の Git 方針は未決定。
- **間接的に決定したもの**:Topic 6(Adaptive で algorithm 差別化不要)。

### 未解決事項

- **Topic 7 — コード規約**:Norm、命名、ラッパー署名。
- **Topic 8(詳細)— Git 運用**:ブランチ命名規則、レビュープロセス、コミットメッセージ形式、並行 Phase 以外のマージ戦略、最終版ブランチの置き場所。
- **Topic 9 — マイルストーン**:締切と各 Phase の目標期日。
- **データ構造の follow-up**:`t_stack` で a と b を統合するか、別 instance にするか?(実装詳細、Phase 1 実装前に決定)
- **README 執筆の細分化**:文書化の分担をもう一段細かく。

### 次のアクション

- [ ] 両者:Phase 1 用の自分のブランチを作成(並行ドラフト開始)。
- [ ] So:Phase 6(Radix LSD)の実装計画。座標圧縮は Phase 1 で提供される。
- [ ] Javier:Phase 4(Insertion Sort)と Phase 5(Chunk Sort + in-chunk sort)の実装計画。
- [ ] 両者:Topic 7、Topic 9、Topic 8 の詳細を解決するための follow-up ミーティング日程調整。

---

## 2026-06-02 Async update / 非同期更新 — Phase 2 plan & integration decisions

### English

#### Context

So could not attend the planned follow-up meeting on 2026-06-01. Async exchange with Javier (2026-06-01 evening → 2026-06-02 morning) resolved the immediate path forward without a synchronous meeting. This section records what was decided async and updates the open-questions / action-items lists above.

#### Decided async (2026-06-02)

- **Pair meeting deferred** until after both partners complete Phase 2 (the 11 operations). Integration meeting will combine: parser merge review + ops integration discussion.
- **Parser merge ownership**: So takes the parser merge unilaterally. Javier acknowledged the 12 integration topics in [`2026-05-31_phase2-integration-prep.md`](2026-05-31_phase2-integration-prep.md), accepted So's recommendations, and committed to fixing his side's `safe_atol` independently on his branch.
- **Phase 2 workflow confirmed**: both partners continue implementing their own version of the 11 operations in parallel on their existing branches (consistent with Topic 3's parallel-draft pattern). Each partner drafts ALL 11 operations; integration happens after both are complete.

#### 12-topic integration decisions

Detailed rationale per topic is in [`2026-05-31_phase2-integration-prep.md`](2026-05-31_phase2-integration-prep.md) under the "Solo decisions log" section.

| # | Topic | Adopted approach | Source |
|---|---|---|---|
| 1 | Library strategy | libft as subproject | So |
| 2 | Error stream | stderr (fd=2) | So |
| 3 | Error emission | Centralized in `main` | So |
| 4 | Error return | `t_status` enum | So |
| 5 | t_stack allocation | Stack-allocated (`stack_init(&a)`) | Javier |
| 6 | Field name | `bottom` (not `tail`) | Javier |
| 7 | Node creation | `create_node` + `stack_add_back` split | Javier |
| 8 | Duplicate check timing | After all pushes, once | So |
| 9 | coord_compress algorithm | In-place counting | So |
| 10 | Flag duplication | Error on ≥ 2 flags | Javier |
| 11 | Flag-only input | Accept (no-op success) | So |
| 12 | atoi safety | `safe_atoi` (long long + digit cap) | So |

Tally: 7 from So's draft, 5 from Javier's draft — balanced adoption.

#### Updated open questions

- **Topic 9 — Milestones**: still pending. Target dates per phase from the campus deadline.
- **Topic 8 (residual details)**: commit message format remains provisional (`<type>(<scope>):` form used informally). Final convention to be set at the integration meeting.
- **Final branch policy**: where the merged codebase lives (after parser merge). Currently TBD; decide at integration meeting.

#### Updated action items

- [x] Both: keep operating on individual branches for Phase 2 (parallel draft).
- [ ] So: complete the parser merge per the 12 decisions above, on a new branch.
- [ ] So: implement own draft of Phase 2 (11 operations).
- [ ] Javier: implement own draft of Phase 2 (11 operations).
- [ ] Javier: fix his side's `safe_atol` per Topic 12 (acknowledged).
- [ ] Both: schedule integration meeting after both Phase 2 drafts are complete.

---

### 日本語

#### 背景

2026-06-01 の follow-up ミーティングに So が参加できなかったため、2026-06-01 夕方〜2026-06-02 朝の Javier との非同期やりとりで当面の方針を決定。同期ミーティングなしで進捗を維持するための合意内容を本セクションに記録し、上記の「未解決事項」「次のアクション」を更新する。

#### 非同期決定事項(2026-06-02)

- **ペアミーティングを延期**:両者の Phase 2(11 命令)完了後に再設定。統合ミーティングでは parser merge レビューと ops 統合議論を併せて実施。
- **Parser merge 担当**:So が単独で parser merge を担当。Javier は [`2026-05-31_phase2-integration-prep.md`](2026-05-31_phase2-integration-prep.md) の 12 topics を読了し、So の推奨方針を受諾、`safe_atol` は自身のブランチで独立に修正することを宣言。
- **Phase 2 ワークフロー確定**:両者は既存のブランチで 11 命令を独立に並行実装(Topic 3 の並行ドラフトパターンと整合)。各自が全 11 命令をドラフト、統合は両者完了後。

#### 12 topic 統合決定

各トピックの詳細根拠は [`2026-05-31_phase2-integration-prep.md`](2026-05-31_phase2-integration-prep.md) の "Solo decisions log" 参照。

| # | Topic | 採用方針 | 出所 |
|---|---|---|---|
| 1 | ライブラリ路線 | libft をサブプロジェクト | So |
| 2 | Error 出力先 | stderr (fd=2) | So |
| 3 | Error 出力の集約 | `main` で 1 箇所 | So |
| 4 | エラー戻り値 | `t_status` enum | So |
| 5 | t_stack 配置 | stack 上(`stack_init(&a)`) | Javier |
| 6 | フィールド名 | `bottom`(`tail` ではなく) | Javier |
| 7 | ノード生成 | `create_node` + `stack_add_back` 分離 | Javier |
| 8 | duplicate 検出タイミング | 全 push 後に 1 回 | So |
| 9 | coord_compress | in-place カウント | So |
| 10 | flag 重複 | 2 つ以上で Error | Javier |
| 11 | flag-only 入力 | 受け入れる(no-op success) | So |
| 12 | atoi 安全性 | `safe_atoi`(long long + 桁制限) | So |

集計:So 側 7 件、Javier 側 5 件採用 ——バランスの取れた採用。

#### 更新された未解決事項

- **Topic 9 — マイルストーン**:依然 pending。キャンパス締切から逆算した各 Phase 目標期日。
- **Topic 8(残り詳細)**:コミットメッセージ規約は暫定(`<type>(<scope>):` 形式を informal に使用中)。統合ミーティングで正式決定。
- **最終ブランチ方針**:統合後のコードベースをどのブランチに置くか(parser merge 後)。現在 TBD、統合ミーティングで決定。

#### 更新されたアクション

- [x] 両者:Phase 2 用に各自のブランチで作業継続(並行ドラフト)。
- [ ] So:上記 12 決定に従って新ブランチで parser merge 実施。
- [ ] So:Phase 2(11 命令)自分側ドラフト実装。
- [ ] Javier:Phase 2(11 命令)自分側ドラフト実装。
- [ ] Javier:Topic 12 に従って自分側の `safe_atol` 修正(受諾済み)。
- [ ] 両者:両方の Phase 2 ドラフト完了後に統合ミーティング日程調整。

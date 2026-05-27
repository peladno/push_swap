# Implementation Plans Overview / 実装計画 概観

> Big-picture map: how the plan files fit together, how the resulting program is wired at runtime, and what order to implement the phases.

## English

### Purpose of this file

- `README.md` explains the **folder's conventions** (granularity, naming, common structure).
- `overview.md` (this file) shows the **big picture** — file map, dependency graph, runtime call flow, phase order.

Use this file as the navigation entry point when starting any new phase.

### File map

```
implementation_plans/
├── README.md                       — folder conventions, current plans index
├── TEMPLATE.md                     — blank starter for new plans
├── overview.md                     — THIS FILE: big-picture map
│
├── data-structure.md               — Phase 1: t_node, t_stack types + helpers
├── coordinate-compression.md       — Phase 1: value → rank index mapping
├── parser.md                       — Phase 1: argv → validated stack a (orchestrator)
│
├── simple-insertion-sort.md        — Phase 4: Insertion Sort
├── medium-chunk-sort.md            — Phase 5: Chunk Sort + in-chunk sort
├── complex-radix-lsd.md            — Phase 6: Radix Sort LSD (binary)
└── adaptive-dispatcher.md          — Phase 7: disorder → tier dispatch
```

### Dependency graph (which plan uses which)

```
main()
 └── parse_args() ............................... parser.md
      ├── stack primitives ...................... data-structure.md
      └── coord_compress() ...................... coordinate-compression.md

main() then dispatches to ONE of:
 ├── insertion_sort() ............................ simple-insertion-sort.md
 ├── chunk_sort() ............................... medium-chunk-sort.md
 ├── radix_sort_lsd() ............................ complex-radix-lsd.md
 └── adaptive_sort() ............................. adaptive-dispatcher.md
      └── inspects disorder, then calls ONE
          of the three sort algorithms above.

All four sort algorithms:
 • use stack primitives from data-structure.md (sa/pa/ra/... build on these)
 • read t_node->index, set up by coord_compress() at parse time
```

### Runtime call flow (one invocation)

```
1. main() receives argv, argc.

2. main() calls parse_args() from parser.md:
   2a. Detect strategy flag (--simple / --medium / --complex / --adaptive)
   2b. Validate each token (digit-only, in INT range)
   2c. Push validated values onto stack_a (uses data-structure helpers)
   2d. Detect duplicates → "Error\n" + exit on failure
   2e. Call coord_compress(stack_a) → assigns t_node->index everywhere

3. main() dispatches on the strategy:
     STRATEGY_SIMPLE    → insertion_sort(a, b, bench)
     STRATEGY_MEDIUM    → chunk_sort(a, b, bench)
     STRATEGY_COMPLEX   → radix_sort_lsd(a, b, bench)
     STRATEGY_ADAPTIVE  → adaptive_sort(a, b, bench)
                          └── compute_disorder(a) then dispatch internally

4. The chosen algorithm emits push_swap ops to stdout (and updates bench counters).

5. If --bench was specified, main() prints the bench summary to stderr.

6. main() frees memory and exits 0.
```

### Phase implementation order

From the kickoff role table. **Suggested order top to bottom**:

| Phase | Plan files in scope | Drafter | Workflow |
| --- | --- | --- | --- |
| 1 | `data-structure.md`, `coordinate-compression.md`, `parser.md` | both | parallel draft on separate branches → compare → merge |
| 2 | (11 push_swap operations — no separate plan yet; consider `operations-11.md`) | both | parallel draft on separate branches → compare → merge |
| 4 | `simple-insertion-sort.md` | Javier | solo draft |
| 5 | `medium-chunk-sort.md` | Javier | solo draft |
| 6 | `complex-radix-lsd.md` | So | solo draft |
| 7 | `adaptive-dispatcher.md` | both | small enough to co-build |
| 8 | (benchmark mode — no plan yet; consider `benchmark-mode.md`) | So + Javier split | solo drafts |

> Phase 3 was `mini_sort`; excluded per kickoff Topic 2.

### Phase 1 internal order (most useful right now)

Inside Phase 1, the dependency direction matters. Implement in this order so each piece can call into the one before:

1. **`data-structure.md`** — the foundation; everything else uses `t_node` and stack helpers.
2. **`coordinate-compression.md`** — needs `t_stack` from step 1; called by the parser in step 3.
3. **`parser.md`** — orchestrator; uses both of the above.

### Reading guide for a new phase

When you sit down to implement a phase:

1. Read **`overview.md`** (this file) to refresh the big picture.
2. Read **the plan files for your phase** in dependency order.
3. Follow each plan's "Depends on" metadata to find related plans.
4. Consult `learning_log/` only when you need the **why** (rationale, alternatives considered).

If something in a plan is unclear or needs revising: **update the plan first, then implement**. The plan should always reflect what we actually intend to build.

### What this overview does NOT cover

- Per-function pseudo-code → see the individual plan files.
- Rationale for design choices (e.g., why Option A for data structure) → see `learning_log/`.
- Meeting decisions and role assignments → see `pair_communication/kickoff.md`.
- Daily progress notes → see `work_log/`.

---

## 日本語

### このファイルの目的

- `README.md` は**フォルダの規約**(粒度、命名、共通構造)を説明する。
- `overview.md`(本ファイル)は**全体像**を示す ——ファイルマップ、依存グラフ、実行時の呼び出しフロー、Phase の順序。

新しい Phase に着手する時、最初に開くナビゲーションエントリーポイントとして使う。

### ファイルマップ

```
implementation_plans/
├── README.md                       — フォルダ規約、現在のプラン一覧
├── TEMPLATE.md                     — 新規プラン用の空テンプレート
├── overview.md                     — このファイル:全体像マップ
│
├── data-structure.md               — Phase 1: t_node, t_stack 型 + helper
├── coordinate-compression.md       — Phase 1: 値 → rank index 割り当て
├── parser.md                       — Phase 1: argv → 検証済みスタック a(orchestrator)
│
├── simple-insertion-sort.md        — Phase 4: Insertion Sort
├── medium-chunk-sort.md            — Phase 5: Chunk Sort + in-chunk sort
├── complex-radix-lsd.md            — Phase 6: Radix Sort LSD(2 進)
└── adaptive-dispatcher.md          — Phase 7: disorder → tier ディスパッチ
```

### 依存グラフ(どのプランがどれを使うか)

```
main()
 └── parse_args() ............................... parser.md
      ├── スタック primitives .................... data-structure.md
      └── coord_compress() ...................... coordinate-compression.md

main() は次のいずれか 1 つにディスパッチ:
 ├── insertion_sort() ............................ simple-insertion-sort.md
 ├── chunk_sort() ............................... medium-chunk-sort.md
 ├── radix_sort_lsd() ............................ complex-radix-lsd.md
 └── adaptive_sort() ............................. adaptive-dispatcher.md
      └── disorder を測り、上記 3 つのいずれかを
          内部で呼ぶ

4 つのソートアルゴリズムすべて:
 • data-structure.md のスタック primitives を使う(sa/pa/ra/... はこの上に乗る)
 • parse 時に coord_compress() が設定した t_node->index を読む
```

### 実行時の呼び出しフロー(1 回の起動)

```
1. main() が argv、argc を受け取る。

2. main() が parser.md の parse_args() を呼ぶ:
   2a. 戦略フラグ(--simple / --medium / --complex / --adaptive)を識別
   2b. 各トークンを検証(数字のみ、INT 範囲内)
   2c. 検証済みの値を stack_a に積む(data-structure helper を使う)
   2d. 重複検出 → 失敗時 "Error\n" + exit
   2e. coord_compress(stack_a) で全ノードに t_node->index を割り当て

3. main() が strategy でディスパッチ:
     STRATEGY_SIMPLE    → insertion_sort(a, b, bench)
     STRATEGY_MEDIUM    → chunk_sort(a, b, bench)
     STRATEGY_COMPLEX   → radix_sort_lsd(a, b, bench)
     STRATEGY_ADAPTIVE  → adaptive_sort(a, b, bench)
                          └── compute_disorder(a) して内部ディスパッチ

4. 選ばれたアルゴリズムが push_swap 命令を stdout に出力(bench カウンタも更新)。

5. --bench 指定があれば、main() が bench サマリを stderr に出力。

6. main() がメモリ解放、exit 0。
```

### Phase 実装順序

kickoff の役割分担テーブルから。**上から順に着手するのが推奨**:

| Phase | 対応するプランファイル | 下書き担当 | ワークフロー |
| --- | --- | --- | --- |
| 1 | `data-structure.md`、`coordinate-compression.md`、`parser.md` | 両者 | 別々のブランチで並行ドラフト → 比較 → 統合 |
| 2 | (11 個の push_swap 命令 —— プラン未作成。`operations-11.md` の追加を検討) | 両者 | 別々のブランチで並行ドラフト → 比較 → 統合 |
| 4 | `simple-insertion-sort.md` | Javier | 単独ドラフト |
| 5 | `medium-chunk-sort.md` | Javier | 単独ドラフト |
| 6 | `complex-radix-lsd.md` | So | 単独ドラフト |
| 7 | `adaptive-dispatcher.md` | 両者 | 規模が小さいので共同 |
| 8 | (ベンチマークモード —— プラン未作成。`benchmark-mode.md` の追加を検討) | So + Javier 分担 | 単独ドラフト |

> Phase 3 は `mini_sort`、kickoff Topic 2 で除外。

### Phase 1 内部の実装順序(今すぐ役立つ)

Phase 1 の中では依存方向が重要。各ピースが前のものを呼べるように、この順で実装する:

1. **`data-structure.md`** —— 土台。すべてが `t_node` とスタック helper を使う。
2. **`coordinate-compression.md`** —— ステップ 1 の `t_stack` を必要とする。ステップ 3 の parser から呼ばれる。
3. **`parser.md`** —— orchestrator。上記 2 つを使う。

### 新しい Phase を始める時の読み順ガイド

Phase 実装に着手する時:

1. **`overview.md`**(このファイル)で全体像を再確認。
2. **担当 Phase のプランファイル**を依存順に読む。
3. 各プランの metadata の "Depends on" を辿って関連プランも参照。
4. **なぜそうしたか**(根拠、検討した代替案)が知りたい時のみ `learning_log/` を見る。

プランの中で不明瞭な点や見直しが必要な点があれば、**先にプランを更新してから実装**。プランは常に「実際に作るもの」を反映する。

### この overview がカバーしないもの

- 関数単位の擬似コード → 個別のプランファイル参照。
- 設計判断の根拠(例:なぜデータ構造で Option A か) → `learning_log/` 参照。
- ミーティング決定事項と役割分担 → `pair_communication/kickoff.md` 参照。
- 日次の進捗 → `work_log/` 参照。

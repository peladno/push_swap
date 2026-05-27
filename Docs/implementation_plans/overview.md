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

### Project source layout (submitted deliverables)

The proposed layout of the actual code we submit. Per-tier subfolders under `src/algorithms/` follow the partner's original skeleton; rejected items (mini_sort, multiple Simple variants) are removed.

```
push_swap/                              ← Git repo root
├── Makefile                            top-level Makefile
├── README.md                           project README (for graders)
├── Docs/                               internal pair-coordination docs
├── include/
│   └── push_swap.h                     all types + function prototypes
├── libft/                              libft as a subproject
│   ├── Makefile
│   ├── libft.h
│   └── ... (libft sources)
└── src/
    ├── main.c                          entry: parse → dispatch → (bench)
    │
    ├── parser/                         Phase 1 — argv → validated indexed stack a
    │   ├── parse_args.c                orchestrator
    │   ├── validate_input.c            per-token validation
    │   ├── check_duplicates.c
    │   └── coordinate_compression.c
    │
    ├── stack/                          Phase 1 — t_stack lifecycle + primitives
    │   ├── stack_init.c                stack_init, stack_free
    │   └── stack_utils.c               stack_push_top, stack_pop_top
    │
    ├── operations/                     Phase 2 — the 11 push_swap operations
    │   ├── swap.c                      sa, sb, ss
    │   ├── push.c                      pa, pb
    │   ├── rotate.c                    ra, rb, rr
    │   ├── reverse_rotate.c            rra, rrb, rrr
    │   └── operations_utils.c          emit_op() helper (print + bench update)
    │
    ├── algorithms/                     Phases 4–7
    │   ├── simple/
    │   │   └── insertion_sort.c        Phase 4
    │   ├── medium/
    │   │   └── chunk_sort.c            Phase 5
    │   ├── complex/
    │   │   └── radix_sort.c            Phase 6
    │   └── adaptive/
    │       ├── adaptive_sort.c         Phase 7 dispatcher
    │       └── disorder.c              compute_disorder
    │
    ├── benchmark/                      Phase 8 — --bench output
    │   └── benchmark.c                 stderr formatting
    │
    └── utils/
        └── error_exit.c                "Error\n" + cleanup + exit
```

About 20 source files. Each file is small enough to fit Norm constraints (≤ 5 functions / ≤ 25 lines per function).

### Plan ↔ source file mapping

| Plan | Source files implementing it |
| --- | --- |
| `data-structure.md` | `include/push_swap.h` (types), `src/stack/stack_init.c`, `src/stack/stack_utils.c` |
| `coordinate-compression.md` | `src/parser/coordinate_compression.c` |
| `parser.md` | `src/main.c` (driver), `src/parser/parse_args.c`, `validate_input.c`, `check_duplicates.c` |
| (Phase 2 ops — plan TBD) | `src/operations/*.c` |
| `simple-insertion-sort.md` | `src/algorithms/simple/insertion_sort.c` |
| `medium-chunk-sort.md` | `src/algorithms/medium/chunk_sort.c` |
| `complex-radix-lsd.md` | `src/algorithms/complex/radix_sort.c` |
| `adaptive-dispatcher.md` | `src/algorithms/adaptive/adaptive_sort.c`, `disorder.c` |
| (Phase 8 — plan TBD) | `src/benchmark/benchmark.c` |
| — (cross-cutting) | `src/utils/error_exit.c` |

### `include/push_swap.h` — outline

- System includes: `<stdlib.h>` (malloc/free/exit), `<unistd.h>` (write)
- libft: `#include "libft.h"`
- Type definitions: `t_node`, `t_stack`, `t_strategy`, `t_bench`
- Constants (if using integer threshold comparison for disorder): `DISORDER_LOW_NUM`, `DISORDER_LOW_DEN`, etc.
- All public function prototypes

### `Makefile` — outline

| Element | Value |
| --- | --- |
| `NAME` | `push_swap` |
| `CC` | `cc` |
| `CFLAGS` | `-Wall -Wextra -Werror -I include -I libft` |
| Source list | all `src/**/*.c` |
| Required rules | `$(NAME)`, `all`, `clean`, `fclean`, `re` |
| Behavior | builds libft first → compiles sources → links into `push_swap`. **No unnecessary relinking** (subject II) |
| Future | `bonus` rule for the optional `checker` program |

### Submitted files (per subject VI.5 / VII)

- `Makefile` at the repo root
- `include/push_swap.h`
- All `src/**/*.c`
- `libft/` (with its own Makefile)
- `README.md` at the repo root (project README per subject VII)

`Docs/` is tracked in Git for pair coordination and defense support; not strictly required by subject.

### Bonus (future-proofing)

Subject's bonus is the `checker` program. Per common instructions, bonus files go in `*_bonus.{c/h}`. If we add it later:

- `src/checker_bonus.c` (or a `src/checker/` subfolder)
- `include/checker_bonus.h`
- Makefile `bonus` rule

Currently NOT implementing bonus (kickoff Topic 2 decision).

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

### プロジェクトソース構成(提出成果物)

実際に提出するコードの構成案。`src/algorithms/` 下の tier 別サブフォルダは相方の元スケルトンを踏襲。除外項目(mini_sort、Simple の複数バリアント)は削除済み。

```
push_swap/                              ← Git リポジトリのルート
├── Makefile                            トップレベル Makefile
├── README.md                           プロジェクト README(採点者向け)
├── Docs/                               内部ペア連携用 docs
├── include/
│   └── push_swap.h                     全型定義 + 関数プロトタイプ
├── libft/                              サブプロジェクト libft
│   ├── Makefile
│   ├── libft.h
│   └── ... (libft sources)
└── src/
    ├── main.c                          エントリ:parse → dispatch → (bench)
    │
    ├── parser/                         Phase 1 — argv → 検証済み index 付スタック a
    │   ├── parse_args.c                orchestrator
    │   ├── validate_input.c            トークン単位の検証
    │   ├── check_duplicates.c
    │   └── coordinate_compression.c
    │
    ├── stack/                          Phase 1 — t_stack のライフサイクル + primitives
    │   ├── stack_init.c                stack_init, stack_free
    │   └── stack_utils.c               stack_push_top, stack_pop_top
    │
    ├── operations/                     Phase 2 — 11 個の push_swap 命令
    │   ├── swap.c                      sa, sb, ss
    │   ├── push.c                      pa, pb
    │   ├── rotate.c                    ra, rb, rr
    │   ├── reverse_rotate.c            rra, rrb, rrr
    │   └── operations_utils.c          emit_op() helper(print + bench 更新)
    │
    ├── algorithms/                     Phase 4–7
    │   ├── simple/
    │   │   └── insertion_sort.c        Phase 4
    │   ├── medium/
    │   │   └── chunk_sort.c            Phase 5
    │   ├── complex/
    │   │   └── radix_sort.c            Phase 6
    │   └── adaptive/
    │       ├── adaptive_sort.c         Phase 7 ディスパッチャ
    │       └── disorder.c              compute_disorder
    │
    ├── benchmark/                      Phase 8 — --bench 出力
    │   └── benchmark.c                 stderr 整形
    │
    └── utils/
        └── error_exit.c                "Error\n" + クリーンアップ + exit
```

ソースファイル数は約 20 個。各ファイルは Norm 制約(関数 ≤ 5 / 1 関数 ≤ 25 行)に収まる粒度。

### プラン ↔ ソースファイル対応

| プラン | 対応するソースファイル |
| --- | --- |
| `data-structure.md` | `include/push_swap.h`(型)、`src/stack/stack_init.c`、`src/stack/stack_utils.c` |
| `coordinate-compression.md` | `src/parser/coordinate_compression.c` |
| `parser.md` | `src/main.c`(driver)、`src/parser/parse_args.c`、`validate_input.c`、`check_duplicates.c` |
| (Phase 2 命令 — プラン未作成) | `src/operations/*.c` |
| `simple-insertion-sort.md` | `src/algorithms/simple/insertion_sort.c` |
| `medium-chunk-sort.md` | `src/algorithms/medium/chunk_sort.c` |
| `complex-radix-lsd.md` | `src/algorithms/complex/radix_sort.c` |
| `adaptive-dispatcher.md` | `src/algorithms/adaptive/adaptive_sort.c`、`disorder.c` |
| (Phase 8 — プラン未作成) | `src/benchmark/benchmark.c` |
| — (横断) | `src/utils/error_exit.c` |

### `include/push_swap.h` — 構成

- システムインクルード:`<stdlib.h>`(malloc/free/exit)、`<unistd.h>`(write)
- libft:`#include "libft.h"`
- 型定義:`t_node`、`t_stack`、`t_strategy`、`t_bench`
- 定数(disorder のしきい値比較を整数で行う場合):`DISORDER_LOW_NUM`、`DISORDER_LOW_DEN` 等
- 全 public 関数のプロトタイプ

### `Makefile` — 構成

| 要素 | 値 |
| --- | --- |
| `NAME` | `push_swap` |
| `CC` | `cc` |
| `CFLAGS` | `-Wall -Wextra -Werror -I include -I libft` |
| ソースリスト | `src/**/*.c` すべて |
| 必須ルール | `$(NAME)`、`all`、`clean`、`fclean`、`re` |
| 挙動 | libft を先にビルド → ソースをコンパイル → `push_swap` にリンク。**不要な再リンクをしない**(subject II) |
| 将来 | optional な `checker` 用の `bonus` ルール |

### 提出ファイル(subject VI.5 / VII)

- リポジトリルートの `Makefile`
- `include/push_swap.h`
- `src/**/*.c` すべて
- `libft/`(独自 Makefile 含む)
- リポジトリルートの `README.md`(subject VII のプロジェクト README)

`Docs/` は Git で追跡されるがペア連携・ディフェンス補助用で、subject の必須要件ではない。

### Bonus(将来の拡張余地)

subject の bonus は `checker` プログラム。common instructions によるとボーナスファイルは `*_bonus.{c/h}` に置く。後で追加する場合の構成:

- `src/checker_bonus.c`(または `src/checker/` サブフォルダ)
- `include/checker_bonus.h`
- Makefile の `bonus` ルール

現時点では bonus は実装しない(kickoff Topic 2 の決定)。

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

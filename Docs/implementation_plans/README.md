# Implementation Plans / 実装計画

## English

This directory holds **focused implementation plans for the designs adopted at the kickoff meeting**. Each file specifies WHAT to build and HOW (at the design level), without exhaustive code bodies.

> **👉 Start with [`overview.md`](overview.md)** for the big-picture map: file structure, dependency graph, runtime call flow, and recommended phase order. This `README.md` covers the folder conventions only.

### Difference from `learning_log/`

| | `learning_log/` | `implementation_plans/` |
| --- | --- | --- |
| Purpose | Research, comparison, history | Build contracts for adopted designs |
| Includes rejected options? | Yes (kept for defense context) | No |
| Has implementation details? | Conceptual only | Design level (interfaces + step-by-step), no full code |
| Updated after kickoff? | Rarely | Yes — reflects current design |

### File naming

kebab-case: e.g., `data-structure.md`, `complex-radix-lsd.md`. Match the design unit name used in `kickoff.md`.

### Common structure (per file)

- **Metadata**: when decided, drafter, dependencies, references
- **Scope**: what's included, what's explicitly NOT covered
- **Interface**: function signatures and data flow (no function bodies)
- **Implementation outline**: step-by-step approach (pseudo-code where helpful)
- **Edge cases**: boundary conditions
- **Complexity argument**: for README and peer-eval defense
- **Testing**: verification strategy

See `TEMPLATE.md` to start a new plan file.

### Current plans

| File | kickoff Topic | Drafter | Phase |
| --- | --- | --- | --- |
| `data-structure.md` | Topic 4 | both | 1 |
| `coordinate-compression.md` | Topic 5 | both | 1 |
| `parser.md` | (subject-driven) | both | 1 |
| `simple-insertion-sort.md` | Topic 1 (Simple) | Javier | 4 |
| `medium-chunk-sort.md` | Topic 1 (Medium) | Javier | 5 |
| `complex-radix-lsd.md` | Topic 1 (Complex) | So | 6 |
| `adaptive-dispatcher.md` | Topic 1 (Adaptive) + Topic 6 | both | 7 |

### Granularity policy

- **Do include**: function signatures, struct definitions, pseudo-code outlining the algorithm, invariants, edge case lists.
- **Do NOT include**: complete C function bodies, full error-handling boilerplate, low-level memory layout discussions. Those belong in the source code itself.

The goal is "enough specification to start implementing and to compare with the partner's draft", not "code that compiles".

---

## 日本語

このディレクトリは **kickoff ミーティングで採用された設計の実装計画** を集約する場所。各ファイルは「**何を作るか・どう作るか**」を設計レベルで specify する(完全なコード本体は含まない)。

> **👉 まず [`overview.md`](overview.md) を読む** —— ファイル構成、依存グラフ、実行時の呼び出しフロー、推奨 Phase 順序がまとまっている。この `README.md` はフォルダ規約のみ扱う。

### `learning_log/` との違い

| | `learning_log/` | `implementation_plans/` |
| --- | --- | --- |
| 目的 | 研究・比較・経緯 | 採用設計の実装契約 |
| 不採用オプションを含む? | はい(ディフェンス用に保持) | いいえ |
| 実装詳細を含む? | 概念のみ | 設計レベル(インターフェース + 段階的手順)。完全なコードは含まない |
| kickoff 後に更新する? | 稀 | はい — 現在の設計を反映 |

### ファイル命名

kebab-case:例 `data-structure.md`, `complex-radix-lsd.md`。`kickoff.md` で使った設計単位名と一致させる。

### 共通構造(各ファイル)

- **Metadata**:決定日、担当者、依存、参照
- **Scope**:含むもの・明示的に含まないもの
- **Interface**:関数シグネチャとデータの流れ(関数本体は書かない)
- **Implementation outline**:段階的な実装手順(必要なら擬似コード)
- **Edge cases**:境界条件
- **Complexity argument**:README とピア評価ディフェンス用
- **Testing**:動作確認方針

新規作成時は `TEMPLATE.md` を出発点に。

### 現在の計画

| ファイル | kickoff Topic | 担当 | Phase |
| --- | --- | --- | --- |
| `data-structure.md` | Topic 4 | 両者 | 1 |
| `coordinate-compression.md` | Topic 5 | 両者 | 1 |
| `parser.md` | (subject 由来) | 両者 | 1 |
| `simple-insertion-sort.md` | Topic 1(Simple) | Javier | 4 |
| `medium-chunk-sort.md` | Topic 1(Medium) | Javier | 5 |
| `complex-radix-lsd.md` | Topic 1(Complex) | So | 6 |
| `adaptive-dispatcher.md` | Topic 1(Adaptive)+ Topic 6 | 両者 | 7 |

### 粒度方針

- **含めるもの**:関数シグネチャ、struct 定義、アルゴリズム手順の擬似コード、invariant、エッジケース一覧
- **含めないもの**:完全な C 関数本体、エラー処理の boilerplate、低レベルのメモリレイアウト議論 ——これらはソースコード本体に書く

目標は「**実装に着手できる + 相方のドラフトと比較できる仕様**」であって「コンパイル可能なコード」ではない。

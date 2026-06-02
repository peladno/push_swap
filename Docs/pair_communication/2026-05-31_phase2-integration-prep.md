# 2026-05-31 — Phase 2 着手前のコード統合議論 / Pre-Phase 2 code integration discussion

> **2026-06-02 update**: The synchronous pair meeting was deferred. Javier read this doc, accepted So's recommendations, and committed to fixing his side's `safe_atol` independently. So will execute the parser merge unilaterally based on the decisions recorded in the **Solo decisions log** at the bottom of this file. The original "議題と現状比較" sections are preserved as historical context for the integration work.
>
> **2026-06-02 更新**:同期ミーティングは延期。Javier は本 doc を読了し、So の推奨方針を受諾、自身の `safe_atol` は独立に修正することを宣言。So は本ファイル末尾の **Solo decisions log** に基づき parser merge を単独で実施。「議題と現状比較」以下のセクションは統合作業時の参照用に保存。

## English

### Participants
- So, Javier

### Context

Phase 1 (M1–M4) is complete on both sides independently:
- **So**: `src/` (parser, stack, compress) + libft subproject
- **Javier**: `src/javi/` (parser, stack, compress, utils)

Before starting Phase 2 (the 11 push_swap operations), we need to **converge into one shared codebase** with consistent conventions. This document lists the design judgments that diverged between our drafts, presents trade-offs, and proposes a recommended path so the meeting can focus on decisions, not discovery.

### Goal of the meeting
1. Pick one option for each topic below
2. Decide who refactors which side
3. Re-confirm Phase 2 task split

### Scope
- Conventions, types, file layout, error handling
- NOT covered: Phase 2 operation implementation details (separate meeting)

---

## 日本語

### 参加者
- So, Javier

### 背景

Phase 1(M1〜M4)はそれぞれ独立して完了済み:
- **So**:`src/` 配下(parser, stack, compress)+ libft をサブプロジェクト統合
- **Javier**:`src/javi/` 配下(parser, stack, compress, utils)

Phase 2(11 個の push_swap 命令)に着手する前に、**両者のコードを 1 つに収束**させ、規約を統一する必要があります。本文書は両ドラフトで分岐した設計判断を一覧化し、トレードオフと推奨方針を提示することで、ミーティングを「発見」ではなく「決定」に集中させるためのものです。

### ミーティングの目的
1. 各議題について 1 つの方針を選択
2. どちら側を refactor する担当決め
3. Phase 2 のタスク分担再確認

### スコープ
- 規約、型、ファイル配置、エラー処理
- 対象外:Phase 2 命令の実装詳細(別ミーティング)

---

## Topics overview / 議題と現状比較

### Mapping table / 全体マッピング表

| # | Area / 領域 | So's draft / So 流 | Javier's draft / Javier 流 | Priority / 優先度 |
|---|---|---|---|---|
| 1 | Library / ライブラリ路線 | libft as subproject / libft をサブモジュール | Custom copies in `src/javi/utils/` / 自家製コピー | 🔴 |
| 2 | Error stream / Error 出力先 | `fd=2` (stderr) `ft_putstr_fd` | `fd=1` (stdout) `write` | 🔴 |
| 3 | Error emission / Error 出力の集約 | Centralized in `main` / `main` で 1 箇所 | Scattered in helpers / 各 helper で呼ぶ | 🔴 |
| 4 | Error return / エラー戻り値 | `t_status` enum | `int 0/1` (mixed convention) | 🔴 |
| 5 | t_stack allocation / 配置 | Heap (`stack_init` returns `*`) | Stack (`stack_init(&a)`) | 🟡 |
| 6 | Field name / フィールド名 | `top` / **`tail`** / `size` | `top` / **`bottom`** / `size` | 🔴 |
| 7 | Node creation / ノード生成 | All-in-one `stack_push_bottom` | Split: `create_node` + `stack_add_back` | 🟡 |
| 8 | Duplicate check timing / duplicate 検出 | After all pushes (once) / 全 push 後に 1 回 | Per push / push のたびに | 🟢 |
| 9 | coord_compress algorithm | In-place counting / in-place カウント | Copy → sort → search / 配列→ソート→検索 | 🟢 |
| 10 | Flag duplication / flag 重複 | Last one wins / 最後勝ち | Error on ≥ 2 / 2 つ以上で Error | 🟡 |
| 11 | Flag-only input / flag-only 入力 | Accept / 受け入れる | Error | 🟢 |
| 12 | atoi safety / atoi 安全性 | `safe_atoi` (long long + digit limit) | `ft_atol` (long) | 🟡 |

Legend / 凡例:
- 🔴 **Required / 必須**:Must decide before Phase 2 starts (shared header depends on it) / Phase 2 開始前に決定必須(共通ヘッダが必要なため)
- 🟡 **Strongly recommended / 推奨**:Clear recommendation, confirm at meeting / 強い推奨方針あり、最終決定はミーティングで
- 🟢 **Optional / 任意**:Either works, decide by preference / どちらも妥当、好みで決定

---

## Per-topic detail / 各議題の詳細

### 🔴 1. Library strategy / ライブラリ路線

**English**

- **So**: libft integrated as a subproject via `make -C $(LIBFT_DIR)`.
- **Javier**: Custom minimal copies in `src/javi/utils/` (`ft_split.c`, `ft_strcmp.c`, `ft_atol.c`).
- **Trade-off**:
  - libft route: DRY, easier maintenance, subject allows it (section II).
  - Custom copies: minimal dependency, final tarball includes only what's used.
- **Recommended**: **libft route (So's approach)**. Javier's `utils/` to be dropped at integration.
- **Owner of refactor**: Javier (replace utils with libft calls).

**日本語**

- **So**:libft をサブプロジェクトとして `make -C $(LIBFT_DIR)` で統合。
- **Javier**:`src/javi/utils/` に自家製コピー(`ft_split.c`, `ft_strcmp.c`, `ft_atol.c`)。
- **トレードオフ**:
  - libft 路線:DRY、メンテ性◎、subject 許可済(II 節)。
  - 自家製コピー:依存最小、最終 tar に utils/ が含まれる。
- **推奨**:**libft 路線(So 流)**。Javier の `utils/` は統合時に廃止。
- **修正担当**:Javier(utils 呼び出しを libft に置き換え)。

---

### 🔴 2. Error stream / Error 出力先

**English**

- **So**: `ft_putstr_fd("Error\n", 2)` — **stderr**.
- **Javier**: `write(1, "Error\n", 6)` — **stdout** ([src/javi/utils/error_msg.c:17](src/javi/utils/error_msg.c#L17)).
- **Trade-off**:
  - Subject doesn't specify the stream.
  - Convention: errors go to stderr; peer evaluators usually expect stderr.
- **Recommended**: **`fd=2` (stderr)**.
- **Owner of refactor**: Javier (change `error_msg` to write to fd=2, or remove it entirely if we centralize — see Topic 3).

**日本語**

- **So**:`ft_putstr_fd("Error\n", 2)` — **stderr**。
- **Javier**:`write(1, "Error\n", 6)` — **stdout**([src/javi/utils/error_msg.c:17](src/javi/utils/error_msg.c#L17))。
- **トレードオフ**:
  - subject に出力先指定なし。
  - 慣習的にエラーは stderr、ピア評価でも stderr が問われる可能性大。
- **推奨**:**`fd=2`(stderr)** で統一。
- **修正担当**:Javier(`error_msg` 関数の引数変更 or 削除 — Topic 3 と連動)。

---

### 🔴 3. Error emission policy / Error 出力の集約

**English**

- **So**: Parser functions only return `STATUS_ERROR`; `main` emits the string once.
- **Javier**: Each helper calls `error_msg()` (e.g., 4 times in `parse_string_arg.c`).
- **Trade-off**:
  - Centralized: changing stream / format is a one-line edit in `main` (maintainability ◎).
  - Scattered: easy to vary messages per context — but the subject mandates a fixed string, so this advantage is moot.
- **Recommended**: **Centralized (So's approach)**. Drop `error_msg()` or restrict it to `main`.
- **Owner of refactor**: Javier.

**日本語**

- **So**:parser 関数は `STATUS_ERROR` を return するだけ、`main` で 1 度だけ出力。
- **Javier**:各 helper で `error_msg()` を呼ぶ(`parse_string_arg.c` 内で 4 回など)。
- **トレードオフ**:
  - 集約型:出力先や形式変更が `main` の 1 行修正で済む(保守性◎)。
  - 散在型:エラー文脈に応じたメッセージを出しやすい(が、subject は固定文字列)。
- **推奨**:**`main` 集約型(So 流)**。`error_msg()` 関数は廃止 or `main` 専用に。
- **修正担当**:Javier。

---

### 🔴 4. Error return convention / エラー戻り値

**English**

- **So**: `t_status` enum (`STATUS_OK = 0`, `STATUS_ERROR = 1`); success/failure functions return `t_status`.
- **Javier**: Plain `int` — `0`=success, `1`=failure, **except** `coord_compress` which is **inverted** (`1`=success, `0`=failure).
- **Trade-off**:
  - `t_status`: intent encoded in the type; `!= STATUS_OK` reads cleanly.
  - Plain `int`: easier to write, but the **mixed convention** (`coord_compress` inverted) is an accident waiting to happen.
- **Recommended**: **`t_status` unified (So's approach)**. Predicates (e.g., `has_duplicates`) stay as bare `int`.
- **Owner of refactor**: Javier.

**日本語**

- **So**:`t_status` enum(`STATUS_OK = 0`, `STATUS_ERROR = 1`)、success/failure 系関数は `t_status` で揃える。
- **Javier**:`int` のまま、成功 `0` / 失敗 `1`(`coord_compress` だけ逆:成功 `1` / 失敗 `0`)。
- **トレードオフ**:
  - `t_status` 流:意図が型で documented、`!= STATUS_OK` で読みやすい。
  - `int` 流:書きやすいが、`if (!coord_compress())` のような **convention の混在**が事故を呼ぶ。
- **推奨**:**`t_status` 統一(So 流)**。predicate(`has_duplicates`)は `int` で残す。
- **修正担当**:Javier。

---

### 🟡 5. t_stack allocation / t_stack の配置

**English**

- **So**: `t_stack *stack_init(void)` — malloc inside, returns heap pointer.
- **Javier**: `void stack_init(t_stack *stack)` — caller declares `t_stack a;` on the stack, function only initializes.
- **Trade-off**:
  - Javier's approach: **one fewer malloc** (t_stack itself doesn't need heap), one fewer failure path, simpler.
  - So's approach: easy to pass `t_stack *` around — but Javier's already does pointer passing.
- **Recommended**: **Javier's approach (stack-allocated t_stack)**.
- **Owner of refactor**: So (change `stack_init` signature, declare `t_stack a;` in `main`).

**日本語**

- **So**:`t_stack *stack_init(void)` — 関数内で malloc、戻り値が heap pointer。
- **Javier**:`void stack_init(t_stack *stack)` — 呼び出し側で `t_stack a;` と stack 上に確保、関数は初期化のみ。
- **トレードオフ**:
  - Javier 流:**malloc 回数 -1**(t_stack 自体は heap 不要)、failure path も 1 つ減る、シンプル。
  - So 流:`t_stack *` を関数間で渡しやすい(が、すでに pointer 渡し)。
- **推奨**:**Javier 流(stack 上配置)**。
- **修正担当**:So(`stack_init` のシグネチャ変更、`main` で `t_stack a;` 宣言)。

---

### 🔴 6. Field name / フィールド名

**English**

- **So**: `t_stack { t_node *top; t_node *tail; int size; }`
- **Javier**: `t_stack { t_node *top; t_node *bottom; int size; }`
- **Trade-off**:
  - `tail`: linked-list idiom (e.g., STL `list::tail`).
  - `bottom`: stack metaphor (the bottom of a stack), matches subject's "top/bottom" wording, intuitive for beginners.
- **Recommended**: **`bottom` (Javier's approach)** — aligns with the subject's vocabulary.
- **Owner of refactor**: So (rename `tail` → `bottom`).

**日本語**

- **So**:`t_stack { t_node *top; t_node *tail; int size; }`
- **Javier**:`t_stack { t_node *top; t_node *bottom; int size; }`
- **トレードオフ**:
  - `tail`:リンクリスト的、慣習的(STL の `list::tail`)。
  - `bottom`:スタック直感(底)、subject の "top/bottom" 用語に近い、初学者に直感的。
- **推奨**:**`bottom`(Javier 流)** — subject 用語と一致。
- **修正担当**:So(`tail` → `bottom` の rename)。

---

### 🟡 7. Node creation responsibility split / ノード生成の責任分離

**English**

- **So**: `stack_push_bottom(stack, value, index)` — malloc + init + link, all in one function.
- **Javier**: `create_node(value)` for node alloc/init, `stack_add_back(stack, value)` for linking.
- **Trade-off**:
  - All-in-one (So): fewer functions, sufficient for Phase 1.
  - Split (Javier): single responsibility, more reusable.
  - **However**, Phase 2 ops (`sa`, `pa`, `ra`, etc.) **rewire existing nodes only** (no malloc) → `create_node` reuse opportunity is limited.
- **Recommended**: **Either works**. By YAGNI, **So's approach is sufficient**. The name `stack_add_back` (Javier's) is more idiomatic than `stack_push_bottom`.
  - Option A: Keep So's structure, rename `stack_push_bottom` → `stack_add_back`.
  - Option B: Adopt Javier's split (`create_node` + `stack_add_back`).
- **Decision needed at meeting**: A or B.

**日本語**

- **So**:`stack_push_bottom(stack, value, index)` で malloc + 初期化 + リンクを 1 関数。
- **Javier**:`create_node(value)` でノード生成 + `stack_add_back(stack, value)` でリンク。
- **トレードオフ**:
  - 統合型(So):関数数少、Phase 1 で十分。
  - 分離型(Javier):単一責任、再利用性◎。
  - **ただし** Phase 2 命令(`sa`, `pa`, `ra` 等)は **既存ノードの再配線のみ** で malloc しない → `create_node` の再利用機会は限定的。
- **推奨**:**どちらでも OK**。**YAGNI 観点では So 流のままで十分**。`stack_add_back`(Javier)命名は `push_bottom` より一般的で直感的。
  - 案 A:So 流維持で `stack_push_bottom` → `stack_add_back` に rename のみ。
  - 案 B:Javier 流(`create_node` + `stack_add_back` に分離)を採用。
- **ミーティングで決定**:A or B。

---

### 🟢 8. Duplicate check timing / duplicate 検出タイミング

**English**

- **So**: After all pushes, call `has_duplicates(stack_a)` once (single O(n²) sweep).
- **Javier**: Check `has_duplicate(a, value)` before every push (per-push O(n), n times).
- **Trade-off**:
  - Complexity: both O(n²), asymptotically identical.
  - Javier's: detects duplicates **before** the redundant push — fail-fast, but the partial stack is held until error path frees it (handled correctly).
  - So's: parser does one final validation sweep at the end — cleaner separation of phases.
- **Recommended**: **Either works**. Decide by readability.
- **Decision needed at meeting**.

**日本語**

- **So**:全 push 後に `has_duplicates(stack_a)` を 1 回呼ぶ(全 O(n²) 探索)。
- **Javier**:push のたびに `has_duplicate(a, value)` で既存値スキャン(都度 O(n) を n 回)。
- **トレードオフ**:
  - 計算量は両方 O(n²)、漸近的に同じ。
  - Javier 流:重複検出時 **そこまでに push 済みのメモリを抱えたまま** error 経路へ(`free_stack` で解放されるので OK)。fail-fast。
  - So 流:parser の最後で一括検証 — フェーズ分離が綺麗。
- **推奨**:**どちらでも OK**。コード可読性で議論。
- **ミーティングで決定**。

---

### 🟢 9. coord_compress algorithm / coord_compress の実装

**English**

- **So**: For each node, count how many other nodes have a smaller value. In-place.
- **Javier**: Copy values to an array → bubble sort → `find_index` for each node's position.
- **Trade-off**:
  - Both O(n²).
  - So's: no extra malloc, shorter code.
  - Javier's: composed of reusable parts (`stack_to_array`, `sort_int_array`, `find_index`), educationally interesting, useful prep for radix sort.
- **Recommended**: **Either works**. **So's is shorter and has fewer failure paths**, so easier for integration.
- **Decision needed at meeting**.

**日本語**

- **So**:各 node について「自分より小さい node の数をカウント」。in-place。
- **Javier**:配列にコピー → bubble sort → `find_index` で位置検索。
- **トレードオフ**:
  - 両方 O(n²)。
  - So 流:malloc なし、コード短い。
  - Javier 流:汎用パーツ(`stack_to_array`, `sort_int_array`, `find_index`)で構成、教育的、radix の準備にも。
- **推奨**:**どちらでも OK**。**So 流の方が短く失敗パスも少ない**ので、統合作業の楽さでは So 流推奨。
- **ミーティングで決定**。

---

### 🟡 10. Flag duplication detection / flag 重複検出

**English**

- **So**: `check_strategy` overwrites `*out_strategy` each time → **last one wins**. `./push_swap --simple --medium 1 2 3` runs with `MEDIUM`.
- **Javier**: Counts flags via `flag_count`, returns Error on ≥ 2.
- **Trade-off**:
  - Strict (Javier): rejects ambiguous intent, defensive — subject silent on this.
  - Lenient (So): "run what we can parse" — subject silent on this.
- **Recommended**: **Javier's strict approach (Error on ≥ 2)**. Defensive.
- **Owner of refactor**: So (add count logic to `check_strategy` caller).

**日本語**

- **So**:`check_strategy` が flag を見るたび `*out_strategy` を上書き → **最後勝ち**。`./push_swap --simple --medium 1 2 3` は `MEDIUM` で実行される。
- **Javier**:`flag_count` で 2 つ以上の flag を Error。
- **トレードオフ**:
  - 厳格(Javier):ユーザの意図不明を `Error` で拒否、subject に明示無いが defensive。
  - 寛容(So):動くものは動かす、subject に明示無い。
- **推奨**:**Javier 流(2 つ以上で Error)**。defensive。
- **修正担当**:So(`check_strategy` 呼び出し側で count)。

---

### 🟢 11. Flag-only input / flag-only 入力の扱い

**English**

- **Input**: `./push_swap --simple` (no numbers).
- **So**: Accept (empty stack, `STATUS_OK`, main prints debug only).
- **Javier**: `check_empty_after_flag` returns **Error**.
- **Trade-off**:
  - Subject silent on this.
  - Strict: "flag without target is invalid input."
  - Lenient: "no target to sort means no-op, same as `./push_swap` alone."
- **Recommended**: **Discuss**. Personal lean: **lenient** (consistent with `./push_swap` alone).
- **Decision needed at meeting**.

**日本語**

- **入力**:`./push_swap --simple`(数値なし)。
- **So**:受け入れる(空 stack で `STATUS_OK` 返す → main は debug print のみ)。
- **Javier**:`check_empty_after_flag` で **Error**。
- **トレードオフ**:
  - subject に明示無し。
  - 厳格:「ソート対象なしは引数不正」と解釈。
  - 寛容:「ソート対象なしは何もしないのが正しい」と解釈。
- **推奨**:**議論**。個人的には **寛容**(`./push_swap` 単体と同じ扱い)が一貫していて好み。
- **ミーティングで決定**。

---

### 🟡 12. atoi overflow safety / atoi の overflow 安全性

**English**

- **So**: `safe_atoi` — `long long` accumulator + digit-count cap (11 / 12) + INT range check.
- **Javier**: `ft_atol` — `long` accumulator + post-conversion INT range check.
- **Trade-off**:
  - Javier's weakness: `99999999999999999999` (exceeds LONG) → `long` overflow → **undefined behavior**, the subsequent range check is unreliable.
  - So's: digit-count cap prevents `long long` overflow before it happens → **always well-defined**.
- **Recommended**: **So's `safe_atoi`**.
- **Owner of refactor**: Javier (drop `ft_atol`, adopt `safe_atoi`).

**日本語**

- **So**:`safe_atoi`(`long long` 累積 + 桁数制限 11/12 + INT 範囲チェック)。
- **Javier**:`ft_atol`(`long` 累積 + 後段で INT 範囲チェック)。
- **トレードオフ**:
  - Javier 流の弱点:`99999999999999999999`(LONG 超過)で `long` overflow → **未定義動作**、後段のチェックが信用できない。
  - So 流:桁数制限で `long long` overflow を未然に防ぐ → **常に well-defined**。
- **推奨**:**So 流(`safe_atoi`)**。
- **修正担当**:Javier(`ft_atol` を破棄、`safe_atoi` を採用)。

---

## Phase 2 task split (re-confirmation) / Phase 2 担当割り(再確認用)

### English

The 11 operations to be split between So and Javier (confirm at meeting; kickoff.md Topic 9 may already have a tentative split).

### 日本語

11 命令を分担(ミーティングで確定。kickoff.md Topic 9 で仮決め済みなら再掲)。

### Split table / 分担表

| Op / 命令 | Owner / 担当 | Note / メモ |
|---|---|---|
| `sa` / `sb` / `ss` | TBD | swap group / swap 系 |
| `pa` / `pb` | TBD | push group / push 系 |
| `ra` / `rb` / `rr` | TBD | rotate group / rotate 系 |
| `rra` / `rrb` / `rrr` | TBD | reverse rotate group / reverse rotate 系 |

→ To be confirmed at meeting / ミーティングで分担確定。

---

## Solo decisions log (2026-06-02) / 単独決定記録(2026-06-02)

### English

The synchronous meeting was deferred to after Phase 2. Javier reviewed this doc and accepted So's recommendations. So records the final decisions below for the parser merge work, which So owns unilaterally.

#### Decisions

| # | Topic | Decision | Source draft | Rationale source |
|---|---|---|---|---|
| 1 🔴 | Library strategy | libft as subproject | So | Doc Recommended |
| 2 🔴 | Error stream | stderr (fd=2) | So | Doc Recommended |
| 3 🔴 | Error emission | Centralized in `main` | So | Doc Recommended |
| 4 🔴 | Error return | `t_status` enum | So | Doc Recommended |
| 5 🟡 | t_stack allocation | Stack-allocated | Javier | Doc Recommended |
| 6 🔴 | Field name | `bottom` | Javier | Doc Recommended |
| 7 🟡 | Node creation | Option B: `create_node` + `stack_add_back` | Javier | So's solo call — single responsibility, easier to defend in review |
| 8 🟢 | Duplicate check timing | After all pushes, once | So | So's solo call — clean phase separation, already working in Phase 1 |
| 9 🟢 | coord_compress algorithm | In-place counting | So | So's solo call — no extra malloc, shorter code, less failure surface for merge |
| 10 🟡 | Flag duplication | Error on ≥ 2 flags | Javier | Doc Recommended |
| 11 🟢 | Flag-only input | Accept (no-op success) | So | So's solo call — consistent with `./push_swap` alone |
| 12 🟡 | atoi safety | `safe_atoi` (long long + digit cap) | So | Doc Recommended; Javier acknowledged he'll align his side |

Tally: 7 So / 5 Javier. Balanced.

#### Branch layout note (2026-06-02)

The parser-merge work happens on a new branch off `so/phase1-stack-parser` (which already lacks `src/javi/` — it is a parallel-draft branch independent of Javier's tree). The action items below are scoped to that branch.

**Deferred to the future main-integration step**: removing `src/javi/`, `error_msg.c`, and Javier's `stack_to_array.c` / `sort_int_array.c` / `find_index.c` from `main`. These exist on `main` (merged from `feature/parser/javi`) and on `feature/operations`. Javier is currently working on `feature/operations` for Phase 2 swap ops and has touched `src/javi/` files there. Before deleting `src/javi/` from `main`, we need to confirm with Javier that his ongoing ops work is fully captured on `feature/operations` (or another sub-branch) so the deletion doesn't conflict.

#### Action items — today's parser-merge branch

- [ ] So: checkout `so/phase1-stack-parser`, branch off as `so/parser-merge`.
- [ ] So: T6 — rename `tail` → `bottom` across header and stack helpers.
- [ ] So: T5 — change `stack_init` signature to `void stack_init(t_stack *)`, update `main.c` to use `t_stack a;`, reset fields in `stack_free` after node teardown.
- [ ] So: T7 — split node creation into `create_node(int value)` + `stack_add_back(t_stack *, int)`. Drop the unused `index` parameter.
- [ ] So: T10 — track `flag_count` in `parse_args`; return `STATUS_ERROR` on `≥ 2`.
- [ ] So: verify with the existing 17 test cases + new cases (`./push_swap --simple --medium 1 2 3` → Error; `./push_swap --simple` → success) + valgrind across success and error paths.

#### Action items — deferred (future main integration)

- [ ] So: at main integration, remove `src/javi/` (after Javier confirms his ops work is preserved elsewhere).
- [ ] So: at main integration, drop `error_msg.c`, `stack_to_array.c`, `sort_int_array.c`, `find_index.c` from `main` (subsumed by `so/parser-merge`'s design).

#### Action items — Javier-side (independent)

- [ ] Javier: fix `safe_atol` on his side (he committed to this).
- [ ] Both: meet after Phase 2 ops drafts are complete to integrate.

---

### 日本語

同期ミーティングは Phase 2 後に延期。Javier は本 doc をレビューし、So の推奨方針を受諾。以下に So が parser merge 作業のために最終決定を記録(parser merge は So 単独で担当)。

#### 決定事項

| # | Topic | 決定 | 出所ドラフト | 根拠 |
|---|---|---|---|---|
| 1 🔴 | ライブラリ路線 | libft をサブプロジェクト | So | Doc 推奨 |
| 2 🔴 | Error 出力先 | stderr (fd=2) | So | Doc 推奨 |
| 3 🔴 | Error 出力の集約 | `main` で 1 箇所 | So | Doc 推奨 |
| 4 🔴 | エラー戻り値 | `t_status` enum | So | Doc 推奨 |
| 5 🟡 | t_stack 配置 | stack 上 | Javier | Doc 推奨 |
| 6 🔴 | フィールド名 | `bottom` | Javier | Doc 推奨 |
| 7 🟡 | ノード生成 | 案 B:`create_node` + `stack_add_back` | Javier | So の単独判断 —— 単一責任、レビューで説明しやすい |
| 8 🟢 | duplicate 検出タイミング | 全 push 後に 1 回 | So | So の単独判断 —— フェーズ分離が綺麗、Phase 1 で既に動作中 |
| 9 🟢 | coord_compress | in-place カウント | So | So の単独判断 —— 余分な malloc 不要、コード短い、merge での failure surface 少 |
| 10 🟡 | flag 重複 | 2 つ以上で Error | Javier | Doc 推奨 |
| 11 🟢 | flag-only 入力 | 受け入れる(no-op success) | So | So の単独判断 —— `./push_swap` 単体と一貫 |
| 12 🟡 | atoi 安全性 | `safe_atoi`(long long + 桁制限) | So | Doc 推奨;Javier も自身側で対応宣言 |

集計:So 7 件 / Javier 5 件。バランス良好。

#### ブランチ配置に関する注記(2026-06-02)

parser merge 作業は `so/phase1-stack-parser` から派生した新ブランチで実施。`so/phase1-stack-parser` は **すでに `src/javi/` を含まない**(Javier のツリーから独立した並行ドラフトブランチのため)。下記のアクションはこのブランチに閉じた話。

**将来の main 統合段階に繰越**:`main` および `feature/operations` 上の `src/javi/`、`error_msg.c`、Javier の `stack_to_array.c` / `sort_int_array.c` / `find_index.c` の削除。Javier は `feature/operations` で Phase 2 swap ops を進行中、`src/javi/` 配下のファイルに変更を加えている。**`main` から `src/javi/` を削除する前に**、Javier の進行中 ops が `feature/operations`(または別サブブランチ)に完全に保持されていることを本人と確認する必要がある。

#### アクション項目 — 本日の parser-merge ブランチ作業

- [ ] So:`so/phase1-stack-parser` を checkout、`so/parser-merge` として派生ブランチを切る。
- [ ] So:T6 — `tail` → `bottom` に rename(header と stack helper 全体)。
- [ ] So:T5 — `stack_init` シグネチャを `void stack_init(t_stack *)` に変更、`main.c` で `t_stack a;` 使用、`stack_free` 内でノード解放後にフィールド reset。
- [ ] So:T7 — ノード生成を `create_node(int value)` + `stack_add_back(t_stack *, int)` に分離。未使用 `index` 引数を削除。
- [ ] So:T10 — `parse_args` で `flag_count` を追跡、`≥ 2` で `STATUS_ERROR` を return。
- [ ] So:既存 17 テスト + 新テスト(`./push_swap --simple --medium 1 2 3` → Error、`./push_swap --simple` → 成功)+ 成功/失敗 path 両方の valgrind で確認。

#### アクション項目 — 将来繰越(main 統合時)

- [ ] So:main 統合時、`src/javi/` を削除(Javier の ops 作業の保持を本人と確認後)。
- [ ] So:main 統合時、`error_msg.c`、`stack_to_array.c`、`sort_int_array.c`、`find_index.c` を `main` から削除(`so/parser-merge` の設計に置換)。

#### アクション項目 — Javier 側(独立作業)

- [ ] Javier:自分側の `safe_atol` 修正(本人が宣言済み)。
- [ ] 両者:Phase 2 命令ドラフト完了後に統合ミーティング。

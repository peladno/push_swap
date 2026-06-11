# 2026-06-10 — Phase 2 ops integration / Phase 2 オペレーション統合

## English

### Participants
- So, Javier

### Context

Both partners have now completed independent drafts of the 11 push_swap operations:
- **So**: `so/phase2-operations` (4 commits, pushed; ops live in `src/operations/`)
- **Javier**: `feature/operations` merged into `main` on 2026-06-04 (`3ed41a1`); ops live in `src/javi/operations/`

The kickoff Topic 3 workflow ("parallel draft → compare → final version") now applies. This document compares the two drafts side-by-side and proposes a path to a single canonical Phase 2 layer so we can both move forward into Phase 5 / Phase 6 / Phase 7 from a shared base.

Heads-up on a side-effect of the 2026-06-04 merge: it brought `include/push_swap.h` back to the pre-parser-merge form (the form Javier had on `feature/operations`, before the 2026-06-02 parser-merge into `main`). The current `main` doesn't build right now because the existing `src/parser/*.c` and `src/stack/*.c` were compiled against the post-parser-merge header (`t_status` returns, `stack_free` / `stack_push_bottom` / `has_duplicates` naming, etc.). Resolving the ops integration is also the right moment to restore the header to the post-parser-merge state — they're the same change.

### High-level alignment (where both drafts agree)

Both drafts independently arrived at the same structure:

- **Family-grouped layout**: 4 files (`swap.c`, `push.c`, `rotate.c`, `reverse_rotate.c`) — each owning the worker plus the 2–3 wrappers that family generates.
- **`static` worker + thin wrappers**: the per-family worker handles pointer rewiring; each wrapper calls the worker and prints the op name on success.
- **`t_operation` enum** (`OP_NOT_DONE` / `OP_DONE`) — both used the same name and semantics.
- **Pure pointer manipulation**: no malloc/free inside ops.

This is a good sign — the broad design judgment landed identically on both sides without coordination.

### Per-file comparison

#### `swap.c` (Javier `src/javi/operations/swap.c` vs So `src/operations/swap.c`)

| Aspect | Javier's draft | So's draft | Note |
|---|---|---|---|
| Worker name | `swap_top` | `swap` | Naming preference |
| Worker return type | `int` | `t_operation` | Type-honesty |
| Wrapper return type | `void` | `t_operation` | Affects algorithm-tier signaling |
| NULL guard on stack | `if (!stack \|\| stack->size < 2)` | `if (stack->size < 2)` | Defensive vs trust-caller |
| Pointer rewiring | Correct | Correct | Both work for size 2 / size 3+ |
| `sa` / `sb` wrapper compares to | `STATUS_OK` | `OP_DONE` | See note below ¹ |
| `ss` semantics | Non-atomic (worker called on both, print only if both succeed) | Atomic (precondition checked on both BEFORE any mutation) | See note below ² |

¹ **Comparison values**: in the current enums, `STATUS_OK == 0 == OP_NOT_DONE`. Comparing the `t_operation` return against `STATUS_OK` is numerically equivalent to comparing against `OP_NOT_DONE` — so the `if (... == STATUS_OK) print` branch fires when the op did *not* happen, and is silent when the op *did* happen. Output ends up inverted from intent. Javier's `rotate.c` and `reverse_rotate.c` (added 2026-06-04, after `swap.c`) already use `== OP_DONE` — the earlier files just predate that pattern.

² **`ss` (and `rr` / `rrr`) atomicity**: the non-atomic form calls the worker on both stacks independently, then prints only if both succeed. The corner case is when one stack is below `size 2`: the other stack still gets mutated, but no op line is printed. `checker_42` replays the printed ops against the initial input and compares against the program's final state — a silent mutation that no op line explains is reported as KO. The atomic form checks both preconditions before any mutation, so it's either "do both + print" or "do nothing". So's draft adopts the atomic form. (Documented in `Docs/implementation_plans/operations.md` Wrapper / worker pattern section.)

#### `push.c`

| Aspect | Javier's draft | So's draft | Note |
|---|---|---|---|
| Worker name | `push` | `push` | Same |
| Worker parameter order | `(dst, src)` | `(stack_from, stack_to)` | Naming convention |
| Worker return type | `int` | `t_operation` | Type-honesty |
| Edge cases (empty src, empty dst, size 1) | Correctly handled | Correctly handled | Both right after the initial round of fixes |
| `node->prev = NULL` defensive reset | Yes (explicit) | Relies on top->prev invariant | Javier's is more defense-in-depth |
| Wrapper return type | `void` | `t_operation` | Same as swap |
| `pa` / `pb` wrapper compares to | `STATUS_OK` | `OP_DONE` | Same inversion as swap.c |

#### `rotate.c` (added by Javier on 2026-06-04)

| Aspect | Javier's draft | So's draft | Note |
|---|---|---|---|
| Worker name | `rotate` | `rotate` | Same |
| Worker return type | `t_operation` | `t_operation` | Same |
| Local count | 1 (`old_top`) | 3 (`first`, `second`, `last`) | Javier's is more concise; semantic name |
| Wrapper compares to | `OP_DONE` | `OP_DONE` | Both correct here ✅ |
| `rr` semantics | Non-atomic (`&&` short-circuit) | Atomic | Same nuance as `ss` — if `rotate(a)` succeeds but `b` fails, `a` is silently mutated |

#### `reverse_rotate.c` (added by Javier on 2026-06-04)

Same pattern as `rotate.c`: Javier's worker uses a single semantic local (`old_bottom`); So's uses three positional locals. Both pointer rewirings are correct. The `rrr` wrapper has the same non-atomic / silent-mutation nuance as `rr`.

### Proposal — what we converge on

Use **So's `src/operations/` as the canonical base**, with three improvements adopted from Javier's draft.

#### Why So's as base?

- `t_operation` propagated through wrappers gives algorithm tiers (Phase 4–7) a way to react to no-op cases (`if (sa(&a) == OP_NOT_DONE) ...`).
- Wrapper comparisons against `OP_DONE` consistently — no inversion risk.
- `ss`/`rr`/`rrr` atomic — no silent-mutation path into `checker_42`.
- Already documented in `Docs/implementation_plans/operations.md` (interface, edge-case matrix, complexity argument, testing approach).

#### Three improvements to adopt from Javier's draft

1. **Worker local naming** in `rotate.c` and `reverse_rotate.c`: rename `first` / `second` / `last` → `old_top` / `old_bottom` (single semantic local in the rotate variants). Easier to read and matches the intent of the function.
2. **`push.c` defensive reset**: add an explicit `node->prev = NULL` before reattaching the moved node to `dst`. Currently So's draft relies on the `top->prev == NULL` invariant; making it explicit costs one line and removes a load-bearing implicit assumption.
3. **NULL guards in workers**: add `if (!stack || ...)` (and equivalents for `push`'s two arguments). The algorithms shouldn't ever call ops with NULL stacks, but the guard is a one-condition addition that survives accidental misuse.

So will execute these on the `so/phase2-operations` branch before the merge.

### Open questions for Javier

- Are you OK with the proposal above (So's as base + 3 improvements from your draft)?
- Any insight from your draft I missed that you'd like to keep?
- Anything you'd like to weigh in on before the merge? (Naming, NULL-guard policy, etc.)

### Plan once we agree

1. So pushes the refactor (3 adopted improvements) to `so/phase2-operations`.
2. So merges `so/phase2-operations` into `main`. Conflict resolution: take `so/phase2-operations` for `include/push_swap.h`, `Makefile`, `src/main.c`. Restores the post-parser-merge canonical header (fixes the current build break) and adds `src/operations/`.
3. So removes `src/javi/operations/` in the same merge commit (matches the 2026-06-02 pattern for the parser tree, with explicit per-file acknowledgement this time since the ops files post-date the original deletion).
4. So verifies clean build + valgrind sweep on the merged main and pushes.
5. Both continue Phase 4 / 5 (Javier) and Phase 6 (So) from a shared canonical base.

### Action items

- [ ] Javier: review this doc and respond — go / change / additional input.
- [ ] So: while waiting, adopt the 3 improvements on `so/phase2-operations`.
- [ ] So: execute the merge once Javier acks.
- [ ] So: append the final outcome to `operations.md`'s "Known divergence" section so the integration record stays in-repo.

---

## 日本語

### 参加者
- So、Javier

### 背景

両者ともに 11 個の push_swap 命令の独立ドラフトが完成しました:
- **So**:`so/phase2-operations`(4 commit、push 済み、`src/operations/` 配下)
- **Javier**:`feature/operations` を 2026-06-04 に `main` に merge(`3ed41a1`)、`src/javi/operations/` 配下

kickoff Topic 3 のワークフロー(「並行ドラフト → 比較 → 最終版」)を適用するタイミングです。本ドキュメントで両ドラフトを並べて比較し、単一の canonical な Phase 2 層に収束する経路を提案します。これにより両者が Phase 5 / Phase 6 / Phase 7 を共有された基盤から進められます。

2026-06-04 の merge の副作用について共有:この merge で `include/push_swap.h` が parser-merge 前の形(Javier が `feature/operations` 上で持っていた、2026-06-02 の parser-merge 直前の形)に戻りました。現在の `main` はビルドできない状態 —— 既存の `src/parser/*.c` と `src/stack/*.c` は parser-merge 後のヘッダ(`t_status` 返却、`stack_free` / `stack_push_bottom` / `has_duplicates` の命名等)を前提にコンパイルされたためです。ops 統合を解決するタイミングは、同時にヘッダを parser-merge 後の状態に復元する適切なタイミング —— 同じ変更の一部だからです。

### 両ドラフトが合致する点(高レベル)

両者は調整なしに同じ構造に到達しました:

- **family グループ構造**:4 ファイル(`swap.c`、`push.c`、`rotate.c`、`reverse_rotate.c`)、各 family の worker + 2〜3 wrapper を 1 ファイルに集約。
- **`static` worker + 薄い wrapper**:family ごとの worker がポインタ付け替えを担当、各 wrapper は worker を呼んで成功時に op 名を print。
- **`t_operation` enum**(`OP_NOT_DONE` / `OP_DONE`)—— 両者が同じ名前・同じ意味で導入。
- **純粋ポインタ操作**:op 内で malloc/free なし。

設計判断の幅広い部分が無調整で同じ結論に至ったのは、よい兆候です。

### ファイル別比較

#### `swap.c`(Javier `src/javi/operations/swap.c` vs So `src/operations/swap.c`)

| 観点 | Javier ドラフト | So ドラフト | コメント |
|---|---|---|---|
| Worker 名 | `swap_top` | `swap` | 命名好み |
| Worker 返却型 | `int` | `t_operation` | 型の正確性 |
| Wrapper 返却型 | `void` | `t_operation` | algorithm tier への signaling に影響 |
| NULL guard | `if (!stack \|\| stack->size < 2)` | `if (stack->size < 2)` | 防衛的 vs caller 信頼 |
| ポインタ付け替え | 正しい | 正しい | size 2 / size 3+ 両方で動作 |
| `sa` / `sb` の比較対象 | `STATUS_OK` | `OP_DONE` | 下記 ¹ |
| `ss` のセマンティクス | 非 atomic(両 stack 独立に worker 呼び、両方成功時のみ print)| Atomic(両 stack の前提を mutation 前にチェック)| 下記 ² |

¹ **比較値について**:現在の enum 定義では `STATUS_OK == 0 == OP_NOT_DONE`。`t_operation` 返却値を `STATUS_OK` と比較するのは数値的には `OP_NOT_DONE` と比較しているのと同じ —— 結果として「op が動かなかった時に print、動いた時に silent」という反転した出力になります。Javier の `rotate.c` と `reverse_rotate.c`(2026-06-04 追加、`swap.c` より新しい)では既に `== OP_DONE` を使用していて、以前のファイルがそのパターンの前のもの、という時系列です。

² **`ss`(および `rr` / `rrr`)の atomicity**:非 atomic な形は両 stack 独立に worker を呼び、両方成功時のみ print します。問題ケースは片方が `size 2` 未満のとき:もう片方は mutate されるのに op 行が print されない。`checker_42` は print された op 列を初期入力に適用し program の最終状態と比較するため、print されていない silent mutation は KO 判定になります。atomic な形は両 stack の前提を mutation 前にチェック、「両方やって print」か「何もしない」の二択。So ドラフトでは atomic 形を採用しています(`Docs/implementation_plans/operations.md` の Wrapper / worker パターン節に記載)。

#### `push.c`

| 観点 | Javier ドラフト | So ドラフト | コメント |
|---|---|---|---|
| Worker 名 | `push` | `push` | 同じ |
| Worker 引数順 | `(dst, src)` | `(stack_from, stack_to)` | 命名規約 |
| Worker 返却型 | `int` | `t_operation` | 型の正確性 |
| Edge case(空 src、空 dst、size 1)| 正しく処理 | 正しく処理 | 初期修正ラウンド後はどちらも正解 |
| `node->prev = NULL` の明示 reset | 明示的に設定 | top->prev 不変条件に依存 | Javier 流の方が defense-in-depth |
| Wrapper 返却型 | `void` | `t_operation` | swap と同じ |
| `pa` / `pb` の比較対象 | `STATUS_OK` | `OP_DONE` | swap.c と同じ反転 |

#### `rotate.c`(Javier が 2026-06-04 追加)

| 観点 | Javier ドラフト | So ドラフト | コメント |
|---|---|---|---|
| Worker 名 | `rotate` | `rotate` | 同じ |
| Worker 返却型 | `t_operation` | `t_operation` | 同じ |
| ローカル数 | 1(`old_top`)| 3(`first`、`second`、`last`)| Javier の方が簡潔、semantic 命名 |
| Wrapper 比較対象 | `OP_DONE` | `OP_DONE` | 両者正しい ✅ |
| `rr` セマンティクス | 非 atomic(`&&` 短絡)| Atomic | `ss` と同じ nuance —— `rotate(a)` が成功して `b` が失敗すると `a` が silent mutation |

#### `reverse_rotate.c`(Javier が 2026-06-04 追加)

`rotate.c` と同パターン:Javier の worker は単一の semantic ローカル(`old_bottom`)、So は 3 つの位置的ローカル。両者ともポインタ付け替えは正しい。`rrr` wrapper は `rr` と同じ非 atomic / silent-mutation の nuance あり。

### 提案 —— 収束先

**So の `src/operations/` を canonical なベースとして採用**、Javier ドラフトから 3 つの改善を取り入れます。

#### なぜ So 版をベースに?

- `t_operation` が wrapper まで伝播し、algorithm tier(Phase 4-7)が no-op ケースに反応できる(`if (sa(&a) == OP_NOT_DONE) ...`)。
- wrapper 比較が一貫して `OP_DONE` 対象 —— 反転リスクなし。
- `ss`/`rr`/`rrr` が atomic —— `checker_42` への silent-mutation 経路なし。
- 既に `Docs/implementation_plans/operations.md` に文書化済み(インターフェース、edge-case 表、計算量議論、テスト方針)。

#### Javier ドラフトから取り入れる 3 つの改善

1. **Worker ローカル命名**(`rotate.c` と `reverse_rotate.c`):`first` / `second` / `last` を `old_top` / `old_bottom` に rename(rotate 系では単一 semantic ローカルに集約)。読みやすく、関数の意図と一致。
2. **`push.c` の defensive reset**:移動するノードを `dst` に再アタッチする前に明示的に `node->prev = NULL` を設定。現状の So ドラフトは `top->prev == NULL` 不変条件に依存していますが、明示化により 1 行のコストで load-bearing な暗黙の前提を排除。
3. **Worker の NULL guard**:`if (!stack || ...)`(`push` には引数 2 つそれぞれに)を追加。algorithm 側が NULL stack で op を呼ぶことはないはずですが、guard は 1 条件追加で誤用への耐性を増します。

これらを So が `so/phase2-operations` ブランチで merge 前に実施します。

### Javier への確認事項

- 上記の提案で OK ですか?(So 版ベース + あなたのドラフトから 3 点採用)
- あなたのドラフトに含まれる insight で、見落としているもの・残したいものはありますか?
- Merge 前に意見を出したい点(命名、NULL guard ポリシー等)はありますか?

### 合意後の手順

1. So が refactor(採用する 3 改善)を `so/phase2-operations` に push。
2. So が `so/phase2-operations` を `main` に merge。Conflict 解決:`include/push_swap.h`、`Makefile`、`src/main.c` は `so/phase2-operations` 版を採用。これで parser-merge 後の canonical なヘッダが復元される(現在のビルド失敗を解消)+ `src/operations/` が追加される。
3. So が同じ merge commit で `src/javi/operations/` を削除(2026-06-02 の parser ツリー削除と同じパターン。ただしこれらの ops ファイルはその削除より後に追加されたものなので、今回は明示的にファイル単位で承認したい)。
4. So が merge 後の main で clean build + valgrind sweep を実施、push。
5. 両者が共有された canonical ベースから Phase 4 / 5(Javier)と Phase 6(So)を続行。

### アクション項目

- [ ] Javier:本 doc をレビュー、go / change / 追加意見を回答。
- [ ] So:返信を待つ間に `so/phase2-operations` で 3 改善を採用。
- [ ] So:Javier の ack 後に merge を実行。
- [ ] So:最終結果を `operations.md` の "Known divergence" 節に追記して、統合記録を repo 内に残す。

# Operations: 11 push_swap commands / 11 push_swap 命令

> **Decided in**: Kickoff Topic 3 (2026-05-26), refined 2026-06-04 (per-So draft for Phase 2)
> **Drafter**: Both (parallel-draft; this file documents So's draft)
> **Depends on**: `data-structure.md` (t_node / t_stack), `coordinate-compression.md` (index field)
> **References**: subject VI.5.2 (operation list), VI.5.3 (operation count), Topic 1 (algorithm consumers)

## English

### Scope

- Defines the 11 mandatory push_swap operations: `sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`.
- Each is implemented as a thin wrapper over a `static` worker that mutates the stack via pure pointer manipulation (no malloc, no free).
- Each wrapper prints the op name to stdout (subject-mandated output) and returns a `t_operation` so callers (algorithm tiers, Adaptive dispatcher) can detect no-op cases.

NOT covered here:
- Algorithm tiers that *use* these ops (`simple-insertion-sort.md`, `medium-chunk-sort.md`, `complex-radix-lsd.md`, `adaptive-dispatcher.md`).
- Benchmark mode op counting (Phase 8 — to be designed separately so we don't pollute the canonical wrappers).

### Interface

```c
/* New enum, lives in push_swap.h */
typedef enum e_operation
{
    OP_NOT_DONE,    /* preconditions not satisfied (e.g., empty stack) */
    OP_DONE         /* op applied; line printed to stdout */
}                    t_operation;

/* Swap family — exchange first two elements */
t_operation  sa(t_stack *a);
t_operation  sb(t_stack *b);
t_operation  ss(t_stack *a, t_stack *b);

/* Push family — move top of one stack onto top of the other */
t_operation  pa(t_stack *a, t_stack *b);   /* b → a */
t_operation  pb(t_stack *a, t_stack *b);   /* a → b */

/* Rotate family — top becomes bottom */
t_operation  ra(t_stack *a);
t_operation  rb(t_stack *b);
t_operation  rr(t_stack *a, t_stack *b);

/* Reverse-rotate family — bottom becomes top */
t_operation  rra(t_stack *a);
t_operation  rrb(t_stack *b);
t_operation  rrr(t_stack *a, t_stack *b);
```

### File layout

```
src/operations/
├── swap.c             — sa, sb, ss + static swap_top
├── push.c             — pa, pb + static push
├── rotate.c           — ra, rb, rr + static rotate_top
└── reverse_rotate.c   — rra, rrb, rrr + static reverse_rotate_top
```

Family-grouped: each file holds 1 worker + 2-3 wrappers. Stays under Norm's 5-function-per-file limit and matches subject vocabulary.

### Wrapper / worker pattern

```c
static t_operation  swap_top(t_stack *s)
{
    /* if precondition fails → return OP_NOT_DONE */
    /* else: rewire pointers, return OP_DONE */
}

t_operation  sa(t_stack *a)
{
    if (swap_top(a) == OP_DONE)
        ft_putstr_fd("sa\n", 1);
    return (swap_top_result ? OP_DONE : OP_NOT_DONE);
}
```

Two implementation notes:
1. The wrapper calls the worker once and stores the result, then conditionally prints. Calling the worker twice (once to test, once to do) is wrong — the second call would no-op or double-apply.
2. `ss`/`rr`/`rrr` call the worker on each stack independently and print the combined line only if both succeeded. If only one succeeded, the spec is ambiguous — we choose: print the combined op only when both stacks moved, otherwise no print and return `OP_NOT_DONE`. This matches "ss = sa AND sb simultaneously" reading and is consistent with Javier's implementation.

### Implementation outline (per family)

#### swap (sa / sb / ss)
1. Precondition: `size >= 2`. Else `OP_NOT_DONE`.
2. Let `first = top`, `second = top->next`, `third = second->next` (may be NULL).
3. Rewire:
   - `second->prev = NULL`, `second->next = first`
   - `first->prev = second`, `first->next = third`
   - If `third`: `third->prev = first`. Else `bottom = first` (first was second, now becomes the new bottom adjacent).
4. `stack->top = second`. Size unchanged.
5. Return `OP_DONE`.

#### push (pa: b→a, pb: a→b)
1. Precondition: `src->size >= 1`. Else `OP_NOT_DONE`.
2. Detach top of `src`: `node = src->top; src->top = node->next;` adjust `src->top->prev` or `src->bottom`.
3. Attach to top of `dst`: `node->next = dst->top; node->prev = NULL;` adjust `dst->top->prev` or `dst->bottom`.
4. `dst->top = node`. Update sizes.
5. Return `OP_DONE`.

#### rotate (ra / rb / rr)
1. Precondition: `size >= 2`. Else `OP_NOT_DONE` (size 0/1 is a no-op visually but spec says no print).
2. Detach top: `node = top; top = node->next; top->prev = NULL`.
3. Attach as new bottom: `node->prev = bottom; bottom->next = node; node->next = NULL; bottom = node`.
4. Return `OP_DONE`. Size unchanged.

#### reverse rotate (rra / rrb / rrr)
1. Precondition: `size >= 2`. Else `OP_NOT_DONE`.
2. Detach bottom: `node = bottom; bottom = node->prev; bottom->next = NULL`.
3. Attach as new top: `node->prev = NULL; node->next = top; top->prev = node; top = node`.
4. Return `OP_DONE`. Size unchanged.

### Edge cases

| Op family | size == 0 | size == 1 | size == 2 |
|---|---|---|---|
| swap | NOT_DONE | NOT_DONE | DONE (degenerates to top ↔ bottom) |
| push (from src) | NOT_DONE | DONE (src becomes empty) | DONE |
| rotate | NOT_DONE | NOT_DONE | DONE (top ↔ bottom) |
| reverse rotate | NOT_DONE | NOT_DONE | DONE (top ↔ bottom) |

**Important**: the algorithms (Phase 4-7) must NOT call ops in tight loops without checking the return value, or they could spin on `NOT_DONE` forever. The dispatcher / adaptive logic should bail out if a planned op returns `NOT_DONE`.

### Complexity argument (for README / defense)

- **All 11 ops: O(1) worst case.** Each op touches at most 4 nodes (`top`, `top->next`, the moved node, the other stack's `top`/`bottom`). No traversal.
- The `tail`/`bottom` pointer is what keeps rotation/reverse-rotation O(1) — without it, `rra`/`rrb` would need to walk from `top` to the last node (O(n)).
- This O(1) bound is the foundation of every algorithm complexity argument in the higher tiers (Insertion Sort, Chunk Sort, Radix LSD).

### Testing

#### Per-op manual tests
- Each op on size 0 / 1 / 2 / 3 / 5 stacks, verify pointers (`top`, `bottom`, `prev`/`next` chain) after the call.
- Each op on inverted (3 2 1) and pre-sorted (1 2 3) inputs.
- Verify output: `sa\n` (not `SA`, not `sa\r`, no trailing space).

#### Invariant tests after every op
- `size` consistent with the count of nodes reachable from `top`.
- `top->prev == NULL`, `bottom->next == NULL`.
- Forward traversal (`next`) and reverse traversal (`prev`) yield the same set in reverse order.
- For size <= 1: `top == bottom` (or both NULL if size == 0).

#### valgrind
- 0 leaks expected: ops never malloc/free.
- 0 errors: no dangling-pointer access on edge cases (size 0/1).

### Known divergence from Javier's parallel draft (as of 2026-06-02)

For the integration meeting:
- Javier's `swap.c` wrappers contain a **bug**: `if (swap_top(a) == STATUS_OK)` compares the worker's `t_operation` return against `STATUS_OK` (= 0 = `OP_NOT_DONE`). Result: the print fires when the op DID NOT happen, and is silent when it DID happen — inverted output. Source: `feature/operations` `src/javi/operations/swap.c` lines 39, 44, 49 (similar pattern in `push.c`).
- Suggested fix on his side: either compare against `OP_DONE`, or unify the worker on `t_status` (no new enum).
- So's draft avoids this by (a) making the worker return `t_operation` explicitly (not bare `int`) and (b) the wrapper compares against `OP_DONE`, not `STATUS_OK`.
- Javier's `rotate.c` is a stub; `reverse_rotate.c` doesn't exist yet — independent so far.

---

## 日本語

### Scope

- 11 個の push_swap 必須命令を定義:`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`。
- 各命令は **`static` worker**(純粋ポインタ操作、malloc/free 無し)+ thin wrapper として実装。
- 各 wrapper は op 名を stdout に print(subject 仕様)し、`t_operation` を返す。call 元(アルゴリズム tier、Adaptive ディスパッチャ)が no-op を検出できる。

ここでカバーしないもの:
- これらの op を **使う** アルゴリズム tier(`simple-insertion-sort.md`、`medium-chunk-sort.md`、`complex-radix-lsd.md`、`adaptive-dispatcher.md`)。
- Benchmark モードの op カウント(Phase 8 で別途設計 —— canonical wrapper を汚染しないため)。

### Interface

```c
/* 新規 enum、push_swap.h に追加 */
typedef enum e_operation
{
    OP_NOT_DONE,    /* 前提条件未達(空 stack 等)*/
    OP_DONE         /* 適用済み、stdout に op 名 print 済み */
}                    t_operation;

/* swap 系 —— 先頭 2 要素を入れ替え */
t_operation  sa(t_stack *a);
t_operation  sb(t_stack *b);
t_operation  ss(t_stack *a, t_stack *b);

/* push 系 —— 一方の top をもう一方の top に移動 */
t_operation  pa(t_stack *a, t_stack *b);   /* b → a */
t_operation  pb(t_stack *a, t_stack *b);   /* a → b */

/* rotate 系 —— top が bottom に */
t_operation  ra(t_stack *a);
t_operation  rb(t_stack *b);
t_operation  rr(t_stack *a, t_stack *b);

/* reverse-rotate 系 —— bottom が top に */
t_operation  rra(t_stack *a);
t_operation  rrb(t_stack *b);
t_operation  rrr(t_stack *a, t_stack *b);
```

### ファイル構成

```
src/operations/
├── swap.c             — sa, sb, ss + static swap_top
├── push.c             — pa, pb + static push
├── rotate.c           — ra, rb, rr + static rotate_top
└── reverse_rotate.c   — rra, rrb, rrr + static reverse_rotate_top
```

family グルーピング:1 ファイル 1 worker + 2〜3 wrapper。Norm 5 関数制限内、subject 用語と一致。

### Wrapper / worker パターン

```c
static t_operation  swap_top(t_stack *s)
{
    /* 前提条件未達 → OP_NOT_DONE return */
    /* それ以外:ポインタ付け替え、OP_DONE return */
}

t_operation  sa(t_stack *a)
{
    t_operation  result;

    result = swap_top(a);
    if (result == OP_DONE)
        ft_putstr_fd("sa\n", 1);
    return (result);
}
```

実装上の 2 点:
1. wrapper は worker を **1 度だけ呼び**、結果を変数に保存してから print 判定。2 回呼ぶ(test + apply)は誤り —— 2 回目は no-op or 二重適用になる。
2. `ss` / `rr` / `rrr` は 2 つの stack それぞれに worker を呼び、両方成功時のみ複合 op 名を print。片方しか成功しなかった場合の仕様は曖昧 —— 我々は **「両方動いた時のみ print、それ以外は no print + `OP_NOT_DONE` return」** を採用。「`ss` = `sa` AND `sb` を同時実行」の読みと整合、Javier の実装とも一致。

### Implementation outline(family 別)

#### swap(sa / sb / ss)
1. 前提:`size >= 2`。未達なら `OP_NOT_DONE`。
2. `first = top`、`second = top->next`、`third = second->next`(NULL の可能性あり)。
3. ポインタ付け替え:
   - `second->prev = NULL`、`second->next = first`
   - `first->prev = second`、`first->next = third`
   - `third` あれば `third->prev = first`、なければ `bottom = first`(`first` が新 `bottom` 隣接になる)。
4. `stack->top = second`。size は不変。
5. `OP_DONE` return。

#### push(pa: b→a、pb: a→b)
1. 前提:`src->size >= 1`。未達なら `OP_NOT_DONE`。
2. `src` の top を切り離し:`node = src->top; src->top = node->next;` `src->top->prev` か `src->bottom` を調整。
3. `dst` の top に取り付け:`node->next = dst->top; node->prev = NULL;` `dst->top->prev` か `dst->bottom` を調整。
4. `dst->top = node`。size 更新。
5. `OP_DONE` return。

#### rotate(ra / rb / rr)
1. 前提:`size >= 2`。未達なら `OP_NOT_DONE`(size 0/1 は視覚的 no-op だが、仕様上 print しない)。
2. top 切り離し:`node = top; top = node->next; top->prev = NULL`。
3. 新 bottom として取り付け:`node->prev = bottom; bottom->next = node; node->next = NULL; bottom = node`。
4. `OP_DONE` return。size は不変。

#### reverse rotate(rra / rrb / rrr)
1. 前提:`size >= 2`。未達なら `OP_NOT_DONE`。
2. bottom 切り離し:`node = bottom; bottom = node->prev; bottom->next = NULL`。
3. 新 top として取り付け:`node->prev = NULL; node->next = top; top->prev = node; top = node`。
4. `OP_DONE` return。size は不変。

### Edge cases

| Op family | size == 0 | size == 1 | size == 2 |
|---|---|---|---|
| swap | NOT_DONE | NOT_DONE | DONE(top ↔ bottom に縮退)|
| push(from src)| NOT_DONE | DONE(src 空に)| DONE |
| rotate | NOT_DONE | NOT_DONE | DONE(top ↔ bottom)|
| reverse rotate | NOT_DONE | NOT_DONE | DONE(top ↔ bottom)|

**重要**:アルゴリズム(Phase 4-7)は op の戻り値を check せず tight loop で呼ぶと **`NOT_DONE` で永久ループ**する可能性がある。ディスパッチャ / adaptive ロジックは予定された op が `NOT_DONE` を返したら bail out すべき。

### Complexity argument(README / ディフェンス用)

- **全 11 op:O(1) worst case**。各 op は最大 4 ノード(`top`、`top->next`、移動するノード、相手 stack の `top`/`bottom`)を触るのみ。走査なし。
- `bottom` ポインタが rotate / reverse-rotate を O(1) に保つ鍵 —— なければ `rra`/`rrb` で `top` から最終ノードまで走査(O(n))が必要。
- この O(1) 境界が、上位 tier(Insertion Sort、Chunk Sort、Radix LSD)の全アルゴリズム計算量議論の基盤。

### Testing

#### op 単位の手動テスト
- 各 op を size 0 / 1 / 2 / 3 / 5 の stack で実行、call 後のポインタ(`top`、`bottom`、`prev`/`next` chain)を検証。
- 各 op を inverted(3 2 1)と pre-sorted(1 2 3)入力で検証。
- 出力検証:`sa\n`(`SA` ではなく、`sa\r` ではなく、末尾スペース無し)。

#### op 後の invariant test
- `size` が `top` から到達可能なノード数と一致。
- `top->prev == NULL`、`bottom->next == NULL`。
- 前進走査(`next`)と逆走査(`prev`)が同じ集合を逆順で返す。
- size <= 1:`top == bottom`(or 両方 NULL if size == 0)。

#### valgrind
- leak 0 期待:op は malloc/free しない。
- error 0:edge case(size 0/1)で dangling pointer 参照無し。

### Javier の並行ドラフトとの既知の相違(2026-06-02 時点)

統合ミーティング用メモ:
- Javier の `swap.c` の wrapper に **バグ**:`if (swap_top(a) == STATUS_OK)` が worker の `t_operation` return を `STATUS_OK`(= 0 = `OP_NOT_DONE`)と比較。結果:**op が動かなかった時に print、動いた時に silent** —— 出力が逆転。出所:`feature/operations` `src/javi/operations/swap.c` 39, 44, 49 行(`push.c` も同パターン)。
- 修正案(彼の側):`OP_DONE` と比較する、または worker を `t_status` に統一(新 enum 無し)。
- So のドラフトは(a)worker を明示的に `t_operation` 返却(bare `int` ではなく)、(b)wrapper も `OP_DONE` と比較 —— で回避。
- Javier の `rotate.c` は stub、`reverse_rotate.c` は未作成 —— ここまでは独立。

# Data Structure: Doubly Linked List + Tail / データ構造:双方向連結リスト + tail

> **Decided in**: Kickoff Topic 4 (2026-05-26)
> **Drafter**: Both (Phase 1, parallel-draft workflow)
> **Depends on**: nothing
> **References**: `learning_log/linked-list-design.md`, `learning_log/array&linkedlists.md`
>
> **M2 revisions (2026-05-28, So's branch)**:
> - Renamed `stack_push_top` → **`stack_push_bottom`**. The parser iterates argv left-to-right and appends each new value at the tail so the **first argv token ends up on top** (subject VI.5).
> - **Removed `stack_pop_top`**. No production code path needs it: the 11 push_swap operations rewire existing nodes directly without going through stack helpers, the parser only pushes, and cleanup uses `stack_free`. YAGNI applied.

## English

### Scope

- Defines `t_node` and `t_stack` types
- Stack-level helpers: init, push-at-bottom, free
- Used by the parser (to build stack `a`) and by `stack_free` on cleanup paths

NOT covered here:
- The 11 push_swap operations (`sa`, `pa`, `ra`, ... — separate concern, lives in Phase 2). **Important**: operations re-link existing nodes directly with pointer manipulation and do NOT call these helpers (no malloc/free during ops).
- Value-to-index mapping (`coordinate-compression.md`)
- Whether `t_stack` holds A and B together or as separate instances (deferred follow-up; the interface below assumes **separate instances**)

### Interface

```c
typedef struct s_node
{
    int             value;    // original input value (debug; can be dropped if memory tight)
    int             index;    // compressed rank in [0, n)
    struct s_node   *next;
    struct s_node   *prev;
}   t_node;

typedef struct s_stack
{
    t_node  *top;
    t_node  *tail;
    int     size;
}   t_stack;

/* Lifecycle */
t_stack *stack_init(void);
void     stack_free(t_stack *s);

/* Mutation (parser is the only production caller) */
int      stack_push_bottom(t_stack *s, int value, int index);
```

### Implementation outline

**Initial state**: `top = NULL`, `tail = NULL`, `size = 0`.

**Invariants** (must hold after any operation):

1. `size` equals the number of nodes reachable from `top` via `next`.
2. If `size == 0`: `top == NULL` and `tail == NULL`.
3. If `size == 1`: `top == tail`.
4. If `size >= 2`: `top->prev == NULL`, `tail->next == NULL`, forward traversal `top → ... → tail` matches reverse traversal via `prev`.

**`stack_push_bottom` (used by the parser)**:

1. Allocate a new node, store `value` and `index`. On malloc failure, return non-zero.
2. Set `new->next = NULL` (new node will be the tail).
3. If `size == 0` (empty stack):
   - Set `new->prev = NULL`.
   - Set both `top` and `tail` to the new node.
4. Else (non-empty stack):
   - Set `new->prev = current tail`.
   - Set `current tail->next = new`.
   - Update `tail = new`.
5. Increment `size`. Return 0 on success.

> Rotation and push/pop operations (`ra`, `rra`, `pa`, etc.) operate by re-linking pointers between `top` and `tail`, all O(1). Their detailed steps belong to the operations plan, not this one.

### Edge cases

- Empty stack push: both `top` and `tail` point to the new node.
- `malloc` returns `NULL`: function returns non-zero; caller must check and bail out via the parser's `Error` path.
- `stack_free(NULL)`: must be safe (early return).

### Complexity argument

- `stack_init`, `stack_push_bottom`: **O(1)** — pointer manipulation only, no traversal. The `tail` pointer is what keeps push-at-bottom from degrading to O(n).
- `stack_free`: O(n) — must traverse and free every node.
- Memory: O(n) total nodes, constant size per node.

### Testing

- After `stack_init`: `top == NULL`, `tail == NULL`, `size == 0`.
- After single `stack_push_bottom`: `top == tail == new_node`, `size == 1`.
- After n pushes with values `v1, v2, ..., vn`: `size == n`, `top->value == v1`, `tail->value == vn`, forward traversal yields `v1 → v2 → ... → vn`.
- After `stack_free`: valgrind reports no leaks.

---

## 日本語

### Scope

- `t_node` と `t_stack` 型の定義
- スタックレベルのヘルパー:初期化、push-at-bottom、解放
- parser(`a` 構築用)と cleanup 経路の `stack_free` から呼ばれる

ここでカバーしないもの:
- 11 個の push_swap 命令(`sa`, `pa`, `ra`, ... — 別の関心事、Phase 2)。**重要**:命令は **既存ノードのポインタを直接付け替える** だけで、これらの helper を呼ばない(命令中に malloc/free は走らない)。
- 値から index へのマッピング(`coordinate-compression.md`)
- `t_stack` で A/B を統合するか別 instance にするか(follow-up 保留。以下のインターフェースは**別 instance**を前提)

### Interface

```c
typedef struct s_node
{
    int             value;    // 元の入力値(debug 用、メモリがきつければ削除可)
    int             index;    // 圧縮された rank [0, n)
    struct s_node   *next;
    struct s_node   *prev;
}   t_node;

typedef struct s_stack
{
    t_node  *top;
    t_node  *tail;
    int     size;
}   t_stack;

/* ライフサイクル */
t_stack *stack_init(void);
void     stack_free(t_stack *s);

/* 変更(production の呼び出し元は parser のみ)*/
int      stack_push_bottom(t_stack *s, int value, int index);
```

### Implementation outline

**初期状態**:`top = NULL`、`tail = NULL`、`size = 0`。

**Invariants**(どの操作の後でも保持):

1. `size` は `top` から `next` で到達できるノード数と一致する。
2. `size == 0` のとき:`top == NULL` かつ `tail == NULL`。
3. `size == 1` のとき:`top == tail`。
4. `size >= 2` のとき:`top->prev == NULL`、`tail->next == NULL`、前進(`top → ... → tail`)と `prev` 経由の逆走査が一致。

**`stack_push_bottom`(parser から呼ばれる)**:

1. 新ノードを確保、`value` と `index` を設定。malloc 失敗時は非ゼロを return。
2. `new->next = NULL` を設定(新ノードが tail になるため)。
3. `size == 0`(空スタック)なら:
   - `new->prev = NULL` を設定。
   - `top` と `tail` の両方を新ノードに。
4. それ以外(非空)なら:
   - `new->prev = 現在の tail` を設定。
   - `現在の tail->next = new` を設定。
   - `tail = new` に更新。
5. `size++`。成功時 0 を return。

> 回転命令や push/pop 命令(`ra`, `rra`, `pa` 等)は `top` と `tail` 間のポインタを付け替えるだけで全 O(1)。詳細は本プランではなく operations プランで扱う。

### Edge cases

- 空スタックへの push:`top` と `tail` の両方が新ノードを指す。
- `malloc` が `NULL` を返した:関数は非ゼロを return、呼び出し側で check し parser の `Error` 経路に戻す。
- `stack_free(NULL)`:安全に扱う(早期 return)必要。

### Complexity argument

- `stack_init`、`stack_push_bottom`:**O(1)** —— ポインタ操作のみ、走査なし。`tail` ポインタの存在が push-at-bottom を O(n) に劣化させない鍵。
- `stack_free`:O(n) —— 全ノードを辿って free する必要。
- メモリ:総ノード数 O(n)、ノードあたり定数サイズ。

### Testing

- `stack_init` 後:`top == NULL`、`tail == NULL`、`size == 0`。
- 単一 `stack_push_bottom` 後:`top == tail == new_node`、`size == 1`。
- 値 `v1, v2, ..., vn` を順に push 後:`size == n`、`top->value == v1`、`tail->value == vn`、前進走査が `v1 → v2 → ... → vn` を produces する。
- `stack_free` 後:valgrind がリーク 0 を報告。

# Data Structure: Doubly Linked List + Tail / データ構造:双方向連結リスト + tail

> **Decided in**: Kickoff Topic 4 (2026-05-26)
> **Drafter**: Both (Phase 1, parallel-draft workflow)
> **Depends on**: nothing
> **References**: `learning_log/linked-list-design.md`, `learning_log/array&linkedlists.md`

## English

### Scope

- Defines `t_node` and `t_stack` types
- Stack-level helpers: init, push-at-top, pop-from-top, free, plus simple inspectors
- Used by every push_swap operation and every algorithm tier

NOT covered here:
- The 11 push_swap operations (`sa`, `pa`, `ra`, ... — separate concern, lives in Phase 2)
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

/* Mutation */
int      stack_push_top(t_stack *s, int value, int index);
t_node  *stack_pop_top(t_stack *s);
```

### Implementation outline

**Initial state**: `top = NULL`, `tail = NULL`, `size = 0`.

**Invariants** (must hold after any operation):

1. `size` equals the number of nodes reachable from `top` via `next`.
2. If `size == 0`: `top == NULL` and `tail == NULL`.
3. If `size == 1`: `top == tail`.
4. If `size >= 2`: `top->prev == NULL`, `tail->next == NULL`, forward traversal `top → ... → tail` matches reverse traversal via `prev`.

**`stack_push_top` (used by `pa`/`pb`)**:

1. Allocate a new node, store `value` and `index`.
2. Link: `new->next = old_top`, `new->prev = NULL`.
3. If `old_top != NULL`: `old_top->prev = new`.
4. If `size == 0`: `tail = new` (single-element case).
5. Update `top = new`, increment `size`.

**`stack_pop_top` (used by `pa`/`pb`)**:

1. If `size == 0`: return `NULL` (caller's responsibility to check).
2. Save `node = top`.
3. Update `top = top->next`.
4. If new `top != NULL`: `top->prev = NULL`. Else: `tail = NULL`.
5. Decrement `size`. Return `node` (caller may free or re-push elsewhere).

> Rotation operations (`ra`, `rra`, etc.) operate by re-linking pointers between `top` and `tail`, all O(1). Their detailed steps belong to the operations plan, not this one.

### Edge cases

- Empty stack push: both `top` and `tail` point to the new node.
- Single-element pop: stack becomes empty (`top = tail = NULL`).
- `malloc` returns `NULL`: caller must check return value and bail out via the parser's `Error` path.

### Complexity argument

- All helpers are **O(1)** — pointer manipulation only, no traversal.
- Memory: O(n) total nodes, constant size per node.
- This O(1) per primitive is what enables every push_swap operation to remain O(1) in C-side time.

### Testing

- After `stack_init`: `top == NULL`, `tail == NULL`, `size == 0`.
- After single `push_top`: `top == tail == new_node`, `size == 1`.
- After n pushes: `size == n`, `top` is the latest, `tail` is the first pushed.
- After n pushes then n pops: stack returns to empty state with all invariants.
- Stress test: random sequence of pushes/pops with invariant assertions after each op.

---

## 日本語

### Scope

- `t_node` と `t_stack` 型の定義
- スタックレベルのヘルパー:初期化、top への push、top からの pop、解放、簡単な inspector
- 全 push_swap 命令と全アルゴリズム tier から呼ばれる

ここでカバーしないもの:
- 11 個の push_swap 命令(`sa`, `pa`, `ra`, ... — 別の関心事、Phase 2)
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

/* 変更 */
int      stack_push_top(t_stack *s, int value, int index);
t_node  *stack_pop_top(t_stack *s);
```

### Implementation outline

**初期状態**:`top = NULL`、`tail = NULL`、`size = 0`。

**Invariants**(どの操作の後でも保持):

1. `size` は `top` から `next` で到達できるノード数と一致する。
2. `size == 0` のとき:`top == NULL` かつ `tail == NULL`。
3. `size == 1` のとき:`top == tail`。
4. `size >= 2` のとき:`top->prev == NULL`、`tail->next == NULL`、前進(`top → ... → tail`)と `prev` 経由の逆走査が一致。

**`stack_push_top`(`pa`/`pb` から呼ばれる)**:

1. 新ノードを確保、`value` と `index` を設定。
2. リンク:`new->next = old_top`、`new->prev = NULL`。
3. `old_top != NULL` なら:`old_top->prev = new`。
4. `size == 0` なら:`tail = new`(単一要素ケース)。
5. `top = new`、`size++`。

**`stack_pop_top`(`pa`/`pb` から呼ばれる)**:

1. `size == 0` なら `NULL` を返す(呼び出し側で check)。
2. `node = top` を保存。
3. `top = top->next` に更新。
4. 新しい `top != NULL` なら `top->prev = NULL`。そうでなければ `tail = NULL`。
5. `size--`、`node` を返す(呼び出し側が free するか別の場所に push する)。

> 回転命令(`ra`, `rra` など)は `top` と `tail` 間のポインタを付け替えるだけで全 O(1)。詳細は本プランではなく operations プランで扱う。

### Edge cases

- 空スタックへの push:`top` と `tail` の両方が新ノードを指す。
- 単一要素の pop:スタックが空に戻る(`top = tail = NULL`)。
- `malloc` が `NULL` を返した:呼び出し側で check し、parser の `Error` 経路に戻す。

### Complexity argument

- 全ヘルパーは **O(1)** —— ポインタ操作のみ、走査なし。
- メモリ:総ノード数 O(n)、ノードあたり定数サイズ。
- この C 側 O(1) primitive が、全 push_swap 命令を C 側で O(1) に保つ前提。

### Testing

- `stack_init` 後:`top == NULL`、`tail == NULL`、`size == 0`。
- 単一 `push_top` 後:`top == tail == new_node`、`size == 1`。
- n 回 push 後:`size == n`、`top` が最新、`tail` が最初に push されたもの。
- n 回 push して n 回 pop:全 invariants を保ったまま空に戻る。
- ストレステスト:ランダムな push/pop 列、各操作後に invariant をアサート。

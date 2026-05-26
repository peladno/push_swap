# Linked List Design for push_swap / push_swap 用の連結リスト設計

> Follow-up to `array&linkedlists.md` (Javier's initial proposal). This file consolidates the pair's analysis on **what structure makes every push_swap operation O(1)**.

## English

### Conclusion (target for pair agreement)

The stack should be a **doubly linked list**, with one of:

- **(A) `t_stack` holds a `tail` pointer in addition to `top`**, OR
- **(B) Circular doubly linked list** (so `top->prev` IS the tail)

Either choice makes **all 11 push_swap operations O(1)**. Without `tail` (or a circular structure), `rotate` and `reverse_rotate` degrade to O(n), which destroys the complexity argument for every higher-tier algorithm.

### What each push_swap operation needs to access

| Op                        | Required access                   | Comment                                                                 |
| ------------------------- | --------------------------------- | ----------------------------------------------------------------------- |
| `sa` / `sb`           | First two nodes (top + top->next) | Swap their values, no link changes                                      |
| `pa` / `pb`           | Top of source, top of destination | Detach top of source, attach as top of destination                      |
| `ra` / `rb`           | Top**and tail**             | Move current top to become new tail; old `tail->next` becomes new top |
| `rra` / `rrb`         | Top**and tail**             | Move current tail to become new top                                     |
| `ss` / `rr` / `rrr` | Both stacks simultaneously        | Compositions of the above                                               |

**Key observation**: every operation touches at most 2 specific nodes (top and/or tail). With direct pointers to both, every operation is O(1) — no traversal needed.

### Why Javier's current proposal as written falls short

The struct in `array&linkedlists.md`:

```c
typedef struct s_node {
    int           value;
    int           index;
    struct s_node *next;   // ← prev is missing
} t_node;

typedef struct s_stack {
    t_node *a;
    t_node *b;
    int     size_a;
    int     size_b;
} t_stack;
```

Two problems:

1. **`*prev` is missing on `t_node`** — despite the text saying "double linked list", this struct is **singly linked**. Without `prev`, you cannot walk backward from any node.
2. **No `tail` pointer** on either stack — to find the last node, you must walk from head through every node.

Cost consequences if implemented as-is:

| Op                | Cost on singly-linked, no tail                             | Cost on doubly-linked + tail |
| ----------------- | ---------------------------------------------------------- | ---------------------------- |
| `sa` / `sb`   | O(1)                                                       | O(1)                         |
| `pa` / `pb`   | O(1)                                                       | O(1)                         |
| `ra` / `rb`   | **O(n)** — walk from head to find tail              | O(1)                         |
| `rra` / `rrb` | **O(n)** — walk from head AND no way to go backward | O(1)                         |

At n=500 with thousands of rotates (Chunk Sort or Radix), this O(n) overhead per rotate **kills the runtime** even though the push_swap operation count is unchanged.

> Note: The subject measures complexity in **push_swap operations generated**, not C-side time. So technically the O(n²) wall-clock from naïve rotation doesn't violate the subject's complexity claim. **But** during peer-eval, the evaluator will read the code and ask "why is your rotate O(n) when it could be O(1)?" — and we should have an answer.

### The "linked list is slower than array" claim is misleading in push_swap

| Operation                             | Array                                     | Doubly linked + tail                 |
| ------------------------------------- | ----------------------------------------- | ------------------------------------ |
| Access top                            | O(1)                                      | O(1)                                 |
| Swap top two                          | O(1)                                      | O(1)                                 |
| **Rotate** (move top to bottom) | **O(n)** — shift all elements      | **O(1)** — re-link 3 pointers |
| **Push** to top                 | **O(n)** — shift all elements down | **O(1)** — re-link 2 pointers |

In push_swap, the dominant operations are rotate and push. **The linked list with tail is strictly faster than an array for our workload.** "Slower but simpler" is the wrong trade-off framing.

### Option A vs Option B: which doubly-linked variant?

| Aspect                   | (A) Doubly linked +`tail`                                          | (B) Circular doubly linked                                                                     |
| ------------------------ | -------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- |
| Struct fields            | `t_node {value, index, next, prev}`, `t_stack {top, tail, size}` | `t_node {value, index, next, prev}`, `t_stack {top, size}` (tail derived as `top->prev`) |
| Memory                   | One extra pointer per stack                                          | Slightly less (no separate `tail`)                                                           |
| Reading code             | Direct:`stack->tail`                                               | Indirect:`stack->top->prev`                                                                  |
| End-of-list check        | `node == NULL`                                                     | `node == stack->top` (cycle detection)                                                       |
| Bug-proneness            | Lower —`NULL` is explicit                                         | Higher — easy to spin in an infinite loop                                                     |
| **Recommendation** | **Default for clarity and safety**                             | Elegant but error-prone for first push_swap implementation                                     |

→ **(A) is recommended for this project.** (B) is conceptually appealing but introduces bugs that are painful to debug under the Norm constraints.

### Pair agreement checklist (bring to kickoff Topic 4)

- [ ] Doubly linked list (with `next` AND `prev`)?
- [ ] `t_stack` gets a `tail` pointer? (Option A)
- [ ] OR: circular doubly linked? (Option B)
- [ ] `t_stack` holds `a` and `b` together, or separate instances? (Independent decision; `array&linkedlists.md` proposed combined, original skeleton README proposed separate)

---

## 日本語

> `array&linkedlists.md`(Javier の初期提案)の続編。本ファイルは「**push_swap の全 11 命令を O(1) で実行できる構造は何か**」をペアで詰めた結果をまとめる。

### 結論(ペア合意の目標)

スタックは **双方向連結リスト** とし、以下のどちらかを採用する:

- **(A) `t_stack` に `top` に加えて `tail` ポインタを持たせる**、または
- **(B) 循環双方向連結リスト**(`top->prev` がそのまま末尾になる)

どちらでも **push_swap の全 11 命令が O(1)** になる。`tail`(または循環構造)無しだと、`rotate` と `reverse_rotate` が O(n) に劣化し、上位 tier アルゴリズムの計算量議論が崩れる。

### 各 push_swap 命令が必要とするアクセス

| 命令                      | 必要なノードアクセス               | 備考                                            |
| ------------------------- | ---------------------------------- | ----------------------------------------------- |
| `sa` / `sb`           | 先頭 2 ノード(top と top->next)    | 値を入れ替えるだけ、リンク変更不要              |
| `pa` / `pb`           | 元スタックの top、先スタックの top | 元の top を外し、先の新 top として付ける        |
| `ra` / `rb`           | **top と tail**              | 現 top を新 tail に。旧 `tail->next` が新 top |
| `rra` / `rrb`         | **top と tail**              | 現 tail を新 top に                             |
| `ss` / `rr` / `rrr` | 両スタック同時                     | 上記の合成                                      |

**核心**:どの命令も最大 2 つの特定ノード(top と/または tail)しか触らない。両方への直接ポインタがあれば**全命令が O(1)** ——走査一切不要。

### 現状の Javier 提案ではなぜ不十分か

`array&linkedlists.md` の struct:

```c
typedef struct s_node {
    int           value;
    int           index;
    struct s_node *next;   // ← prev が無い
} t_node;

typedef struct s_stack {
    t_node *a;
    t_node *b;
    int     size_a;
    int     size_b;
} t_stack;
```

2 つの問題:

1. **`t_node` に `*prev` が無い** ——テキストでは「double linked list」と言っているが struct は **single linked**。`prev` 無しでは、どのノードからも逆方向に辿れない。
2. **どちらのスタックにも `tail` ポインタが無い** ——末尾を見つけるには head から全ノードを辿る必要。

このまま実装した場合のコスト:

| 命令              | single linked + tail 無し                           | doubly linked + tail |
| ----------------- | --------------------------------------------------- | -------------------- |
| `sa` / `sb`   | O(1)                                                | O(1)                 |
| `pa` / `pb`   | O(1)                                                | O(1)                 |
| `ra` / `rb`   | **O(n)** ——head から tail を探す            | O(1)                 |
| `rra` / `rrb` | **O(n)** ——head から辿る + 逆方向に動けない | O(1)                 |

n=500 で rotate が何千回も発生する Chunk Sort や Radix では、rotate 1 回ごとに O(n) のオーバーヘッドが**実行時間を破壊する**(生成される push_swap 操作数は変わらないが)。

> 補足:subject の計算量は **生成される push_swap 操作数** で測られるので、厳密には素朴な rotate の O(n) は subject の計算量主張を破らない。**しかし**ピア評価でコードを読まれた時、「rotate を O(1) にできるのになぜ O(n) なのか?」と必ず問われる。答えられる必要がある。

### 「linked list は array より遅い」は push_swap では誤り

| 操作                           | 配列                            | 双方向リスト + tail                        |
| ------------------------------ | ------------------------------- | ------------------------------------------ |
| top アクセス                   | O(1)                            | O(1)                                       |
| 先頭 2 つの swap               | O(1)                            | O(1)                                       |
| **rotate**(top を末尾へ) | **O(n)** ——全要素シフト | **O(1)** ——ポインタ 3 本の付け替え |
| **push** 先頭挿入        | **O(n)** ——全要素シフト | **O(1)** ——ポインタ 2 本の付け替え |

push_swap で支配的なのは rotate と push。**この用途では双方向リスト + tail が配列より厳密に速い**。「速さを犠牲にして simple さを取る」というトレードオフ自体が成立していない。

### (A) と (B) のどちらを採るか

| 観点               | (A) 双方向リスト +`tail`                                           | (B) 循環双方向リスト                                                                         |
| ------------------ | -------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- |
| struct フィールド  | `t_node {value, index, next, prev}`, `t_stack {top, tail, size}` | `t_node {value, index, next, prev}`, `t_stack {top, size}`(tail は `top->prev` で取得) |
| メモリ             | スタックあたりポインタ 1 本多い                                      | やや少ない(別 `tail` 不要)                                                                 |
| コードの読みやすさ | 直接的:`stack->tail`                                               | 間接的:`stack->top->prev`                                                                  |
| 終端判定           | `node == NULL`                                                     | `node == stack->top`(循環検出)                                                             |
| バグの起きやすさ   | 低い ——`NULL` で明示的                                           | 高い ——無限ループに陥りやすい                                                              |
| **推奨度**   | **明快さと安全性で推奨**                                       | エレガントだが初実装ではデバッグが辛い                                                       |

→ **このプロジェクトでは (A) を推奨**。(B) は概念的に美しいが、Norm 制約下では debug が苦しい。

### ペア合意チェックリスト(kickoff Topic 4 に持っていく)

- [ ] 双方向連結リスト(`next` と `prev` 両方)を採用するか?
- [ ] `t_stack` に `tail` ポインタを持たせるか?(Option A)
- [ ] それとも循環双方向にするか?(Option B)
- [ ] `t_stack` は a と b を統合するか、別 instance にするか?(独立した判断材料。`array&linkedlists.md` は統合提案、元スケルトン README は別 instance 提案)

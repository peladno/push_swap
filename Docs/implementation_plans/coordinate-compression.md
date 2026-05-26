# Coordinate Compression / 座標圧縮

> **Decided in**: Kickoff Topic 5 (2026-05-26, Approach A)
> **Drafter**: Both (Phase 1, parser stage)
> **Depends on**: `data-structure.md`
> **References**: `learning_log/algorithms-complex-o-nlogn.md`(Radix の動機), `kickoff.md` Topic 5

## English

### Scope

- Maps input `value` fields to rank-based indices in `[0, n)`.
- Sets `t_node->index` for every node in stack `a`.
- Called **once** at parser stage, after input validation, before disorder calculation and dispatch.
- After this runs, all algorithm tiers operate on `t_node->index` rather than `value`.

NOT covered:
- The argv parsing itself (token splitting, integer conversion, duplicate detection) — that's the parser's job.
- How each algorithm consumes the `index` field (covered in each algorithm's plan).

### Interface

```c
/* Assigns t_node->index for every node in `stack_a`, based on the rank
 * of node->value across all nodes. Indices form a permutation of [0, n).
 * Returns 0 on success, -1 on allocation failure. */
int coord_compress(t_stack *stack_a);
```

### Implementation outline

**Approach (recommended)**: rank via sorting.

1. Walk `stack_a` once, copying each node's `value` into a temporary array `values[n]`.
2. Sort `values[]` ascending (O(n log n)).
3. Walk `stack_a` again. For each node:
   - Binary search `values[]` for `node->value`.
   - The found position becomes `node->index`.

**Alternative (naive, O(n²))**: for each node, count how many other nodes have a strictly smaller `value`. That count is the node's rank.

Choose based on Norm-friendliness. Either is acceptable since this generates **0 push_swap operations** and is one-time pre-processing.

### Edge cases

- `n == 0`: nothing to do, return success.
- `n == 1`: the single node gets `index = 0`.
- Duplicates: the parser must reject them before calling this function (subject error case).
- Allocation failure for `values[]`: return -1; caller handles via `Error` path.

### Complexity argument

- C-side: **O(n log n)** with sort+binary-search, **O(n²)** with naive counting.
- **Push_swap operations generated: 0** — does not affect subject's complexity measurement (VI.3.1).
- Memory: O(n) temporary buffer, freed before returning.

This is the load-bearing argument for **Radix LSD reaching O(n log n)** push_swap operations: by guaranteeing values are in `[0, n)`, the bit count is fixed at `⌈log₂(n)⌉`.

### Testing

| Input | Expected indices |
| --- | --- |
| `[42, -5, 100, 2]` | `[2, 0, 3, 1]` |
| `[1, 2, 3]` (sorted) | `[0, 1, 2]` |
| `[3, 2, 1]` (reversed) | `[2, 1, 0]` |
| `[-1000, 1000]` | `[0, 1]` |
| `[INT_MAX, INT_MIN]` | `[1, 0]` |

- Verify: indices form a permutation of `0..n-1` (every value `0..n-1` appears exactly once).
- Verify: relative order is preserved (i.e., `value[i] < value[j]` iff `index[i] < index[j]`).

---

## 日本語

### Scope

- 入力の `value` フィールドを rank ベースの `[0, n)` の index にマッピング。
- スタック `a` の全ノードに `t_node->index` を設定する。
- parser stage で **1 度だけ** 呼ぶ ——入力 validation の後、disorder 計算と dispatch の前。
- これ以降、全アルゴリズム tier は `value` ではなく `t_node->index` で動作する。

カバーしないもの:
- argv のパース自体(トークン分割、整数変換、重複検出)—— parser の仕事。
- 各アルゴリズムが `index` をどう使うか —— 各アルゴリズムのプラン参照。

### Interface

```c
/* stack_a の全ノードについて、node->value の rank に基づき t_node->index を
 * 割り当てる。index は [0, n) の順列になる。
 * 成功時 0、確保失敗時 -1 を返す。 */
int coord_compress(t_stack *stack_a);
```

### Implementation outline

**推奨アプローチ**:sort で rank 化。

1. `stack_a` を 1 回走査し、各ノードの `value` を temporary array `values[n]` にコピー。
2. `values[]` を昇順 sort(O(n log n))。
3. `stack_a` を再度走査。各ノードについて:
   - `values[]` を二分探索で `node->value` の位置を取得。
   - その位置が `node->index` となる。

**代替(naive、O(n²))**:各ノードについて「自分より厳密に小さい value を持つノードの数」を数える。その count が rank。

Norm との相性で選ぶ。どちらでも OK ——**push_swap 操作を 0 個生成する**1 回限りの前処理だから。

### Edge cases

- `n == 0`:何もせず成功を返す。
- `n == 1`:単一ノードに `index = 0`。
- 重複:parser が呼び出し前に reject する(subject のエラーケース)。
- `values[]` の確保失敗:-1 を返す。呼び出し側で `Error` 経路へ。

### Complexity argument

- C 側:sort + 二分探索なら **O(n log n)**、naive count なら **O(n²)**。
- **生成される push_swap 操作:0 個** —— subject の計算量測定(VI.3.1)に影響しない。
- メモリ:O(n) の一時バッファ、return 前に解放。

これが **Radix LSD が O(n log n) push_swap 操作を達成する**ための load-bearing 議論:値が `[0, n)` に収まることを保証するので、bit 数が `⌈log₂(n)⌉` に固定される。

### Testing

| 入力 | 期待 indices |
| --- | --- |
| `[42, -5, 100, 2]` | `[2, 0, 3, 1]` |
| `[1, 2, 3]`(ソート済み) | `[0, 1, 2]` |
| `[3, 2, 1]`(逆順) | `[2, 1, 0]` |
| `[-1000, 1000]` | `[0, 1]` |
| `[INT_MAX, INT_MIN]` | `[1, 0]` |

- 検証:indices が `0..n-1` の順列になっている(各値が厳密に 1 回ずつ現れる)。
- 検証:相対順序が保持されている(`value[i] < value[j]` ⇔ `index[i] < index[j]`)。

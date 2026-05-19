# push_swap

_このプロジェクトは42カリキュラムの一環として作成されました。_

---

# 概要 (Description)

`push_swap` は、2つのスタックと限られた操作のみを使用して整数をソートするアルゴリズムプロジェクトです。

目的は、できるだけ少ない操作数で stack `a` を昇順に並べ替えることです。

このプロジェクトでは以下を学びます：

- アルゴリズム設計
- 計算量（Big-O）
- データ構造
- スタック操作
- 最適化
- エラーハンドリング
- メモリ管理

Subjectでは4種類の戦略を実装する必要があります：

- Simple O(n²)
- Medium O(n√n)
- Complex O(n log n)
- Adaptive strategy

---

# ルール (Rules)

プログラムには2つのスタックがあります：

- stack a
- stack b

使用可能な操作：

## Swap

```txt
sa
sb
ss
```

## Push

```txt
pa
pb
```

## Rotate

```txt
ra
rb
rr
```

## Reverse Rotate

```txt
rra
rrb
rrr
```

---

# プロジェクトの目的 (Project Goal)

このプロジェクトの本当の目的は、単純に数字を並べ替えることではありません。

重要なのは：

- algorithmic complexity
- optimization
- data structures
- stack manipulation
- problem solving

を理解することです。

push_swap は「どれだけ速くソートできるか」を学ぶプロジェクトです。

---

# 推奨アーキテクチャ (Recommended Architecture)

```txt
push_swap/
│
├── include/
│   └── push_swap.h
│
├── src/
│   ├── main.c
│   │
│   ├── parser/
│   │   ├── parse_args.c
│   │   ├── validate_input.c
│   │   ├── check_duplicates.c
│   │   └── coordinate_compression.c
│   │
│   ├── stack/
│   │   ├── stack_init.c
│   │   ├── stack_utils.c
│   │   └── stack_size.c
│   │
│   ├── operations/
│   │   ├── swap.c
│   │   ├── push.c
│   │   ├── rotate.c
│   │   ├── reverse_rotate.c
│   │   └── operations_utils.c
│   │
│   ├── algorithms/
│   │   ├── simple/
│   │   │   ├── selection_sort.c
│   │   │   └── min_extract.c
│   │   │
│   │   ├── medium/
│   │   │   └── chunk_sort.c
│   │   │
│   │   ├── complex/
│   │   │   └── radix_sort.c
│   │   │
│   │   ├── adaptive/
│   │   │   └── adaptive_sort.c
│   │   │
│   │   └── mini_sort/
│   │       ├── sort_2.c
│   │       ├── sort_3.c
│   │       └── sort_5.c
│   │
│   ├── benchmark/
│   │   ├── disorder.c
│   │   ├── benchmark.c
│   │   └── operation_counter.c
│   │
│   ├── utils/
│   │   ├── ft_error.c
│   │   ├── ft_free.c
│   │   └── ft_utils.c
│   │
│   └── error/
│       └── error_exit.c
│
├── libft/
│
├── Makefile
└── README.md
```

---

# データ構造 (Data Structure)

linked list を使用することをおすすめします。

理由：

- rotate が簡単
- reverse rotate が簡単
- push/pop が高速
- stack 操作と相性が良い

---

# Node Structure

```c
typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;
```

---

# Stack Structure

```c
typedef struct s_stack
{
	t_node	*top;
	int		size;
}	t_stack;
```

---

# Bench Structure

```c
typedef struct s_bench
{
	int		sa;
	int		sb;
	int		ss;
	int		pa;
	int		pb;
	int		ra;
	int		rb;
	int		rr;
	int		rra;
	int		rrb;
	int		rrr;
	int		total;
	double	disorder;
	char	*strategy;
}	t_bench;
```

---

# 座標圧縮 (Coordinate Compression)

Radix Sort を実装する前に、数値を index に変換します。

例：

```txt
42 -5 100 2
```

↓

```txt
2 0 3 1
```

これにより：

- 大きな整数
- negative numbers
- huge ranges

を簡単に処理できます。

---

# Disorder Metric

Subjectでは disorder を計算する必要があります。

disorder は：

```txt
0.0 = 完全にソート済み
1.0 = 完全に逆順
```

計算式：

```txt
mistakes / total_pairs
```

---

# Disorder Pseudo Code

```txt
function compute_disorder(stack a):
    mistakes = 0
    total_pairs = 0

    for i from 0 to size(a)-1:
        for j from i+1 to size(a)-1:
            total_pairs += 1

            if a[i] > a[j]:
                mistakes += 1

    return mistakes / total_pairs
```

---

# 必須アルゴリズム (Required Algorithms)

---

# 1. Simple Algorithm — O(n²)

おすすめ：

- Selection Sort
- Insertion Sort
- Min Extraction

---

# Simple Strategy Example

```txt
1. 最小値を探す
2. rotate
3. pb
4. 繰り返す
5. pa で戻す
```

---

# Simple Algorithm の特徴

メリット：

- 実装が簡単
- debug が簡単
- evaluation で説明しやすい

デメリット：

- 大きい input に弱い
- operation 数が増える

---

# 2. Medium Algorithm — O(n√n)

おすすめ：

- Chunk Sort

---

# Chunk Sort

データを chunk に分割します。

例：

```txt
0-19
20-39
40-59
60-79
```

その後：

- chunk ごとに stack b に送る
- 最後に戻す

---

# Chunk Sort の特徴

メリット：

- radix より理解しやすい
- performance が良い
- 実装しやすい

デメリット：

- chunk size 調整が必要

---

# 3. Complex Algorithm — O(n log n)

おすすめ：

- Radix Sort

---

# なぜ Radix Sort？

理由：

- push_swap と非常に相性が良い
- 安定して高速
- 実装が比較的簡単
- benchmark を通しやすい

---

# Radix Sort の流れ

例：

```txt
0 1 2 3 4
```

binary：

```txt
000
001
010
011
100
```

bit ごとに：

- pb
- ra

を使って分類します。

---

# Radix Pseudo Flow

```txt
for each bit:
    for each number:
        if bit == 0:
            pb
        else:
            ra

    while stack_b not empty:
        pa
```

---

# 4. Adaptive Algorithm

disorder に応じてアルゴリズムを変更します。

```c
if (disorder < 0.2)
    simple_sort();
else if (disorder < 0.5)
    medium_sort();
else
    complex_sort();
```

---

# Adaptive の目的

ほぼソート済みの input に radix を使うのは無駄です。

Adaptive strategy は：

- disorder が低い → simple
- disorder が中間 → chunk
- disorder が高い → radix

を選択します。

---

# 実装順序 (Recommended Order)

---

# Phase 1

```txt
- parser
- linked list
- stack init
- error handling
```

---

# Phase 2

基本操作：

```txt
sa
sb
pa
pb
ra
rb
rra
rrb
```

---

# Phase 3

Mini Sort：

```txt
sort_2
sort_3
sort_5
```

---

# Phase 4

Simple algorithm

---

# Phase 5

Chunk sort

---

# Phase 6

Radix sort

---

# Phase 7

Adaptive strategy

---

# Phase 8

Benchmark mode

---

# Mini Sort

---

# sort_2

```c
if (a > b)
    sa();
```

---

# sort_3

3 numbers は hardcoded が一般的です。

---

# sort_5

一般的な方法：

- 最小値を pb
- 次の最小値を pb
- sort_3
- pa
- pa

---

# Parser

以下のケースを処理する必要があります：

```bash
./push_swap "1 2 3"
./push_swap 1 2 3
./push_swap --complex 1 2 3
```

---

# Error Cases

```bash
./push_swap 1 2 2
./push_swap one 2
./push_swap 999999999999999
./push_swap ""
```

出力：

```txt
Error
```

stderr に出力する必要があります。

---

# Duplicate Check

重複は禁止されています。

```txt
1 2 3 2
```

↓

```txt
Error
```

---

# Overflow Check

```txt
INT_MAX  = 2147483647
INT_MIN  = -2147483648
```

これを超えたら Error。

---

# Operation Wrapper

おすすめ：

```c
void	op_sa(t_stack *a, t_bench *bench, int print);
```

理由：

- operation count
- benchmark
- logging

を簡単に管理できる。

---

# Benchmark Mode

`--bench` フラグでは以下を stderr に表示します：

- disorder
- 使用した戦略
- operation 数
- 各 operation の回数

---

# Benchmark Example

```txt
[bench] disorder: 0.49
[bench] strategy: Adaptive / O(n√n)
[bench] total_ops: 7997
[bench] sa: 0
[bench] pb: 500
```

---

# Performance Benchmark

---

# 100 numbers

```txt
< 2000 -> pass
< 1500 -> good
< 700  -> excellent
```

---

# 500 numbers

```txt
< 12000 -> pass
< 8000  -> good
< 5500  -> excellent
```

---

# Testing

---

# Random Input

```bash
shuf -i 0-99 -n 100
```

---

# Count Operations

```bash
ARG=$(shuf -i 0-99 -n 100)
./push_swap $ARG | wc -l
```

---

# Checker

```bash
ARG="4 67 3 87 23"
./push_swap $ARG | ./checker_linux $ARG
```

---

# Visualizer

push_swap visualizer を使うことを強くおすすめします。

理由：

- rotate bug を見つけやすい
- infinite loop を見つけやすい
- algorithm を理解しやすい

---

# よくあるバグ (Common Bugs)

---

# rotate bug

最後の node が壊れる。

---

# reverse rotate bug

prev pointer が壊れる。

---

# push bug

size を更新し忘れる。

---

# memory leak

node free 忘れ。

---

# infinite loop

rotate condition が壊れる。

---

# 最も重要なこと (Most Important Advice)

push_swap で最も重要なのは：

- architecture
- operations correctness
- debugging
- memory management

です。

algorithm よりも operations が重要です。

operations が壊れていると：

- radix
- chunk
- adaptive

全部壊れます。

---

# おすすめの進め方 (Recommended Workflow)

```txt
1. linked list
2. operations
3. mini sort
4. parser
5. simple sort
6. chunk sort
7. radix
8. adaptive
9. benchmark
```

---

# 最後に (Final Advice)

push_swap は：

- algorithm project
- debugging project
- architecture project

です。

最初から perfect algorithm を目指さないでください。

まず：

- clean operations
- stable stacks
- leak free code

を目指してください。

その後 optimization を行うのがおすすめです。

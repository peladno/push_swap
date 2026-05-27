# Push Swap Parser TODO List

---

# 1. parse_args(int argc, char **argv, t_stack *a)

## Description

Main parser function.

Responsibilities:

- Receive `argc` and `argv`
- Detect how arguments were passed
- Get all tokens
- Validate tokens
- Convert tokens into numbers
- Check duplicates
- Create stack `a`

This function controls the entire parsing process.

---

# 2. get_tokens(int argc, char **argv)

## Description

Creates a final list of strings (tokens).

Examples:

```bash
./push_swap 3 2 1
```

Produces:

```txt
"3" "2" "1"
```

---

```bash
./push_swap "3 2 1"
```

Also produces:

```txt
"3" "2" "1"
```

The goal is to always get a clean list of tokens regardless of how the arguments were passed.

---

# 3. is_valid_token(char *str)

## Description

Checks if a token is a valid number.

Valid examples:

```txt
"42"
"-42"
"+42"
"0"
```

Invalid examples:

```txt
""
"-"
"+"
"--42"
"42a"
"4-2"
```

This function only checks the format of the string.

---

# 4. ft_atol_safe(char *str)

## Description

Converts a string into a `long`.

Used to detect overflow before converting to `int`.

Example:

```c
if (num < INT_MIN || num > INT_MAX)
	error_exit();
```

This prevents invalid integer values.

---

# 5. has_duplicate(t_stack *a, int value)

## Description

Checks if a number already exists in the stack.

Example:

```bash
./push_swap 1 2 3 2
```

Should produce:

```txt
Error
```

because duplicate values are forbidden.

---

# 6. stack_add_back(t_stack *a, int value)

## Description

Creates a new node and adds it to stack `a`.

Example:

```bash
./push_swap 4 2 8
```

Stack:

```txt
top -> 4
       2
       8
```

The first argument must become the top of the stack.

---

# 7. free_tokens(char **tokens)

## Description

Frees the memory created by `ft_split`.

Very important to avoid memory leaks.

---

# 8. error_exit(t_stack *a, char **tokens)

## Description

Handles program errors.

Responsibilities:

- Free allocated memory
- Print `"Error\n"` to stderr
- Exit the program

Example:

```c
write(2, "Error\n", 6);
exit(1);
```

---

# Recommended Parser Flow

```txt
argc / argv
↓
get_tokens()
↓
validate tokens
↓
convert to numbers
↓
check overflow
↓
check duplicates
↓
create stack A
```

---

# Important Advice

Do not write the parser in one giant function.

Small functions are:

- easier to debug
- easier to explain
- easier to maintain
- better for Norminette

================================================================================

# push_swap パーサー TODO リスト

---

# 1. parse_args(int argc, char **argv, t_stack *a)

## 説明

メインのパーサー関数。

役割:

- `argc` と `argv` を受け取る
- 引数の渡し方を判定する
- すべてのトークンを取得する
- トークンを検証する
- 数値へ変換する
- 重複を確認する
- stack `a` を作成する

パース処理全体を管理する関数。

---

# 2. get_tokens(int argc, char **argv)

## 説明

最終的な文字列リスト（トークン）を作成する関数。

例:

```bash
./push_swap 3 2 1
```

結果:

```txt
"3" "2" "1"
```

---

```bash
./push_swap "3 2 1"
```

結果:

```txt
"3" "2" "1"
```

引数の渡し方に関係なく、同じ形式のトークン配列を作ることが目的。

---

# 3. is_valid_token(char *str)

## 説明

トークンが正しい数値か確認する関数。

有効な例:

```txt
"42"
"-42"
"+42"
"0"
```

無効な例:

```txt
""
"-"
"+"
"--42"
"42a"
"4-2"
```

この関数では文字列の形式のみ確認する。

---

# 4. ft_atol_safe(char *str)

## 説明

文字列を `long` に変換する関数。

`int` に変換する前にオーバーフローを検出するために使う。

例:

```c
if (num < INT_MIN || num > INT_MAX)
	error_exit();
```

不正な整数値を防ぐ。

---

# 5. has_duplicate(t_stack *a, int value)

## 説明

stack 内に同じ数値が既に存在するか確認する関数。

例:

```bash
./push_swap 1 2 3 2
```

結果:

```txt
Error
```

push_swap では重複値は禁止されている。

---

# 6. stack_add_back(t_stack *a, int value)

## 説明

新しいノードを作成して stack `a` に追加する関数。

例:

```bash
./push_swap 4 2 8
```

Stack:

```txt
top -> 4
       2
       8
```

最初の引数が stack の top になる必要がある。

---

# 7. free_tokens(char **tokens)

## 説明

`ft_split` で確保したメモリを解放する関数。

メモリリーク防止のため非常に重要。

---

# 8. error_exit(t_stack *a, char **tokens)

## 説明

エラー処理を行う関数。

役割:

- 確保したメモリを解放する
- `"Error\n"` を stderr に出力する
- プログラムを終了する

例:

```c
write(2, "Error\n", 6);
exit(1);
```

---

# 推奨パーサーフロー

```txt
argc / argv
↓
get_tokens()
↓
トークン検証
↓
数値変換
↓
オーバーフローチェック
↓
重複チェック
↓
stack A 作成
```

---

# 重要なアドバイス

パーサーを巨大な1つの関数で書かないこと。

小さい関数の方が:

- デバッグしやすい
- 説明しやすい
- 保守しやすい
- Norminette に適している
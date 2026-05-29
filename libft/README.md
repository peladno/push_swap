*This project has been created as part of the 42 curriculum by skusakab.*

# Libft

[English](#english) | [日本語](#日本語)

---

# English

## Description

`libft` is the very first individual project at 42, in which the goal is to
re-implement a set of standard C library functions as a personal static
library (`libft.a`). This library serves as a reusable toolbox for the rest
of the 42 curriculum, and the project is designed to deepen understanding
of low-level C concepts such as memory allocation, pointer manipulation,
and data structures.

The project is organized into three parts:

- **Part 1** — Reimplementation of standard libc functions (23 functions)
- **Part 2** — Additional helper functions not present in libc (11 functions)
- **Part 3** — Singly-linked list manipulation functions (9 functions)

## Instructions

### Build

```bash
make           # Compile libft.a
make clean     # Remove .o files
make fclean    # Remove .o files and libft.a
make re        # Rebuild from scratch
```

### Use in your project

```c
#include "libft.h"
```

```bash
gcc -Wall -Wextra -Werror -I path/to/libft <your_file.c> \
    -L path/to/libft -lft -o your_program
```

## Library reference

### Part 1 — Libc functions (23)

#### Character classification (5)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_isalpha` | `int ft_isalpha(int c);` | Test if `c` is an alphabetic character |
| `ft_isdigit` | `int ft_isdigit(int c);` | Test if `c` is a digit (0-9) |
| `ft_isalnum` | `int ft_isalnum(int c);` | Test if `c` is an alphabetic character or a digit (0-9) |
| `ft_isascii` | `int ft_isascii(int c);` | Test if `c` is an ASCII character (0-127) |
| `ft_isprint` | `int ft_isprint(int c);` | Test if `c` is a printable character |

#### Character conversion (2)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_toupper` | `int ft_toupper(int c);` | Convert an alphabetic `c` to uppercase; other characters are returned unchanged |
| `ft_tolower` | `int ft_tolower(int c);` | Convert an alphabetic `c` to lowercase; other characters are returned unchanged |

#### String length (1)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_strlen` | `size_t ft_strlen(const char *str);` | Return the length of the string (excluding the null terminator) |

#### Comparison (2)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_strncmp` | `int ft_strncmp(const char *s1, const char *s2, size_t n);` | Compare up to `n` bytes of two strings and return the difference at the first differing byte |
| `ft_memcmp` | `int ft_memcmp(const void *s1, const void *s2, size_t n);` | Compare up to `n` bytes of two memory areas and return the difference at the first differing byte |

#### Search (4)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_strchr` | `char *ft_strchr(const char *s, int c);` | Return a pointer to the first occurrence of `c` in string `s` |
| `ft_strrchr` | `char *ft_strrchr(const char *s, int c);` | Return a pointer to the last occurrence of `c` in string `s` |
| `ft_strnstr` | `char *ft_strnstr(const char *big, const char *little, size_t len);` | Find the first occurrence of `little` within the first `len` bytes of `big`, and return a pointer to it |
| `ft_memchr` | `void *ft_memchr(const void *s, int c, size_t n);` | Scan the first `n` bytes of memory area `s` for byte `c`, and return a pointer to the first occurrence |

#### Copy & concat (3)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_strlcpy` | `size_t ft_strlcpy(char *dst, const char *src, size_t size);` | Copy up to `size - 1` bytes from `src` to `dst` and null-terminate the result. Returns the length of `src` |
| `ft_strlcat` | `size_t ft_strlcat(char *dst, const char *src, size_t size);` | Append `src` to `dst` such that the total length never exceeds `size - 1`, and null-terminate the result. Returns the length the concatenated string would have had without truncation |
| `ft_strdup` | `char *ft_strdup(const char *s);` | Allocate a new string with the same content as `s` and return a pointer to it |

#### Memory operations (4)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_memset` | `void *ft_memset(void *s, int c, size_t n);` | Fill the first `n` bytes of memory area `s` with the byte `c` |
| `ft_bzero` | `void ft_bzero(void *s, size_t n);` | Fill the first `n` bytes of memory area `s` with zero |
| `ft_memcpy` | `void *ft_memcpy(void *dest, const void *src, size_t n);` | Copy `n` bytes from memory area `src` to memory area `dest` |
| `ft_memmove` | `void *ft_memmove(void *dest, const void *src, size_t n);` | Copy `n` bytes from `src` to `dest`, handling overlapping regions correctly as if `src` were first copied to a temporary buffer |

#### Dynamic memory (1)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_calloc` | `void *ft_calloc(size_t nmemb, size_t size);` | Allocate memory for `nmemb` elements of `size` bytes, initialize it to zero, and return a pointer to it |

#### Numeric conversion (1)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_atoi` | `int ft_atoi(const char *nptr);` | Convert the initial numeric portion of the string `nptr` to an `int` |

### Part 2 — Additional functions (11)

#### String creation (5)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_substr` | `char *ft_substr(char const *s, unsigned int start, size_t len);` | Allocate and return a substring of `s` starting at index `start` and up to `len` characters long |
| `ft_strjoin` | `char *ft_strjoin(char const *s1, char const *s2);` | Allocate and return a new string that is the concatenation of `s1` and `s2` |
| `ft_strtrim` | `char *ft_strtrim(char const *s1, char const *set);` | Allocate and return a copy of `s1` with consecutive characters from `set` removed from both the beginning and the end |
| `ft_split` | `char **ft_split(char const *s, char c);` | Split `s` using the delimiter `c` and return a newly allocated NULL-terminated array of strings |
| `ft_itoa` | `char *ft_itoa(int n);` | Allocate and return a string representing the integer `n` |

#### String transformation (2)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_strmapi` | `char *ft_strmapi(char const *s, char (*f)(unsigned int, char));` | Apply `f` to each character of `s` (passing its index) and return a newly allocated string containing the results |
| `ft_striteri` | `void ft_striteri(char *s, void (*f)(unsigned int, char *));` | Apply `f` to each character of `s` in place, passing its index and the character's address |

#### File descriptor output (4)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_putchar_fd` | `void ft_putchar_fd(char c, int fd);` | Write the character `c` to the file descriptor `fd` |
| `ft_putstr_fd` | `void ft_putstr_fd(char *s, int fd);` | Write the string `s` to the file descriptor `fd` |
| `ft_putendl_fd` | `void ft_putendl_fd(char *s, int fd);` | Write the string `s` followed by a newline to the file descriptor `fd` |
| `ft_putnbr_fd` | `void ft_putnbr_fd(int n, int fd);` | Write the integer `n` as a decimal string to the file descriptor `fd` |

### Part 3 — Linked list functions (9)

The linked list uses the following structure, declared in `libft.h`:

```c
typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}                   t_list;
```

#### Construction (3)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_lstnew` | `t_list *ft_lstnew(void *content);` | Allocate a new node, set its `content` to the given value and `next` to NULL, and return a pointer to it |
| `ft_lstadd_front` | `void ft_lstadd_front(t_list **lst, t_list *new);` | Insert the node `new` at the beginning of the list pointed to by `lst` |
| `ft_lstadd_back` | `void ft_lstadd_back(t_list **lst, t_list *new);` | Insert the node `new` at the end of the list pointed to by `lst` |

#### Inspection (2)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_lstsize` | `int ft_lstsize(t_list *lst);` | Count and return the number of nodes in the list |
| `ft_lstlast` | `t_list *ft_lstlast(t_list *lst);` | Return a pointer to the last node of the list |

#### Iteration & transformation (2)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_lstiter` | `void ft_lstiter(t_list *lst, void (*f)(void *));` | Apply `f` to the `content` of each node in the list |
| `ft_lstmap` | `t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));` | Apply `f` to each node's `content` and build a new list with the results. On allocation failure, free the partially built list using `del` |

#### Destruction (2)

| Function | Prototype | Description |
| --- | --- | --- |
| `ft_lstdelone` | `void ft_lstdelone(t_list *lst, void (*del)(void *));` | Free the `content` of `lst` using `del`, then free the node itself. Does not free `lst->next` |
| `ft_lstclear` | `void ft_lstclear(t_list **lst, void (*del)(void *));` | Free every node in the list using `del` for each `content`, and set `*lst` to NULL |

## Resources

### Documentation

- `man` pages for each libc function reimplemented (e.g., `man strlen`,
  `man memcpy`)

### AI usage

I used **Claude (Anthropic)** as an interactive learning assistant during
this project, primarily for:

- **Concept explanation**: clarifying low-level C concepts that were
  unclear from documentation alone (e.g., self-referential structs,
  function pointers, two-level pointers)
- **Code review**: reviewing my draft implementations to identify bugs,
  off-by-one errors, memory leaks, and Norm violations
- **Debugging support**: interpreting `valgrind` output and external
  test tool failure messages
- **README drafting**: structuring the document, generating boilerplate
  (function tables with prototypes and category headings), and
  proofreading the bilingual content. The English translations of the
  Japanese descriptions were also produced with AI assistance

I deliberately wrote all code myself — AI was used for hints, design
discussion, and reviews, not for generating implementations. When AI
suggested specific code patterns, I rewrote them in my own way after
understanding the underlying logic.

---

# 日本語

## 概要

このプロジェクトは、C 言語で自分のプログラム用の汎用関数を多数含むライブラリを
コーディングする課題である。それらの関数を自分自身で実装し、効果的に使えるよう
になることで、その仕組みを理解することをプロジェクトの目的としている。

このプロジェクトは 3 つのパートで構成されている。

- **Part 1** — 標準 libc 関数の再実装(23 関数)
- **Part 2** — libc にはない補助関数(11 関数)
- **Part 3** — 単方向連結リスト操作関数(9 関数)

## ビルド方法

### ビルドコマンド

```bash
make           # libft.a をコンパイル
make clean     # .o ファイルを削除
make fclean    # .o ファイルと libft.a を削除
make re        # 一からビルドし直す
```

### プロジェクトでの使用方法

```c
#include "libft.h"
```

```bash
gcc -Wall -Wextra -Werror -I path/to/libft <your_file.c> \
    -L path/to/libft -lft -o your_program
```

## ライブラリリファレンス

### Part 1 — libc 関数(23)

#### 文字種判定(5)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_isalpha` | `int ft_isalpha(int c);` | c がアルファベット(英字)かどうかをテストする。 |
| `ft_isdigit` | `int ft_isdigit(int c);` | c が数字(0-9)かどうかをテストする。 |
| `ft_isalnum` | `int ft_isalnum(int c);` | c がアルファベットまたは数字かどうかをテストする。 |
| `ft_isascii` | `int ft_isascii(int c);` | c が ASCII 文字(0-127)かどうかをテストする。 |
| `ft_isprint` | `int ft_isprint(int c);` | c が印字可能な文字かどうかをテストする。 |

#### 文字変換(2)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_toupper` | `int ft_toupper(int c);` | アルファベット c を大文字に変換する。その他の文字はそのまま返す。 |
| `ft_tolower` | `int ft_tolower(int c);` | アルファベット c を小文字に変換する。その他の文字はそのまま返す。 |

#### 文字列長(1)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_strlen` | `size_t ft_strlen(const char *str);` | 文字列の長さ(終端 `'\0'` を含まない)を返す。 |

#### 比較(2)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_strncmp` | `int ft_strncmp(const char *s1, const char *s2, size_t n);` | 2 つの文字列を最大 n バイトまで比較し、最初に差異が現れた位置のバイトの差を返す。 |
| `ft_memcmp` | `int ft_memcmp(const void *s1, const void *s2, size_t n);` | 2 つのメモリ領域を最大 n バイトまで比較し、最初に差異が現れた位置のバイトの差を返す。 |

#### 検索(4)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_strchr` | `char *ft_strchr(const char *s, int c);` | 文字列 s 内で、文字 c が最初に出現する位置のポインタを返す。 |
| `ft_strrchr` | `char *ft_strrchr(const char *s, int c);` | 文字列 s 内で、文字 c が最後に出現する位置のポインタを返す。 |
| `ft_strnstr` | `char *ft_strnstr(const char *big, const char *little, size_t len);` | 文字列 big の先頭 len バイト以内で、部分文字列 little が最初に出現する位置のポインタを返す。 |
| `ft_memchr` | `void *ft_memchr(const void *s, int c, size_t n);` | メモリ領域 s の先頭から n バイトを走査し、バイト c が最初に出現する位置のポインタを返す。 |

#### コピー・連結(3)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_strlcpy` | `size_t ft_strlcpy(char *dst, const char *src, size_t size);` | src から dst へ最大 size - 1 バイトをコピーし、末尾を `'\0'` で終端する。戻り値は src の長さ。 |
| `ft_strlcat` | `size_t ft_strlcat(char *dst, const char *src, size_t size);` | dst の末尾に src を連結する。連結後の長さが size - 1 を超えないようにし、末尾を `'\0'` で終端する。戻り値は切り詰めなしで生成されるはずだった文字列の長さ。 |
| `ft_strdup` | `char *ft_strdup(const char *s);` | 文字列 s をコピーした新しいメモリを確保し、その先頭ポインタを返す。 |

#### メモリ操作(4)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_memset` | `void *ft_memset(void *s, int c, size_t n);` | メモリ領域 s の先頭から n バイトをバイト c で埋める。 |
| `ft_bzero` | `void ft_bzero(void *s, size_t n);` | メモリ領域 s の先頭から n バイトを 0 で埋める。 |
| `ft_memcpy` | `void *ft_memcpy(void *dest, const void *src, size_t n);` | src の先頭から n バイトを dest にコピーする。 |
| `ft_memmove` | `void *ft_memmove(void *dest, const void *src, size_t n);` | src の先頭から n バイトを dest にコピーする。dest と src が重なっていても、あたかも一時バッファを経由するかのように正しく動作する。 |

#### 動的メモリ(1)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_calloc` | `void *ft_calloc(size_t nmemb, size_t size);` | nmemb * size バイトのメモリを確保し、中身を 0 で埋める。確保した領域の先頭ポインタを返す。 |

#### 数値変換(1)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_atoi` | `int ft_atoi(const char *nptr);` | 文字列 nptr の数値部分を int 型に変換して返す。 |

### Part 2 — 補助関数(11)

#### 文字列の生成(5)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_substr` | `char *ft_substr(char const *s, unsigned int start, size_t len);` | 文字列 s のインデックス start から最大 len 文字までを新たなメモリにコピーする。戻り値は確保したメモリの先頭ポインタ。 |
| `ft_strjoin` | `char *ft_strjoin(char const *s1, char const *s2);` | 文字列 s1 と s2 を連結した文字列を新たなメモリに確保する。戻り値は確保したメモリの先頭ポインタ。 |
| `ft_strtrim` | `char *ft_strtrim(char const *s1, char const *set);` | 文字列 s1 の先頭と末尾から、set に含まれる文字が連続している部分を取り除いた文字列を新たなメモリにコピーする。戻り値は確保したメモリの先頭ポインタ。 |
| `ft_split` | `char **ft_split(char const *s, char c);` | 文字列 s を区切り文字 c で分割し、分割された文字列群を新たなメモリに NULL 終端の配列としてコピーする。戻り値は配列の先頭ポインタ。 |
| `ft_itoa` | `char *ft_itoa(int n);` | int 型の数 n を文字列に変換する。戻り値は確保したメモリの先頭ポインタ。 |

#### 文字列の変換(2)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_strmapi` | `char *ft_strmapi(char const *s, char (*f)(unsigned int, char));` | 文字列 s の各文字に関数 f を適用した結果を新たなメモリに格納する。戻り値は確保したメモリの先頭ポインタ。 |
| `ft_striteri` | `void ft_striteri(char *s, void (*f)(unsigned int, char *));` | 文字列 s の各文字に関数 f を適用し、その場で書き換える。f には各文字のインデックスとアドレスが渡される。 |

#### ファイルディスクリプタへの出力(4)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_putchar_fd` | `void ft_putchar_fd(char c, int fd);` | 文字 c をファイルディスクリプタ fd に出力する。 |
| `ft_putstr_fd` | `void ft_putstr_fd(char *s, int fd);` | 文字列 s をファイルディスクリプタ fd に出力する。 |
| `ft_putendl_fd` | `void ft_putendl_fd(char *s, int fd);` | 文字列 s をファイルディスクリプタ fd に出力し、続けて改行を出力する。 |
| `ft_putnbr_fd` | `void ft_putnbr_fd(int n, int fd);` | int 型の数 n を10進数の文字列としてファイルディスクリプタ fd に出力する。 |

### Part 3 — 連結リスト関数(9)

連結リストは `libft.h` で定義された以下の構造体を使用する。

```c
typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}                   t_list;
```

#### 構築(3)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_lstnew` | `t_list *ft_lstnew(void *content);` | 新たなノードのメモリを確保し、メンバ content に引数の値、next に NULL を設定して、そのポインタを返す。 |
| `ft_lstadd_front` | `void ft_lstadd_front(t_list **lst, t_list *new);` | リスト lst の先頭にノード new を挿入する。 |
| `ft_lstadd_back` | `void ft_lstadd_back(t_list **lst, t_list *new);` | リスト lst の末尾にノード new を挿入する。 |

#### 検査(2)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_lstsize` | `int ft_lstsize(t_list *lst);` | リストのノードの数を数えて返す。 |
| `ft_lstlast` | `t_list *ft_lstlast(t_list *lst);` | リストの最後のノードのポインタを返す。 |

#### 走査・変換(2)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_lstiter` | `void ft_lstiter(t_list *lst, void (*f)(void *));` | リストの各ノードのメンバ content に関数 f を適用する。 |
| `ft_lstmap` | `t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));` | リストの各ノードのメンバ content に関数 f を適用し、その結果を新たなリストとして構築する。途中でメモリ確保に失敗した場合は、それまでに作ったノードを del を使って解放する。 |

#### 解放(2)

| 関数 | プロトタイプ | 説明 |
| --- | --- | --- |
| `ft_lstdelone` | `void ft_lstdelone(t_list *lst, void (*del)(void *));` | 関数 del を使って content を解放し、ノード自体も解放する。lst->next には触れない。 |
| `ft_lstclear` | `void ft_lstclear(t_list **lst, void (*del)(void *));` | リストの全ノードの content を関数 del で解放し、ノード自体も解放する。最終的に `*lst` を NULL に設定する。 |

## 参考資料

### ドキュメント

- 再実装した各 libc 関数の `man` ページ(例:`man strlen`, `man memcpy`)

### AI の活用

このプロジェクトでは、対話型の学習アシスタントとして **Claude (Anthropic)** を
活用した。主な用途は以下の通り。

- **概念の解説**:ドキュメントだけでは理解しにくい C 言語の低レベル概念
  (自己参照構造体、関数ポインタ、二重ポインタなど)の説明を依頼
- **コードレビュー**:自分で書いたドラフト実装に対して、バグ・off-by-one
  エラー・メモリリーク・Norm 違反を指摘してもらう
- **デバッグ支援**:`valgrind` の出力や外部テストツールのエラーメッセージの
  解釈を依頼
- **README の作成補助**:全体構成の設計、定型部分の生成(関数表のプロトタイプ
  欄やカテゴリ見出しなど)、および日英両言語の文章の推敲を依頼。日本語版の説明
  に対応する英語版の翻訳も AI の補助を得て作成した

すべてのコードは自分自身で書いた。AI はヒント、設計の議論、レビューに使用し、
実装そのものを生成してもらうことはしなかった。AI が具体的なコードパターンを
提案した場合も、その背後のロジックを理解した上で、自分の言葉で書き直した。

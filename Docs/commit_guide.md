# Git Commit Guide (English & Japanese)

## What is a Commit?

A commit is a snapshot of your changes in Git.

When you create a commit, Git saves:

* what changed
* who made the changes
* when the changes were made

---

# Basic Workflow

## 1. Check changed files

```bash
git status
```

---

## 2. Add files

Add one file:

```bash
git add main.c
```

Add all files:

```bash
git add .
```

---

## 3. Create a commit

```bash
git commit -m "fix(parser): handle NULL input"
```

---

## 4. Push to GitHub 

(to push from a master branch to a main branch, github primary branch is "main" not "master")

```bash
git push origin master:main 
```

---

# Good Commit Message Format

Professional teams usually use this format:

```text
type(scope): short description
```

Example:

```text
fix(gnl): prevent memory leak
```

---

# Common Commit Types

| Type     | Meaning                                |
| -------- | -------------------------------------- |
| feat     | New feature                            |
| fix      | Bug fix                                |
| refactor | Code cleanup without changing behavior |
| docs     | Documentation changes                  |
| test     | Add or update tests                    |
| style    | Formatting only                        |
| chore    | Maintenance/configuration              |
| remove   | Delete code/files                      |

---

# Good Examples

```bash
git commit -m "feat(push_swap): add radix sort"

git commit -m "fix(gnl): handle EOF correctly"

git commit -m "docs(readme): update installation guide"

git commit -m "refactor(libft): simplify ft_split"
```

---

# Bad Examples

```text
update
fix
last version
aaaa
changes
```

These messages are too vague.

---

# Important Rules

## Keep commits small

Each commit should do ONE logical thing.

Good:

```text
fix(parser): prevent segfault on empty input
```

Bad:

```text
fix everything
```

---

# Branch Workflow Example

Create a branch:

```bash
git checkout -b feature/auth
```

Commit changes:

```bash
git commit -m "feat(auth): add login endpoint"
```

Push branch:

```bash
git push origin feature/auth
```

---

# Japanese Version (日本語)

# Gitのコミットとは？

コミットは、変更内容を保存するGitのスナップショットです。

コミットを作成すると、Gitは以下を保存します：

* 何を変更したか
* 誰が変更したか
* いつ変更したか

---

# 基本的な流れ

## 1. 変更されたファイルを確認

```bash
git status
```

---

## 2. ファイルを追加

1つのファイルを追加：

```bash
git add main.c
```

すべて追加：

```bash
git add .
```

---

## 3. コミットを作成

```bash
git commit -m "fix(parser): handle NULL input"
```

---

## 4. GitHubへpush

```bash
git push origin main
```

---

# 良いコミットメッセージの形式

多くのチームでは以下の形式を使います：

```text
type(scope): short description
```

例：

```text
fix(gnl): prevent memory leak
```

---

# よく使われるtype

| Type     | 意味       |
| -------- | -------- |
| feat     | 新機能      |
| fix      | バグ修正     |
| refactor | リファクタリング |
| docs     | ドキュメント変更 |
| test     | テスト追加・更新 |
| style    | フォーマット変更 |
| chore    | 設定・保守    |
| remove   | コード削除    |

---

# 良い例

```bash
git commit -m "feat(push_swap): add radix sort"

git commit -m "fix(gnl): handle EOF correctly"

git commit -m "docs(readme): update installation guide"
```

---

# 悪い例

```text
update
fix
last version
aaaa
changes
```

内容が分かりにくいので避けましょう。

---

# 大事なルール

## コミットは小さくする

1つのコミットでは、1つの論理的な変更だけを行うべきです。

良い例：

```text
fix(parser): prevent segfault on empty input
```

悪い例：

```text
fix everything
```

---

# ブランチの例

ブランチ作成：

```bash
git checkout -b feature/auth
```

コミット：

```bash
git commit -m "feat(auth): add login endpoint"
```

Push：

```bash
git push origin feature/auth
```

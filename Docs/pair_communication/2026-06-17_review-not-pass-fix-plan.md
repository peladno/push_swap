# 2026-06-17 — First evaluation: NOT PASS, feedback & fix plan / 初回評価:不合格・FB と修正計画

## English

### Participants

- So, Javier

### Context

The first peer evaluation came back **NOT PASS**. This document records the
reviewer's feedback, the root-cause analysis (with measured data), and the plan
to fix each item before the next attempt. No coding was done today; this is the
planning record.

### Review feedback (verbatim intent)

1. **README** did not document the **work split** between So and Javier.
2. **`total_ops` too high for small `n`.** Requirement: `n = 3 → total_ops ≤ 5`,
   `n = 5 → total_ops ≤ 15`. Because the adaptive dispatcher picks the algorithm
   from **disorder** (O(n²) / O(n√n) / O(n log n)), small-`n` inputs can land on a
   tier that is far from minimal. The subject needs a re-read to reconcile this.
3. **Already-sorted input (disorder = 0) still emits operations.** A sorted input
   must produce **no output** (zero operations).
4. **Live coding** (implement an arbitrary flag that prints only `total_ops`):
   So implemented it by triggering on `bench->enabled == -2`. It passed, but the
   magic value **does not coexist with `--bench`**. Next time, use the clean
   design (a separate `silent` flag + an emit helper).

### Root-cause analysis (measured on current `main`)

| Input         | n | disorder → tier | actual ops | requirement | verdict  |
| ------------- | - | ---------------- | ---------- | ----------- | -------- |
| `1 2 3`     | 3 | 0 → Simple      | 6          | **0** | ❌ (FB3) |
| `2 1 3`     | 3 | 0.33 → Medium   | 7          | ≤ 5        | ❌ (FB2) |
| `3 2 1`     | 3 | 1.0 → Complex   | 10         | ≤ 5        | ❌ (FB2) |
| `2 1 3 5 4` | 5 | → Medium        | 12         | ≤ 15       | ✅       |
| `5 4 3 2 1` | 5 | 1.0 → Complex   | 25         | ≤ 15       | ❌ (FB2) |

Findings:

- The already-sorted case is **not short-circuited** — `disorder` is computed but
  not used to early-exit, so every tier pushes to B and back even when the input
  is already ordered, costing ops instead of zero.
- There are **no small-`n` special cases** (`sort_three` / `sort_five`). The
  push-to-B strategies and especially radix are far from minimal for tiny `n`,
  so disorder-driven dispatch alone cannot meet `n=3 ≤ 5` / `n=5 ≤ 15`.

### Decisions (fix plan)

1. **FB3 — sorted ⇒ zero ops.** `disorder` is already computed for the dispatch,
   and `disorder == 0` ⟺ zero inversions ⟺ already sorted. So **reuse it**: an
   early `return` in `dispatch` when `disorder == 0` — no separate `is_sorted`
   scan. (`disorder == 0.0` is an exact comparison here: `0 / pairs` is exactly
   `0.0`.) Size-independent, so it also covers large already-sorted inputs.
2. **FB2 — small-`n` minimal sorts, size before disorder.** Route by size at the
   top of `dispatch`, *before* the disorder logic:
   ```
   if (disorder == 0) return;
   if (size <= 3) { sort_three(a); return; }
   if (size <= 5) { sort_five(a, b); return; }
   /* n >= 6: disorder-based Simple / Medium / Complex (unchanged) */
   ```

   - `sort_three` — the 5 unsorted permutations via `sa`/`ra`/`rra`, ≤ 2–3 ops.
   - `sort_five` — push the 2 smallest to B, `sort_three` the rest, `pa pa`; ≤ 12 ops.
   - **Reconciliation with the subject:** the disorder regimes (VI.3) govern the
     *general-case* strategy choice; the small-`n` op limits and the sorted-input
     rule are *separate, baseline* requirements. Size-priority handling does not
     remove disorder dispatch — it stays for `n ≥ 6`. To confirm we read it right,
     re-read VI.3 / VI.4 together.
3. **FB1 — README work split.** Add a "Contributions" section (EN + JA) per the
   subject's requirement to document both partners' contributions. Pending: an
   accurate split confirmed by both (file headers are not authoritative).
4. **Live coding (next time).** Replace the `bench->enabled == -2` magic value
   with a dedicated `silent` flag (parsed like `--bench`, orthogonal to it) and an
   `emit` helper that does `if (!silent) print; count++`, so the op stream can be
   suppressed in one place, `--bench` still works, and per-wrapper line counts
   stay within the Norm.

### Open questions

- **Work split (kickoff role table + what actually happened)** — basis for the
  README "Contributions" section:
  - **Joint (both — parallel draft → compared → integrated):** the stack, the
    parser (incl. coordinate compression), and the 11 operations. (kickoff
    Phase 1 & 2: "both, parallel-draft-then-compare".)
  - **Javier:** Simple (insertion sort) and Medium (chunk sort).
  - **So:** Complex (radix LSD), Adaptive (dispatcher / compute_disorder /
    strategy_label), and the Benchmark mode (disorder metric, `--bench`, and the
    op-counting threaded through `t_stack`).
  - **Deltas from the kickoff plan:** benchmark was planned as So → `disorder` /
    Javier → `benchmark.c` + op-counter, but So took the whole benchmark over
    (work_log 2026-06-11); Adaptive was planned as "both" but ended up So-led;
    the README was first drafted by Javier, then rewritten by So to match the
    implementation.
  - Javier: please confirm/correct — especially the README authorship line and
    the benchmark hand-over.
- **Subject reading on small-`n` vs disorder** — do we agree that size-priority
  special-casing (before the disorder regimes) is the intended reconciliation?

### Action items

- [ ] Javier: confirm/correct the work split for the README.
- [ ] So: implement the `disorder == 0` early return in `dispatch` (FB3).
- [ ] So: implement `sort_three` / `sort_five` + size-priority routing (FB2).
- [ ] So: add the README "Contributions" section once the split is confirmed (FB1).
- [ ] Both: re-read subject VI.3 / VI.4 and agree on the small-`n` reconciliation.
- [ ] Later: implement the clean `silent` flag + `emit` helper for the live-coding flag.

---

## 日本語

### 参加者

- So、Javier

### 背景

初回のピア評価が **不合格(NOT PASS)** でした。本ドキュメントはレビュアーの FB、
根本原因の分析(実測データ付き)、次回までの各項目の修正計画を記録します。本日は
コーディングはせず、計画の記録のみです。

### レビュー FB(主旨)

1. **README** に So と Javier の **分担**が記載されていなかった。
2. **小さい `n` で `total_ops` が大きすぎる。** 要件:`n = 3 → total_ops ≤ 5`、
   `n = 5 → total_ops ≤ 15`。adaptive dispatcher は **disorder** でアルゴリズム
   (O(n²) / O(n√n) / O(n log n))を選ぶため、小 `n` の入力が最小から遠い tier に
   割り当てられうる。subject の再読で整合を取る必要あり。
3. **整列済み入力(disorder = 0)でも操作が出力される。** 整列済みは **何も出力
   しない**(0 操作)のが正。
4. **ライブコーディング**(`total_ops` のみ出力する任意フラグの実装):So は
   `bench->enabled == -2` で発火させた。合格はしたが、このマジック値は **`--bench`
   と両立しない**。次回はクリーンな設計(独立 `silent` フラグ + emit ヘルパー)に。

### 根本原因の分析(現 `main` で実測)

| 入力          | n | disorder → tier | 実 ops | 要件        | 判定     |
| ------------- | - | ---------------- | ------ | ----------- | -------- |
| `1 2 3`     | 3 | 0 → Simple      | 6      | **0** | ❌ (FB3) |
| `2 1 3`     | 3 | 0.33 → Medium   | 7      | ≤ 5        | ❌ (FB2) |
| `3 2 1`     | 3 | 1.0 → Complex   | 10     | ≤ 5        | ❌ (FB2) |
| `2 1 3 5 4` | 5 | → Medium        | 12     | ≤ 15       | ✅       |
| `5 4 3 2 1` | 5 | 1.0 → Complex   | 25     | ≤ 15       | ❌ (FB2) |

判明したこと:

- **整列済みの短絡が無い** —— `disorder` は計算されるのに早期終了に使われず、どの
  tier も整列済みでも B に押して戻すので、整列済み入力が 0 でなく操作を消費する。
- **小 `n` の特別扱い(`sort_three` / `sort_five`)が無い**。push-to-B 系、特に
  radix は小 `n` で最小から程遠く、disorder dispatch だけでは `n=3 ≤ 5` /
  `n=5 ≤ 15` を満たせない。

### 決定事項(修正計画)

1. **FB3 —— 整列済み ⇒ 0 操作。** `disorder` は dispatch 用に既に計算済みで、
   `disorder == 0` ⟺ 転倒 0 ⟺ 整列済み。これを **再利用** し、`dispatch` で
   `disorder == 0` のとき早期 return(別途 `is_sorted` スキャン不要)。(`== 0.0`
   はここでは厳密:`0 / pairs` は正確に `0.0`。)サイズ非依存で大きい整列済みにも効く。
2. **FB2 —— 小 `n` の最小ソート、size を disorder より優先。** `dispatch` 冒頭で、
   disorder ロジックの **前に** size で分岐:
   ```
   if (disorder == 0) return;
   if (size <= 3) { sort_three(a); return; }
   if (size <= 5) { sort_five(a, b); return; }
   /* n >= 6: disorder で Simple / Medium / Complex(従来どおり)*/
   ```

   - `sort_three` —— 未整列の 5 パターンを `sa`/`ra`/`rra` で ≤ 2〜3 操作。
   - `sort_five` —— 最小 2 個を B に push、残り 3 個を `sort_three`、`pa pa`;≤ 12 操作。
   - **subject との整合:** disorder レジーム(VI.3)は **一般(大きい n)の戦略選択**
     の規定で、小 `n` の操作数上限と整列済みルールは **別の baseline 要件**。size 優先
     は disorder dispatch を消さず、`n ≥ 6` ではそのまま使う。読み違いがないか VI.3 /
     VI.4 を一緒に再読する。
3. **FB1 —— README 分担。** subject の「両者の貢献を明記」要件に従い "分担
   (Contributions)" 節(英日)を追加。保留:両者で確認した正確な分担(ファイル
   ヘッダは当てにならない)。
4. **ライブコーディング(次回)。** `bench->enabled == -2` のマジック値を、専用の
   `silent` フラグ(`--bench` と同様にパース、直交)+ `emit` ヘルパー
   (`if (!silent) print; count++`)に置き換える。出力抑制が 1 箇所で済み、`--bench`
   も維持、wrapper の行数も Norm 内に収まる。

### 未解決事項

- **分担(kickoff の役割表 + 実際に起きたこと)** —— README "分担" 節の根拠:
  - **共同(両者 —— 並行ドラフト → 比較 → 統合):** stack、parser(座標圧縮含む)、
    11 個の operations。(kickoff Phase 1 & 2:「両者、並行ドラフト → 比較 → 統合」)
  - **Javier:** Simple(insertion sort)、Medium(chunk sort)。
  - **So:** Complex(radix LSD)、Adaptive(dispatcher / compute_disorder /
    strategy_label)、Benchmark モード(disorder 指標、`--bench`、`t_stack` 経由の
    op カウント)。
  - **kickoff 計画との差分:** benchmark は計画では So → `disorder` / Javier →
    `benchmark.c` + op counter だったが、実際は So が benchmark 全体を引き取った
    (work_log 2026-06-11);Adaptive は計画「両者」だが実際は So 主導;README は
    Javier が初版を書き、その後 So が実装に合わせて書き直した。
  - Javier:確認/修正お願いします —— 特に README の著者行と benchmark の引き継ぎ。
- **小 `n` vs disorder の subject 解釈** —— size 優先の特別扱い(disorder レジームの
  前)が意図した整合で合っているか、合意したい。

### 次のアクション

- [ ] Javier:README 用の分担を確認/修正。
- [ ] So:`dispatch` で `disorder == 0` 早期 return を実装(FB3)。
- [ ] So:`sort_three` / `sort_five` + size 優先ルーティングを実装(FB2)。
- [ ] So:分担確定後、README に "分担" 節を追加(FB1)。
- [ ] 両者:subject VI.3 / VI.4 を再読し、小 `n` の整合方針を合意。
- [ ] 後日:ライブコーディング用フラグをクリーンな `silent` フラグ + `emit` ヘルパーで実装。

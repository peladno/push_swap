*This project has been created as part of the 42 curriculum by skusakab, jperez-u.*

## Description

`push_swap` is a sorting program written for the 42 curriculum. The goal is to sort
a list of distinct integers using two stacks (A and B) and a limited set of operations
(`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`). The primary
objective is to produce a valid sequence of operations that sorts the input while
minimizing the number of moves.

This repository contains an implementation that handles small and large inputs by
choosing appropriate strategies (minimal-move insertion for very small N, chunk-based
and radix-style approaches for larger N), plus supporting utilities and a checker.

## Instructions

Compilation

Run `make` at the project root. That produces the `push_swap` binary (and `checker`
when provided by the subject/Makefile).

```bash
make
```

Execution

Provide a sequence of integers as arguments (space-separated). Example:

```bash
./push_swap 3 2 1 5 4
# output: a sequence of operations that sorts the stack

# To validate the output (if you have `checker`):
./push_swap 3 2 1 5 4 | ./checker 3 2 1 5 4
```

Notes
- Input must be integers (within int range) and should not contain duplicates.
- See the `Makefile` and `include/push_swap.h` for build details and available targets.

## Algorithms — Explanation and Justification

Overview

Sorting via `push_swap` requires designing sequences of atomic stack operations that
collectively form higher-level sorting strategies. This implementation selects among
several complementary algorithms based on input size and characteristics.

Selected algorithms and rationale

- Small-N strategy (N ≤ 3 / N ≤ 5):
  - Insertion Sort
  - For N ≤ 3 the optimal minimal-move solutions are hard-coded / pattern-driven.
  - For N ≤ 5 an adapted insertion-like strategy (push smallest/next to B, rotate,
    then push back) produces near-optimal results with very low operation counts.
  - Justification: For very small N the overhead of complex heuristics is unnecessary
    and brute-force / pattern solutions yield fewer operations.

- Chunk-based strategy (medium N, e.g., up to ~100):
  - The list is first coordinate-compressed to indices 0..N-1 (preserving order).
  - The algorithm splits the range into chunks and pushes elements from A to B
    chunk-by-chunk, choosing rotations to move each target with minimal rotations.
  - After accumulating chunks in B, elements are pushed back into A in sorted order.
  - Justification: Chunking reduces overall rotations by focusing moves on contiguous
    ranges of target values; it balances the number of pushes and rotates and performs
    well in practice for medium-sized inputs.

- Radix / binary LSD strategy (large N, e.g., 500):
  - With coordinate compression applied, perform a least-significant-bit radix pass
    using only `pb`/`pa` and rotates. Each bit is used to split elements between A and B
    across repeated passes until sorted.
  - Complexity: O(n * log M) where M is the max value range (after compression log M ≈ log n).
  - Justification: Radix sorting with integer keys maps well to the allowed operations
    and provides predictable performance and simplicity for large N.

Supporting techniques

- Coordinate compression: maps input values to their rank (0..N-1) to make numeric
  ranges small and stable for bitwise/radix operations.
- Adaptive dispatch: the implementation selects the best-of strategies at runtime
  depending on `N` to minimize operations.

Trade-offs and choices

- Insertion/optimal patterns are used for very small inputs to minimize moves.
- Chunking adds heuristic choices (chunk sizes, when to rotate) tuned for good
  practical performance rather than worst-case optimality.
- Radix provides predictable scaling for large inputs at the cost of producing
  more moves than a perfect-optimal solution but with significantly simpler logic.

## Features

- Handles input parsing and validation (duplicates, integer overflow checks).
- Coordinate compression to normalize values.
- Multiple sorting strategies (small-size optimal, chunking, radix).
- A `checker` utility is available to validate operation sequences.

## Usage Examples

- Sort five numbers (small-case strategy chosen):
```bash
./push_swap 2 1 3 6 5
```

- Sort a larger random dataset (chunking or radix will be selected automatically):
```bash
./push_swap $(shuf -i 1-1000 -n 500)
```

## Resources

- The subject and rules: check the assignment PDF provided by 42 (search for "push_swap subject").
- Radix sort: classic algorithm descriptions and textbooks (CLRS, Radix Sort sections).
- Stack-based sorting articles, blog posts and sample solutions by peers.
- Coordinate compression: tutorials that explain ranking and mapping values to 0..N-1.

AI usage

- AI assistance (GPT-5 mini / GitHub Copilot) was used to draft and proof the
  `README.md`, to help structure the algorithms explanations, and to generate
  concise examples and documentation text. Core code, test cases, and algorithm
  implementations were written and verified by the author; AI suggested wording
  and organization only.

## Where to look next

- Implementation details and rationale: see the `Docs/` folder for deep dives
  on the dispatching, chunk-sort, and parser designs.
- Header files and operations: `include/push_swap.h` and `operations/`.

If you'd like, I can now run `make` and a few quick examples to verify the build.

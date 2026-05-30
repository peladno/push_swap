# Push Swap Parser Implementation Plan

---

# Phase 1 — Base Structures ✅

Before implementing the parser, create the core data structures:

```c
typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_stack
{
	t_node	*top;
	t_node	*bottom;
	int		size;
}	t_stack;
```

You also need the strategy enum:

```c
typedef enum e_strategy
{
	STRATEGY_ADAPTIVE,
	STRATEGY_SIMPLE,
	STRATEGY_MEDIUM,
	STRATEGY_COMPLEX
}	t_strategy;
```

The parser depends on these structures because its final job is to create stack `a`.

---

# Phase 2 — Main Parser Function ✅

Implement:

```c
int	parse_args(int argc, char **argv,
				t_stack *a,
				t_strategy *strategy);
```

Responsibilities:

1. Handle `argc == 1` ✅
2. Detect strategy flags
3. Extract numeric arguments
4. Split arguments into tokens
5. Validate tokens
6. Convert tokens into integers
7. Check duplicates
8. Build stack `a`
9. Run coordinate compression

After this function succeeds, `stack_a` should be fully initialized and ready for sorting.

---

# Phase 3 — Strategy Flags

Create a helper function:

```c
int	parse_strategy(char *arg, t_strategy *strategy);
```

Recognized flags:

```txt
--simple
--medium
--complex
--adaptive
```

Recommended rules:

- multiple flags → Error
- unknown flags → Error
- no flag → STRATEGY_ADAPTIVE

---

# Phase 4 — Token Extraction

Create:

```c
char	**get_tokens(int argc, char **argv);
```

This function converts every valid input format into the same token list.

Examples:

```bash
./push_swap 3 1 2
```

and

```bash
./push_swap "3 1 2"
```

must both produce:

```txt
"3" "1" "2"
```

The sorting logic should not care how the arguments were originally passed.

---

# Phase 5 — Token Validation

Create:

```c
int	is_valid_token(char *str);
```

Valid examples:

```txt
42
-42
+42
0
```

Invalid examples:

```txt
""
-
+
--42
42a
4-2
```

This function only checks if the string format represents a valid integer.

---

# Phase 6 — Safe Integer Conversion

Create:

```c
long	ft_atol_safe(char *str);
```

Convert the token into a `long`.

After conversion:

```c
if (num < INT_MIN || num > INT_MAX)
	return (ERROR);
```

This prevents integer overflow and invalid values.

---

# Phase 7 — Stack Construction

Create:

```c
int	stack_add_back(t_stack *a, int value);
```

This function creates a new node and appends it to stack `a`.

Example:

```bash
./push_swap 4 2 8
```

Stack result:

```txt
top -> 4
       2
       8
```

The first argument must become the top of the stack.

---

# Phase 8 — Duplicate Detection

Start with a simple implementation:

```c
int	has_duplicate(t_stack *a, int value);
```

Before inserting a number, scan the stack to see if it already exists.

Example:

```bash
./push_swap 1 2 3 2
```

Should produce:

```txt
Error
```

Later you can optimize duplicate detection using arrays and sorting.

---

# Phase 9 — Coordinate Compression

After building stack `a`, run:

```c
coord_compress(a);
```

Each node should now contain:

```c
node->value
node->index
```

Example:

```txt
42 -5 100 2
```

Becomes:

```txt
2 0 3 1
```

Coordinate compression makes radix sort much easier.

---

# Recommended Implementation Order

```txt
1. t_node / t_stack
2. stack_init
3. stack_add_back
4. free_stack
5. is_valid_token
6. ft_atol_safe
7. get_tokens
8. parse_strategy
9. parse_args
10. duplicate check
11. coord_compress
12. tests
```

---

# Important Design Advice

The parser should NOT:

- sort numbers
- perform radix sort
- calculate chunks
- generate operations

The parser's only responsibility is:

```txt
argv -> validated stack_a
```

After parsing finishes successfully, the sorting algorithms can safely assume that:

- all numbers are valid
- there are no duplicates
- stack `a` is correctly initialized
- indexes are already assigned

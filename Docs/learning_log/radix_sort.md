# Radix Sort in Push Swap

- [Geekforgeeks info](https://www.geeksforgeeks.org/dsa/radix-sort/)

# Simple Explanation of Radix Sort

## What is Radix Sort?

Radix Sort is a sorting algorithm that sorts numbers by looking at their digits one at a time.

Unlike algorithms such as:

- Bubble Sort
- Selection Sort
- Insertion Sort

Radix Sort does **not compare numbers directly**.

Instead, it groups numbers according to their digits.

---

# Basic Idea

Suppose we have:

```txt
170 45 75 90 802 24 2 66
```

Radix Sort sorts the numbers in multiple passes:

1. Sort by the last digit
2. Sort by the tens digit
3. Sort by the hundreds digit

After enough passes, the numbers become fully sorted.

---

# Step 1 — Sort by Units Digit

Look only at the LAST digit of each number.

| Number | Last Digit |
|--------|--------|
| 170 | 0 |
| 45 | 5 |
| 75 | 5 |
| 90 | 0 |
| 802 | 2 |
| 24 | 4 |
| 2 | 2 |
| 66 | 6 |

Now group them according to that digit.

Result:

```txt
170 90 802 2 24 45 75 66
```

Notice:

- numbers ending in `0` come first
- then numbers ending in `2`
- then `4`
- then `5`
- then `6`

The list is not fully sorted yet.

---

# Step 2 — Sort by Tens Digit

Now look at the SECOND digit from the right.

| Number | Tens Digit |
|--------|--------|
| 170 | 7 |
| 90 | 9 |
| 802 | 0 |
| 2 | 0 |
| 24 | 2 |
| 45 | 4 |
| 75 | 7 |
| 66 | 6 |

Sort again using the tens digit.

Result:

```txt
802 2 24 45 66 170 75 90
```

Still not fully sorted.

---

# Step 3 — Sort by Hundreds Digit

Now check the hundreds digit.

| Number | Hundreds Digit |
|--------|--------|
| 802 | 8 |
| 2 | 0 |
| 24 | 0 |
| 45 | 0 |
| 66 | 0 |
| 170 | 1 |
| 75 | 0 |
| 90 | 0 |

Final result:

```txt
2 24 45 66 75 90 170 802
```

Now the list is completely sorted.

---

# Why Does This Work?

Radix Sort works because it preserves the previous ordering of numbers while sorting by the next digit.

This property is called:

# Stable Sorting

It means that numbers with the same digit keep their relative order from previous passes.

---

# Simple Visual Explanation

Imagine several boxes:

```txt
box 0
box 1
box 2
box 3
...
box 9
```

For each pass:

- place numbers into boxes according to the current digit
- collect the boxes from left to right
- repeat for the next digit

Eventually the numbers become sorted.

---

# Binary Radix Sort

In `push_swap`, Radix Sort usually works with:

```txt
binary numbers
```

Instead of decimal digits.

Example:

```txt
5 = 101
2 = 010
```

Instead of checking:

```txt
units
tens
hundreds
```

We check:

```txt
bit 0
bit 1
bit 2
```

One bit at a time.

---

# Why Radix Sort Feels Strange

Most sorting algorithms compare numbers directly:

```txt
8 > 3
```

Radix Sort does not do this.

Instead, it repeatedly groups numbers according to digits or bits.

That is why many beginners find it confusing at first.

---

# Simple Mental Model

Radix Sort is basically:

> "Group numbers by one digit at a time until they naturally become sorted."

---

# Why Radix Sort is Fast

Radix Sort avoids many comparisons.

Instead of:

```txt
compare
compare
compare
compare
```

it repeatedly distributes numbers into groups.

This can make it very efficient for large datasets.

---

# Advantages

- Fast for large inputs
- Easy to automate
- Stable sorting algorithm
- Very predictable behavior
- Works well with binary operations

---

# Disadvantages

- Can be harder to understand initially
- Requires multiple passes
- Usually needs extra memory for grouping

---

# Conclusion

Radix Sort is a unique sorting algorithm because it sorts numbers digit by digit instead of comparing them directly.

Even though it may feel unusual at first, its logic becomes simple once you understand the idea of:

```txt
grouping by digits repeatedly
```

Until the numbers become fully sorted.

Also we can use compare using Binary using this operator >> tha using unit, decimal, and hundred to compare. I think the logic it makes more simple.

# Basic Radix Sort Implementation

```c
void	radix_sort(t_stack *a, t_stack *b)
{
	int	i;
	int	j;
	int	size;
	int	max_bits;

	size = a->size;
	max_bits = get_max_bits(size);
	i = 0;
	while (i < max_bits)
	{
		j = 0;
		while (j < size)
		{
			if (((a->top->index >> i) & 1) == 1) //here is the >> operator 
				ra(a);
			else
				pb(a, b);
			j++;
		}
		while (b->size > 0)
			pa(a, b);
		i++;
	}
}
```
#Array vs linked lists

I was thinking what we should use to make the project.
I think we should use linked lists, to swap, rotate, etc is super simple to do.
Maybe they are not that fast like an array, but I think we should keep it simple with a double linked list.

ex:

```
typedef struct s_node
{
    int             value;
    int             index;
    struct s_node   *next;
}   t_node;

typedef struct s_stack
{
    t_node  *a;     // stack A
    t_node  *b;     // stack B
    int     size_a;
    int     size_b;
}   t_stack;
```
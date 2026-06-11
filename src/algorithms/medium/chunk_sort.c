/* chunk_sort.c */
#include <math.h>
#include "push_swap.h"

/* Cuenta cuántos elementos quedan en A que pertenezcan al chunk [lower, upper) */
static int count_elements_in_chunk(t_stack *a, int chunk_idx, int chunk_size)
{
    t_node *cur;
    int lower = chunk_idx * chunk_size;
    int upper = (chunk_idx + 1) * chunk_size;
    int count = 0;

    cur = a->top;
    while (cur)
    {
        if (cur->index >= lower && cur->index < upper)
            count++;
        cur = cur->next;
    }
    return (count);
}

/* Devuelve la posición (0..size-1) del primer elemento del chunk en A, o -1 */
static int find_first_in_chunk_pos(t_stack *a, int lower, int upper)
{
    t_node *cur = a->top;
    int pos = 0;

    while (cur)
    {
        if (cur->index >= lower && cur->index < upper)
            return (pos);
        cur = cur->next;
        pos++;
    }
    return (-1);
}

/* Rota A de forma óptima para llevar la posición 'pos' al tope */
static void rotate_a_to_pos(t_stack *a, int pos)
{
    int steps;

    if (pos <= 0 || a->size < 2)
        return ;
    if (pos <= a->size / 2)
    {
        while (pos-- > 0)
            ra(a);
    }
    else
    {
        steps = a->size - pos;
        while (steps-- > 0)
            rra(a);
    }
}

/*
 * Encuentra la posición en B del nodo que es el mayor con index < new_index.
 * Si existe, devuelve su posición (0..size-1). Si no existe, devuelve la
 * posición del máximo absoluto en B (para insertar antes del máximo).
 * Si b->size == 0 devuelve 0.
 */
static int get_target_position_b(t_stack *b, int new_index)
{
    t_node *cur;
    int cur_pos = 0;
    int best_below = -1;
    int best_below_pos = -1;
    int max_index = -1;
    int max_pos = 0;

    if (b->size == 0)
        return (0);
    cur = b->top;
    while (cur)
    {
        if (cur->index > max_index)
        {
            max_index = cur->index;
            max_pos = cur_pos;
        }
        if (cur->index < new_index && cur->index > best_below)
        {
            best_below = cur->index;
            best_below_pos = cur_pos;
        }
        cur = cur->next;
        cur_pos++;
    }
    if (best_below_pos != -1)
        return (best_below_pos);
    return (max_pos);
}

/* Rota B de forma óptima para llevar target_pos al tope */
static void rotate_b_to_target(t_stack *b, int target_pos)
{
    int steps;

    if (b->size < 2 || target_pos <= 0)
        return ;
    if (target_pos <= b->size / 2)
    {
        while (target_pos-- > 0)
            rb(b);
    }
    else
    {
        steps = b->size - target_pos;
        while (steps-- > 0)
            rrb(b);
    }
}

/* Lleva el máximo absoluto de B a la cima */
static void align_stack_b(t_stack *b)
{
    t_node *cur;
    int max_index = -1;
    int max_pos = 0;
    int pos = 0;
    int steps;

    if (b->size < 2)
        return ;
    cur = b->top;
    while (cur)
    {
        if (cur->index > max_index)
        {
            max_index = cur->index;
            max_pos = pos;
        }
        cur = cur->next;
        pos++;
    }
    if (max_pos <= b->size / 2)
    {
        while (max_pos-- > 0)
            rb(b);
    }
    else
    {
        steps = b->size - max_pos;
        while (steps-- > 0)
            rrb(b);
    }
}

/* chunk_sort conforme a Medium Tier spec, firma acorde a push_swap.h */
void chunk_sort(t_stack *a, t_stack *b)
{
    int n;
    int chunk_size;
    int num_chunks;
    int chunk_idx;
    int lower;
    int upper;
    int elements_in_chunk;
    int pos_in_a;
    int target_pos;

    if (!a || !b || a->size <= 1)
        return ;

    n = a->size;
    chunk_size = (int)ceil(sqrt((double)n));
    if (chunk_size < 1)
        chunk_size = 1;
    num_chunks = (n + chunk_size - 1) / chunk_size;

    chunk_idx = 0;
    while (chunk_idx < num_chunks)
    {
        lower = chunk_idx * chunk_size;
        upper = lower + chunk_size;
        elements_in_chunk = count_elements_in_chunk(a, chunk_idx, chunk_size);
        while (elements_in_chunk > 0)
        {
            pos_in_a = find_first_in_chunk_pos(a, lower, upper);
            if (pos_in_a == -1)
                break;
            rotate_a_to_pos(a, pos_in_a);
            /* ahora a->top pertenece al chunk */
            target_pos = get_target_position_b(b, a->top->index);
            rotate_b_to_target(b, target_pos);
            /* push desde A a B */
            pb(a, b);
            elements_in_chunk--;
        }
        chunk_idx++;
    }

    align_stack_b(b);
    while (b->size > 0)
        pa(a, b);
}

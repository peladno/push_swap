
static int count_elements_in_chunk(t_stack *a, int i, int chunk_size)
{
    int count = 0;
    t_node *cur = a->top;
    int lower = i * chunk_size;
    int upper = (i + 1) * chunk_size;

    while (cur)
    {
        if (cur->index >= lower && cur->index < upper)
            count++;
        cur = cur->next;
    }
    return count;
}



void    chunk_sort(t_stack *a, t_stack *b)
{
    int num_chunks;
    int chunk_size;
    int i;

    // 1. Conseguir o definir el tamaño del chunk
    chunk_size = 15; // O el número optimizado que elijas para 100

    // 2. Calcular el número total de chunks con el truco del redondeo
    num_chunks = (a->size + chunk_size - 1) / chunk_size;

    // 3. Recorrer cada chunk uno por uno
   // ... código anterior donde calculas num_chunks ...
    i = 0;
    while (i < num_chunks)
    {
            // 1. Contar cuántos elementos del chunk 'i' quedan actualmente en A
        int elements_in_chunk = count_elements_in_chunk(a, i, chunk_size);

            // 2. Mientras queden elementos de este bloque en A, los procesamos
        while (elements_in_chunk > 0)
        {
            // Miramos el nodo que está arriba del todo en A: a->top
        
            if ( /* ¿El nodo a->top pertenece al chunk i? */ )
            {
                // ¡Lo encontramos! 
                // Aquí irá la lógica para meterlo ordenadamente en B (Fase 3)
            
                elements_in_chunk--; // Encontrado y restado del contador
            }
            else
            {
                // Si el de arriba no nos sirve en este momento...
                // ¿Qué operación de push_swap usas para pasar al siguiente nodo?
            }
        }
        i++;
    }
}
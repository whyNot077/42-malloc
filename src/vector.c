#include "malloc.h"

Pointer g_extend_vector = 0;
static Pointer *get_address(size_t index);
static Pointer create_vector(size_t size);
static int resize_vector();
static int vector_is_full();
const int CAPACITY = 100;
#ifdef DEBUG
static void print_vector();
#endif

int init_vector()
{
    if (g_extend_vector)
    {
        return OK;
    }
    g_extend_vector = create_vector(CAPACITY);
    if (!g_extend_vector)
    {
        return ERROR;
    }
    return OK;
}

static Pointer create_vector(size_t size)
{
    size_t required_size = get_aligned_size(size * WSIZE);
    Pointer vector = Mmap(required_size);
    if (!vector)
    {
        return 0;
    }
    VECTOR_CAPACITY(vector) = required_size;
    VECTOR_SIZE(vector) = 2;
    return vector;
}

static int resize_vector()
{
    size_t current_size = VECTOR_CAPACITY(g_extend_vector);
    size_t new_size = get_aligned_size(current_size * 2 * WSIZE);

    Pointer new_vector = Mmap(new_size);
    if (!new_vector)
    {
        return ERROR;
    }
    ft_memcpy((void *)new_vector, (void *)g_extend_vector, current_size * WSIZE);
    free((void *)g_extend_vector);

    g_extend_vector = new_vector;
    VECTOR_CAPACITY(g_extend_vector) = new_size;
    return OK;
}

static int vector_is_full()
{
    return VECTOR_SIZE(g_extend_vector) >= VECTOR_CAPACITY(g_extend_vector);
}

size_t get_vector_index(Pointer bp)
{
    size_t count = VECTOR_SIZE(g_extend_vector);
    size_t i = 0;
    if (count == 2)
    {
        return 0;
    }
    while (i < count && VECTOR_ELEMENT(g_extend_vector, i) && VECTOR_ELEMENT(g_extend_vector, i) < bp)
    {
        i++;
    }
    return i;
}

int insert_to_vector(Pointer bp)
{
    add_log_detail("insert_to_vector");
    if (vector_is_full() && (resize_vector() == ERROR))
    {
        return ERROR;
    }
    size_t count = VECTOR_SIZE(g_extend_vector);
    size_t index = get_vector_index(bp);
    ft_memmove(NEXT(&VECTOR_ELEMENT(g_extend_vector, index)),
               &VECTOR_ELEMENT(g_extend_vector, index),
               (count - index) * WSIZE);
    VECTOR_ELEMENT(g_extend_vector, index) = bp;
    VECTOR_SIZE(g_extend_vector)++;
    return OK;
}

int delete_from_vector(Pointer bp)
{
    size_t count = VECTOR_SIZE(g_extend_vector);
    size_t index = get_vector_index(bp);

    add_log_detail("delete_from_vector");
    if (index >= count || VECTOR_ELEMENT(g_extend_vector, index) != bp)
    {
        return ERROR;
    }
    ft_memmove(&VECTOR_ELEMENT(g_extend_vector, index),
               NEXT(&VECTOR_ELEMENT(g_extend_vector, index)),
               (count - index - 1) * WSIZE);
    VECTOR_SIZE(g_extend_vector)--;
    return OK;
}

static Pointer *get_address(size_t index)
{
    size_t count = VECTOR_SIZE(g_extend_vector);
    if (index >= count)
    {
        return NULL;
    }
    return &VECTOR_ELEMENT(g_extend_vector, index);
}

Pointer *find_start_point(Pointer bp)
{
    size_t index = get_vector_index(bp);
    return get_address(index);
}

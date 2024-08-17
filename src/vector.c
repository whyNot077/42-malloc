#include "malloc.h"

static Pointer *get_address(Pointer vector, size_t index);
static Pointer create_vector(size_t size);
static int resize_vector();
static int vector_is_full(Pointer vector);
static size_t get_vector_index(Pointer vector, Pointer bp);
const int CAPACITY = 100;

int init_vector()
{
    if (!g_segregated_list)
    {
        return ERROR;
    }
    Pointer vector = GET_VECTOR_START_POINT();
    if (vector)
    {
        return OK;
    }
    Pointer new_vector = create_vector(CAPACITY);
    if (!new_vector)
    {
        return ERROR;
    }
    SET_VECTOR_START_POINT((Pointer)(g_segregated_list) + 3 * BLOCK_SIZE + 2 * WSIZE, new_vector);
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
    Pointer vector = GET_VECTOR_START_POINT();
    if (!vector)
    {
        return ERROR;
    }
    size_t current_size = VECTOR_CAPACITY(vector);
    size_t new_size = get_aligned_size(current_size * 2 * WSIZE);
    Pointer new_vector = Mmap(new_size);
    if (!new_vector)
    {
        return ERROR;
    }
    ft_memcpy((void *)new_vector, (void *)vector, current_size * WSIZE);
    Munmap(vector, current_size * WSIZE);
    SET_VECTOR_START_POINT((Pointer)(g_segregated_list) + 3 * BLOCK_SIZE + 2 * WSIZE, new_vector);
    VECTOR_CAPACITY(new_vector) = new_size;
    return OK;
}

static int vector_is_full(Pointer vector)
{
    return VECTOR_SIZE(vector) >= VECTOR_CAPACITY(vector);
}

static size_t get_vector_index(Pointer vector, Pointer bp)
{
    size_t count = VECTOR_SIZE(vector);
    size_t i = 0;
    if (count == 2)
    {
        return 0;
    }
    while (i < count && VECTOR_ELEMENT(vector, i) && VECTOR_ELEMENT(vector, i) < bp)
    {
        i++;
    }
    return i;
}

int insert_to_vector(Pointer bp)
{
    add_log_detail("insert_to_vector");
    Pointer vector = GET_VECTOR_START_POINT();
    if (vector_is_full(vector) && (resize_vector() == ERROR))
    {
        return ERROR;
    }

    size_t count = VECTOR_SIZE(vector);
    size_t index = get_vector_index(vector, bp);
    ft_memmove(NEXT(&VECTOR_ELEMENT(vector, index)),
               &VECTOR_ELEMENT(vector, index),
               (count - index) * WSIZE);
    VECTOR_ELEMENT(vector, index) = bp;
    VECTOR_SIZE(vector)++;
    return OK;
}

int delete_from_vector(Pointer bp)
{
    Pointer vector = GET_VECTOR_START_POINT();
    size_t count = VECTOR_SIZE(vector);
    size_t index = get_vector_index(vector, bp);

    add_log_detail("delete_from_vector");
    if (index >= count || VECTOR_ELEMENT(vector, index) != bp)
    {
        return ERROR;
    }
    ft_memmove(&VECTOR_ELEMENT(vector, index),
               NEXT(&VECTOR_ELEMENT(vector, index)),
               (count - index - 1) * WSIZE);
    VECTOR_SIZE(vector)--;

    return OK;
}

static Pointer *get_address(Pointer vector, size_t index)
{
    size_t count = VECTOR_SIZE(vector);
    if (index >= count)
    {
        return NULL;
    }
    return &VECTOR_ELEMENT(vector, index);
}

Pointer *find_start_point(Pointer bp)
{
    if (!g_segregated_list)
    {
        return NULL;
    }
    Pointer vector = GET_VECTOR_START_POINT();
    size_t index = get_vector_index(vector, bp);
    return get_address(vector, index);
}

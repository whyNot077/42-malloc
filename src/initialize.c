#include "malloc.h"

Base g_segregated_list = 0;
size_t g_small_threshold = 0;
size_t g_large_threshold = 0;

static void initialize_pool_info();
static int initialize_start_point(void);

int init_malloc()
{
    add_log_detail("init_malloc");
    if (g_segregated_list)
    {
        return OK;
    }
    initialize_pool_info();
    if (initialize_start_point() == ERROR)
    {
        return ERROR;
    }
    return init_vector();
}

static void initialize_pool_info()
{
    int page_size = getpagesize();

    size_t tiny_pool_size = 4 * page_size;
    size_t small_pool_size = 128 * page_size;

    g_small_threshold = (tiny_pool_size - PROLOGUE_SIZE - EPILOGUE_SIZE) / 128;
    g_large_threshold = (small_pool_size - PROLOGUE_SIZE - EPILOGUE_SIZE) / 128;
}

void initialize(Base bp, int is_alloc)
{
    PUT(HEAD(bp), PACK(BLOCK_SIZE, is_alloc));
    PUT(FOOT(bp), PACK(BLOCK_SIZE, is_alloc));
    PUT_PTR(NEXT_PTR(bp), 0);
    PUT_PTR(PREV_PTR(bp), bp);
}

static int initialize_start_point(void)
{
    g_segregated_list = Mmap(3 * BLOCK_SIZE);
    if (!g_segregated_list)
    {
        return ERROR;
    }
    Base tiny_root = g_segregated_list + WSIZE;
    Base small_root = tiny_root + BLOCK_SIZE;
    Base large_root = small_root + BLOCK_SIZE;

    initialize(tiny_root, true);
    initialize(small_root, true);
    initialize(large_root, true);
    return OK;
}

t_pool get_pool_type(size_t size)
{
    if (size <= g_small_threshold)
    {
        return TINY;
    }
    else if (size <= g_large_threshold)
    {
        return SMALL;
    }
    else
    {
        return LARGE;
    }
}

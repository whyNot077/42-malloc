#include "malloc.h"

Pointer g_list = 0;

static void get_threshold(int *small_threshold, int *large_threshold);
static int initialize_start_point(void);

int init_malloc()
{
    add_log_detail("init_malloc");
    if (g_list)
    {
        return OK;
    }
    if (initialize_start_point() == ERROR)
    {
        return ERROR;
    }
    init_vector();
    return extend_heap(getpagesize() * 20);
}

static void get_threshold(int *small_threshold, int *large_threshold)
{
    int page_size = getpagesize();

    size_t tiny_pool_size = page_size * 4 - PROLOGUE_SIZE - EPILOGUE_SIZE;
    size_t large_pool_size = page_size * 16 - PROLOGUE_SIZE - EPILOGUE_SIZE;

    *small_threshold = tiny_pool_size / 128 + 1;
    *large_threshold = large_pool_size / 128 + 1;
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
    int small_threshold, large_threshold;
    get_threshold(&small_threshold, &large_threshold);

    g_list = Mmap(4 * BLOCK_SIZE);
    if (!g_list)
    {
        return ERROR;
    }
    Base tiny_root = g_list + WSIZE;
    Base small_root = tiny_root + BLOCK_SIZE;
    Base large_root = small_root + BLOCK_SIZE;

    initialize(tiny_root, true);
    initialize(small_root, true);
    initialize(large_root, true);

    PUT((Pointer)(g_list + 3 * BLOCK_SIZE), small_threshold);
    PUT((Pointer)(g_list + 3 * BLOCK_SIZE + WSIZE), large_threshold);
    PUT_PTR((Pointer)(g_list + 3 * BLOCK_SIZE + 2 * WSIZE), 0);
    PUT((Pointer)(g_list + 3 * BLOCK_SIZE + 3 * WSIZE), EMPTY);

    atexit(release_all);
    return OK;
}

t_pool get_pool_type(size_t size)
{
    if (size <= GET_SMALL_THRESHOLD())
    {
        return TINY;
    }
    else if (size <= GET_LARGE_THRESHOLD())
    {
        return SMALL;
    }
    else
    {
        return LARGE;
    }
}

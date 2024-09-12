#include "malloc.h"
static void free_segregated_list();
static void free_bp(Pointer bp);
static int free_vector();
static void free_empty_heap();

void free(void *bp)
{
    if (bp == 0)
        return;
    lock();
    open_log_file();
    free_bp((Pointer)bp);
    free_empty_heap();
    unlock();
    close_log_file();
}

void release_all()
{
    free_vector();
    free_segregated_list();
    close_log_file();
    destroy_lock();
}

int is_heap_empty(Base prologue_bp)
{
    Base next_block = NEXT_BLK(prologue_bp);
    Base epilogue_block = NEXT_BLK(next_block);
    return IS_ALLOC(HEAD(epilogue_block)) && GET_SIZE(HEAD(epilogue_block)) == 0;
}

static void free_empty_heap()
{
    Pointer vector = GET_VECTOR_START_POINT();
    for (size_t i = VECTOR_DATA_NUM(vector); i > 0; i--)
    {
        Base prologue_bp = VECTOR_ELEMENT(vector, i - 1);
        if (VECTOR_DATA_NUM(vector) > 1 && is_heap_empty(prologue_bp))
        {
            remove_from_list(NEXT_BLK(prologue_bp));
            delete_from_vector(prologue_bp);
            size_t size = GET_SIZE(HEAD(NEXT_BLK(prologue_bp))) + PROLOGUE_SIZE + EPILOGUE_SIZE;
            Munmap(HEAD(prologue_bp), size);
            add_log(DELETE_HEAP, prologue_bp, 0);
        }
    }
}

static void free_bp(Pointer bp)
{
    size_t size = GET_SIZE(HEAD(bp));
    PUT(HEAD(bp), PACK(size, false));
    PUT(FOOT(bp), PACK(size, false));
    PUT_PTR(NEXT_PTR(bp), 0);
    PUT_PTR(PREV_PTR(bp), 0);

    add_log_detail("free_bp");
    add_log(FREE, bp, size);
    t_pool type = get_pool_type(GET_SIZE(HEAD(bp)));
    coalesce((Pointer)bp, type);
}

static void free_segregated_list()
{
    add_log_detail("free_segregated_list");
    Munmap(g_list, 4 * BLOCK_SIZE);
    g_list = 0;
}

static int free_vector()
{
    add_log_detail("free_vector");
    Pointer vector = GET_VECTOR_START_POINT();
    if (vector)
    {
        Munmap(vector, VECTOR_SIZE(vector) * WSIZE);
        SET_VECTOR_START_POINT((Pointer)(g_list) + 3 * BLOCK_SIZE + 2 * WSIZE, 0);
    }

    return OK;
}

#include "malloc.h"
static void free_segregated_list();
static void free_bp(Pointer bp);
static int free_vector();
static int not_exist_allocated_block();

void free(void *bp)
{
    if (bp == 0)
        return;
    lock();
    open_log_file();
    free_bp((Pointer)bp);
    if (not_exist_allocated_block())
    {
        free_vector();
        free_segregated_list();
        close_log_file();
        destroy_lock();
    }
    unlock();
    close_log_file();
}

int is_heap_empty(Base prologue_bp)
{
    Base next_block = NEXT_BLK(prologue_bp);
    Base epilogue_block = NEXT_BLK(next_block);
    return IS_ALLOC(HEAD(epilogue_block)) && GET_SIZE(HEAD(epilogue_block)) == 0;
}

static int not_exist_allocated_block()
{
    for (size_t i = 0; i < VECTOR_SIZE(g_extend_vector) - 2; i++)
    {
        Base prologue_bp = VECTOR_ELEMENT(g_extend_vector, i);
        if (is_heap_empty(prologue_bp))
        {
            remove_from_list(NEXT_BLK(prologue_bp));
            delete_from_vector(prologue_bp);
            Munmap(HEAD(prologue_bp));
            add_log(DELETE_HEAP, prologue_bp, 0);
        }
    }
    return VECTOR_SIZE(g_extend_vector) - 2 == 0;
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
    Munmap(g_segregated_list);
    g_segregated_list = 0;
}

static int free_vector()
{
    add_log_detail("free_vector");
    Munmap(g_extend_vector);
    g_extend_vector = 0;
    return OK;
}
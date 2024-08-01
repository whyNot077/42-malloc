#include "malloc.h"

static Pointer merge_with_next(Pointer bp, size_t size, int isRealloc);
static Pointer realloc_with_next(Pointer bp, size_t size, size_t newSize, int *shouldMove);
static Pointer merge_with_prev(Pointer bp, size_t size, int isRealloc);
static Pointer realloc_with_prev(Pointer bp, size_t size, size_t newSize);
static Pointer merge_with_prev_and_next(Pointer bp, size_t size, int isRealloc);
static Pointer realloc_with_prev_and_next(Pointer bp, size_t size, size_t newSize);

Base coalesce(Base bp, t_pool type)
{
    size_t size = GET_SIZE(HEAD(bp));
    Base prev_bp = PREV_BLK(bp);
    Base next_bp = NEXT_BLK(bp);
    size_t prev_alloc = IS_ALLOC(FOOT(prev_bp));
    size_t next_alloc = IS_ALLOC(HEAD(next_bp));
    add_log_detail("coalesce");

    if (prev_alloc && !next_alloc)
    {
        size += GET_SIZE(HEAD(next_bp));
        bp = merge_with_next(bp, size, false);
        add_log_detail("merge_with_next");
    }
    else if (!prev_alloc && next_alloc)
    {
        size += GET_SIZE(HEAD(prev_bp));
        bp = merge_with_prev(bp, size, false);
        add_log_detail("merge_with_prev");
    }
    else if (!prev_alloc && !next_alloc)
    {
        size += GET_SIZE(FOOT(next_bp)) + GET_SIZE(HEAD(prev_bp));
        bp = merge_with_prev_and_next(bp, size, false);
        add_log_detail("merge_with_prev_and_next");
    }
    insert_to_root(bp, type);
    return bp;
}

Base realloc_coalesce(Pointer bp, size_t newSize, int *shouldMove)
{
    size_t size = GET_SIZE(HEAD(bp));
    Base prev_bp = PREV_BLK(bp);
    Base next_bp = NEXT_BLK(bp);
    size_t prev_alloc = IS_ALLOC(FOOT(prev_bp));
    size_t next_alloc = IS_ALLOC(HEAD(next_bp));
    add_log_detail("realloc_coalesce");

    if (prev_alloc && !next_alloc)
    {
        size += GET_SIZE(HEAD(NEXT_BLK(bp)));
        bp = realloc_with_next(bp, size, newSize, shouldMove);
        add_log_detail("realloc_with_next");
    }
    else if (!prev_alloc && next_alloc)
    {
        size += GET_SIZE(HEAD(PREV_BLK(bp)));
        bp = realloc_with_prev(bp, size, newSize);
        add_log_detail("realloc_with_prev");
    }
    else if (!prev_alloc && !next_alloc)
    {
        size += GET_SIZE(FOOT(NEXT_BLK(bp))) + GET_SIZE(HEAD(PREV_BLK(bp)));
        bp = realloc_with_prev_and_next(bp, size, newSize);
        add_log_detail("realloc_with_prev_and_next");
    }
    return bp;
}

static Pointer merge_with_next(Pointer bp, size_t size, int isRealloc)
{
    remove_from_list(NEXT_BLK(bp));
    PUT(HEAD(bp), PACK(size, isRealloc));
    PUT(FOOT(bp), PACK(size, isRealloc));
    return bp;
}

static Pointer realloc_with_next(Pointer bp, size_t size, size_t newSize, int *shouldMove)
{
    if (size >= newSize)
    {
        *shouldMove = false;
        return merge_with_next(bp, size, true);
    }
    return bp;
}

static Pointer merge_with_prev(Pointer bp, size_t size, int isRealloc)
{
    Pointer prev_bp = PREV_BLK(bp);
    remove_from_list(prev_bp);
    PUT(FOOT(bp), PACK(size, isRealloc));
    PUT(HEAD(prev_bp), PACK(size, isRealloc));
    return prev_bp;
}

static Pointer realloc_with_prev(Pointer bp, size_t size, size_t newSize)
{
    if (size >= newSize)
    {
        return merge_with_prev(bp, size, true);
    }
    return bp;
}

static Pointer merge_with_prev_and_next(Pointer bp, size_t size, int isRealloc)
{
    Pointer prev_bp = PREV_BLK(bp);
    Pointer next_bp = NEXT_BLK(bp);
    remove_from_list(prev_bp);
    remove_from_list(next_bp);
    PUT(FOOT(next_bp), PACK(size, isRealloc));
    PUT(HEAD(prev_bp), PACK(size, isRealloc));
    return prev_bp;
}

static Pointer realloc_with_prev_and_next(Pointer bp, size_t size, size_t newSize)
{
    if (size >= newSize)
    {
        return merge_with_prev_and_next(bp, size, true);
    }
    return bp;
}

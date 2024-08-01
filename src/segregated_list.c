#include "malloc.h"

static void find_insertion_point(Base root, Base new, Base *prevp, Base *nextp);
static void link_block(Base new, Base prevp, Base nextp);
static void unlink_block(Base prevp, Base nextp);
static Base first_fit(size_t request_size, t_pool type);

void insert_to_root(Pointer bp, t_pool type)
{
    Base root = GET_PTR(get_free_root_by_type(type));
    Base prevp;
    Base nextp;

    add_log_detail("before insert_to_root");
    add_log_linked_blocks(bp);

    find_insertion_point(root, bp, &prevp, &nextp);
    link_block(bp, prevp, nextp);

    add_log_detail("after insert_to_root");
    add_log_linked_blocks(bp);
}

Base get_free_root_by_type(t_pool type)
{
    if (g_segregated_list == 0)
    {
        return 0;
    }
    return g_segregated_list + ((type - 1) * BLOCK_SIZE) + WSIZE;
}

static void unlink_block(Base prevp, Base nextp)
{
    if (nextp)
    {
        PUT_PTR(PREV_PTR(nextp), prevp);
    }
    PUT_PTR(NEXT_PTR(prevp), nextp);
}

void remove_from_list(Pointer bp)
{
    add_log_detail("remove_from_list");
    t_pool type = get_pool_type(GET_SIZE(HEAD(bp)));
    Base root = GET_PTR(get_free_root_by_type(type));
    add_log_detail("before remove_from_list");
    add_log_linked_blocks(root);

    Pointer prevp = GET_PTR(PREV_PTR(bp));
    Pointer nextp = GET_PTR(NEXT_PTR(bp));

    unlink_block(prevp, nextp);

    PUT_PTR(NEXT_PTR(bp), 0);
    PUT_PTR(PREV_PTR(bp), 0);

    add_log_detail("after remove_from_list");
    add_log_linked_blocks(root);
}

size_t get_aligned_size(size_t size)
{
    if (size <= DSIZE)
    {
        return BLOCK_SIZE;
    }
    return ALIGN(size + DSIZE);
}

void place(Pointer bp, size_t required)
{
    size_t total = GET_SIZE(HEAD(bp));
    if ((total - required) >= BLOCK_SIZE)
    {
        /* If the remaining part of the block is larger than the minimum block size,
        it splits the block and places the remaining part back into the free list.
        */
        PUT(HEAD(bp), PACK(required, 1));
        PUT(FOOT(bp), PACK(required, 1));

        Pointer next_bp = NEXT_BLK(bp);
        PUT(HEAD(next_bp), PACK(total - required, 0));
        PUT(FOOT(next_bp), PACK(total - required, 0));
        PUT_PTR(NEXT_PTR(next_bp), 0);
        PUT_PTR(PREV_PTR(next_bp), 0);
        add_log_detail("place and split");
        add_log(PLACE, bp, required);
        coalesce(next_bp, get_pool_type(next_bp));
    }
    else
    {
        PUT(HEAD(bp), PACK(total, 1));
        PUT(FOOT(bp), PACK(total, 1));
        add_log_detail("place");
        add_log(PLACE, bp, total);
    }
}

static Base first_fit(size_t request_size, t_pool type)
{
    Base tmp = GET_PTR(get_free_root_by_type(type));
    add_log_detail("first_fit");
    add_log_detail("before first_fit");
    add_log_linked_blocks(tmp);
    tmp = GET_PTR(NEXT_PTR(tmp));
    while (tmp)
    {
        size_t block_size = GET_SIZE(HEAD(tmp));
        if (block_size >= request_size)
        {
            add_log_detail("after first_fit");
            add_log_linked_blocks(tmp);
            return tmp;
        }
        tmp = GET_PTR(NEXT_PTR(tmp));
    }
    add_log_detail("cannot found");
    return 0;
}

Base find_location(size_t size)
{
    add_log_detail("find_location");
    for (int i = TINY; i <= LARGE; i++)
    {
        Base bp = first_fit(size, i);
        if (bp)
        {
            remove_from_list(bp);
            place(bp, size);
            return bp;
        }
    }
    add_log_detail("not found location");
    return 0;
}

static void find_insertion_point(Base root, Base new, Base *prevp, Base *nextp)
{
    *prevp = root;
    *nextp = GET_PTR(NEXT_PTR(root));
    while (*nextp)
    {
        if (GET_SIZE(HEAD(new)) <= GET_SIZE(HEAD(*nextp)))
        {
            break;
        }
        *prevp = *nextp;
        *nextp = GET_PTR(NEXT_PTR(*nextp));
    }
}

static void link_block(Base new, Base prevp, Base nextp)
{
    PUT_PTR(NEXT_PTR(prevp), new);
    PUT_PTR(PREV_PTR(new), prevp);
    if (new != nextp)
    {
        PUT_PTR(NEXT_PTR(new), nextp);
    }
    if (nextp)
    {
        PUT_PTR(PREV_PTR(nextp), new);
    }
}

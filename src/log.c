#include "malloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef BONUS
static void print_event(t_event t_event, Pointer ptr, size_t size);
static void print_vector();
static void print_nearby_blocks(Base bp);
static void print_ptr_and_size(Pointer ptr, size_t size);
static t_status getenv_cached(t_env env);
static void ft_itoa_fd(size_t nb, char base, int fd);
#endif

void open_log_file()
{
    if (!g_segregated_list)
    {
        return;
    }
    int log_fd = GET_LOG_FD();
    if (log_fd != EMPTY)
    {
        return;
    }
#ifdef BONUS
    log_fd = open(LOG_PATH, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (log_fd == -1)
    {
        log_fd = EMPTY;
        return;
    }
    PUT(g_segregated_list + 3 * BLOCK_SIZE + 3 * WSIZE, log_fd);
#endif
}

void close_log_file()
{
#ifdef BONUS
    if (!g_segregated_list)
    {
        return;
    }
    int log_fd = GET_LOG_FD();
    if (log_fd)
        close(log_fd);
    PUT(g_segregated_list + 3 * BLOCK_SIZE + 3 * WSIZE, EMPTY);
#endif
}

void add_log_detail(const char *msg)
{
#ifdef BONUS
    if (!g_segregated_list)
    {
        return;
    }
    int log_fd = GET_LOG_FD();
    if (log_fd == EMPTY)
    {
        return;
    }
    if (getenv_cached(LOG_DETAIL))
    {
        ft_putstr_fd(log_fd, "\n");
        ft_putstr_fd(log_fd, msg);
        ft_putstr_fd(log_fd, "\n");
    }
#else
    (void)msg;
#endif
}

void add_log(t_event t_event, Pointer ptr, size_t size)
{
#ifdef BONUS
    int log_fd = GET_LOG_FD();
    if (log_fd == EMPTY)
    {
        return;
    }
    if (getenv_cached(LOG) || getenv_cached(LOG_DETAIL))
    {
        print_event(t_event, ptr, size);
    }
    if (getenv_cached(LOG_DETAIL))
    {
        if (t_event == PLACE)
            print_nearby_blocks(ptr);
        else if (t_event == NEW_HEAP)
            print_vector();
    }
#else
    (void)t_event;
    (void)ptr;
    (void)size;
#endif
}

void add_log_linked_blocks(Pointer root)
{
#ifdef BONUS
    int log_fd = GET_LOG_FD();
    if (getenv_cached(LOG_DETAIL))
    {
        return;
    }
    Pointer tmp = root;
    ft_putstr_fd(log_fd, "\n\nlinked_blocks: ");
    ft_putstr_fd(log_fd, "\nroot = ");
    ft_itoa_fd((size_t)root, 16, log_fd);
    while (tmp)
    {
        print_ptr_and_size(tmp, GET_SIZE(HEAD(tmp)));
        tmp = GET_PTR(NEXT_PTR(tmp));
    }
#else
    (void)root;
#endif
}

#ifdef BONUS
static void print_event(t_event t_event, Pointer ptr, size_t size)
{
    int log_fd = GET_LOG_FD();
    ft_putstr_fd(log_fd, "\n\n\n---- ");
    if (t_event == FREE)
        ft_putstr_fd(log_fd, "Free");
    else if (t_event == PLACE)
        ft_putstr_fd(log_fd, "Place");
    else if (t_event == NEW_HEAP)
        ft_putstr_fd(log_fd, "New Heap");
    else if (t_event == DELETE_HEAP)
        ft_putstr_fd(log_fd, "Delete Heap");
    print_ptr_and_size(ptr, size);
}

static void print_ptr_and_size(Pointer ptr, size_t size)
{
    int log_fd = GET_LOG_FD();
    if (!ptr)
    {
        ptr = 0;
        size = 0;
    }
    ft_putstr_fd(log_fd, " ( ");
    ft_itoa_fd((size_t)ptr, 16, log_fd);
    ft_putstr_fd(log_fd, ", ");
    ft_itoa_fd(size, 10, log_fd);
    ft_putstr_fd(log_fd, " )\n");
}

static t_status getenv_cached(t_env env)
{
    static uint32_t env_cache;
    static t_status init;

    if (!init)
    {
        if (getenv("log"))
            env_cache |= LOG;
        if (getenv("log_detail"))
            env_cache |= LOG_DETAIL;
        init = OK;
    }
    return (env & env_cache);
}

static void print_bp_and_size(Base bp)
{
    size_t size = 0;
    if (bp)
    {
        size = GET_SIZE(HEAD(bp));
    }
    print_ptr_and_size(bp, size);
}

static void print_nearby_blocks(Base bp)
{
    int log_fd = GET_LOG_FD();
    Base prev_bp = PREV_BLK(bp);
    Base next_bp = NEXT_BLK(bp);

    ft_putstr_fd(log_fd, "\n print_nearby_blocks (prev - bp - next)\n");
    print_bp_and_size(prev_bp);
    print_bp_and_size(bp);
    print_bp_and_size(next_bp);
}

static void print_vector()
{
    if (!g_segregated_list)
    {
        return;
    }
    int log_fd = GET_LOG_FD();
    Pointer vector = GET_VECTOR_START_POINT();
    size_t count = VECTOR_SIZE(vector) - 2;
    ft_putstr_fd(log_fd, "\n\n--------Print vector : Vector contents ");
    ft_putstr_fd(log_fd, " (");
    ft_itoa_fd(count, 10, log_fd);
    ft_putstr_fd(log_fd, " elements)  : (ptr, size)--------\n");
    ft_putstr_fd(log_fd, "vector = ");
    ft_itoa_fd((size_t)vector, 16, log_fd);

    for (size_t i = 0; i < count; i++)
    {
        void *ptr = (void *)VECTOR_ELEMENT(vector, i);
        ft_putstr_fd(log_fd, "(");
        ft_itoa_fd((size_t)ptr, 16, log_fd);
        if (ptr)
        {
            ft_putstr_fd(log_fd, ", ");
            ft_itoa_fd(GET_SIZE(HEAD(ptr)), 10, log_fd);
            ft_putstr_fd(log_fd, ")\n");
        }
        else
        {
            ft_putstr_fd(log_fd, ", 0)\n");
        }
    }
}

static void ft_itoa_fd(size_t nb, char base, int fd)
{
    char *str;

    str = "0123456789ABCDEFGHIJKLMNOPQRSTUIVWXYZ";
    if (nb / base)
        ft_itoa_fd(nb / base, base, fd);
    else if (base == 16)
        write(fd, "0x", 2);
    write(fd, &str[nb % base], 1);
}
#endif
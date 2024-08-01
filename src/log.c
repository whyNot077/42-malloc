#include "malloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void print_event(t_event t_event, Pointer ptr, size_t size);
static void print_nearby_blocks(Base bp);
static void print_vector();
static void print_ptr_and_size(Pointer ptr, size_t size);
static t_status getenv_cached(t_env env);
#ifdef BONUS
pthread_mutex_t g_log_lock = PTHREAD_MUTEX_INITIALIZER;
#endif
static void file_lock();
static void file_unlock();
static void ft_itoa_fd(size_t nb, char base, int fd);

int log_fd = EMPTY;

void open_log_file()
{
    file_lock();
    if (log_fd != EMPTY)
    {
        file_unlock();
        return;
    }
#ifdef BONUS
    log_fd = open(LOG_PATH, O_CREAT | O_WRONLY | O_APPEND, 0644);
    file_unlock();
    if (log_fd == -1)
    {
        log_fd = EMPTY;
        return;
    }
#endif
}

void close_log_file()
{
#ifdef BONUS
    file_lock();
    if (log_fd)
        close(log_fd);
    log_fd = EMPTY;
    file_unlock();
#endif
}

void add_log_detail(const char *msg)
{
    file_lock();
    if (log_fd == EMPTY)
    {
        file_unlock();
        return;
    }
    if (getenv_cached(LOG_DETAIL))
    {
        ft_putstr_fd(log_fd, "\n");
        ft_putstr_fd(log_fd, msg);
        ft_putstr_fd(log_fd, "\n");
    }
    file_unlock();
}

void add_log(t_event t_event, Pointer ptr, size_t size)
{
    file_lock();
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
    file_unlock();
}

static void print_event(t_event t_event, Pointer ptr, size_t size)
{
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

static void file_lock()
{
#ifdef BONUS
    pthread_mutex_lock(&g_log_lock);
#endif
}

static void file_unlock()
{
#ifdef BONUS
    pthread_mutex_unlock(&g_log_lock);
#endif
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

void print_nearby_blocks(Base bp)
{
    Base prev_bp = PREV_BLK(bp);
    Base next_bp = NEXT_BLK(bp);

    ft_putstr_fd(log_fd, "\n print_nearby_blocks (prev - bp - next)\n");
    print_bp_and_size(prev_bp);
    print_bp_and_size(bp);
    print_bp_and_size(next_bp);
}

void add_log_linked_blocks(Pointer root)
{
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
}

static void print_vector()
{
    size_t count = VECTOR_SIZE(g_extend_vector) - 2;
    ft_putstr_fd(log_fd, "\n\n--------Print vector : Vector contents ");
    ft_putstr_fd(log_fd, " (");
    ft_itoa_fd(count, 10, log_fd);
    ft_putstr_fd(log_fd, " elements)  : (ptr, size)--------\n");
    ft_putstr_fd(log_fd, "g_extend_vector = ");
    ft_itoa_fd((size_t)g_extend_vector, 16, log_fd);

    for (size_t i = 0; i < count; i++)
    {
        void *ptr = (void *)VECTOR_ELEMENT(g_extend_vector, i);
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
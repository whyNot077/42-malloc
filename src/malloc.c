#include "malloc.h"

static rlim_t get_system_limit(void);
static Pointer make_bigger(Pointer ptr, size_t current_size, size_t new_size);
static Pointer request(size_t size);

void *malloc(size_t size)
{
    lock();
    open_log_file();
    add_log_detail("malloc");
    if (size == 0 || size > INT_MAX)
    {
        unlock();
        return NULL;
    }
    Pointer res = request(size);
    if (res == 0)
    {
        unlock();
        return NULL;
    }
    unlock();
    return (void *)res;
}

Pointer Mmap(size_t size)
{
    if (size >= get_system_limit())
    {
        return raise_error("Memory allocation error: Total pool size exceeds system limit.");
    }
    void *new_memory = (void *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_memory == MAP_FAILED)
    {
        return raise_error("Mmap: Unable to map memory.");
    }
    return (Pointer)new_memory;
}

void Munmap(Pointer head)
{
    if (munmap((void *)head, GET_SIZE(head)) < 0)
    {
        raise_error("Munmap: Unable to unmap memory.");
    }
}

void *realloc(void *ptr, size_t size)
{
    lock();
    add_log_detail("realloc");
    if (ptr == NULL)
    {
        unlock();
        return malloc(size);
    }
    if (size == 0)
    {
        free(ptr);
        unlock();
        return NULL;
    }
    size_t current_size = GET_SIZE(HEAD(ptr));
    size_t new_size = get_aligned_size(size);
    if (current_size == new_size)
    {
        unlock();
        return ptr;
    }
    else if (current_size > new_size)
    {
        place((Pointer)ptr, new_size);
        unlock();
        return ptr;
    }
    else
    {
        return (void *)make_bigger((Pointer)ptr, current_size, new_size);
    }
}

Pointer make_bigger(Pointer ptr, size_t current_size, size_t new_size)
{
    int shouldMove = true;
    Pointer new_ptr;
    Pointer bp = realloc_coalesce(ptr, new_size, &shouldMove);
    add_log_detail("make bigger");
    if (shouldMove == false)
    {
        add_log_detail("do not move");
        place(bp, new_size);
        unlock();
        return bp;
    }
    if (bp != ptr)
    {
        add_log_detail("move");
        ft_memcpy((void *)bp, (void *)ptr, current_size);
        place(bp, new_size);
        unlock();
        return bp;
    }
    else
    {
        add_log_detail("make another block");
        new_ptr = (Pointer)malloc(new_size);
        if (!new_ptr)
        {
            free((void *)ptr);
            unlock();
            return 0;
        }
        ft_memcpy((void *)new_ptr, (void *)ptr, current_size);
        free((void *)ptr);
        unlock();
        return new_ptr;
    }
}

static Pointer request(size_t size)
{
    if (init_malloc() == ERROR)
    {
        return 0;
    }
    size_t aligned_size = get_aligned_size(size);
    Base bp = find_location(aligned_size);
    if (bp)
    {
        return bp;
    }
    if (extend_heap(aligned_size) == ERROR)
    {
        return 0;
    }
    add_log(NEW_HEAP, bp, aligned_size);
    return find_location(aligned_size);
}

static rlim_t get_system_limit(void)
{
    struct rlimit rpl;

    if (getrlimit(RLIMIT_DATA, &rpl) < 0)
    {
        return -1;
    }
    return rpl.rlim_max;
}

int raise_error(char *message)
{
    perror(message);
    return ERROR;
}

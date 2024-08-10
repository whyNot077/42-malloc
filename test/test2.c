#include "../include/malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *test_malloc(size_t size, int is_show_mem);
void test_free(void *ptr, int is_show_mem);
void *test_realloc(void *ptr, size_t size, int is_show_mem);
void show_deeper(void *ptr, int size);
void show_mem(int extension);

int main(void)
{
#ifdef DEBUG
    printf("Debug mode is enabled.\n");
#endif

    void *ptr1 = test_realloc(NULL, 100, 1);
    size_t max_alloc = INT_MAX - DSIZE - PROLOGUE_SIZE - EPILOGUE_SIZE;
    ptr1 = test_realloc(ptr1, 500, 1);
    ptr1 = test_realloc(ptr1, 0, 1);
    ptr1 = test_realloc(ptr1, 3725, 1);
    ptr1 = test_realloc(ptr1, 100000, 1);
    ptr1 = test_realloc(ptr1, max_alloc, 1);
    ptr1 = test_realloc(ptr1, 100000, 1);
    ptr1 = test_realloc(ptr1, 1000, 1);
    void* ptr2 = test_malloc(500, 1);
    void* ptr3 = test_malloc(1000, 1);
    void* ptr4 = test_malloc(10000, 1);
    void* ptr5 = test_malloc(100000, 1);
    void* ptr6 = test_malloc(1000000, 1);

    test_free(ptr1, 1);
    test_free(ptr2, 1);
    test_free(ptr3, 1);
    test_free(ptr4, 1);
    test_free(ptr5, 1);
    test_free(ptr6, 1);
    printf("\ntest(tiny, small, large alloc + realloc) successn");
    return 0;
}

void *test_malloc(size_t size, int is_show_mem)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        printf("Failed to allocate %zu bytes.\n", size);
        return NULL;
    }
    printf("Successfully allocated %zu bytes at %p.\n", size, ptr);
    if (is_show_mem)
    {
        show_mem(false);
    }
    return ptr;
}

void test_free(void *ptr, int is_show_mem)
{
    if (ptr)
    {
        printf("size: %zu, ", GET_SIZE(HEAD(ptr)));
    }
    else 
    {
        printf("size: 0, ");
    }
    free(ptr);
    printf("Freed %p.\n", ptr);
    if (is_show_mem)
    {
        show_mem(false);
    }
}

void *test_realloc(void *ptr, size_t size, int is_show_mem)
{
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL)
    {
        printf("free %p\n", ptr);
    }
    else
    {
        printf("Successfully reallocated %zu bytes at %p.\n", size, new_ptr);
    }
    if (is_show_mem)
    {
        show_mem(false);
    }
    return new_ptr;
}


void show_deeper(void *ptr, int size)
{
    for (int i = 0; i < size; i++)
    {
        ((char *)ptr)[i] = 'a';
    }
    show_mem(true);
}

void show_mem(int extension)
{
    ft_putstr("\n====================================\n");
    ft_putstr("Free blocks\n");
    show_free_mem();
    ft_putstr("\nAllocated blocks\n");
    if (extension)
        show_alloc_mem_ex();
    else
        show_alloc_mem();

    ft_putstr("\nprologue + epilogue = ");
    print_base(PROLOGUE_SIZE + EPILOGUE_SIZE, 10, 0);
    ft_putstr("\nheader + footer = ");
    print_base(DSIZE, 10, 0);
    ft_putstr("\n====================================\n\n");

}

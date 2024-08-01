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

    int size = 800;
    void* tiny[size];
    void* small[size];
    void* large[size];
    for (int i = 0; i < size; i++)
    {
        tiny[i] = test_malloc(50, 0);
        small[i] = test_malloc(300, 0);
        large[i] = test_malloc(INT_MAX, 0);
    }
    show_mem(0);

    for (int i = 0; i < size; i+= 2)
    {
        test_free(tiny[i], 0);
        test_free(small[i], 0);
        test_free(large[i], 0);
    }
    show_mem(0);
    for (int i = 1; i < size; i+= 2)
    {
        test_free(tiny[i], 0);
        test_free(small[i], 0);
        test_free(large[i], 0);
    }
    show_mem(0);
    printf("INT_MAX = %d\n", INT_MAX);
    // while (true)
    // {
    //     sleep(10000);
    // }
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

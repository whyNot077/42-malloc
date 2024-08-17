#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <fcntl.h>
#include "../libft/libft.h"
#include <pthread.h>
#include <assert.h>

typedef uintptr_t Base;
typedef uintptr_t Pointer;

typedef enum
{
  NONE,
  TINY,
  SMALL,
  LARGE
} t_pool;

typedef enum
{
  ERROR,
  OK
} t_status;

typedef enum
{
  LOG = 1 << 0,
  LOG_DETAIL = 1 << 1,
} t_env;

typedef enum
{
  FREE,
  PLACE,
  NEW_HEAP,
  DELETE_HEAP,
} t_event;

/* malloc.c*/
void *malloc(size_t size) __attribute__((malloc));
Pointer Mmap(size_t size);
void Munmap(Pointer head);
int raise_error(char *message);

void free(void *bp);
void *realloc(void *ptr, size_t size);

/* initialize.c */
int init_malloc();
Pointer init_heap(t_pool type);

/* heap.c */
t_pool get_pool_type(size_t size);
Base new_heap(size_t words);
Pointer extend_heap(size_t words);
int is_heap_empty(Base prologue_bp);

/* vector.c */
Pointer *find_start_point(Pointer bp);
int init_vector();
int insert_to_vector(Pointer bp);
size_t get_vector_index(Pointer bp);
int delete_from_vector(Pointer bp);

/* segregated_list.c*/
void insert_to_root(Pointer bp, t_pool type);
void remove_from_list(Pointer bp);
Base get_free_root_by_type(t_pool type);
size_t get_aligned_size(size_t size);
Base coalesce(Base bp, t_pool type);
void place(Pointer bp, size_t required);
Base realloc_coalesce(Pointer bp, size_t newSize, int *shouldMove);
Base find_location(size_t size);

/* show.c */
void show_alloc_mem(void);
void show_alloc_mem_ex(void);
void show_free_mem(void);
void print_base(Pointer ptr, int base, int prefix);

/* mutex.c */
void lock();
void unlock();
void destroy_lock();

/* log.c */
void open_log_file();
void close_log_file();
void add_log(t_event t_event, Pointer ptr, size_t size);
void add_log_detail(const char *msg);
void add_log_linked_blocks(Pointer root);

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)
#define EMPTY -1
#define LOG_PATH "./trace/malloc.log"

#define WSIZE sizeof(Pointer) /* Word size */
#define DSIZE (2 * WSIZE)     /* Double word size */
#define BLOCK_SIZE (2 * DSIZE)
#define PROLOGUE_SIZE (DSIZE)
#define EPILOGUE_SIZE (WSIZE)

#define PACK(size, alloc) ((size) | (alloc))

#define GET(p) (*(Pointer *)(p))
#define PUT(p, val) (*(Pointer *)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define IS_ALLOC(p) (GET(p) & 0x1)

#define HEAD(bp) ((Pointer)(bp) - WSIZE)
#define FOOT(bp) ((Pointer)(bp) + GET_SIZE(HEAD(bp)) - DSIZE)
#define PREV_FOOT(bp) ((Pointer)(bp) - DSIZE)

#define NEXT_BLK(bp) ((Pointer)(bp) + GET_SIZE(HEAD(bp)))
#define PREV_BLK(bp) ((Pointer)(bp) - GET_SIZE(PREV_FOOT(bp)))

#define PREV_PTR(bp) ((Pointer *)(bp))
#define NEXT_PTR(bp) ((Pointer *)((Pointer)(bp) + WSIZE))

#define GET_PTR(p) ((Pointer)(GET(p)))
#define PUT_PTR(p, val) (PUT(p, (Pointer)(val)))

#define VECTOR_CAPACITY(ptr) (*((size_t *)(ptr)))
#define VECTOR_SIZE(ptr) (*((size_t *)((Pointer)(ptr) + WSIZE)))
#define VECTOR_ELEMENT(ptr, index) (*((Pointer *)((Pointer)(ptr) + ((index) + 2) * WSIZE)))
#define NEXT(ptr) ((Pointer *)((Pointer)(ptr) + WSIZE))
#define PREV(ptr) ((Pointer *)((Pointer)(ptr) - WSIZE))

extern pthread_mutex_t g_memory_lock;
extern Base g_segregated_list; // free block list
extern Pointer g_extend_vector;
extern size_t g_small_threshold;
extern size_t g_large_threshold;
extern int log_fd;

/*
 * Simple allocator based on implicit free lists with boundary
 * tag coalescing. Each block has HEAD and FOOT of the form:
 *
 *      31                     3  2  1  0
 *      -----------------------------------
 *     | s  s  s  s  ... s  s  s  0  0  a/f
 *      -----------------------------------
 *
 * where s are the meaningful size bits and a/f is set
 * iff the block is allocated. The list has the following form:
 *
 * begin                                                          end
 * heap                                                           heap
 *  -----------------------------------------------------------------
 * |  pad   | hdr(8:a) | ftr(8:a) | zero or more usr blks | hdr(8:a) |
 *  -----------------------------------------------------------------
 *          |       prologue      |                       | epilogue |
 *          |         block       |                       | block    |
 *
 * The allocated prologue and epilogue blocks are overhead that
 * eliminate edge conditions during coalescing.
 */

#endif // MALLOC_H

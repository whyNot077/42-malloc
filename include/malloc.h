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
void Munmap(Pointer ptr, size_t size);
int raise_error(char *message);

void free(void *bp);
void *realloc(void *ptr, size_t size);
void release_all();

/* initialize.c */
int init_malloc();
Pointer init_heap(t_pool type);

/* heap.c */
t_pool get_pool_type(size_t size);
Pointer extend_heap(size_t words);
int is_heap_empty(Base prologue_bp);

/* vector.c */
Pointer *find_start_point(Pointer bp);
int init_vector();
int insert_to_vector(Pointer bp);
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

#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define EMPTY -1
#define LOG_PATH "./trace/malloc.log"

#define WSIZE sizeof(Pointer) /* Word size */
#define DSIZE (2 * WSIZE)     /* Double word size */
#define BLOCK_SIZE (2 * DSIZE)
#define PROLOGUE_SIZE (WSIZE)
#define EPILOGUE_SIZE (WSIZE)

#define PACK(size, alloc) ((size) | (alloc))

#define GET(p) (*(Pointer *)(p))
#define PUT(p, val) (*(Pointer *)(p) = (val))
#define GET_PTR(p) ((Pointer)(GET(p)))
#define PUT_PTR(p, val) (PUT(p, (Pointer)(val)))

#define GET_SMALL_THRESHOLD() GET(((Pointer)(g_list) + 3 * BLOCK_SIZE))
#define GET_LARGE_THRESHOLD() GET(((Pointer)(g_list) + 3 * BLOCK_SIZE + WSIZE))
#define GET_VECTOR_START_POINT() GET_PTR((Pointer)(g_list) + 3 * BLOCK_SIZE + 2 * WSIZE)
#define SET_VECTOR_START_POINT(p, val) PUT_PTR(p, val)
#define GET_LOG_FD() GET((Pointer)(g_list) + 3 * BLOCK_SIZE + 3 * WSIZE)

#define GET_SIZE(p) (GET(p) & ~0x7)
#define IS_ALLOC(p) (GET(p) & 0x1)

#define HEAD(bp) ((Pointer)(bp) - WSIZE)
#define FOOT(bp) ((Pointer)(bp) + GET_SIZE(HEAD(bp)) - DSIZE)
#define PREV_FOOT(bp) ((Pointer)(bp) - DSIZE)

#define NEXT_BLK(bp) ((Pointer)(bp) + GET_SIZE(HEAD(bp)))
#define PREV_BLK(bp) ((Pointer)(bp) - GET_SIZE(PREV_FOOT(bp)))

#define PREV_PTR(bp) ((Pointer *)(bp))
#define NEXT_PTR(bp) ((Pointer *)((Pointer)(bp) + WSIZE))

#define VECTOR_CAPACITY(ptr) (*((size_t *)(ptr)))
#define VECTOR_SIZE(ptr) (*((size_t *)((Pointer)(ptr) + WSIZE)))
#define VECTOR_DATA_NUM(ptr) VECTOR_SIZE(ptr) - 2
#define VECTOR_ELEMENT(ptr, index) (*((Pointer *)((Pointer)(ptr) + ((index) + 2) * WSIZE)))
#define NEXT(ptr) ((Pointer *)((Pointer)(ptr) + WSIZE))
#define PREV(ptr) ((Pointer *)((Pointer)(ptr) - WSIZE))

extern pthread_mutex_t g_memory_lock;
extern Base g_list;
// segregated list + tiny threshold + small threshold + vector start point + log_fd

#endif // MALLOC_H

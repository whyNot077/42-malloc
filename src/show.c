
#include "malloc.h"

static size_t print_memory(t_pool called_type, int extension, int print_alloc);
static const char *pool_type_to_string(t_pool type);
static size_t print_blocks(Base prologue_bp, t_pool called_type,
                           int extension, int print_alloc, int *print_info, int final_block);
static size_t print_block(Pointer bp, int extension);
static void print_two_ptr(Pointer ptr1, Pointer ptr2);
static void print_hex_dump(Pointer start_bp, Pointer end_bp);
static void print_type_info(Base start_bp, t_pool called_type);
static int can_print(t_pool called_type, Base bp, int print_alloc);
static int is_epilogue_block(Base bp);

void show_free_mem(void)
{
    lock();
    size_t total_size = 0;
    total_size += print_memory(TINY, false, false);
    total_size += print_memory(SMALL, false, false);
    total_size += print_memory(LARGE, false, false);
    unlock();

    ft_putstr("Total : ");
    print_base(total_size, 10, 0);
    ft_putstr(" bytes\n");
}

void show_alloc_mem(void)
{
    size_t total_size = 0;

    lock();
    total_size += print_memory(TINY, false, true);
    total_size += print_memory(SMALL, false, true);
    total_size += print_memory(LARGE, false, true);
    unlock();

    ft_putstr("Total : ");
    print_base(total_size, 10, 0);
    ft_putstr(" bytes\n");
}

void show_alloc_mem_ex(void)
{
    size_t total_size = 0;
    lock();
    total_size += print_memory(TINY, true, true);
    total_size += print_memory(SMALL, true, true);
    total_size += print_memory(LARGE, true, true);
    unlock();

    ft_putstr("Total : ");
    print_base(total_size, 10, 0);
    ft_putstr(" bytes\n");
}

static size_t print_block(Pointer bp, int extension)
{
#ifdef DEBUG_SHOW
    printf("\n print block, bp = %p, ", (void *)bp);
#endif
    size_t block_size = GET_SIZE(HEAD(bp));
#ifdef DEBUG_SHOW
    printf("block_size = %lu, ", block_size);
#endif
    print_two_ptr(bp, bp + block_size - 1);
    if (extension)
    {
        print_hex_dump(bp, bp + block_size);
    }
    return block_size;
}

static void print_type_info(Base start_bp, t_pool called_type)
{
    t_pool type;
    if (start_bp == 0)
    {
        type = called_type;
    }
    else
    {
        type = get_pool_type(GET_SIZE(HEAD(start_bp)));
    }
    if (type == called_type)
    {
        ft_putstr(pool_type_to_string(type));
        ft_putstr(" : ");
        print_base(start_bp, 16, 1);
        ft_putstr("\n");
    }
}

static size_t print_blocks(Base prologue_bp, t_pool called_type,
                           int extension, int print_alloc, int *print_info, int final_block)
{
#ifdef DEBUG_SHOW
    printf("\n\n!!! print_blocks\n");
    printf("prologue bp = %p \n", (void *)prologue_bp);
#endif
    size_t total_size = 0;
    Base start_bp = NEXT_BLK(prologue_bp);
#ifdef DEBUG_SHOW
    printf("start bp = %p, size = %lu\n", (void *)start_bp, GET_SIZE(HEAD(start_bp)));
    printf("\nblock list (bp, size, alloc)\n: ");
#endif
    while (start_bp && !is_epilogue_block(start_bp))
    {
#ifdef DEBUG_SHOW
        printf("(%p, %lu, %lu) ", (void *)start_bp, GET_SIZE(HEAD(start_bp)), IS_ALLOC(HEAD(start_bp)));
#endif
        if (can_print(called_type, start_bp, print_alloc))
        {
#ifdef DEBUG_SHOW
            printf("\nprint block\n");
#endif
            if (*print_info == 0)
            {
                print_type_info(start_bp, called_type);
                *print_info = 1;
            }
            total_size += print_block(start_bp, extension);
        }
        start_bp = NEXT_BLK(start_bp);
    }
    if (*print_info == 0 && final_block)
    {
        print_type_info(0, called_type);
        *print_info = 1;
    }
#ifdef DEBUG_SHOW
    printf("finish print_blocks, total_size = %lu\n", total_size);
#endif
    return total_size;
}

static size_t print_memory(t_pool called_type, int extension, int print_alloc)
{
    if (!g_list)
    {
        print_type_info(0, called_type);
        return 0;
    }
    Pointer vector = GET_VECTOR_START_POINT();
    if (vector == 0)
    {
        print_type_info(0, called_type);
        return 0;
    }
    int count = VECTOR_SIZE(vector) - 2;
#ifdef DEBUG_SHOW
    printf("\n!!! print_memory, count = %d\n", count);
#endif
    size_t total_size = 0;
    int print_info = 0;
    for (int i = 0; i < count; i++)
    {
#ifdef DEBUG_SHOW
        printf("\n\n!!! print_memory, i = %d\n", i);
#endif
        Base prologue_block = VECTOR_ELEMENT(vector, i);
        if (prologue_block == 0)
        {
            continue;
        }
#ifdef DEBUG_SHOW
        printf("prologue block = %p, size = %lu\n", (void *)prologue_block, GET_SIZE(HEAD(prologue_block)));
#endif
        total_size += print_blocks(prologue_block, called_type, extension, print_alloc, &print_info, i == count - 1);
    }
    return total_size;
}

static const char *pool_type_to_string(t_pool type)
{
    switch (type)
    {
    case TINY:
        return "TINY";
    case SMALL:
        return "SMALL";
    case LARGE:
        return "LARGE";
    default:
        return "UNKNOWN";
    }
}

static int is_epilogue_block(Base bp)
{
    return GET_SIZE(HEAD(bp)) == 0 && IS_ALLOC(HEAD(bp));
}

static int can_print(t_pool called_type, Base bp, int print_alloc)
{
    t_pool type = get_pool_type(GET_SIZE(HEAD(bp)));
    return type == called_type && GET_SIZE(HEAD(bp)) &&
           !is_epilogue_block(bp) && IS_ALLOC(HEAD(bp)) == print_alloc;
}

static void print_two_ptr(Pointer ptr1, Pointer ptr2)
{
    print_base(ptr1, 16, 1);
    ft_putstr(" - ");
    print_base(ptr2, 16, 1);
    ft_putstr(" : ");
    print_base(ptr2 - ptr1 + 1, 10, 0);
    ft_putstr(" bytes\n");
}

static void put_prefix(int base)
{
    if (base == 16)
        ft_putstr("0x");
    else if (base == 8)
        ft_putstr("0");
    else if (base == 2)
        ft_putstr("0b");
}

void print_base(Pointer ptr, int base, int prefix)
{
    char *str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char buffer[65];
    int i = 64;

    if (base < 2 || base > 36)
        return;

    buffer[i] = '\0';
    i--;

    if (ptr == 0)
    {
        buffer[i--] = '0';
    }
    else
    {
        while (ptr > 0)
        {
            buffer[i--] = str[ptr % base];
            ptr /= base;
        }
    }

    if (prefix)
    {
        put_prefix(base);
    }

    ft_putstr(&buffer[i + 1]);
}

static void print_hex_dump(Pointer start_bp, Pointer end_bp)
{
    unsigned char *pc = (unsigned char *)start_bp;
    size_t len = end_bp - start_bp;
    size_t i;
    char hex[4];

    for (i = 0; i < len; i++)
    {
        if (i % 16 == 0)
        {
            print_base(start_bp + i, 16, 1);
            ft_putstr(" ");
        }

        hex[0] = "0123456789ABCDEF"[pc[i] >> 4];
        hex[1] = "0123456789ABCDEF"[pc[i] & 0xF];
        hex[2] = ' ';
        hex[3] = '\0';
        ft_putstr(hex);

        if ((i + 1) % 16 == 0)
            ft_putstr("\n");
    }

    if (i % 16 != 0)
        ft_putstr("\n");
}

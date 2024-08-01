◦ mmap(2)
◦ munmap(2)
◦ getpagesize(2 or 3 depending on OS)
◦ getrlimit(2)
◦ The functions from libpthread
• You are allowed to use other functions to complete the bonus part as long as their use is justified during your defence. Be smart!    

With performance in mind, you must limit the number of calls to mmap(), but also to munmap(). You have to “pre-allocate” some memory zones to store your “small” and “medium” malloc.

• The size of these zones must be a multiple of getpagesize().

You are allowed a global variable to manage your allocations and one for the threadsafe.

gcc -o a.out main.c -L. -lft_malloc_arm64_Darwin -lpthread
valgrind --leak-check=full ./a.out



insert vector에 문제 있으.ㅁ.. ㄹㄴㄹ샹..1!
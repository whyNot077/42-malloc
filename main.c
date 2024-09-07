#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define POOL_SIZE 1024 * 1024  // 1MB 크기의 메모리 풀

void *memory_pool = NULL;  // 메모리 풀 포인터
size_t pool_size = POOL_SIZE;  // 메모리 풀 크기

// 메모리 풀 해제 함수
void release_memory_pool() {
    if (memory_pool) {
        if (munmap(memory_pool, pool_size) == -1) {
            perror("메모리 풀 해제에 실패했습니다");
        } else {
            printf("메모리 풀이 성공적으로 해제되었습니다.\n");
        }
    }
}

// 메모리 풀 초기화 함수
void initialize_memory_pool() {
    memory_pool = mmap(NULL, pool_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (memory_pool == MAP_FAILED) {
        perror("메모리 풀 생성에 실패했습니다");
        exit(EXIT_FAILURE);
    }

    // atexit에 메모리 풀 해제 함수 등록
    if (atexit(release_memory_pool) != 0) {
        perror("atexit 등록에 실패했습니다");
        exit(EXIT_FAILURE);
    }

    printf("메모리 풀이 성공적으로 생성되었습니다 (크기: %zu 바이트).\n", pool_size);
}

// 간단한 malloc 함수 (메모리 풀에서 할당)
void *my_malloc(size_t size) {
    // 필요한 메모리 할당 로직 구현 (단순 예시로 할당)
    // 실제로는 더 복잡한 메모리 관리 로직이 필요함
    static size_t offset = 0;
    if (offset + size > pool_size) {
        return NULL;  // 메모리 풀이 부족할 경우
    }
    void *ptr = (char *)memory_pool + offset;
    offset += size;
    return ptr;
}

int main() {
    // 프로그램 시작 시 메모리 풀 초기화
    initialize_memory_pool();

    // 메모리 풀에서 메모리 할당 예시
    char *data = (char *)my_malloc(100);
    if (data != NULL) {
        printf("100바이트 메모리 할당 성공\n");
    } else {
        printf("메모리 할당 실패\n");
    }

    // 프로그램이 종료될 때 자동으로 atexit에 등록된 release_memory_pool 호출
    return 0;
}

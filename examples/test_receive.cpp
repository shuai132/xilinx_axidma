#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstdint>

#define LTC2324_BASE_ADDR   0x43C20000
#define LTC2324_REG_START   0
#define LTC2324_REG_LENGTH  4
#define LTC2324_REG_SIZE    64  // 没必要映射太多

static void printRegs(uint32_t* addr, uint32_t size) {
    for (uint32_t i=0; i<size; i++) {
        printf("%08x ", addr[i]);
        if ((i+1)%10 == 0) printf("\n");
    }
    printf("\n");
}


#define PAGE_SIZE  ((size_t)getpagesize())
#define PAGE_MASK ((uint64_t) (long)~(PAGE_SIZE - 1))


int main(int argc, char **argv)
{

    uint64_t base = LTC2324_BASE_ADDR & PAGE_MASK;
    uint64_t pgoffset = LTC2324_BASE_ADDR & (~PAGE_MASK);

    int dev_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (dev_fd < 0) {
        printf("open(/dev/mem) failed.");
        return 0;
    }

    auto map_base = (uint32_t*)mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, LTC2324_BASE_ADDR);

    if(dev_fd) close(dev_fd);
    // sample len
    *(volatile uint32_t*)(map_base + pgoffset + LTC2324_REG_LENGTH*8) = 0x0123;
////    // start
    *(volatile uint32_t*)(map_base + pgoffset + LTC2324_REG_START*8) = 0x12;
//
    // sample len
    auto len = *(volatile uint32_t*)(map_base + pgoffset + LTC2324_REG_LENGTH);
    // start
    auto start = *(volatile uint32_t*)(map_base + pgoffset + LTC2324_REG_START);

    printf("len: %04x, start: %04x\n", len, start);
    printf("len: %04x, start: %04x\n", map_base + pgoffset, map_base);

    printRegs(map_base + pgoffset, LTC2324_REG_SIZE);


    munmap(map_base, LTC2324_REG_SIZE);

    return 0;
}

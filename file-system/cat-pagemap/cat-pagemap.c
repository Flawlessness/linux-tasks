#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint64_t PAGE_SIZE = 4096;

int main(int argc, char **argv) {
    if(argc != 4)
    {
        printf("Wrong number of arguments\n");
        return 1;
    }

    char path[256];
    if (strcmp(argv[1], "self") == 0)
    {
        strcpy(path, "/proc/self/pagemap");
    }
    else
    {
        int pid = strtol(argv[1], NULL, 0);
        if (pid == 0)
        {
            printf("Incorrect pid\n");
            return 1;
        }
        sprintf(path, "/proc/%d/pagemap", pid);
    }

    FILE *fp = fopen(path, "rb");
    if (!fp)
    {
        printf("Failed to open %s\n", path);
        return 1;
    }

    uint64_t start_address = strtoul(argv[2], NULL, 0);
    uint64_t end_address = strtoul(argv[3], NULL, 0);

    uint64_t pagemap_entry;
    uint64_t virtual_address;

    fseek(fp, (start_address / PAGE_SIZE) * sizeof(uint64_t), SEEK_SET);

    for(virtual_address = start_address; virtual_address < end_address; virtual_address += PAGE_SIZE)
    {
        if(fread(&pagemap_entry, sizeof(uint64_t), 1, fp) != 1)
        {
            printf("Failed to read file\n");
            return 1;
        }
        if ((pagemap_entry >> 62) & 1)
        {
            printf("0x%016lx : swap-type %ld offset %ld soft-dirty %ld exclusive %ld file/shared-anon %ld swapped %ld present %ld\n", virtual_address, pagemap_entry & 0x000000000000001F, (pagemap_entry >> 5) & 0x0003FFFFFFFFFFFF, (pagemap_entry >> 55) & 1, (pagemap_entry >> 56) & 1, (pagemap_entry >> 61) & 1, (pagemap_entry >> 62) & 1, (pagemap_entry >> 63) & 1);
        }
        printf("0x%016lx : pfn %016lx soft-dirty %ld exclusive %ld file/shared-anon %ld swapped %ld present %ld\n", virtual_address, pagemap_entry & 0x007FFFFFFFFFFFFF, (pagemap_entry >> 55) & 1, (pagemap_entry >> 56) & 1, (pagemap_entry >> 61) & 1, (pagemap_entry >> 62) & 1, (pagemap_entry >> 63) & 1);
    }
    fclose(fp);
    return 0;
}

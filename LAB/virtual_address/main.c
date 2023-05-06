#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s virtual_address\n", argv[0]);
        return 1;
    }

    unsigned int virtual_address = atoi(argv[1]);
    unsigned int page_number = virtual_address / PAGE_SIZE;
    unsigned int offset = virtual_address % PAGE_SIZE;

    printf("The address %u contains:\n", virtual_address);
    printf("page number = %u\n", page_number);
    printf("offset = %u\n", offset);

    return 0;
}

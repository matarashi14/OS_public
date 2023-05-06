#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Fixed values of our system
#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define TLB_SIZE 16
#define TOTAL_FRAMES 256
#define PHYSICAL_MEM_SIZE (TOTAL_FRAMES * FRAME_SIZE)

// The page table
// Each index is the page number and the value is the frame number
// Invalid pages should have a value of -1
int page_table[PAGE_TABLE_SIZE];

void initialize_page_table()
{
    for (int i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        page_table[i] = -1;
    }
}

// return frame number or -1
int search_page_table(int page_num)
{
    return page_table[page_num];
}

// The translation lookaside buffer
// Column 0 is the page, column 1 is the frame
// Empty entries should have -1 for both page and frame
int tlb[TLB_SIZE][2]; // 2 columns for page number and frame number

void initialize_tlb()
{
    for (int i = 0; i < TLB_SIZE; i++)
    {
        tlb[i][0] = -1;
        tlb[i][1] = -1;
    }
}

// return frame number or -1
int search_tlb(int page_num)
{
    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (tlb[i][0] == page_num)
        {
            return tlb[i][1];
        }
    }

    return -1;
}

// This is our physical memory
// Each physical address can store one byte of data
uint8_t physical_mem[PHYSICAL_MEM_SIZE];

int tlb_index = 0;

void update_tlb(int page_num, int frame_num)
{
    tlb[tlb_index][0] = page_num;
    tlb[tlb_index][1] = frame_num;
    // FIFO
    tlb_index = (tlb_index + 1) % TLB_SIZE;
}

FILE *backing_store;

void load_page(int page_num, int frame_num)
{
    int physical_address_start = frame_num * FRAME_SIZE;
    fseek(backing_store, page_num * PAGE_SIZE, SEEK_SET);
    fread(&physical_mem[physical_address_start], 1, FRAME_SIZE, backing_store);
}

int main()
{

    // initialize the global arrays
    initialize_page_table();
    initialize_tlb();

    // open the file of logical addresses
    FILE *addresses = fopen("addresses.txt", "r");
    FILE *output = fopen("out.txt", "w");

    // open the backing store file
    backing_store = fopen("BACKING_STORE.bin", "r");

    if (addresses == NULL || backing_store == NULL)
    {
        printf("Error: problem in opening file");
        return 1;
    }

    // repeat for each address in the file of logical addresses
    char line[10];
    int free_frame = 0;
    while (fgets(line, 10, addresses) != NULL)
    {

        // read in logical address
        int logical_address = atoi(line);

        // calculate the page number and the page offset
        int page_num = logical_address / PAGE_SIZE;
        int offset = logical_address % PAGE_SIZE;

        // check TLB for page number
        int frame_num = search_tlb(page_num);

        // if not found, check page table for page number

        if (frame_num == -1)
        {
            frame_num = search_page_table(page_num);
            // if not found, then must be read in from backing store
            // this loading the page into a free frame in the physical memory
            if (frame_num == -1)
            {
                // load the page from the backing store into a free frame of the physical memory.
                frame_num = free_frame++;
                load_page(page_num, frame_num);
                page_table[page_num] = frame_num;
            }
            else
            {
                // If the entry was not in the TLB, update the TLB to include it
                update_tlb(page_num, frame_num);
            }
        }

        int physical_address = frame_num * FRAME_SIZE + offset;
        int value = (int8_t)physical_mem[physical_address];

        // print the final data
        printf("Virtual address: %d Physical address: %d Value: %d\n", logical_address, physical_address, value);
        fprintf(output, "Virtual address: %d Physical address: %d Value: %d\n", logical_address, physical_address, value);
    }

    // close files
    fclose(addresses);
    fclose(backing_store);
    fclose(output);

    return 0;
}

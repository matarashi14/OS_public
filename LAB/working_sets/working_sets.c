#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void printSet(int arr[], int size, int maxPage){
    bool hash_table[maxPage];
    for(int i = 0; i < maxPage; i++){
        hash_table[i] = false;
    }
    printf("WS = { ");
    // Iterate through the array and print each unique element
    for (int i = 0; i < size; i++) {
        if (arr[i] != -1 && !hash_table[arr[i]]) {
            printf("%d ", arr[i]);
            hash_table[arr[i]] = true;
        }
    }
    printf("}\n");

} 

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <delta> <pages>\n", argv[0]);
        return 1;
    }
    int delta = atoi(argv[1]);
    int maxPage = atoi(argv[2]);

    int workingSet[delta];
    int head = 0;
    for(int i = 0; i < delta; i++){
        workingSet[i] = -1;
    }

    while (1) { 
        printf("Enter a page request: ");
        int page;
        scanf("%d", &page);

        if( page <= -1 || maxPage <= page){
            break;
        } 

        workingSet[head] = page;
        head = (head + 1) % delta;
        
        printSet(workingSet, delta, maxPage);

    }
    return 0;
}
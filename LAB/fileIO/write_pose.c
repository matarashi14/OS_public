#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 100

void write_at_pos(char* file, int pos, char* text) {
    char* buffer = malloc(MAX_LINE_LENGTH);

    FILE* fp = fopen(file, "r+");

    int i = 0;
    while (i < pos && fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
        i++;
    }

    if (i < pos){
        printf("Failed to write at position %d: file has fewer than %d lines\n", pos, pos);
    }
    else{    
        fputs(text, fp);
    }
    free(buffer);
    fclose(fp);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <filename> <line_number> <text>\n", argv[0]);
        return 1;
    }

    char* file = argv[1];
    int pos = atoi(argv[2]);
    char text[MAX_LINE_LENGTH];
    strncpy(text, argv[3], MAX_LINE_LENGTH);
    strcat(text, "\n");

    write_at_pos(file, pos, text);

    return 0;
}
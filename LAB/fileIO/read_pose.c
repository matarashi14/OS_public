#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

char* read_at_pos(char* file, int pos) {
    char* line = malloc(MAX_LINE_LENGTH);

    FILE* fp = fopen(file, "r");

    int i = 0;
    while (i < pos && fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        i++;
    }
    fclose(fp);

    if (i < pos) {
        free(line);
        return NULL;
    }

    return line;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s file pos\n", argv[0]);
        return 1;
    }

    char* file = argv[1];
    int pos = atoi(argv[2]);

    char* line = read_at_pos(file, pos);
    if (line == NULL) {
        printf("Reached end of file before position %d\n", pos);
    } else {
        printf("Line %d: %s\n", pos, line);
        free(line);
    }

    return 0;
}


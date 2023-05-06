#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CYLINDERS 5000
#define MAX_REQUESTS 1000

void print_array(int arr[], int n)
{
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int FCFS(int requests[], int start){
    int sum = abs(requests[0] - start);
    for(int i = 0; i < MAX_REQUESTS-1; i++){
        sum = sum + abs(requests[i] - requests[i+1]);
    }
    return sum;
}

int SCAN(int requests[], int start){
    int min = requests[0];
    int max = requests[0];
    for(int i = 0; i < MAX_REQUESTS; i++){
        if(requests[i] < min){
            min = requests[i];
        }
        if(requests[i] > max){
            max = requests[i];
        }
    }
    if(start <= min){
        return abs(max - start);
    }
    else{
        return abs(MAX_CYLINDERS - start) + abs(MAX_CYLINDERS - min);
    }
}

int SSTF(int requests[], int start)
{
    int total_tracks = 0;
    int i, j, shortest_distance, shortest_index;

    int* visited = (int*) calloc(MAX_REQUESTS, sizeof(int));

    // loop through all requests
    for (i = 0; i < MAX_REQUESTS; i++) {
        shortest_distance = MAX_CYLINDERS;

        // loop through all unvisited requests to find the closest one
        for (j = 0; j < MAX_REQUESTS; j++) {
            if (!visited[j]) {
                int distance = abs(requests[j] - start);
                if (distance < shortest_distance) {
                    shortest_distance = distance;
                    shortest_index = j;
                }
            }
        }
        visited[shortest_index] = 1; // mark the request as visited
        total_tracks += shortest_distance; // add the distance to the total tracks traversed
        start = requests[shortest_index]; // move the head to the closest request
    }

    free(visited);

    return total_tracks;
}


int main(int argc, char *argv[])
{
    int start = atoi(argv[1]);
    int reads[MAX_REQUESTS];

    // Generate 1000 random integers between 0 and 4999
    srand(time(NULL));
    for (int i = 0; i < MAX_REQUESTS; i++) {
       reads[i] = rand() % MAX_CYLINDERS;
    }

    printf("FSFS: %d\n", FCFS(reads, start));
    printf("SCAN: %d\n", SCAN(reads, start));
    printf("SSTF: %d\n", SSTF(reads, start));
    return 0; 
}
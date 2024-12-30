#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../announce/announce.h"
int main() {
    Announce *announces = NULL;
    int count = 0;

    int result = fetch_announces(&announces, &count);
    if (result != 0) {
        fprintf(stderr, "Failed to fetch announces. Error code: %d\n", result);
        return 1;
    }

    printf("Fetched %d announces:\n", count);
    for (int i = 0; i < count; i++) {
        printf("Announce ID: %d\n", announces[i].announce_id);
        printf("Flight ID: %s\n", announces[i].flight_id);
        printf("Content: %s\n", announces[i].content);
        printf("Updated At: %s\n", announces[i].updated_at);
        printf("--------------------------------------\n");
    }
    free(announces);

    return 0;
}
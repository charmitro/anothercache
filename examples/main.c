#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "anothercache.h"

int main()
{
    cache_t cache;
    cache_init(&cache, 100003);  // Prime number for better hash distribution
    
    clock_t start = clock();
    
    // Pre-allocate arrays to avoid repeated malloc
    char **keys = malloc(100000 * sizeof(char*));
    char **values = malloc(100000 * sizeof(char*));
    
    // Insert 100000 keys
    for (int i = 0; i < 100000; i++) {
        keys[i] = malloc(32);
        values[i] = malloc(32);
        sprintf(keys[i], "key_%d", i);
        sprintf(values[i], "value_%d", i);
        cache_set(&cache, keys[i], values[i]);
    }
    
    clock_t end = clock();
    double insertion_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Hash table capacity: %d\n", cache.capacity);
    printf("Total items: %d\n", cache.size);
    printf("Insertion time: %.3f seconds\n", insertion_time);
    
    // Test retrieving some keys
    start = clock();
    const char *data = cache_get(&cache, "key_0");
    if (data)
        printf("key_0: %s\n", data);
    
    data = cache_get(&cache, "key_999");
    if (data)
        printf("key_999: %s\n", data);
    
    data = cache_get(&cache, "key_99999");
    if (data)
        printf("key_99999: %s\n", data);
    
    end = clock();
    double lookup_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Lookup time: %.6f seconds\n", lookup_time);
    
    // Clean up
    for (int i = 0; i < 100000; i++) {
        free(keys[i]);
        free(values[i]);
    }
    free(keys);
    free(values);
    
    cache_free(&cache);
    return 0;
}

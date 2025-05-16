#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "anothercache.h"

#define ITEMS 1000000
#define LOOKUPS 1000000
#define KEY_SIZE 32
#define VALUE_SIZE 64

typedef struct {
    double insert_time;
    double lookup_time;
    double update_time;
    double miss_time;
} bench_results_t;

void benchmark_cache(cache_t *cache, bench_results_t *results) {
    char **keys = malloc(ITEMS * sizeof(char*));
    char **values = malloc(ITEMS * sizeof(char*));
    
    // Prepare data
    for (int i = 0; i < ITEMS; i++) {
        keys[i] = malloc(KEY_SIZE);
        values[i] = malloc(VALUE_SIZE);
        sprintf(keys[i], "benchmark_key_%d", i);
        sprintf(values[i], "benchmark_value_%d", i);
    }
    
    // Benchmark insertions
    clock_t start = clock();
    for (int i = 0; i < ITEMS; i++) {
        cache_set(cache, keys[i], values[i]);
    }
    clock_t end = clock();
    results->insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Benchmark lookups (existing keys)
    start = clock();
    for (int i = 0; i < LOOKUPS; i++) {
        int index = rand() % ITEMS;
        const char *value = cache_get(cache, keys[index]);
        (void)value; // Avoid unused warning
    }
    end = clock();
    results->lookup_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Benchmark updates
    start = clock();
    for (int i = 0; i < ITEMS / 2; i++) {
        cache_set(cache, keys[i], "updated_value");
    }
    end = clock();
    results->update_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Benchmark misses
    start = clock();
    for (int i = 0; i < LOOKUPS; i++) {
        char miss_key[KEY_SIZE];
        sprintf(miss_key, "nonexistent_key_%d", i);
        const char *value = cache_get(cache, miss_key);
        (void)value; // Avoid unused warning
    }
    end = clock();
    results->miss_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Cleanup
    for (int i = 0; i < ITEMS; i++) {
        free(keys[i]);
        free(values[i]);
    }
    free(keys);
    free(values);
}

void print_results(const char *name, bench_results_t *results) {
    printf("\n%s Performance Results:\n", name);
    printf("==================================\n");
    printf("Insert %d items:  %.4f seconds (%.0f ops/sec)\n", 
           ITEMS, results->insert_time, ITEMS / results->insert_time);
    printf("Lookup %d items:  %.4f seconds (%.0f ops/sec)\n", 
           LOOKUPS, results->lookup_time, LOOKUPS / results->lookup_time);
    printf("Update %d items:  %.4f seconds (%.0f ops/sec)\n", 
           ITEMS/2, results->update_time, (ITEMS/2) / results->update_time);
    printf("Miss %d lookups:  %.4f seconds (%.0f ops/sec)\n", 
           LOOKUPS, results->miss_time, LOOKUPS / results->miss_time);
}

int main() {
    printf("anothercache Benchmark Suite\n");
    printf("======================\n");
    printf("Items: %d (1M)\n", ITEMS);
    printf("Lookups: %d (1M)\n", LOOKUPS);
    
    cache_t cache;
    bench_results_t results;
    
    // Benchmark with small capacity (many collisions)
    printf("\nBenchmarking with small capacity (10007)...\n");
    cache_init(&cache, 10007);
    benchmark_cache(&cache, &results);
    print_results("Small Capacity", &results);
    cache_free(&cache);
    
    // Benchmark with optimal capacity (fewer collisions)
    printf("\nBenchmarking with optimal capacity (1000003)...\n");
    cache_init(&cache, 1000003);
    benchmark_cache(&cache, &results);
    print_results("Optimal Capacity", &results);
    cache_free(&cache);
    
    // Benchmark with large capacity (minimal collisions)
    printf("\nBenchmarking with large capacity (2000003)...\n");
    cache_init(&cache, 2000003);
    benchmark_cache(&cache, &results);
    print_results("Large Capacity", &results);
    cache_free(&cache);
    
    return 0;
}

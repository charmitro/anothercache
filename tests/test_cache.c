#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "anothercache.h"

#define RUN_TEST(test) do { \
    printf("Running %s...", #test); \
    test(); \
    printf(" PASSED\n"); \
} while(0)

void test_init_free() {
    cache_t cache;
    cache_init(&cache, 100);
    assert(cache.capacity == 100);
    assert(cache.size == 0);
    assert(cache.buckets != NULL);
    cache_free(&cache);
    assert(cache.buckets == NULL);
    assert(cache.capacity == 0);
    assert(cache.size == 0);
}

void test_set_get() {
    cache_t cache;
    cache_init(&cache, 100);
    
    cache_set(&cache, "key1", "value1");
    cache_set(&cache, "key2", "value2");
    cache_set(&cache, "key3", "value3");
    
    assert(cache.size == 3);
    assert(strcmp(cache_get(&cache, "key1"), "value1") == 0);
    assert(strcmp(cache_get(&cache, "key2"), "value2") == 0);
    assert(strcmp(cache_get(&cache, "key3"), "value3") == 0);
    
    cache_free(&cache);
}

void test_update_existing() {
    cache_t cache;
    cache_init(&cache, 100);
    
    cache_set(&cache, "key1", "value1");
    assert(strcmp(cache_get(&cache, "key1"), "value1") == 0);
    
    cache_set(&cache, "key1", "updated_value");
    assert(cache.size == 1);  // Size shouldn't increase
    assert(strcmp(cache_get(&cache, "key1"), "updated_value") == 0);
    
    cache_free(&cache);
}

void test_get_nonexistent() {
    cache_t cache;
    cache_init(&cache, 100);
    
    cache_set(&cache, "key1", "value1");
    assert(cache_get(&cache, "nonexistent") == NULL);
    
    cache_free(&cache);
}

void test_collision_handling() {
    cache_t cache;
    cache_init(&cache, 10);  // Small capacity to force collisions
    
    // Add many items to force collisions
    // Allocate separate buffers for each key/value
    char **keys = malloc(50 * sizeof(char*));
    char **values = malloc(50 * sizeof(char*));
    
    for (int i = 0; i < 50; i++) {
        keys[i] = malloc(32);
        values[i] = malloc(32);
        sprintf(keys[i], "key_%d", i);
        sprintf(values[i], "value_%d", i);
        cache_set(&cache, keys[i], values[i]);
    }
    
    assert(cache.size == 50);
    
    // Verify all items are retrievable
    for (int i = 0; i < 50; i++) {
        const char *retrieved = cache_get(&cache, keys[i]);
        assert(retrieved != NULL);
        assert(strcmp(retrieved, values[i]) == 0);
    }
    
    // Clean up
    for (int i = 0; i < 50; i++) {
        free(keys[i]);
        free(values[i]);
    }
    free(keys);
    free(values);
    
    cache_free(&cache);
}

void test_edge_cases() {
    cache_t cache;
    cache_init(&cache, 100);
    
    // Empty string keys and values
    cache_set(&cache, "", "empty_key");
    cache_set(&cache, "empty_value", "");
    
    assert(strcmp(cache_get(&cache, ""), "empty_key") == 0);
    assert(strcmp(cache_get(&cache, "empty_value"), "") == 0);
    
    // Very long keys
    char long_key[1024];
    memset(long_key, 'a', 1023);
    long_key[1023] = '\0';
    
    cache_set(&cache, long_key, "long_key_value");
    assert(strcmp(cache_get(&cache, long_key), "long_key_value") == 0);
    
    cache_free(&cache);
}

void test_stress() {
    cache_t cache;
    cache_init(&cache, 10007);
    
    // Add 10000 items - allocate separate buffers
    char **keys = malloc(10000 * sizeof(char*));
    char **values = malloc(10000 * sizeof(char*));
    
    for (int i = 0; i < 10000; i++) {
        keys[i] = malloc(32);
        values[i] = malloc(32);
        sprintf(keys[i], "k%d", i);
        sprintf(values[i], "v%d", i);
        cache_set(&cache, keys[i], values[i]);
    }
    
    assert(cache.size == 10000);
    
    // Verify a sample of items
    for (int i = 0; i < 10000; i += 100) {
        assert(strcmp(cache_get(&cache, keys[i]), values[i]) == 0);
    }
    
    // Clean up
    for (int i = 0; i < 10000; i++) {
        free(keys[i]);
        free(values[i]);
    }
    free(keys);
    free(values);
    
    cache_free(&cache);
}

int main() {
    printf("Running anothercache test suite...\n\n");
    
    RUN_TEST(test_init_free);
    RUN_TEST(test_set_get);
    RUN_TEST(test_update_existing);
    RUN_TEST(test_get_nonexistent);
    RUN_TEST(test_collision_handling);
    RUN_TEST(test_edge_cases);
    RUN_TEST(test_stress);
    
    printf("\nAll tests passed!\n");
    return 0;
}

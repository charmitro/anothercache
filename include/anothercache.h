#ifndef ANOTHERCACHE_H
#define ANOTHERCACHE_H

#include <stdint.h>

/**
 * @brief Cache entry structure
 * 
 * Represents a key-value pair in the cache with collision chaining support
 */
typedef struct item_t {
	const char *key; /**< Key string */
	const char *data; /**< Value string */
	struct item_t *next; /**< Next item in collision chain */
} item_t;

/**
 * @brief Cache structure
 * 
 * Hash table-based cache with separate chaining for collision resolution
 */
typedef struct cache_t {
	int size; /**< Current number of items in cache */
	int capacity; /**< Number of hash table buckets */
	item_t **buckets; /**< Array of bucket heads */
} cache_t;

/**
 * @brief Initialize a cache with specified capacity
 * 
 * @param cache Pointer to cache structure
 * @param capacity Number of hash buckets (use prime numbers for better distribution)
 */
void cache_init(cache_t *cache, int capacity);

/**
 * @brief Free all resources associated with a cache
 * 
 * @param cache Pointer to cache structure
 */
void cache_free(cache_t *cache);

/**
 * @brief Set a key-value pair in the cache
 * 
 * If the key already exists, its value will be updated.
 * 
 * @param cache Pointer to cache structure
 * @param key Key string (must remain valid for lifetime of cache)
 * @param data Value string (must remain valid for lifetime of cache)
 */
void cache_set(cache_t *cache, const char *key, const char *data);

/**
 * @brief Get value associated with a key
 * 
 * @param cache Pointer to cache structure
 * @param key Key to look up
 * @return Associated value or NULL if key not found
 */
const char *cache_get(cache_t *cache, const char *key);

/**
 * @brief DJB2 hash function
 * 
 * Fast string hash function by Dan Bernstein
 * 
 * @param str String to hash
 * @return 32-bit hash value
 */
uint32_t hash_djb2(const char *str);

#endif

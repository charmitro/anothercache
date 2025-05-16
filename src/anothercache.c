#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "anothercache.h"

// DJB2 hash function
uint32_t hash_djb2(const char *str)
{
	uint32_t hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; // hash * 33 + c

	return hash;
}

void cache_init(cache_t *cache, int capacity)
{
	cache->size = 0;
	cache->capacity = capacity;
	cache->buckets = calloc(capacity, sizeof(item_t *));
	if (!cache->buckets)
		exit(1);
}

const char *cache_get(cache_t *cache, const char *key)
{
	uint32_t index = hash_djb2(key) % cache->capacity;
	item_t *item = cache->buckets[index];

	// Search in the chain
	while (item) {
		if (strcmp(item->key, key) == 0) {
			return item->data;
		}
		item = item->next;
	}

	return NULL;
}

void cache_set(cache_t *cache, const char *key, const char *data)
{
	uint32_t index = hash_djb2(key) % cache->capacity;
	item_t *item = cache->buckets[index];

	// Check if key already exists
	while (item) {
		if (strcmp(item->key, key) == 0) {
			item->data = data;
			return;
		}
		item = item->next;
	}

	// Key doesn't exist, add new item at the beginning of the chain
	item_t *new_item = malloc(sizeof(item_t));
	if (!new_item) {
		return;
	}

	new_item->key = key;
	new_item->data = data;
	new_item->next = cache->buckets[index];
	cache->buckets[index] = new_item;
	cache->size++;
}

void cache_free(cache_t *cache)
{
	// Free all items in each bucket
	for (int i = 0; i < cache->capacity; i++) {
		item_t *item = cache->buckets[i];
		while (item) {
			item_t *next = item->next;
			free(item);
			item = next;
		}
	}

	free(cache->buckets);
	cache->buckets = NULL;
	cache->size = 0;
	cache->capacity = 0;
}

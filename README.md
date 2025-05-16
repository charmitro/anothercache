# anothercache

A fast and lightweight hash table-based cache implementation in C with O(1) average-case performance.

## Features

- **Fast lookups**: O(1) average-case time complexity
- **Collision handling**: Uses chaining for hash collisions
- **Memory efficient**: Minimal overhead per entry
- **Simple API**: Easy to integrate into existing projects
- **Well-tested**: Comprehensive test suite included

## Installation

```bash
git clone https://github.com/yourusername/anothercache.git
cd anothercache
make
```

## Usage

```c
#include "anothercache.h"

int main() {
    cache_t cache;
    cache_init(&cache, 10007);  // Initialize with prime capacity
    
    // Store key-value pairs
    cache_set(&cache, "user:123", "John Doe");
    cache_set(&cache, "user:456", "Jane Smith");
    
    // Retrieve values
    const char *name = cache_get(&cache, "user:123");
    if (name) {
        printf("Found: %s\n", name);
    }
    
    // Clean up
    cache_free(&cache);
    return 0;
}
```

## API Reference

### Functions

- `void cache_init(cache_t *cache, int capacity)` - Initialize cache with given capacity
- `void cache_set(cache_t *cache, const char *key, const char *data)` - Store key-value pair
- `const char *cache_get(cache_t *cache, const char *key)` - Retrieve value by key
- `void cache_free(cache_t *cache)` - Free cache resources

## Performance

The cache uses a hash table with chaining for collision resolution, providing:
- O(1) average-case lookup time
- O(1) average-case insertion time
- O(n) worst-case time (all keys hash to same bucket)

## Building and Testing

```bash
# Build library and examples
make

# Run tests
make test

# Run benchmarks
make bench

# Clean build artifacts
make clean
```

# Benchmark output

```
======================
Items: 1000000 (1M)
Lookups: 1000000 (1M)

Benchmarking with small capacity (10007)...

Small Capacity Performance Results:
==================================
Insert 1000000 items:  1.7457 seconds (572850 ops/sec)
Lookup 1000000 items:  6.4033 seconds (156170 ops/sec)
Update 500000 items:  1.2240 seconds (408484 ops/sec)
Miss 1000000 lookups:  6.6006 seconds (151501 ops/sec)

Benchmarking with optimal capacity (1000003)...

Optimal Capacity Performance Results:
==================================
Insert 1000000 items:  0.1032 seconds (9685981 ops/sec)
Lookup 1000000 items:  0.7368 seconds (1357217 ops/sec)
Update 500000 items:  0.0694 seconds (7204819 ops/sec)
Miss 1000000 lookups:  0.1549 seconds (6457112 ops/sec)

Benchmarking with large capacity (2000003)...

Large Capacity Performance Results:
==================================
Insert 1000000 items:  0.0875 seconds (11428049 ops/sec)
Lookup 1000000 items:  0.8614 seconds (1160933 ops/sec)
Update 500000 items:  0.0685 seconds (7294052 ops/sec)
Miss 1000000 lookups:  0.1421 seconds (7036802 ops/sec)
```

## License

MIT License - see LICENSE file for details

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

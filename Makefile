CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 -I./include
LDFLAGS = 
ARFLAGS = rcs
EXAMPLE_TARGET = bin/example
TEST_TARGET = bin/test_cache
BENCH_TARGET = bin/bench_cache
LIB_TARGET = lib/libanothercache.a
LIB_SHARED_TARGET = lib/libanothercache.so

# Installation directories
PREFIX ?= /usr/local
INSTALL_LIBDIR = $(PREFIX)/lib
INSTALL_INCLUDEDIR = $(PREFIX)/include

SRCDIR = src
TESTDIR = tests
BENCHDIR = benchmarks
EXAMPLEDIR = examples
OBJDIR = obj
BINDIR = bin
LIBDIR = lib

# Source files
SOURCES = $(SRCDIR)/anothercache.c
MAIN_SOURCE = $(EXAMPLEDIR)/main.c
TEST_SOURCE = $(TESTDIR)/test_cache.c
BENCH_SOURCE = $(BENCHDIR)/bench_cache.c

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
MAIN_OBJECT = $(MAIN_SOURCE:$(EXAMPLEDIR)/%.c=$(OBJDIR)/%.o)
TEST_OBJECT = $(TEST_SOURCE:$(TESTDIR)/%.c=$(OBJDIR)/%.o)
BENCH_OBJECT = $(BENCH_SOURCE:$(BENCHDIR)/%.c=$(OBJDIR)/%.o)

.PHONY: all clean test bench run libs example install uninstall

all: libs example test

libs: $(LIB_TARGET) $(LIB_SHARED_TARGET)

example: $(EXAMPLE_TARGET)

$(LIB_TARGET): $(OBJECTS) | $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_SHARED_TARGET): $(OBJECTS) | $(LIBDIR)
	$(CC) -shared -o $@ $^

$(EXAMPLE_TARGET): $(MAIN_OBJECT) $(LIB_TARGET) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJECT) -L$(LIBDIR) -lanothercache $(LDFLAGS)

$(TEST_TARGET): $(OBJECTS) $(TEST_OBJECT) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BENCH_TARGET): $(OBJECTS) $(BENCH_OBJECT) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(TESTDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(BENCHDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(EXAMPLEDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(LIBDIR):
	mkdir -p $(LIBDIR)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

bench: $(BENCH_TARGET)
	./$(BENCH_TARGET)

run: $(EXAMPLE_TARGET)
	./$(EXAMPLE_TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR)

install: libs
	install -d $(INSTALL_LIBDIR)
	install -d $(INSTALL_INCLUDEDIR)
	install -m 644 $(LIB_TARGET) $(INSTALL_LIBDIR)
	install -m 755 $(LIB_SHARED_TARGET) $(INSTALL_LIBDIR)
	install -m 644 include/anothercache.h $(INSTALL_INCLUDEDIR)
	ldconfig

uninstall:
	rm -f $(INSTALL_LIBDIR)/libanothercache.a
	rm -f $(INSTALL_LIBDIR)/libanothercache.so
	rm -f $(INSTALL_INCLUDEDIR)/anothercache.h

# Development helpers
format:
	clang-format -i $(SRCDIR)/*.c $(TESTDIR)/*.c $(BENCHDIR)/*.c $(EXAMPLEDIR)/*.c include/*.h

valgrind: $(EXAMPLE_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXAMPLE_TARGET)

valgrind-test: $(TEST_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_TARGET)

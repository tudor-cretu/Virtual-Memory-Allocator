#pragma once
#include <inttypes.h>
#include <stddef.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Macro for memory error handling (defensive programming)
#define DIE(assertion, call_description) \
	do {                                  \
		if (assertion) {                   \
			fprintf(stderr, "(%s, %d): ",   \
					  __FILE__, __LINE__);   \
			perror(call_description);         \
			exit(errno);                       \
		}                                       \
	} while (0)

#define STRING_SIZE 100

// node structure for doubly linked list
typedef struct dll_node_t dll_node_t;
struct dll_node_t {
	void *data;
	dll_node_t *prev, *next;
};

// doubly linked list structure
typedef struct {
	dll_node_t *head;
	unsigned int data_size;
	unsigned int size;
} list_t;

// block structure
typedef struct {
	uint64_t start_address; // start address of the block, a value in the arena
	size_t size;            // total size of the block (sum of miniblock sizes)
	void *miniblock_list;   // list of miniblocks in the block
} block_t;

// miniblock structure
typedef struct {
	uint64_t start_address; // start address of the miniblock, a value in the block
	size_t size;            // total size of the miniblock
	uint8_t perm;           // permissions allocated to the minblock, (RW- by default)
	void *rw_buffer;        // buffer used for read/write operations
} miniblock_t;

// arena structure
typedef struct {
	uint64_t arena_size; // total size of the arena
	list_t *alloc_list;  // list of blocks in the arena
} arena_t;

list_t *dll_create(unsigned int data_size);
dll_node_t *dll_create_node(void *new_data, int data_size);
dll_node_t *dll_get_nth_node(list_t *list, unsigned int n);
void dll_add_nth_node(list_t *list, unsigned int n, void *new_data);
void dll_remove_nth_node(list_t *list, unsigned int n);

dll_node_t *get_node_by_address_block(list_t *list, uint64_t address);
dll_node_t *get_node_by_address_miniblock(list_t *list, uint64_t address);
dll_node_t *get_node_by_address_in_block(list_t *list, uint64_t address);
dll_node_t *get_node_by_address_in_miniblock(list_t *list, uint64_t address);
int get_pos_greater_than_address(list_t *list, uint64_t address);
int get_pos_greater_than_address(list_t *list, uint64_t address);

void dll_free(list_t **pp_list);

arena_t *alloc_arena(const uint64_t size);
void dealloc_arena(arena_t *arena);

void alloc_block(arena_t *arena, const uint64_t address, const uint64_t size);
void free_block(arena_t *arena, const uint64_t address);

void read(arena_t *arena, uint64_t address, uint64_t size);
void write(arena_t *arena, const uint64_t address, const uint64_t size);

void pmap(const arena_t *arena);

void mprotect(arena_t *arena, uint64_t address);

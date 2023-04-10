// Copyright Cretu Mihnea Tudor 315CAa 2022 - 2023

#include "vma.h"
// information about all the commands can be found in the README file

int main(void)
{
	arena_t *arena;
	char command[STRING_SIZE];
	while (scanf("%s", command)) {
		if (strcmp(command, "ALLOC_ARENA") == 0) {
			uint64_t size;
			scanf("%lu", &size);
			arena = alloc_arena(size);
		} else if (strcmp(command, "DEALLOC_ARENA") == 0) {
			dealloc_arena(arena);
			return 0;
		} else if (strcmp(command, "ALLOC_BLOCK") == 0) {
			uint64_t address, block_size;
			scanf("%lu %lu", &address, &block_size);
			alloc_block(arena, address, block_size);
		} else if (strcmp(command, "FREE_BLOCK") == 0) {
			uint64_t address;
			scanf("%lu", &address);
			free_block(arena, address);
		} else if (strcmp(command, "READ") == 0) {
			uint64_t address, size;
			scanf("%lu %lu", &address, &size);
			read(arena, address, size);
		} else if (strcmp(command, "WRITE") == 0) {
			uint64_t address, size;
			scanf("%lu %lu", &address, &size);
			write(arena, address, size);
		} else if (strcmp(command, "PMAP") == 0) {
			pmap(arena);
		} else if (strcmp(command, "MPROTECT") == 0) {
			uint64_t address;
			scanf("%lu", &address);
			mprotect(arena, address);
		} else {
			printf("Invalid command. Please try again.\n");
		}
	}
	return 0;
}

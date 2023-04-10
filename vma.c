// Copyright Cretu Mihnea Tudor 315CAa 2022 - 2023
#include "vma.h"

// function to create an empty doubly linked list
list_t *dll_create(unsigned int data_size)
{
	list_t *list = malloc(sizeof(list_t));
	DIE(!list, "malloc for list failed");

	list->data_size = data_size;
	list->head = NULL;
	list->size = 0;

	return list;
}

// function to create a node for doubly linked list
dll_node_t *dll_create_node(void *new_data, int data_size)
{
	dll_node_t *new_node = malloc(sizeof(dll_node_t));
	DIE(!new_node, "malloc for new node failed");

	new_node->data = malloc(data_size);
	DIE(!new_node->data, "malloc for new node data failed");

	new_node->next = NULL;
	memcpy(new_node->data, new_data, data_size);

	return new_node;
}

// function to get the nth node from a doubly linked list
dll_node_t *dll_get_nth_node(list_t *list, unsigned int n)
{
	if (n >= list->size)
		n = list->size - 1;
	dll_node_t *aux = list->head;
	for (unsigned int i = 0; i < n; i++)
		aux = aux->next;
	return aux;
}

// function to add a node at the nth position in a doubly linked list
void dll_add_nth_node(list_t *list, unsigned int n, void *new_data)
{
	if (!list)
		return;
	if (n >= list->size)
		n = list->size;

	if (!n) {
		dll_node_t *new_node = dll_create_node(new_data, list->data_size);
		new_node->next = list->head;
		if (list->head)
			list->head->prev = new_node;
		list->head = new_node;
		list->size++;
		return;
	}

	dll_node_t *prev_node = dll_get_nth_node(list, n - 1);
	if (!prev_node)
		return;
	dll_node_t *next_node = prev_node->next;
	dll_node_t *new_node = dll_create_node(new_data, list->data_size);
	if (!new_node)
		return;
	prev_node->next = new_node;
	new_node->prev = prev_node;
	if (next_node) {
		new_node->next = next_node;
		next_node->prev = new_node;
	}
	list->size++;
}

// function to remove the nth node from a doubly linked list
void dll_remove_nth_node(list_t *list, unsigned int n)
{
	if (n >= list->size)
		n = list->size - 1;
	if (!n) {
		dll_node_t *node_to_remove = list->head;
		list->head = node_to_remove->next;
		list->size--;
		node_to_remove->prev = NULL;
		node_to_remove->next = NULL;
		free(node_to_remove->data);
		free(node_to_remove);
		return;
	}
	dll_node_t *node_to_remove = dll_get_nth_node(list, n);
	if (!node_to_remove)
		return;
	if (node_to_remove == list->head)
		list->head = node_to_remove->next;
	if (node_to_remove->prev)
		node_to_remove->prev->next = node_to_remove->next;
	if (node_to_remove->next)
		node_to_remove->next->prev = node_to_remove->prev;
	list->size--;
	node_to_remove->prev = NULL;
	node_to_remove->next = NULL;
	free(node_to_remove->data);
	free(node_to_remove);
}

// function to get the block with the start address equal to the address given
dll_node_t *get_node_by_address_block(list_t *list, uint64_t address)
{
	dll_node_t *aux = list->head;
	while (aux) {
		if (((block_t *)aux->data)->start_address == address)
			return aux;
		aux = aux->next;
	}
	return NULL;
}

// function to get the miniblock with the start address equal to the address given
dll_node_t *get_node_by_address_miniblock(list_t *list, uint64_t address)
{
	dll_node_t *aux = list->head;
	while (aux) {
		if (((miniblock_t *)aux->data)->start_address == address)
			return aux;
		aux = aux->next;
	}
	return NULL;
}

// function to get the block with the address given in the range of the block
dll_node_t *get_node_by_address_in_block(list_t *list, uint64_t address)
{
	dll_node_t *aux = list->head;
	while (aux) {
		if (address >= ((block_t *)aux->data)->start_address &&
			address < ((block_t *)aux->data)->start_address + ((block_t *)aux->data)->size)
			return aux;
		aux = aux->next;
	}
	return NULL;
}

// function to get the miniblock with the address given in the range of the miniblock
dll_node_t *get_node_by_address_in_miniblock(list_t *list, uint64_t address)
{
	dll_node_t *aux = list->head;
	while (aux) {
		if (address >= ((miniblock_t *)aux->data)->start_address &&
			address < ((miniblock_t *)aux->data)->start_address + ((miniblock_t *)aux->data)->size)
			return aux;
		aux = aux->next;
	}
	return NULL;
}

// function to get the position of the block with the start address greater than the address given
int get_pos_greater_than_address(list_t *list, uint64_t address)
{
	dll_node_t *aux = list->head;
	unsigned int position = 0;
	while (aux) {
		if (((block_t *)aux->data)->start_address > address)
			return position;
		aux = aux->next;
		position++;
	}
	return position;
}

// function to get the position of the block with the start address equal to the address given
int get_pos_equal_to_address(list_t *list, uint64_t address)
{
	dll_node_t *aux = list->head;
	unsigned int position = 0;
	while (aux) {
		if (((block_t *)aux->data)->start_address == address)
			return position;
		aux = aux->next;
		position++;
	}
	return position;
}

// function to free a doubly linked list
void dll_free(list_t **pp_list)
{
	dll_node_t *current = (*pp_list)->head;
	while (current) {
		dll_node_t *next = current->next;
		free(current->data); // free the data first
		free(current);       // then free the node itself
		current = next;
	}

	// free the list itself
	free(*pp_list);

	// setted the pointer to NULL
	*pp_list = NULL;
}

// function to allocate memory for the virtual arena
arena_t *alloc_arena(const uint64_t size)
{
	arena_t *arena = malloc(sizeof(arena_t));
	DIE(!arena, "malloc for virtual arena failed");
	arena->arena_size = size;
	arena->alloc_list = dll_create(sizeof(block_t));
	return arena;
}

// function to free the virtual arena and all its allocated data
void dealloc_arena(arena_t *arena)
{
	dll_node_t *node = arena->alloc_list->head;
	while (node) {
		dll_node_t *aux = node;
		node = node->next;
		list_t *list = ((list_t *)((block_t *)aux->data)->miniblock_list);
		dll_node_t *miniblock = list->head;
		while (miniblock) {
			free(((miniblock_t *)miniblock->data)->rw_buffer);
			miniblock = miniblock->next;
		}
		dll_free(&list);
	}
	dll_free(&arena->alloc_list);
	free(arena);
}

// function to allocate memory for a block
void alloc_block(arena_t *arena, const uint64_t address, const uint64_t size)
{
	if (!arena)
		return;
	if (address >= arena->arena_size) {
		printf("The allocated address is outside the size of arena\n");
		return;
	}
	if (address + size > arena->arena_size) {
		printf("The end address is past the size of the arena\n");
		return;
	}

	block_t *block = NULL;
	miniblock_t *miniblock = NULL;
	if (!arena->alloc_list->head) {// adding the first block in the arena
		block = malloc(sizeof(block_t));
		DIE(!block, "malloc for block failed");
		block->miniblock_list = dll_create(sizeof(miniblock_t));
		miniblock = malloc(sizeof(miniblock_t));
		DIE(!miniblock, "malloc for miniblock failed");

		block->size = size;
		miniblock->size = size;
		block->start_address = address;
		miniblock->start_address = address;
		miniblock->perm = 6;
		miniblock->rw_buffer = NULL;

		// adding the first block to the arena
		dll_add_nth_node(arena->alloc_list, 0, block);
		// adding the first miniblock to the miniblock list of the block
		dll_add_nth_node(block->miniblock_list, 0, miniblock);

		free(block);
		free(miniblock);
	} else {
		dll_node_t *node = arena->alloc_list->head; // the first block in the arena
		unsigned int position = 0;
		short okay = 0; // 0 if the block is not connected to any other block

		while (node) {
			if (address == ((block_t *)node->data)->start_address + ((block_t *)node->data)->size)
				okay = 1; // block will be connected to the right side of another block

			if (address + size == ((block_t *)node->data)->start_address)
				okay = 2; // block will be connected to the left side of another block

			if (arena->alloc_list->size > 1 && node->next)
				if (address == ((block_t *)node->data)->start_address + ((block_t *)node->data)->size &&
					address + size == ((block_t *)node->next->data)->start_address)
					okay = 3; // block will be connected to the left and right side of 2 different blocks

			if ((address + size > ((block_t *)node->data)->start_address &&
			address + size < ((block_t *)node->data)->start_address + ((block_t *)node->data)->size) ||
			(address < ((block_t *)node->data)->start_address + ((block_t *)node->data)->size &&
			address > ((block_t *)node->data)->start_address) ||
			(address < ((block_t *)node->data)->start_address &&
			address + size > ((block_t *)node->data)->start_address + ((block_t *)node->data)->size) ||
			(address == ((block_t *)node->data)->start_address &&
			address + size == ((block_t *)node->data)->start_address + ((block_t *)node->data)->size)) {
				okay = 4; // the zone where the block would be allocated is already in use by another block
				printf("This zone was already allocated.\n");
			}
			if (okay)
				break;
			position++;
			node = node->next;
		}

		if (okay == 0) { // not connected to any other block
			block = malloc(sizeof(block_t));
			DIE(!block, "malloc for block failed");
			block->miniblock_list = dll_create(sizeof(miniblock_t));
			miniblock = malloc(sizeof(miniblock_t));
			DIE(!miniblock, "malloc for miniblock failed");

			block->size = size;
			miniblock->size = size;
			block->start_address = address;
			miniblock->start_address = address;
			miniblock->perm = 6;
			miniblock->rw_buffer = NULL;

			// adding the block to the arena
			dll_add_nth_node(arena->alloc_list, get_pos_greater_than_address(arena->alloc_list, address), block);
			// adding the first miniblock to the miniblock list of the block
			dll_add_nth_node(block->miniblock_list, 0, miniblock);

			free(block);
			free(miniblock);
		}

		else if (okay == 1) { // connected to the right
			miniblock = malloc(sizeof(miniblock_t));
			DIE(!miniblock, "malloc for miniblock failed");

			// changing the size of the block correspondingly
			((block_t *)node->data)->size += size;

			miniblock->size = size;
			miniblock->start_address = address;
			miniblock->perm = 6;
			miniblock->rw_buffer = NULL;

			// adding the last miniblock to the miniblock list of the block
			dll_add_nth_node(((list_t *)((block_t *)node->data)->miniblock_list), ((list_t *)((block_t *)node->data)->miniblock_list)->size + 1, miniblock);

			free(block);
			free(miniblock);
		}

		else if (okay == 2) { // connected to the left
			miniblock = malloc(sizeof(miniblock_t));
			DIE(!miniblock, "malloc for miniblock failed");

			// changing the size and start address of the block correspondingly
			((block_t *)node->data)->size += size;
			((block_t *)node->data)->start_address = address;

			miniblock->size = size;
			miniblock->start_address = address;
			miniblock->perm = 6;
			miniblock->rw_buffer = NULL;

			// adding the first miniblock to the miniblock list of the block
			dll_add_nth_node(((list_t *)((block_t *)node->data)->miniblock_list), 0, miniblock);
			free(block);
			free(miniblock);
		}

		else if (okay == 3) { // connected in the middle of 2 blocks
			miniblock = malloc(sizeof(miniblock_t));
			DIE(!miniblock, "malloc for miniblock failed");

			// changing the size of the block correspondingly
			((block_t *)node->data)->size += size + ((block_t *)node->next->data)->size;

			miniblock->size = size;
			miniblock->start_address = address;
			miniblock->perm = 6;
			miniblock->rw_buffer = NULL;

			// adding the last miniblock to the miniblock list of this block before merging with the next block
			dll_add_nth_node(((list_t *)((block_t *)node->data)->miniblock_list), ((list_t *)((block_t *)node->data)->miniblock_list)->size + 1, miniblock);

			// merging the 2 blocks and the miniblock lists
			dll_node_t *last_node = ((list_t *)((block_t *)node->data)->miniblock_list)->head;
			while (last_node->next)
				last_node = last_node->next;
			last_node->next = ((list_t *)((block_t *)node->next->data)->miniblock_list)->head;
			((list_t *)((block_t *)node->data)->miniblock_list)->size += ((list_t *)((block_t *)node->next->data)->miniblock_list)->size;
			((list_t *)((block_t *)node->next->data)->miniblock_list)->head->prev = last_node;
			((list_t *)((block_t *)node->next->data)->miniblock_list)->head = NULL;

			// removing the next block and its data from the arena
			dll_free(((list_t *)&((block_t *)node->next->data)->miniblock_list));
			dll_remove_nth_node(arena->alloc_list, position + 1);
			free(block);
			free(miniblock);
		}
	}
}

// function to free a block and its data
void free_block(arena_t *arena, const uint64_t address)
{
	if (!arena->alloc_list->size) {
		printf("Invalid address for free.\n");
		return;
	}

	dll_node_t *node = get_node_by_address_in_block(arena->alloc_list, address);
	if (!node) {
		printf("Invalid address for free.\n");
		return;
	}

	list_t *list_of_miniblocks = ((list_t *)((block_t *)node->data)->miniblock_list);
	dll_node_t *miniblock = get_node_by_address_miniblock(list_of_miniblocks, address);

	if (!miniblock) {
		printf("Invalid address for free.\n");
		return;
	}

	if (list_of_miniblocks->size == 1) { 
		// miniblock lists consists of only one miniblock
		dll_remove_nth_node(list_of_miniblocks, 0);
		dll_free(&list_of_miniblocks);
		dll_remove_nth_node(arena->alloc_list, get_pos_equal_to_address(arena->alloc_list, address));
		return;
	} 
	if (miniblock == list_of_miniblocks->head) {
		// miniblock is the first in the list
		((block_t *)node->data)->size -= ((miniblock_t *)miniblock->data)->size;
		((block_t *)node->data)->start_address += ((miniblock_t *)miniblock->data)->size;
		dll_remove_nth_node(list_of_miniblocks, 0);
	} else if (!miniblock->next) {
		// miniblock is the last in the list
		((block_t *)node->data)->size -= ((miniblock_t *)miniblock->data)->size;
		dll_remove_nth_node(list_of_miniblocks, list_of_miniblocks->size - 1);
	} else { 
		// miniblock is nor the first nor the last in the list
		dll_node_t *aux = miniblock->next;
		block_t *new_block = malloc(sizeof(block_t));
		DIE(!new_block, "malloc for block failed");

		// the block and its miniblock list will be split in 2
		new_block->miniblock_list = dll_create(sizeof(miniblock_t));
		new_block->size = ((block_t *)node->data)->start_address + ((block_t *)node->data)->size - ((miniblock_t *)aux->data)->start_address;
		new_block->start_address = ((miniblock_t *)aux->data)->start_address;

		((list_t *)new_block->miniblock_list)->head = aux;
		dll_node_t *curr = list_of_miniblocks->head;
		size_t block_size = 0;
		while (curr != miniblock) {
			block_size += ((miniblock_t *)curr->data)->size;
			curr = curr->next;
		}

		((block_t *)node->data)->size = block_size;
		// disconnecting the 2 miniblock lists
		miniblock->next = NULL;
		aux->prev = NULL;

		while (aux) {
			((list_t *)((block_t *)new_block)->miniblock_list)->size++;
			aux = aux->next;
		}

		// adding the new block and its new miniblock list to the arena
		((list_t *)((block_t *)node->data)->miniblock_list)->size -= ((list_t *)((block_t *)new_block)->miniblock_list)->size;
		dll_remove_nth_node(list_of_miniblocks, get_pos_equal_to_address(list_of_miniblocks, address));
		dll_add_nth_node(arena->alloc_list, get_pos_greater_than_address(arena->alloc_list, address), new_block);
		free(new_block);
	}
}

// function to read the buffer of a block in which was written in before
void read(arena_t *arena, uint64_t address, uint64_t size)
{
	if (!arena->alloc_list->size) {
		printf("Invalid address for read.\n");
		return;
	}

	dll_node_t *list_node = get_node_by_address_in_block(arena->alloc_list, address);
	if (!list_node) {
		printf("Invalid address for read.\n");
		return;
	}

	list_t *list_of_miniblocks = ((block_t *)list_node->data)->miniblock_list;
	dll_node_t *miniblock = get_node_by_address_in_miniblock(list_of_miniblocks, address);

	if (((miniblock_t *)miniblock->data)->perm == 0 || ((miniblock_t *)miniblock->data)->perm == 2 ||
		((miniblock_t *)miniblock->data)->perm == 1 || ((miniblock_t *)miniblock->data)->perm == 3) {
		printf("Invalid permissions for read.\n");
		return;
	}

	if (!((miniblock_t *)miniblock->data)->rw_buffer) {
		printf("Invalid address for read.\n");
		free(miniblock);
		return;
	}

	// checking if the read size is bigger than the size remaining in the block
	uint64_t available_size = size;
	if (((block_t *)list_node->data)->size + ((block_t *)list_node->data)->start_address - address < size) {
		available_size = ((block_t *)list_node->data)->size + ((block_t *)list_node->data)->start_address - address;
		printf("Warning: size was bigger than the block size. Reading %lu characters.\n", available_size);
	}

	// reading character by character from the rw_buffer in each miniblock in the miniblock list
	uint64_t iterating_write_address = address;
	unsigned int char_pos = address - ((miniblock_t *)miniblock->data)->start_address;
	for (unsigned int i = 0; i < available_size; i++) {
		// switching to the next miniblock if the current one has been read from
		if (iterating_write_address >= ((miniblock_t *)miniblock->data)->start_address + ((miniblock_t *)miniblock->data)->size) {
			miniblock = miniblock->next;
			((miniblock_t *)miniblock->data)->rw_buffer = calloc(((miniblock_t *)miniblock->data)->size, sizeof(char));
			DIE(!((miniblock_t *)miniblock->data)->rw_buffer, "calloc for rw_buffer failed");
			char_pos = 0;
		}
		printf("%c", ((char *)((miniblock_t *)miniblock->data)->rw_buffer)[char_pos]);
		iterating_write_address++;
		char_pos++;
	}
	printf("\n");
}

// function to write to a block
void write(arena_t *arena, const uint64_t address, const uint64_t size)
{
	if (!arena->alloc_list->size) {
		printf("Invalid address for write.\n");
		char character;
		scanf("%c", &character);
		for (unsigned int i = 0; i < size; i++)
			scanf("%c", &character);
		return;
	}

	dll_node_t *list_node = get_node_by_address_in_block(arena->alloc_list, address);
	if (!list_node) {
		printf("Invalid address for write.\n");
		char character;
		scanf("%c", &character);
		for (unsigned int i = 0; i < size; i++)
			scanf("%c", &character);
		return;
	}

	list_t *list_of_miniblocks = ((block_t *)list_node->data)->miniblock_list;
	dll_node_t *miniblock = get_node_by_address_in_miniblock(list_of_miniblocks, address);

	if (((miniblock_t *)miniblock->data)->perm == 0 || ((miniblock_t *)miniblock->data)->perm == 4 ||
		((miniblock_t *)miniblock->data)->perm == 1 || ((miniblock_t *)miniblock->data)->perm == 5) {
		printf("Invalid permissions for write.\n");
		char character;
		scanf("%c", &character);
		for (unsigned int i = 0; i < size; i++)
			scanf("%c", &character);
		return;
	}

	// checking if the write size is bigger than the size remaining in the block
	uint64_t available_size = size;
	if (((block_t *)list_node->data)->size + ((block_t *)list_node->data)->start_address - address < size) {
		available_size = ((block_t *)list_node->data)->size + ((block_t *)list_node->data)->start_address - address;
		printf("Warning: size was bigger than the block size. Writing %lu characters.\n", available_size);
	}

	((miniblock_t *)miniblock->data)->rw_buffer = calloc(((miniblock_t *)miniblock->data)->size, sizeof(char));
	DIE(!((miniblock_t *)miniblock->data)->rw_buffer, "calloc for rw_buffer failed");

	// writing character by character in the rw_buffer in each miniblock in the miniblock list
	char character;
	uint64_t iterating_write_address = address;
	unsigned int char_pos = address - ((miniblock_t *)miniblock->data)->start_address;
	scanf("%c", &character);
	for (unsigned int i = 0; i < available_size; i++) {
		// switching to the next miniblock if the current one has been written to
		if (iterating_write_address >= ((miniblock_t *)miniblock->data)->start_address + ((miniblock_t *)miniblock->data)->size) {
			miniblock = miniblock->next;
			((miniblock_t *)miniblock->data)->rw_buffer = calloc(((miniblock_t *)miniblock->data)->size, sizeof(char));
			DIE(!((miniblock_t *)miniblock->data)->rw_buffer, "calloc for rw_buffer failed");
			char_pos = 0;
		}
		scanf("%c", &character);
		((char *)((miniblock_t *)miniblock->data)->rw_buffer)[char_pos] = character;
		iterating_write_address++;
		char_pos++;
	}

	// reading the remaining characters from the input in case that the write size was bigger than the available size
	// so that the program doesn't read it as an invalid command
	if (size > available_size) {
		available_size = size - available_size;
		for (unsigned int i = 0; i < available_size; i++)
			scanf("%c", &character);
	}
}

// function to see the memory usage and properties in the virtual arena
void pmap(const arena_t *arena)
{
	printf("Total memory: 0x%lX bytes\n", arena->arena_size);

	dll_node_t *list_block = arena->alloc_list->head;
	size_t total_size = 0;
	unsigned int number_of_miniblocks = 0;
	while (list_block) {
		dll_node_t *miniblock = ((list_t *)((block_t *)list_block->data)->miniblock_list)->head;
		while (miniblock) {
			number_of_miniblocks++;
			miniblock = miniblock->next;
		}
		total_size += ((block_t *)list_block->data)->size;
		list_block = list_block->next;
	}
	printf("Free memory: 0x%lX bytes\n", arena->arena_size - total_size);

	printf("Number of allocated blocks: %d\n", arena->alloc_list->size);
	printf("Number of allocated miniblocks: %d\n", number_of_miniblocks);

	dll_node_t *node = arena->alloc_list->head;
	for (unsigned int i = 1; i <= arena->alloc_list->size; i++) {
		printf("\n");
		printf("Block %d begin\n", i);
		printf("Zone: 0x%lX - 0x%lX\n", ((block_t *)node->data)->start_address, ((block_t *)node->data)->start_address + ((block_t *)node->data)->size);

		int miniblock_list_size = ((list_t *)((block_t *)node->data)->miniblock_list)->size;
		dll_node_t *miniblock_node = ((list_t *)((block_t *)node->data)->miniblock_list)->head;
		for (int j = 1; j <= miniblock_list_size; j++) {
			printf("Miniblock %d:\t\t", j);
			printf("0x%lX\t\t-\t\t0x%lX\t\t|", ((miniblock_t *)miniblock_node->data)->start_address, ((miniblock_t *)miniblock_node->data)->start_address + ((miniblock_t *)miniblock_node->data)->size);
			if (((miniblock_t *)miniblock_node->data)->perm == 6)
				printf(" RW-\n");
			if (((miniblock_t *)miniblock_node->data)->perm == 4)
				printf(" R--\n");
			if (((miniblock_t *)miniblock_node->data)->perm == 2)
				printf(" -W-\n");
			if (((miniblock_t *)miniblock_node->data)->perm == 1)
				printf(" --X\n");
			if (((miniblock_t *)miniblock_node->data)->perm == 0)
				printf(" ---\n");
			if (((miniblock_t *)miniblock_node->data)->perm == 7)
				printf(" RWX\n");
			if (((miniblock_t *)miniblock_node->data)->perm == 5)
				printf(" R-X\n");
			if (((miniblock_t *)miniblock_node->data)->perm == 3)
				printf(" -WX\n");
			miniblock_node = miniblock_node->next;
		}
		printf("Block %d end\n", i);
		node = node->next;
	}
}

// function that changes the permissions of a miniblock's actions
// multiple permissions can be changed at once: read, write, execute (R, W, X)
void mprotect(arena_t *arena, uint64_t address)
{
	char permission[STRING_SIZE];
	fgets(permission, STRING_SIZE, stdin);

	if (!arena->alloc_list->size) {
		printf("Invalid address for mprotect.\n");
		return;
	}

	dll_node_t *list_node = get_node_by_address_in_block(arena->alloc_list, address);
	if (!list_node) {
		printf("Invalid address for mprotect.\n");
		return;
	}

	list_t *list_of_miniblocks = ((block_t *)list_node->data)->miniblock_list;
	dll_node_t *miniblock = get_node_by_address_miniblock(list_of_miniblocks, address);
	if (!miniblock) {
		printf("Invalid address for mprotect.\n");
		return;
	}

	((miniblock_t *)miniblock->data)->perm = 0;
	char *token = strtok(permission, " \n");
	while (token) {
		if (strcmp(token,"PROT_NONE") == 0)
			((miniblock_t *)miniblock->data)->perm += 0;
		else if (strcmp(token, "PROT_READ") == 0)
			((miniblock_t *)miniblock->data)->perm += 4;
		else if (strcmp(token, "PROT_WRITE") == 0)
			((miniblock_t *)miniblock->data)->perm += 2;
		else if (strcmp(token, "PROT_EXEC") == 0)
			((miniblock_t *)miniblock->data)->perm += 1;
		token = strtok(NULL, " \n");
	}
}

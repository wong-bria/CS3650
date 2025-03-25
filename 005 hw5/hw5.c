#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM 128

struct line {
    int address;
    int valid;
    int modified;
  };

struct line cache[NUM];

void init_cache() {
	for (int i = 0; i < NUM; i++) {
        	cache[i].address = -1; 
        	cache[i].valid = 0;     // Mark the line as invalid
        	cache[i].modified = 0; 
    }
}

struct node {
  int cacheIndex;
  struct node *next;
  struct node *prev;
};

struct node list_of_nodes[NUM];
struct node headFreeList;  // Special head node

struct node headBusyList; // Special head node


void init_free_list() {
  struct node *myNode = &headFreeList;
  myNode->cacheIndex = -1;
  myNode->next = &(list_of_nodes[0]);  // RECALL: '&' means "address of"

  for (int i = 0; i < NUM-1; i++){
    myNode = &list_of_nodes[i];
    //myNode->prev = NULL; // Why is prev set to NULL? given in instruction
    list_of_nodes[i+1].prev = myNode; // testing
    myNode->cacheIndex = i;
    myNode->next = &(list_of_nodes[i+1]);
  }
  //myNode->prev = NULL; // Why NULL? given in instruction
  myNode = &list_of_nodes[NUM-1]; // testing
  myNode->cacheIndex = NUM-1;
  myNode->next = NULL; // last node has no next
}

void init_busy_list() {
  headBusyList.cacheIndex = -1;
  headBusyList.next = NULL;
}

// move cache line to front (most recently used)
void move_to_front(struct node *line) {
	//return;
	if (headBusyList.next == NULL) {
        	headBusyList.next = line;
        	line->prev = &headBusyList;
        	line->next = NULL;
        	return;
    	}

	struct node *find_last = headBusyList.next;
        while (find_last->next != NULL) {
        	find_last = find_last->next;
        }

	if (line->prev) line->prev->next = line->next;
        if (line->next) line->next->prev = line->prev;

        line->prev = find_last;
        line->next = NULL;
	find_last->next = line;
}

// evict least recently used (LRU) cache line from end of list
void evict_cache_line() {
	struct node *evicted = headBusyList.next;
	if (evicted == NULL) {
		return; // no cache line to evict
	}
	// make second element the first now
	headBusyList.next = evicted->next;
	if (headBusyList.next != NULL) {
        	headBusyList.next->prev = &headBusyList;
    	}
	
	cache[evicted->cacheIndex].valid = 0;
        cache[evicted->cacheIndex].address = -1;

	// Add the evicted node to the front of the free list
    	evicted->next = headFreeList.next;
    	if (headFreeList.next != NULL) {
        	headFreeList.next->prev = evicted;
    	}
    	evicted->prev = &headFreeList;
   	headFreeList.next = evicted;
}

// Access type of cache depending on type parameter
void access_cache(int address, char * type) {
	if (strcmp(type, "-d") == 0) {
		int offset = address % 4;
        	int index = (address / 4) % NUM;
        	int tag = address / (4 * NUM);

        	// check if cache hit or miss
        	if (cache[index].valid && cache[index].address == address) {
                	printf("%d: HIT (Tag/Index/Offset: %d/%d/%d)\n", address, tag, index, offset);
        	} else {
                	printf("%d: MISS (Tag/Index/Offset: %d/%d/%d)\n", address, tag, index, offset);
                	// since miss, update cache
                	cache[index].valid = 1;
                	cache[index].address = address;
                	cache[index].modified = 0;
        	}
	} else {
		int offset = address % 4;
		int tag = address / 4;

		struct node *current = headBusyList.next;
		struct node *hit_node = NULL;

		// check if node already in cache
		while (current) {
			if (cache[current->cacheIndex].valid && cache[current->cacheIndex].address == tag) {
				hit_node = current;
				break;
			}
			current = current->next; // go next node in busy
		}

		if (hit_node) {
			printf("%d: HIT (Tag/-/Offset: %d/0/%d)\n", address, tag, offset);
			move_to_front(hit_node);
		} else {
			printf("%d: MISS (Tag/-/Offset: %d/0/%d)\n", address, tag, offset);


			// evict LRU if cache full
			if(!headFreeList.next) {
				evict_cache_line();
			}

			// find free cache line to update
			struct node *new_node = headFreeList.next;
			if (new_node) {
				// remove from free list
				headFreeList.next = new_node->next;
				if (new_node->next) {
					new_node->next->prev = &headFreeList;
				}
				
				struct node *find_last = headBusyList.next;
				while (find_last != NULL && find_last->next != NULL) {
					find_last = find_last->next;
				}

				new_node->prev = find_last;
				new_node->next = NULL;
				if (find_last != NULL) {
					find_last->next = new_node;
				} else {
					headBusyList.next = new_node;
				}

				// update cache
				cache[new_node->cacheIndex].valid = 1;
				cache[new_node->cacheIndex].address = tag;
				cache[new_node->cacheIndex].modified = 0;
			}
		}
	}
}

// Print cache contents depending on type parameter
void print_cache_content(char * type) {
	if (strcmp(type, "-d") == 0) {
		printf("Cache contents (for each cache row index):\n");
		for (int i = 0; i < NUM; i++) {
			if (cache[i].valid) {
			  printf("%d: Valid: True ;  Tag:  %d  (Index:  %d)\n", i, cache[i].address / (4 * NUM), i);
			} else {
				printf("%d: Valid: False ;  Tag:  -  (Index:  %d)\n", i, i);
			}
		}
	} else {
		printf("Cache contents (for each cache row index):\n");
		for (int i = 0; i < NUM; i++) {
			if (cache[i].valid) {
			  printf("%3d: Valid: True ;  Tag:  %d  (Set #:  0)\n", i, cache[i].address);
			} else {
			  printf("%3d: Valid: False ;  Tag:  %d  (Set #:  0)\n", i, cache[i].address);
			}
		}
	}
}

int read_address() {
	int num = 0;
	int has_digits = 0;

  	while (1) {
    		char mychar;
    		int rc = read(0, &mychar, 1);
    		if (rc == 0) { // if end-of-file
      			if (has_digits) {
				return num;
			} else {
				return -1;
			}
    		}
    		if (mychar == ' ' || mychar == '\n' || mychar == ',') { break; }
    		num = 10 * num + (mychar - '0'); // ASCII 0, 1, 2, ... == '0', '1', '2'
  	}
  	// 'num' is the number read./
  	return num;
}

int main(int argc, char *argv[]) {
	if (argc!= 2) {
		return 1;
	}

	int is_fully_associative = 0;

	if (strcmp(argv[1], "-a") == 0) {
		is_fully_associative = 1;
	} else if (strcmp(argv[1], "-d") == 0) {
		is_fully_associative = 0;
	} else {
		return 1;
	}

	init_cache();

	if (is_fully_associative) {
		init_free_list();
		init_busy_list();
	}
	
	int address;
	while((address = read_address()) != -1) {
		access_cache(address, argv[1]);
	}
	print_cache_content(argv[1]);

	return 0;
}

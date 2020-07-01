#include "HashTable.h"

#include <stdlib.h>
#include <string.h>

#define MAX_LOAD_AVG 8

typedef struct node {
  char *key;
  void *data;
  struct node *link;
} Node;

struct hashtable {
  Node **table;
  long nEntries; // M
  long size; // N
};

static long hash (char *s, long size) {
    unsigned long sum;
    int shift;
    const unsigned long prime = 3141592653589793239L;

    for (sum = 0, shift = 0; *s; s++) {         // Compress string to long
        sum ^= *s<<shift;
        shift += 4;
        if (shift >= 57)
            shift -= 57;
    }
    sum += sum >> 33;                           // Condition the bits
    sum *= prime;
    sum += sum >> 33;

    return (sum % size);                        // Reduce to [0,SIZE)
    // return 0;
}

/*    initializes a hashtable with 'size' slots for linked lists.
      higher size means more time before dictionary has to resize due to high load average*/
void dictCreate(Dict *d, int size) {
  
  *d = malloc(sizeof(struct hashtable));

  (*d)->table = calloc(size, sizeof(Node *));

  (*d)->size = size;
  (*d)->nEntries = 0;
}

/*    inserts t into the hashtable, resolving collisions with chaining, and resizing if
      load average (= size/nEntries) exceeds MAX_LOAD_AVG */
void dictInsert(Dict *d, char *key, void *data) {
  Node **chain = &(((*d)->table)[hash(key, (*d)->size)]);

  Node newEntry = (Node) {key, data, NULL};

  if (*chain == NULL) {
    *chain = malloc(sizeof(Node));
    memcpy(*chain, &newEntry, sizeof(Node));
  }
  else {
    Node *tail = NULL;

    char replacedFlag = 0;
    for (Node *next = (*chain); next != NULL; next = next->link) {
      tail = next;

      if (strcmp(next->key, key) == 0) {
        // if key already exists, replace existing data with new data
        next->data = data;
        replacedFlag = 1;
        break;
      }
    }

    if (! replacedFlag) {
      tail->link = malloc(sizeof(Node));
      memcpy(tail->link, &newEntry, sizeof(Node));
    }
  }

  (*d)->nEntries += 1;

  // if load avg (N/M) exceeds MAX_LOAD_AVG, resize
  if ((*d)->nEntries > MAX_LOAD_AVG*((*d)->size)) {

    Dict new = NULL;
    dictCreate(&new, (*d)->size * MAX_LOAD_AVG);
    new->nEntries = (*d)->nEntries;

    for (int i = 0; i < (*d)->size; ++i) {
      for (Node *next = (*d)->table[i]; next != NULL; next = next->link) {
        dictInsert(&new, next->key, next->data);
        free(next);
      }
    }

    free((*d)->table);
    free((*d));

    *d = new;
  }
}

/* returns data pointer if key is in the hashtable, NULL otherwise */
void * dictLookup(Dict *d, char *key) {
  Node *chain = ((*d)->table)[hash(key, (*d)->size)];

  for (Node *next = chain; next != NULL; next = next->link) {
      if (strcmp(next->key, key) == 0) {
        return next->data;
      }
  }

  return NULL;
}

/* removes node at 'key' from hashtable, returning data pointer in case it still needs to be freed */
void * dictRemove(Dict *d, char *key) {
  Node **chain = &(((*d)->table)[hash(key, (*d)->size)]);
  
  for (Node *next = (*chain), *prev = NULL; next != NULL; prev = next, next = next->link) {
      if (strcmp(next->key, key) == 0) {
        if (prev != NULL) {
          prev->link = next->link;
        }
        else {
          *chain = next->link;
        }

        void *data = next->data;
        free(next);
        (*d)->nEntries = (*d)->nEntries - 1;
        return data;
      }
  }

  return NULL;
}

/* frees all nodes. Does not free keys nor data. */
void dictClear(Dict *d) {
  for (int i = 0; i < (*d)->size; ++i) {
    Node **chain = &((*d)->table[i]);
    
    if (*chain != NULL) {
      Node *prev = *chain;

      for (Node *next = ((*d)->table[i])->link; next != NULL; next = next->link) {
        prev = NULL;
        free(prev);
        prev = next;
      }
      free(prev);

      *chain = NULL;
    }
  }
}

/* returns array of all keys inside the hashtable */
char **dictKeys(Dict *d) {

  char **dictKeys = malloc(((*d)->nEntries)*sizeof(char *));
  long j = 0;

  for (int i = 0; i < (*d)->size; ++i) {
      for (Node *next = (*d)->table[i]; next != NULL; next = next->link) {
        dictKeys[j] = next->key;
        ++j;
      }
    }
  
  return dictKeys;
}

/* Returns nEntries, the number of items inserted into the array */
long dictLen(Dict *d) {
  return (*d)->nEntries;
}


/* frees all nodes, the table of nodes, and the hashtable. Does not free keys nor data. */
void dictDestroy(Dict *d) {

  dictClear(d);

  free((*d)->table);
  free((*d));
}


#ifdef TEST_HASHTABLE
#include <stdio.h>

/* Main function for testing purposes */
int main(int argc, char *argv[]) {

  Dict d = NULL;

  dictCreate(&d, 5);
  dictInsert(&d, "t1", "a");
  dictInsert(&d, "t2", "b");
  dictInsert(&d, "t3", "c");
  dictInsert(&d, "t4", "d");
  dictInsert(&d, "t5", "e");

  printf("%s\n",(char *) dictLookup(&d, "t1"));
  printf("%s\n",(char *) dictLookup(&d, "t2"));
  printf("%s\n",(char *) dictLookup(&d, "t3"));
  printf("%s\n",(char *) dictLookup(&d, "t4"));
  printf("%s\n",(char *) dictLookup(&d, "t5"));
  printf("%p\n", dictLookup(&d, "t6"));

  char **foo = dictKeys(&d);
  for (long i = 0, len = dictLen(&d); i < len; ++i) {
    printf("%s\n", foo[i]);
  }
  free(foo);
  
  dictRemove(&d, "t3");
  dictRemove(&d, "t2");
  dictRemove(&d, "t1");
  printf("------------------\n");

  char **bar = dictKeys(&d);
  for (long i = 0, len = dictLen(&d); i < len; ++i) {
    printf("%s\n", bar[i]);
  }
  free(bar);

  dictClear(&d);

  dictInsert(&d, "t1", "a");
  dictInsert(&d, "t2", "b");

  dictDestroy(&d);

}
#endif
#include "HashTable.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct node {
  char *key;
  void *data;
  struct node *link;
} Node;

struct hashtable {
  Node **table;
  int nEntries; // M
  int size; // N
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

// int cstruct_cmp(const void *a, const void *b) {
//   // comparison function for qsort, created with help from http://www.anyexample.com/programming/c/qsort__sorting_array_of_strings__integers_and_structs.xml
//     Pair *ia = (Pair *)a;
//     Pair *ib = (Pair *)b;
//     return strcmp(ia->pos, ib->pos);
// }

int main(int argc, char *argv[]) {
//   Dict d = NULL;

//   Pair t1 = {"hi", "no", 3};
//   Pair t2 = {"aa", "yo", 3};
//   Pair t3 = {"ab", "aye", 3};
//   Pair t4 = {"ad", "a", 4};
//   Pair t5 = {"ae", "a", 4};
//   Pair t6 = {"af", "a", 4};
//   Pair t7 = {"ag", "a", 4};
//   Pair t8 = {"ah", "a", 4};
//   Pair t9 = {"ai", "a", 4};
//   dictCreate(&d, 1);
//   dictInsert(&d, t1);
//   dictInsert(&d, t2);
//   dictInsert(&d, t3);
//   dictInsert(&d, t4);
//   dictInsert(&d, t5);
//   dictInsert(&d, t6);
//   dictInsert(&d, t7);
//   dictInsert(&d, t8);
//   dictInsert(&d, t9);

//   printf("%d\n",dictLookup(&d, t1));
//   printf("%d\n",dictLookup(&d, t2));
//   printf("%d\n",dictLookup(&d, t3));
//   printf("%d\n",dictLookup(&d, t4));
//   printf("%d\n",dictLookup(&d, t5));
//   printf("%d\n",dictLookup(&d, t6));
//   printf("%d\n",dictLookup(&d, t7));
//   printf("%d\n",dictLookup(&d, t8));
//   printf("%d\n",dictLookup(&d, t9));

//   // printf("%d\n", d->nEntries);

//   // for (int i = 0; i < d->nEntries; i++) {
//   //   Bucket currChain = d->table[i];
//   //   printf("%d\n",currChain.size);
//   // }

//   exit(1);
}


void dictCreate(Dict *d, int size) {
  // initializes a hashtable with size slots for linked lists
  *d = malloc(sizeof(struct hashtable));

  (*d)->table = calloc(size, sizeof(Node *));

  (*d)->size = size;
  (*d)->nEntries = 0;
}

void dictInsert(Dict *d, char *key, void *data) {
  // inserts t into the hashtable, resolving collisions with chaining, and resizing if
  // load average exceeds 8

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

  // if load avg (N/M) exceeds 8, resize
  if ((*d)->nEntries > 8*((*d)->size)) {

    Dict new = NULL;
    dictCreate(&new, (*d)->size * 8);
    new->nEntries = (*d)->nEntries;

    for (int i = 0; i < (*d)->size; ++i) {
      Node **currChain = &((*d)->table[i]);

      for (Node *next = (*currChain); next != NULL; next = next->link) {
        dictInsert(&new, next->key, next->data);
        free(next);
      }
    }

    free((*d)->table);
    free((*d));

    *d = new;
  }
}

void * dictLookup(Dict *d, char *key) {
  // returns data pointer if key is in the hashtable, NULL otherwise
  Node **chain = &(((*d)->table)[hash(key, (*d)->size)]);

  for (Node *next = (*chain); next != NULL; next = next->link) {
      if (strcmp(next->key, key) == 0) {
        return next->data;
      }
  }

  return NULL;
}


// void dictRemove(Dict *d, char *key) {
//   // removes key/value pair from hashtable

//   Node **chain = &(((*d)->table)[hash(key, (*d)->size)]);
  
//   for (Node *next = (*chain); next != NULL; next = next->link) {
//       if (strcmp(next->key, key) == 0) {
//         next->data = data;
//         break;
//       }
//   }

// }
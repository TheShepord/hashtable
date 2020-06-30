// Lucas Shepard, ljs83
#include "HashTable.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct bucket {
  Pair *entries;
  int size;
} Bucket;

struct hashtable {
  Bucket *table;
  int nEntries; // M
  int nBuckets; // N
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


void dictCreate(Dict *d, int nBuckets) {
  // initializes a hashtable with nBuckets buckets
  *d = malloc(sizeof(struct hashtable));

  (*d)->table = (Bucket *) calloc(nBuckets, sizeof(Bucket));

  (*d)->nBuckets = nBuckets;
  (*d)->nEntries = 0;
}

void dictInsert(Dict *d, Pair entry) {
  // inserts t into the hash table, resolving collisions with sorted chaining, and resizing if
  // load average exceeds 8

  Bucket *bucket = &(((*d)->table)[hash(entry.key, (*d)->nEntries)]);
  if (bucket->entries == NULL) {
    bucket->entries = malloc(sizeof(Pair));
    bucket->entries[0] = entry;
    bucket->size = 1;
  }
  else {
    bucket->size += 1;
    bucket->entries = realloc(bucket->entries, (bucket->size)*sizeof(Pair));
    bucket->entries[(bucket->size)-1] = entry;
  }

  (*d)->nEntries += 1;

  // if load avg (N/M) exceeds 8, resize
  if ((*d)->nEntries > 8*(*d)->nBuckets) {

    Dict new = NULL;
    dictCreate(&new, (*d)->nBuckets * 8);
    new->nEntries = (*d)->nEntries;

    for (int i = 0; i < (*d)->nBuckets; ++i) {
      Bucket currChain = (*d)->table[i];

      if (currChain.entries != NULL) {
        for (int j = 0; j < currChain.size; ++j) {
          dictInsert(&new, currChain.entries[j]);
        }
        free(currChain.entries);
      }
    }

    free((*d)->table);
    free((*d));

    *d = new;
  }
}

void *dictLookup(Dict *d, char key[]) {
  // returns Pair* if pos is in the hashtable, NULL otherwise
  Bucket chainArr = ((*d)->table)[hash(key, (*d)->nBuckets)];

  if (chainArr.entries == NULL) {
    return NULL;
  }

  // else {
    // int l = 0;
    // int h = chainArr.size-1;
    // int m = l + (h-l)/2;
    // int cmp;

    // while (l <= h) {
    //   m = l + (h-l)/2;
    //   // printf("%d\n",m);
    //   cmp = strcmp(key, chainArr.entries[m].pos);

    //   if (cmp > 0) {  // ignore left half
    //     l = m+1;
    //   }
    //   else if (cmp < 0) {  // ignore right half
    //     h = m-1;
    //   }
    //   else {
    //     return &chainArr.entries[m];
    //   }
    // }
    // return NULL;
}
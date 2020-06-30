// Lucas Shepard, ljs83

typedef struct pair {
  char *key;
  void *data;
} Pair;

typedef struct hashtable *Dict;

void dictCreate(Dict *d, int nBuckets);
void dictInsert(Dict *d, Pair t);
void *dictLookup(Dict *d, char key[]);
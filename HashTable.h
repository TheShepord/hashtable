typedef struct hashtable *Dict;

void dictCreate(Dict *d, int nLists);
void dictInsert(Dict *d, char *key, void *data);
void *dictLookup(Dict *d, char *key);
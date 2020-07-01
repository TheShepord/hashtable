typedef struct hashtable *Dict;

void dictCreate(Dict *d, int nLists);
void dictInsert(Dict *d, char *key, void *data);
void * dictLookup(Dict *d, char *key);
void * dictRemove(Dict *d, char *key);
char **dictKeys(Dict *d);
long dictLen(Dict *d);
void dictClear(Dict *d);
void dictDestroy(Dict *d);
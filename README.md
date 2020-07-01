# hashtable
[![Github license](https://img.shields.io/github/license/mashape/apistatus.svg?style=flat-square)](https://github.com/TheShepord/hashtable/blob/master/LICENSE)]

`Hashtable` is a feature-complete ADT using separate chaining with linked lists, written in pure C.

## Usage
```C
#include "HashTable.h"

int main (int argc, char *argv[]) {
    Dict hashtable = NULL;

    /* Choosing initial size of 10 */
    dictCreate(&hashtable, 10);

    dictInsert(&d, "t1", "a");
    dictInsert(&d, "t2", "b");
    dictInsert(&d, "t3", "c");

    printf("%s\n",(char *) dictLookup(&d, "t1"));
    printf("%s\n",(char *) dictLookup(&d, "t2"));
    printf("%s\n",(char *) dictLookup(&d, "t3"));
    printf("%p\n", dictLookup(&d, "FOOBAR"));

    char **keys = dictKeys(&d);
    for (long i = 0, len = dictLen(&d); i < len; ++i) {
        printf("%s\n", keys[i]);
    }
    free(keys);

    dictRemove(&d, "t3");
    dictRemove(&d, "t2");

    char **newKeys = dictKeys(&d);
    for (long i = 0, len = dictLen(&d); i < len; ++i) {
        printf("%s\n", newKeys[i]);
    }
    free(newKeys);

    dictClear(&d);

    dictInsert(&d, "foo", "bar");
    dictInsert(&d, "dead", "beef");

    dictDestroy(&d);
}
```

## License

This project is released under the [MIT License](https://github.com/TheShepord/hashtable/blob/master/LICENSE).
#include <string.h>
#include <sqlite3ext.h>

SQLITE_EXTENSION_INIT1

static int cmp(void *_, int n1, const void *p1, int n2, const void *p2) {
    if (!p1 || !p2 || n1 < 0 || n2 < 0) {
        return 0;
    }

    const char* s1 = (const char*)p1;
    const char* s2 = (const char*)p2;
    const int len = (n1 < n2) ? n1 : n2;

    for (int i = 0; i < len; ++i) {
        if (s1[i] != s2[i]) {
            return (s1[i] > s2[i]) ? 1 : -1;
        }
    }

    return n1 - n2;
}

static void create(sqlite3_api_routines *pRoutines, sqlite3 *db,
                   void *NotUsed, int textRep, const char *coll) {

    if (!db || !coll || !pRoutines) {
        return;
    }

    pRoutines->sqlite3_create_collation(db, coll, textRep, NULL, cmp);
}

int sqlite3_anycollseq_init(sqlite3 *db, char **pzErrMsg, 
                            const sqlite3_api_routines *pRoutines) {

    if (!db || !pRoutines) {
        return SQLITE_ERROR;
    }

    SQLITE_EXTENSION_INIT2(pRoutines);
    pRoutines->sqlite3_collation_needed(db, NULL, create);

    return SQLITE_OK;
}
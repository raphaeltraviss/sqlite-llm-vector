#include <string.h>
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1

static int cmp(void *_, int n1, const void *p1, int n2, const void *p2) {
    int rc;
    const int n = (n1 < n2 ? n1 : n2);
    for (int i = 0; i < n; ++i) {
        if (((const char *)p1)[i] != ((const char *)p2)[i]) {
            rc = ((const char *)p1)[i] - ((const char *)p2)[i];
            return (rc > 0 ? 1 : (rc < 0 ? -1 : n1 - n2));
        }
    }
   return (n1 - n2); 
}

static void create(sqlite3_api_routines *pRoutines, sqlite3 *db,
                   void *NotUsed, int textRep, const char *coll) {
    (void)NotUsed;
    pRoutines->sqlite3_create_collation(db, coll, textRep, NULL, cmp);
}

int sqlite3_anycollseq_init(sqlite3 *db, char **pzErrMsg, 
                            const sqlite3_api_routines *pRoutines) {
    (void)pzErrMsg;
    SQLITE_EXTENSION_INIT2(pRoutines);
    pRoutines->sqlite3_collation_needed(db, NULL, create);
    return SQLITE_OK;
}
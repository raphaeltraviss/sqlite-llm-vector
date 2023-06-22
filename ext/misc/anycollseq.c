#include <string.h>
#include <sqlite3ext.h>

SQLITE_EXTENSION_INIT1

static int cmp(const void *p1, int n1, const void *p2, int n2) {
    if (!p1 || !p2 || n1 < 0 || n2 < 0) {
        return 0;
    }

    const unsigned char* s1 = (const unsigned char*)p1;
    const unsigned char* s2 = (const unsigned char*)p2;
    const int len = (n1 < n2) ? n1 : n2;

    for (int i = 0; i < len; ++i) {
        if (s1[i] != s2[i]) {
            return (s1[i] > s2[i]) ? 1 : -1;
        }
    }

    return n1 - n2;
}

// Function to create collation
static void create_collation(void *ctx, int textRep, const char *coll, void *pAux) {
    sqlite3* db = (sqlite3*)ctx;
    // Check if db and coll are not null and textRep is valid.
    if (db && coll && textRep) {
        // Create the collation
        sqlite3_create_collation(db, coll, textRep, NULL, cmp);
    }
}

// Function to initialize any collation sequence.
int sqlite3_anycollseq_init(sqlite3 *db, char **pzErrMsg, 
                            const sqlite3_api_routines *pRoutines) {

    // Check if db and pRoutines are not null.
    if (!db || !pRoutines) {
        return SQLITE_ERROR;
    }

    SQLITE_EXTENSION_INIT2(pRoutines);
    int err;
    // Try to create the collation
    err = sqlite3_collation_needed(db, (void *)db, create_collation);
    if (err != SQLITE_OK) {
        // Set the error message if collation creation fails
        *pzErrMsg = sqlite3_mprintf("Collation creation failed: %s", 
            sqlite3_errstr(err));
        return err;
    }

    return SQLITE_OK;
}
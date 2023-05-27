/* File: sqlite_noop.c */

/*
** This is an SQLite extension to implement noop() function used for testing.
**
** Variants:
**
**    noop(X)           The default.  Deterministic.
**    noop_i(X)         Deterministic and innocuous.
**    noop_do(X)        Deterministic and direct-only.
**    noop_nd(X)        Non-deterministic.
*/

#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1

#include <assert.h>
#include <string.h>

/* Implementation of the noop function. Returns the argument of this function */
static void noopfunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    assert(argc == 1);
    sqlite3_result_value(context, argv[0]);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
/* Initialize the noop functions in SQLite */
int sqlite3_noop_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);

    (void)pzErrMsg;  /* Unused parameter */

    rc = sqlite3_create_function(
        db, "noop", 1, SQLITE_UTF8 | SQLITE_DETERMINISTIC, 0, noopfunc, 0, 0
    );
    if( rc != SQLITE_OK ) return rc;

    rc = sqlite3_create_function(
        db, "noop_i", 1,
        SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS, 0, noopfunc, 0, 0
    );
    if( rc != SQLITE_OK ) return rc;

    rc = sqlite3_create_function(
        db, "noop_do", 1,
        SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY, 0, noopfunc, 0, 0
    );
    if( rc != SQLITE_OK ) return rc;

    rc = sqlite3_create_function(
        db, "noop_nd", 1, SQLITE_UTF8, 0, noopfunc, 0, 0
    );
    return rc;
} 

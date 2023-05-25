/*
** 2016-08-09
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
*/

#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1

#include <assert.h>

/**
 * Remember function
 * 
 * Return the integer value V. Also, save the value of V in a
 * C-language variable whose address is PTR.
 */
static void rememberFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    sqlite3_int64 v;
    sqlite3_int64 *ptr;
    assert(argc == 2);

    v = sqlite3_value_int64(argv[0]);
    ptr = sqlite3_value_pointer(argv[1], "carray");

    if (ptr) {
        *ptr = v;
    }

    sqlite3_result_int64(ctx, v);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_remember_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    SQLITE_EXTENSION_INIT2(pApi);

    int rc = sqlite3_create_function(db, "remember", 2, SQLITE_UTF8, 0, rememberFunc, 0, 0);

    return rc;
}
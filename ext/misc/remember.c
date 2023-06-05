#include <sqlite3.h>
#include <assert.h>

/**
 * Remember function
 * 
 * Return the integer value V. Also, save the value of V in a
 * C-language variable whose address is PTR.
 */
static void remember_func(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    sqlite3_int64 *ptr;
    sqlite3_int64 v;

    assert(argc == 2);

    v = sqlite3_value_int64(argv[0]);
    ptr = sqlite3_value_pointer(argv[1], "carray");

    if (ptr)
    {
        *ptr = v;
    }

    sqlite3_result_int64(ctx, v);
}

int sqlite3_remember_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int rc = SQLITE_ERROR;

    SQLITE_EXTENSION_INIT2(pApi);

    rc = sqlite3_create_function(db, "remember", 2, SQLITE_UTF8, 0, remember_func, 0, 0);

    return rc;
}
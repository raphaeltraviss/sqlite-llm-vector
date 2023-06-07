#include <sqlite3.h>
#include <assert.h>
#include <stdlib.h>

typedef struct {
    sqlite3_int64 value;
} ClientData;

/*
 * Remember function
 * Return the integer value V. Also, save the value of V in a
 * C-language variable whose address is PTR.
 */
static void remember_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    assert(argc == 2);
    
    sqlite3_int64 *ptr = sqlite3_value_pointer(argv[1], "carray");
    if (!ptr) {
        sqlite3_result_error(ctx, "Failed to retrieve pointer value", -1);
        return;
    }
    ClientData *data = sqlite3_user_data(ctx);
    sqlite3_int64 v = sqlite3_value_int64(argv[0]);
    *ptr = v;
    data->value = v;
    sqlite3_result_int64(ctx, v);
}

int sqlite3_remember_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi) {
    int rc = SQLITE_ERROR;
    sqlite3_create_function_v2(db, "remember", 2, SQLITE_UTF8, NULL, &remember_func, NULL, NULL, &sqlite3_free);
    return SQLITE_OK;
}
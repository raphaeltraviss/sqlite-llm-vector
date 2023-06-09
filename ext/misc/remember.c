#include <sqlite3.h>
#include <assert.h>
#include <stdlib.h>

typedef struct {
    sqlite3_int64 value;
} ClientData;

static void remember_func(sqlite3_context *, int, sqlite3_value **);
static int sqlite3_remember_init(sqlite3 *, char **, const sqlite3_api_routines *);

static void remember_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    assert(argc == 2);
    
    sqlite3_int64 *ptr = sqlite3_value_pointer(argv[1], "carray");
    if (!ptr) {
        sqlite3_result_error(ctx, "Failed to retrieve pointer value", -1);
        return;
    }
    ClientData *data = (ClientData*)sqlite3_user_data(ctx);
    sqlite3_int64 v = sqlite3_value_int64(argv[0]);
    *ptr = v;
    data->value = v;
    sqlite3_result_int64(ctx, v);
}

static int sqlite3_remember_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi) {
    int rc = SQLITE_ERROR;
    rc = sqlite3_create_function_v2(db, "remember", 2, SQLITE_UTF8, NULL, &remember_func, NULL, NULL, &free);
    return rc;
}

int sqlite3_extension_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi) {
    int rc = SQLITE_ERROR;
    ClientData* clientData = (ClientData*)malloc(sizeof(ClientData));
    if (!clientData) {
        return SQLITE_ERROR;
    }
    clientData->value = 0;
    rc = sqlite3_remember_init(db, pzErrMsg, pApi);
    if (rc != SQLITE_OK) {
        sqlite3_free(*pzErrMsg);
        free(clientData);
        return rc;
    }
    sqlite3_create_function_v2(db, "clientData",0, SQLITE_UTF8, clientData,&clientDataFunc, NULL,NULL, &free);
    return rc;
}
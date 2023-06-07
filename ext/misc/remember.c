#include <sqlite3.h>
#include <assert.h>

/**
 * Remember function
 *
 * Return the integer value V. Also, save the value of V in a
 * C-language variable whose address is PTR.
 */
static void remember_func(sqlite3_context *ctx, int argc, sqlite3_value **argv);

int sqlite3_remember_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

typedef struct{
    sqlite3_int64* value;
}ClientData;

typedef struct{
    ClientData* clientData;
    int count;
}FunctionData;

static void remember_func(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    assert(argc == 2);
    sqlite3_int64 *ptr = sqlite3_value_pointer(argv[1], "carray");
    if (!ptr){
        sqlite3_result_error(ctx, "Failed to retrieve pointer value", -1);
        return;
    }
    ClientData* data = sqlite3_user_data(ctx);
    sqlite3_int64 v = sqlite3_value_int64(argv[0]);
    *ptr = v;
    *data->value = v;
    sqlite3_result_int64(ctx, v);
}

int sqlite3_remember_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int rc = SQLITE_ERROR;
    SQLITE_EXTENSION_INIT2(pApi);
    FunctionData* funcData = malloc(sizeof(FunctionData));
    if (!funcData){
        sqlite3_result_error(db, "Malloc for FunctionData failed", -1);
        return SQLITE_ERROR;
    }
    ClientData* clientData = malloc(sizeof(ClientData));
    if (!clientData){
        sqlite3_result_error(db, "Malloc for ClientData failed", -1);
        return SQLITE_ERROR;
    }
    funcData->clientData = clientData;
    funcData->count = 0;
    clientData->value = malloc(sizeof(sqlite3_int64));
    if (!clientData->value){
        sqlite3_result_error(db, "Malloc for sqlite3_int64 failed", -1);
        return SQLITE_ERROR;
    }
    *clientData->value = 0;
    rc = sqlite3_create_function(db, "remember", 2, SQLITE_UTF8, clientData, remember_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_result_error(db, "Failed to create function", -1);
        return SQLITE_ERROR;
    }
    return SQLITE_OK;
}
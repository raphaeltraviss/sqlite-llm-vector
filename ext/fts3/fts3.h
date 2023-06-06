#include <string.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "fts3.h"

typedef struct Module {
    const char* name;
    const sqlite3_module* module;
} Module;

static const Module aModule[] = {
    {"fts3",  &fts3Module},
    {"fts4",  &fts4Module},
    {"fts5",  &fts5Module},
    {NULL, NULL},
};

static const char* zStmt = "SELECT fts3_tokenizer(?, ?)";

int sqlite3Fts3Init(sqlite3* db) {
    int rc;
    rc = sqlite3_create_module_v2(db, "fts3", &fts3Module, NULL, NULL, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }
    if (db->mallocFailed) {
        return SQLITE_NOMEM;
    }
    rc = sqlite3_create_function(db, "fts3_tokenizer", -1, SQLITE_UTF8 | SQLITE_DETERMINISTIC, 0,
                                 fts3TokenizerSqlFunc, 0, 0);
    if (rc != SQLITE_OK) {
        return rc;
    }
    for (const Module* module = aModule; module->name; module++) {
        rc = sqlite3_create_module(db, module->name, module->module, 0);
        if (rc != SQLITE_OK) {
            return rc;
        }
    }
    rc = sqlite3_prepare_v2(db, zStmt, -1, &(db->pTokenizer), 0);
    return rc;
}
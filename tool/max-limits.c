#include "sqlite3.h"
#include <stdio.h>

typedef struct {
    int limitCode;
    const char *limitName;
} LimitData;

static const LimitData limitData[] = {
    { SQLITE_LIMIT_LENGTH, "SQLITE_MAX_LENGTH" },
    { SQLITE_LIMIT_SQL_LENGTH, "SQLITE_MAX_SQL_LENGTH" },
    { SQLITE_LIMIT_COLUMN, "SQLITE_MAX_COLUMN" },
    { SQLITE_LIMIT_EXPR_DEPTH, "SQLITE_MAX_EXPR_DEPTH" },
    { SQLITE_LIMIT_COMPOUND_SELECT, "SQLITE_MAX_COMPOUND_SELECT" },
    { SQLITE_LIMIT_VDBE_OP, "SQLITE_MAX_VDBE_OP" },
    { SQLITE_LIMIT_FUNCTION_ARG, "SQLITE_MAX_FUNCTION_ARG" },
    { SQLITE_LIMIT_ATTACHED, "SQLITE_MAX_ATTACHED" },
    { SQLITE_LIMIT_LIKE_PATTERN_LENGTH, "SQLITE_MAX_LIKE_PATTERN_LENGTH" },
    { SQLITE_LIMIT_VARIABLE_NUMBER, "SQLITE_MAX_VARIABLE_NUMBER" },
    { SQLITE_LIMIT_TRIGGER_DEPTH, "SQLITE_MAX_TRIGGER_DEPTH" },
    { SQLITE_LIMIT_WORKER_THREADS, "SQLITE_MAX_WORKER_THREADS" }
};

static int getMaxLimit(sqlite3 *db, int limitCode) {
    const int initialLimit = sqlite3_limit(db, limitCode, 0x7FFFFFFF);
    return sqlite3_limit(db, limitCode, initialLimit);
}

static void printLimits(sqlite3 *db) {
    const size_t count = sizeof(limitData) / sizeof(LimitData);

    for (size_t i = 0; i < count; ++i) {
        const LimitData *limit = &limitData[i];
        printf("%-35s %10d\n", limit->limitName, getMaxLimit(db, limit->limitCode));
    }
}

int main(void) {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    printLimits(db);
    sqlite3_close(db);
    return 0;
}
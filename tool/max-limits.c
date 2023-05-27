#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

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
#define LIMIT_DATA_SIZE (sizeof(limitData) / sizeof(LimitData))

static const char *getErrorMsg(int err) {
    switch (err) {
        case SQLITE_OK: {
            return "Successful result";
        }
        case SQLITE_ERROR: {
            return "SQL error or missing database";
        }
        case SQLITE_INTERNAL: {
            return "Internal logic error in SQLite";
        }
        case SQLITE_PERM: {
            return "Access permission denied";
        }
        case SQLITE_ABORT: {
            return "Callback routine requested an abort";
        }
        case SQLITE_BUSY: {
            return "The database file is locked";
        }
        case SQLITE_LOCKED: {
            return "A table in the database is locked";
        }
        case SQLITE_NOMEM: {
            return "A malloc() failed";
        }
        case SQLITE_READONLY: {
            return "Attempt to write a readonly database";
        }
        case SQLITE_INTERRUPT: {
            return "Operation terminated by sqlite3_interrupt()";
        }
        case SQLITE_IOERR: {
            return "Some kind of disk I/O error occurred";
        }
        case SQLITE_CORRUPT: {
            return "The database disk image is malformed";
        }
        case SQLITE_NOTFOUND: {
            return "Unknown operation";
        }
        case SQLITE_FULL: {
            return "Insertion failed because database is full";
        }
        case SQLITE_CANTOPEN: {
            return "Unable to open the database file";
        }
        case SQLITE_PROTOCOL: {
            return "Database lock protocol error";
        }
        case SQLITE_EMPTY: {
            return "Database is empty";
        }
        case SQLITE_SCHEMA: {
            return "The database schema changed";
        }
        case SQLITE_CONSTRAINT: {
            return "Abort due to constraint violation";
        }
        case SQLITE_MISMATCH: {
            return "Data type mismatch";
        }
        case SQLITE_MISUSE: {
            return "Library used incorrectly";
        }
        case SQLITE_NOLFS: {
            return "Large file support is not available";
        }
        case SQLITE_AUTH: {
            return "Authorization denied";
        }
        case SQLITE_FORMAT: {
            return "Auxiliary database format error";
        }
        case SQLITE_RANGE: {
            return "Abort due to out-of-range input values";
        }
        case SQLITE_NOTADB: {
            return "File opened that is not a database file";
        }
        case SQLITE_NOTICE: {
            return "Notifications from sqlite3_log()";
        }
        case SQLITE_WARNING: {
            return "Warnings from sqlite3_log()";
        }
        default: {
            return "Unknown error code";
        }
    }
}

static void printError(int code) {
    fprintf(stderr, "SQLite Error: %s\n", getErrorMsg(code));
}

static void handleError(sqlite3 *db, int res) {
    if (res != SQLITE_OK) {
        printError(res);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
}

static int executeWithPrint(sqlite3 *db, const char *sql) {
    char *msg;

    const int res = sqlite3_exec(db, sql, NULL, NULL, &msg);

    if (res != SQLITE_OK) {
        fprintf(stderr, "SQLite Error: %s\n", msg);
        sqlite3_free(msg);
        return res;
    }

    return SQLITE_OK;
}

static int getMaxLimit(sqlite3 *db, int limitCode) {
    const int initialLimit = sqlite3_limit(db, limitCode, 0x7FFFFFFF);
    return sqlite3_limit(db, limitCode, initialLimit);
}

static void printLimits(sqlite3 *db) {
    for (int i = 0; i < LIMIT_DATA_SIZE; ++i) {
        const LimitData *limit = &limitData[i];
        printf("%-35s %10d\n", limit->limitName, getMaxLimit(db, limit->limitCode));
    }
}

int main(void) {
    sqlite3 *db;

    const int res = sqlite3_open(":memory:", &db);
    handleError(db, res);

    printLimits(db);

    res = sqlite3_close(db);
    handleError(db, res);

    return EXIT_SUCCESS;
}
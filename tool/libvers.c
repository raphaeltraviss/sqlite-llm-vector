#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#define MAX_LIBRARY_VERSION_LENGTH 256
#define MAX_SOURCE_ID_LENGTH 256

static int init_sqlite(sqlite3 **db) {
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize SQLite library: %s\n", sqlite3_errstr(rc)); 
        return rc;
    }

    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create database connection: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    
    return SQLITE_OK;
}

static int shutdown_sqlite(sqlite3 *db) {
    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to close SQLite library: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    rc = sqlite3_shutdown();
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to shutdown SQLite library: %s\n", sqlite3_errstr(rc)); 
        return rc;
    }
    return SQLITE_OK;
}

static void print_library_version_and_source_id() {
    char version[MAX_LIBRARY_VERSION_LENGTH], source[MAX_SOURCE_ID_LENGTH];

    const char *lib_version = sqlite3_libversion();
    if (lib_version == NULL) {
        fprintf(stderr, "Failed to retrieve SQLite library version\n");
        return;
    }

    const char *source_id = sqlite3_sourceid();
    if (source_id == NULL) {
        fprintf(stderr, "Failed to retrieve SQLite library source ID\n");
        return;
    }

    snprintf(version, MAX_LIBRARY_VERSION_LENGTH, "%s", lib_version);
    snprintf(source, MAX_SOURCE_ID_LENGTH, "%s", source_id);

    printf("SQLite version %s\n", version);
    printf("SQLite source  %s\n", source);
}

int main(int argc, char **argv) {
    int rc = EXIT_SUCCESS;
    sqlite3 *db = NULL;

    rc = init_sqlite(&db);
    if (rc != SQLITE_OK) {
        goto cleanup;
    }

    print_library_version_and_source_id();

cleanup:
    rc = shutdown_sqlite(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to shutdown SQLite library: %s\n", sqlite3_errstr(rc)); 
        return EXIT_FAILURE;
    }

    return rc == SQLITE_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
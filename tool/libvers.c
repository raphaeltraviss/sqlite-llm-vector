#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_LIBRARY_VERSION_LENGTH 256
#define MAX_SOURCE_ID_LENGTH 256

typedef struct {
    sqlite3 *db;
} Database;

static int init_sqlite(Database *db) {
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize SQLite library: %s\n", sqlite3_errstr(rc));
        return rc;
    }

    rc = sqlite3_open(":memory:", &(db->db));
    if (rc != SQLITE_OK) {
        sqlite3_shutdown();
        fprintf(stderr, "Failed to create database connection: %s\n", sqlite3_errmsg(db->db));
        return rc;
    }

    return SQLITE_OK;
}

static int shutdown_sqlite(Database *db) {
    int rc = sqlite3_close(db->db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to close SQLite library: %s\n", sqlite3_errmsg(db->db));
        return rc;
    }

    rc = sqlite3_shutdown();
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to shutdown SQLite library: %s\n", sqlite3_errstr(rc));
        return rc;
    }
    return SQLITE_OK;
}

static char* safe_copy_string(const char* str) {
    size_t length = strlen(str);
    char* copy = malloc(length + 1);
    if (copy != NULL) {
        memcpy(copy, str, length + 1);
    }
    return copy;
}

static void print_library_version_and_source_id() {
    char version[MAX_LIBRARY_VERSION_LENGTH];
    char source[MAX_SOURCE_ID_LENGTH];

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

    char *version_copy = safe_copy_string(lib_version);
    char *source_copy = safe_copy_string(source_id);

    if (version_copy == NULL || source_copy == NULL) {
        fprintf(stderr, "Memory allocation failed");
        free(version_copy);
        free(source_copy);
        return;
    }

    strncpy(version, version_copy, MAX_LIBRARY_VERSION_LENGTH);
    strncpy(source, source_copy, MAX_SOURCE_ID_LENGTH);

    printf("SQLite version: %s\n", version);
    printf("SQLite source: %s\n", source);

    free(version_copy);
    free(source_copy);
}

int main(int argc, char **argv) {
    int rc = EXIT_SUCCESS;
    Database db;

    rc = init_sqlite(&db);
    if (rc != SQLITE_OK) {
        goto cleanup;
    }

    print_library_version_and_source_id();

cleanup:
    rc = shutdown_sqlite(&db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to shutdown SQLite library: %s\n", sqlite3_errstr(rc));
        return EXIT_FAILURE;
    }

    return rc == SQLITE_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
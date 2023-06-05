#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

// Declare global pointers
const char *version = NULL, *source = NULL;

int init_sqlite() {
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize SQLite library: %s\n", sqlite3_errstr(rc)); 
        return rc;
    }
    return SQLITE_OK;
}

int shutdown_sqlite() {
    int rc = sqlite3_shutdown();
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to shutdown SQLite library: %s\n", sqlite3_errstr(rc)); 
        return rc;
    }
    return SQLITE_OK;
}

int get_version() {
    version = sqlite3_libversion();
    if (version == NULL) {
        fprintf(stderr, "Failed to retrieve SQLite library version\n");
        return SQLITE_ERROR;
    }
    return SQLITE_OK;
}

int get_source() {
    source = sqlite3_sourceid();
    if (source == NULL) {
        fprintf(stderr, "Failed to retrieve SQLite library source ID\n");
        return SQLITE_ERROR;
    }
    return SQLITE_OK;
}

int main(int argc, char **argv){

    int rc = EXIT_SUCCESS;

    rc = init_sqlite();
    if (rc != SQLITE_OK) {
        goto cleanup;
    }

    rc = get_version();
    if (rc != SQLITE_OK) {
        goto cleanup;
    }

    rc = get_source();
    if (rc != SQLITE_OK) {
        goto cleanup;
    }

    printf("SQLite version %s\n", version);
    printf("SQLite source  %s\n", source);

cleanup:
    rc = shutdown_sqlite();
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to shutdown SQLite library: %s\n", sqlite3_errstr(rc)); 
        return EXIT_FAILURE;
    }

    return rc == SQLITE_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
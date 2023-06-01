#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

// Declare global pointers
const char *version, *source;

int init_sqlite() {
    if (sqlite3_initialize() != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize SQLite library\n"); 
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int shutdown_sqlite() {
    if (sqlite3_shutdown() != SQLITE_OK) {
        fprintf(stderr, "Failed to shutdown SQLite library\n"); 
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void get_version() {
    version = sqlite3_libversion();
}

void get_source() {
    source = sqlite3_sourceid();
}

int main(int argc, char **argv){

    if(init_sqlite() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    get_version();

    get_source();

    printf("SQLite version %s\n", version);
    printf("SQLite source  %s\n", source);

    if(shutdown_sqlite() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
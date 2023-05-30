#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h> // Added header for exit() function

//error handling function
static inline void handle_errors (const char * message, int result) {
    if(result != SQLITE_OK) {
        fprintf(stderr,"%s: %s(): %s\n", __FILE__, __func__, message);
        exit(EXIT_FAILURE);
    }
}

int sqlite3_wasm_extra_init(const char *z){
    fprintf(stderr,"%s: %s()\n", __FILE__, __func__);
    return 0; 
}

int main() {
    //initialize database connection
    sqlite3 *db = NULL; // Set the database pointer to null
    char *sql = NULL; // Set the SQL string pointer to null
    int result = sqlite3_open_v2("test.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    handle_errors("Failed to initialize database connection.", result);
    sqlite3_busy_timeout(db, 5000); // Set a busy timeout to avoid busy operations

    //create table
    sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";
    result = sqlite3_exec(db, sql, NULL, NULL, NULL);
    handle_errors("Failed to create table.", result);

    //insert data
    sql = "INSERT INTO users (name, age) VALUES ('Alice', 25), ('Bob', 30);";
    result = sqlite3_exec(db, sql, NULL, NULL, NULL);
    handle_errors("Failed to insert data.", result);

    //query data
    sql = "SELECT * FROM users;";
    result = sqlite3_exec(db, sql, NULL, NULL, NULL);
    handle_errors("Failed to query data.", result);

    // Free the sql pointer
    sqlite3_free(sql);
    sql = NULL;

    sqlite3_close_v2(db); // Use the sqlite3_close_v2 function to release resources efficiently
    db = NULL;

    return EXIT_SUCCESS;
}
#include "sqlite3.h"
#include <stdio.h>

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
    sqlite3 *db;
    int result = sqlite3_open("test.db", &db);
    handle_errors("Failed to initialize database connection.", result);

    //create table
    char *sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";
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

    sqlite3_close(db);
    return EXIT_SUCCESS;
}
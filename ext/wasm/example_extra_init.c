/**
* FILE: sqlite_example.c
* AUTHOR: Your Name (your_email@example.com)
* REFERENCE: https://www.sqlite.org/cintro.html
* VERSION: 1.0
* DATE: 2022-06-21
* DESCRIPTION: Demonstrates creating a table, inserting data, and querying it using SQLite library.
*/

#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

// Function signature to handle errors
static inline void handle_errors(const char *message, int result);

int sqlite3_wasm_extra_init(const char *z);

int main(void) {

    // Connect to database
    sqlite3 *db = NULL; // Set the database pointer to null
    char *sql = NULL; // Set the SQL string pointer to null
    int result = sqlite3_open_v2("test.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); // Initialize db connection
    handle_errors("Failed to initialize database connection.", result);
    sqlite3_busy_timeout(db, 5000); // Set a busy timeout to avoid busy operations

    // Create table
    sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";
    result = sqlite3_exec(db, sql, NULL, NULL, NULL);
    handle_errors("Failed to create table.", result);

    // Insert data
    sql = "INSERT INTO users (name, age) VALUES ('Alice', 25), ('Bob', 30);";
    result = sqlite3_exec(db, sql, NULL, NULL, NULL);
    handle_errors("Failed to insert data.", result);

    // Query data
    sql = "SELECT * FROM users;";
    result = sqlite3_exec(db, sql, NULL, NULL, NULL);
    handle_errors("Failed to query data.", result);

    // Free the sql pointer
    sqlite3_free(sql);
    sql = NULL;

    // Release resources
    sqlite3_close_v2(db); // Use the sqlite3_close_v2 function to release resources efficiently
    db = NULL;

    return EXIT_SUCCESS;
}

/**
* handle_errors - A function that handles errors more effectively.
* @message: The error message to print.
* @result: The result of the executed query.
* 
* This function checks if the result of the SQLite3 library function call was successful or not. If the function failed, this function
* uses fprintf to print the file name, function name, and error message to stderr. It also exits the program with a status code of 
* EXIT_FAILURE.
* 
* Return: Nothing.
*/
static inline void handle_errors(const char *message, int result) {
    if (result != SQLITE_OK) {
        fprintf(stderr, "%s: %s(): %s\n", __FILE__, __func__, message);
        exit(EXIT_FAILURE); // Exit program with failure status
    }
}

/**
* sqlite3_wasm_extra_init - Initializes SQLite when running on Web Assembly (WASM).
* @z: A string of initialization options.
*
* This function is required to be defined when SQLite is compiled for use with Web Assembly (WASM). If SQLite is not compiled
* for use with WASM, this function will be a no-op.
*
* Return: Always 0.
*/
int sqlite3_wasm_extra_init(const char *z) {
    fprintf(stderr, "%s: %s()\n", __FILE__, __func__);
    return 0; 
}
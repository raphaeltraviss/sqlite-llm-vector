/* 
** This program uses SQLite functions to print out the SQLite 
** version and source information.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(int argc, char **argv){

  // Initialize SQLite library
  if (sqlite3_initialize() != SQLITE_OK) {
    fprintf(stderr, "Failed to initialize SQLite library\n");
    return EXIT_FAILURE;
  }

  // Declare pointers for SQLite version and source information
  const char *version, *source;

  // Get SQLite version information
  version = sqlite3_libversion();

  // Get SQLite source information
  source = sqlite3_sourceid();

  // Print SQLite version and source information
  printf("SQLite version %s\n", version);
  printf("SQLite source  %s\n", source);

  // Shutdown SQLite library
  if (sqlite3_shutdown() != SQLITE_OK) {
    fprintf(stderr, "Failed to shutdown SQLite library\n");
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
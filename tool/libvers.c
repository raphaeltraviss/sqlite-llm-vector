#define DECLARE_PTRS const char *version, *source;
#define INIT_SQLLIB(p) if (sqlite3_initialize() != SQLITE_OK) {\
                              fprintf(stderr, "Failed to initialize SQLite library\n"); \
                              return p; \
                          }

#define SHUTDOWN_LIB(p) if (sqlite3_shutdown() != SQLITE_OK) {\
                              fprintf(stderr, "Failed to shutdown SQLite library\n"); \
                              return p; \
                          }

#define GET_VERSION version = sqlite3_libversion();
#define GET_SOURCE  source = sqlite3_sourceid();

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>


int main(int argc, char **argv){

  DECLARE_PTRS

  INIT_SQLLIB(EXIT_FAILURE)

  
  GET_VERSION

  
  GET_SOURCE

  
  printf("SQLite version %s\n", version);
  printf("SQLite source  %s\n", source);

  
  SHUTDOWN_LIB(EXIT_FAILURE)
  
  return EXIT_SUCCESS;
}
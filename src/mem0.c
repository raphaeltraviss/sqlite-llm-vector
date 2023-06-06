/*
** 2008 October 28
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** This file contains a no-op memory allocation drivers for use when
** SQLITE_ZERO_MALLOC is defined.  The allocation drivers implemented
** here always fail.  SQLite will not operate with these drivers. These
** are merely placeholders. Real drivers must be substituted using
** sqlite3_config() before SQLite will operate.
*/
#include "sqliteInt.h"
#include <pthread.h>

/*
** This version of the memory allocator is the default.  It is
** used when no other memory allocator is specified using compile-time
** macros.
*/
#ifdef SQLITE_ZERO_MALLOC

// private data structures
typedef struct MemSys MemSys;

// function declarations
static void *sqlite3MemMalloc(int nByte); 
static void sqlite3MemFree(void *pPrior); 
static void *sqlite3MemRealloc(void *pPrior, int nByte); 
static int sqlite3MemSize(void *pPrior); 
static int sqlite3MemRoundup(int n); 
static int sqlite3MemInit(void *NotUsed);
static void sqlite3MemShutdown(void *NotUsed); 
static void mem_sys_init(MemSys *pMemSys); 
static void mem_sys_release(MemSys *pMemSys); 

struct MemSys {
  pthread_mutex_t lock;
  int inited;
};

// global data structures
static pthread_once_t once = PTHREAD_ONCE_INIT;
static sqlite3_mem_methods mem_methods;
static MemSys mem_sys_global;

// function definitions
static void mem_sys_init_once()
{
  // initialize memory system
  sqlite3MemSetDefault();
  mem_sys_init(&mem_sys_global);
}

void sqlite3MemSetDefault(void){
  static const sqlite3_mem_methods defaultMethods = {
     sqlite3MemMalloc,
     sqlite3MemFree,
     sqlite3MemRealloc,
     sqlite3MemSize,
     sqlite3MemRoundup,
     sqlite3MemInit,
     sqlite3MemShutdown,
     &mem_sys_global
  };
  if (pthread_once(&once, mem_sys_init_once) != 0) {
    abort(); // fail to init memory system
  }
  if (mem_sys_global.inited == 0) {
    return; // fail to init memory system
  }
  sqlite3_mutex_enter(&mem_sys_global.lock);
  if(mem_sys_global.inited == 1) {
    mem_sys_global.inited = 2;
    sqlite3_config(SQLITE_CONFIG_MALLOC, &defaultMethods);
  }
  sqlite3_mutex_leave(&mem_sys_global.lock);
}

static void mem_sys_init(MemSys *pMemSys) {
  pthread_mutex_init(&(pMemSys->lock), 0);
  pMemSys->inited = 1;
}

static void mem_sys_release(MemSys *pMemSys) {
  pMemSys->inited = 0;
  pthread_mutex_destroy(&(pMemSys->lock));
}

static void *sqlite3MemMalloc(int nByte){
  pthread_mutex_lock(&mem_sys_global.lock);
  void *pMalloc = malloc(nByte);;
  sqlite3_mutex_leave(&mem_sys_global.lock);
  return pMalloc;
}

static void sqlite3MemFree(void *pPrior){
  pthread_mutex_lock(&mem_sys_global.lock);
  free(pPrior);
  sqlite3_mutex_leave(&mem_sys_global.lock);
}

static void *sqlite3MemRealloc(void *pPrior, int nByte){
  pthread_mutex_lock(&mem_sys_global.lock);
  void *pRealloc = realloc(pPrior, nByte);
  sqlite3_mutex_leave(&mem_sys_global.lock);
  return pRealloc;
}

static int sqlite3MemSize(void *pPrior){
  pthread_mutex_lock(&mem_sys_global.lock);
  int nByte = malloc_usable_size(pPrior);
  sqlite3_mutex_leave(&mem_sys_global.lock);
  return nByte;
}

static int sqlite3MemRoundup(int n){
  return ((n+7)&~7);
}

static int sqlite3MemInit(void *NotUsed){
  return SQLITE_OK;
}

static void sqlite3MemShutdown(void *NotUsed){
  mem_sys_release((MemSys *)NotUsed);
  memset(&mem_sys_global, 0, sizeof(mem_sys_global));
}

#endif /* SQLITE_ZERO_MALLOC */
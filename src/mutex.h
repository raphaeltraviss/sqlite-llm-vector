/*
** 2007 August 28
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
** This file contains the common header for all mutex implementations.
** The sqliteInt.h header #includes this file so that it is available
** to all source files.  We break it out in an effort to keep the code
** better organized.
**
** NOTE:  source files should *not* #include this header file directly.
** Source files should #include the sqliteInt.h file and let that file
** include this one indirectly.
*/

/*
** Figure out what version of the code to use.  The choices are
**
**   SQLITE_MUTEX_OMIT         No mutex logic.  Not even stubs.  The
**                             mutexes implementation cannot be overridden
**                             at start-time.
**
**   SQLITE_MUTEX_NOOP         For single-threaded applications.  No
**                             mutual exclusion is provided.  But this
**                             implementation can be overridden at
**                             start-time.
**
**   SQLITE_MUTEX_PTHREADS     For multi-threaded applications on Unix.
**
**   SQLITE_MUTEX_W32          For multi-threaded applications on Win32.
*/
#if !SQLITE_THREADSAFE
# define SQLITE_MUTEX_OMIT
#endif

#if SQLITE_THREADSAFE && !defined(SQLITE_MUTEX_NOOP)
#  if SQLITE_OS_UNIX
#    define SQLITE_MUTEX_PTHREADS
#  elif SQLITE_OS_WIN
#    define SQLITE_MUTEX_W32
#  else
#    define SQLITE_MUTEX_NOOP
#  endif
#endif


/*
** If SQLITE_MUTEX_OMIT is defined, that means that we are not
** implementing any mutex functions at all.  Make the various mutex
** routines into no-ops.
**
** If SQLITE_MUTEX_NOOP is defined, that means we are implementing
** a mutex that does no real locking, but provides a means to 
** disable the mutexes at run-time.  This implementation is used 
** for testing purposes only.  It is not suitable for a multi-threaded 
** application.
*/
#ifdef SQLITE_MUTEX_OMIT
  /*
  ** If this is a no-op implementation, implement everything as macros.
  */
  #define sqlite3_mutex_alloc(X)    ((sqlite3_mutex*)8)
  #define sqlite3_mutex_free(X)
  #define sqlite3_mutex_enter(X)    
  #define sqlite3_mutex_try(X)      SQLITE_OK
  #define sqlite3_mutex_leave(X)    
  #define sqlite3_mutex_held(X)     ((void)(X),1)
  #define sqlite3_mutex_notheld(X)  ((void)(X),1)
  #define sqlite3MutexAlloc(X)      ((sqlite3_mutex*)8)
  #define sqlite3MutexInit()        SQLITE_OK
  #define sqlite3MutexEnd()
  #define MUTEX_LOGIC(X)
#else /* SQLITE_MUTEX_OMIT not defined. */

  /* Include the header file for the chosen mutex implementation. */
  #if defined(SQLITE_MUTEX_PTHREADS)
    #include "mutex_unix.h"
  #elif defined(SQLITE_MUTEX_W32)
    #include "mutex_w32.h"
  #elif defined(SQLITE_MUTEX_NOOP)
    #include "mutex_noop.h"
  #else
    #error No mutex implementation specified.
  #endif

  /* Define the SQLITE_MUTEX_STATIC_MASTER macro.
  **
  ** When this macro is defined, the mutex subsystem has a mutex that
  ** controls access to the static mutexes.
  **
  ** When this macro is not defined, the mutex subsystem lacks the
  ** ability to internally synchronize access to mutexes.
  **
  ** When this macro is defined, the mutex subsystem must implement
  ** the sqlite3_mutex_alloc() logic using the sqlite3_mutex_alloc()
  ** function supplied by the application.  When this macro is not
  ** defined, sqlite3_mutex_alloc() is an internal routine.
  */
  #if defined(SQLITE_THREADSAFE) && defined(SQLITE_MUTEX_PTHREADS)
    #define SQLITE_MUTEX_STATIC_MASTER 1
  #endif

  /* When SQLITE_MUTEX_DEBUG is defined (to any value) then the mutex
  ** subsystem generates a lot of extra debugging information about
  ** mutex operations.  This information is used for testing and debugging
  ** the mutex library only. */
  #ifdef SQLITE_MUTEX_DEBUG
    extern int sqlite3MutexDebug;
    #define MUTEX_LOGIC(X)  X
  #else
    #define MUTEX_LOGIC(X)
  #endif

  /*
  ** These are the mutex routines that form the API to the mutex subsystem.
  **
  ** The sqlite3_mutex_alloc() routine returns a pointer to a new
  ** mutex.  If it returns NULL that means that a mutex could
  ** not be allocated.  The argument to sqlite3_mutex_alloc()
  ** which is normally the address of a integer which is 1 or 0 to 
  ** indicate static or dynamic allocation of the mutex being 
  ** requested may be a null pointer when the SQLITE_MUTEX_STATIC_MASTER 
  ** is defined. This is used to create a mutex that 
  ** protects the mutexes that are statically allocated by SQLite.
  **
  ** If successful, the sqlite3_mutex_alloc() routine returns a 
  ** pointer to the new mutex. The sqlite3_mutex_free() routine 
  ** deallocates a previously allocated mutex.  Attempting to
  ** deallocate a mutex that is not currently allocated results
  ** in undefined behavior.
  **
  ** The mutex routines must be called from a single thread.
  ** In a multithreaded application, SQLite expects that all
  ** mutexes will be allocated early, before other threads are
  ** running.
  **
  ** The sqlite3_mutex_enter() and sqlite3_mutex_try() routines attempt
  ** to enter a mutex.  If another thread is already within the mutex,
  ** sqlite3_mutex_enter() will block and sqlite3_mutex_try() will return
  ** SQLITE_BUSY.  The sqlite3_mutex_try() interface returns SQLITE_OK
  ** upon successful entry.  Mutexes created using SQLITE_MUTEX_RECURSIVE
  ** can be entered multiple times by the same thread.  In such cases
  ** the mutex must be exited an equal number of times before another
  ** thread can enter.  If the same thread attempts to enter any other
  ** kind of mutex more than once, the behavior is undefined.
  **
  ** Some systems (Example:  Solaris) do not support unnamed mutexes.  
  ** On systems that do not support unnamed mutexes, the implementation 
  ** must simulate them.  Because simulating an unnamed mutex requires 
  ** dynamically allocated memory, failure to release a mutex by 
  ** invoking sqlite3_mutex_leave() will result in a leak.
  **
  ** If compiled with SQLITE_DEBUG then additional logic is inserted 
  ** that does error checking on the mutexes to make sure they are being
  ** called correctly.
  */
  SQLITE_PRIVATE sqlite3_mutex *sqlite3_mutex_alloc(int);
  SQLITE_PRIVATE void sqlite3_mutex_free(sqlite3_mutex*);
  SQLITE_PRIVATE void sqlite3_mutex_enter(sqlite3_mutex*);
  SQLITE_PRIVATE int sqlite3_mutex_try(sqlite3_mutex*);
  SQLITE_PRIVATE void sqlite3_mutex_leave(sqlite3_mutex*);
  SQLITE_PRIVATE int sqlite3_mutex_held(sqlite3_mutex*);
  SQLITE_PRIVATE int sqlite3_mutex_notheld(sqlite3_mutex*);

  /*
  ** The sqlite3_mutex_held() and sqlite3_mutex_notheld() routine are
  ** intended for use only inside assert() statements.  On some platforms,
  ** there might be race conditions that can cause these routines to
  ** deliver incorrect results.  In particular, if pthreads are used to
  ** implement the mutexes, then these routines are always unsound.
  **
  ** These routine must be macros for the NDEBUG version of the code so 
  ** that the argument is not evaluated and thus any race condition will 
  ** be optimized away.
  */
#ifdef NDEBUG
#define sqlite3_mutex_held(X)      1
#define sqlite3_mutex_notheld(X)   1
#else
  SQLITE_PRIVATE int sqlite3_mutex_held(sqlite3_mutex*);
  SQLITE_PRIVATE int sqlite3_mutex_notheld(sqlite3_mutex*);
#endif

  /*
  ** This is a set of macros that implement the ENTERING_MUTEX logic.
  **
  ** In its normal form, the ENTERING_MUTEX() macro does nothing.  This
  ** is appropriate for production code.  But when testing or debugging
  ** the locking and unlocking behavior of a mutex, the ENTERING_MUTEX()
  ** macro can be defined to record information about each mutex enter
  ** and exit.  That information is written into a mutex.csv file for
  ** subsequent analysis by a separate program.
  **
  ** The following macros are available:
  **
  **   ENTERING_MUTEX(X,Y)
  **     Invoke whenever mutex X is being entered by thread Y.  The
  **     implementation does nothing.  X is a pointer.  Y is an integer
  **     thread ID.  Once you have configured the system to capture
  **     information using ENTERING_MUTEX(), simply grep the resulting
  **     mutex.csv file for the thread ID of interest in order to see when
  **     and how that thread entered each mutex in the system.
  **
  **   LEAVING_MUTEX(X,Y)
  **     Invoke whenever mutex X is being exited by thread Y.  The
  **     implementation does nothing.  X is a pointer.  Y is an integer
  **     thread ID.
  **
  **   SQLITE_MUTEX_LOGFULL
  **     Boolean (true/false) -- is logging of all mutex activity is enabled?
  **
  **   SQLITE_MUTEX_LOG_PARTIAL(x)
  **     Invoke at the beginning of any function that is a potential entry
  **     point when executing under logic test to avoid logging spurious
  **     mutex activity.  In the current implementation, the "partial" part
  **     of the name stems from the fact that this macro is a partial replacement
  **     for the SQLITE_MUTEX_LOGONLY feature, which is deprecated.
  **
  ** An appropriate configuration of the ENTERING_MUTEX macro for GCC might look
  ** like the following:
  **
  **   #define ENTERING_MUTEX(X,Y)  mutex_debug(#X, 1, __LINE__)
  **   #define LEAVING_MUTEX(X,Y)   mutex_debug(#X, 0, __LINE__)
  **
  ** This configuration will not work in production use because the
  ** mutex_debug() routine does not exist in that usage.
  */
#ifndef SQLITE_MUTEX_OMIT
  #ifndef ENTERING_MUTEX
    #define ENTERING_MUTEX(X,Y)
    #define LEAVING_MUTEX(X,Y)
  #endif
  extern int sqlite3MutexLog;
  #define SQLITE_MUTEX_LOGFULL (sqlite3MutexLog==2)
  #define SQLITE_MUTEX_LOGPARTIAL (sqlite3MutexLog==1)
  #define SQLITE_MUTEX_LOGNONE (sqlite3MutexLog==0)
  #define SQLITE_MUTEX_TRACE(X,F) \
     if(sqlite3MutexTrace&(X)){\
        sqlite3DebugPrintf("%s:%d %s\n",__FILE__,__LINE__,#F);\
     }
  #define SQLITE_MUTEX_LOGONLY(X) ((void)(0))
  #define SQLITE_MUTEX_INIT
#else /* defined(SQLITE_MUTEX_OMIT) */
  #define ENTERING_MUTEX(X,Y)
  #define LEAVING_MUTEX(X,Y)
  #define SQLITE_MUTEX_LOGFULL   0
  #define SQLITE_MUTEX_LOGPARTIAL 0
  #define SQLITE_MUTEX_LOGNONE   1
  #define SQLITE_MUTEX_TRACE(X,F)
  #define SQLITE_MUTEX_LOGONLY(X)
  #define SQLITE_MUTEX_INIT
#endif

#endif /* defined(SQLITE_MUTEX_OMIT) */
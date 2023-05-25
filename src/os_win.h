/*
** 2013 November 25
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This file contains code that is specific to Windows.
*/

#ifndef SQLITE_OS_WIN_H
#define SQLITE_OS_WIN_H

/*
** Include the primary Windows SDK header file.
*/
#include "windows.h"

#ifdef __CYGWIN__
# include <sys/cygwin.h>
# include <errno.h> /* amalgamator: dontcache */
#endif

#define SQLITE_OS_WINNT 1
#define SQLITE_OS_WIN (1)
#define SQLITE_WIN32_VOLATILE [[gnu::unused]]
#define SQLITE_OS_WINCE 0
#define SQLITE_OS_WINRT 0

#if SQLITE_OS_WIN && !SQLITE_OS_WINCE && !SQLITE_OS_WINRT && SQLITE_THREADSAFE
#define SQLITE_OS_WIN_THREADS 1
#else
#define SQLITE_OS_WIN_THREADS 0
#endif

#endif /* SQLITE_OS_WIN_H */
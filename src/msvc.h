/*
** 2015 January 12
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
** This file contains code that is specific to MSVC.
*/
#ifndef SQLITE_MSVC_H
#define SQLITE_MSVC_H

#if defined(_MSC_VER)
#define DISABLE_WARNING(n) \
    __pragma(warning(push)) \
    __pragma(warning(disable : n))

DISABLE_WARNING(4054)
DISABLE_WARNING(4055)
DISABLE_WARNING(4100)
DISABLE_WARNING(4127)
DISABLE_WARNING(4130)
DISABLE_WARNING(4152)
DISABLE_WARNING(4189)
DISABLE_WARNING(4206)
DISABLE_WARNING(4210)
DISABLE_WARNING(4232)
DISABLE_WARNING(4244)
DISABLE_WARNING(4305)
DISABLE_WARNING(4306)
DISABLE_WARNING(4702)
DISABLE_WARNING(4706)
#define END_DISABLE_WARNING(n) __pragma(warning(pop))

END_DISABLE_WARNING(4054)
END_DISABLE_WARNING(4055)
END_DISABLE_WARNING(4100)
END_DISABLE_WARNING(4127)
END_DISABLE_WARNING(4130)
END_DISABLE_WARNING(4152)
END_DISABLE_WARNING(4189)
END_DISABLE_WARNING(4206)
END_DISABLE_WARNING(4210)
END_DISABLE_WARNING(4232)
END_DISABLE_WARNING(4244)
END_DISABLE_WARNING(4305)
END_DISABLE_WARNING(4306)
END_DISABLE_WARNING(4702)
END_DISABLE_WARNING(4706)

#endif /* defined(_MSC_VER) */

#if defined(_MSC_VER) && !defined(_WIN64)
#undef SQLITE_4_BYTE_ALIGNED_MALLOC
#define SQLITE_4_BYTE_ALIGNED_MALLOC
#endif /* defined(_MSC_VER) && !defined(_WIN64) */

#if !defined(HAVE_LOG2) && defined(_MSC_VER) && _MSC_VER<1800
#define HAVE_LOG2 0
#endif /* !defined(HAVE_LOG2) && defined(_MSC_VER) && _MSC_VER<1800 */

#endif /* SQLITE_MSVC_H */
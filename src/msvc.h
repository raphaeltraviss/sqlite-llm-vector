/**
 * The DISABLE_WARNING macro is used to temporarily hide certain warning messages 
 * to enable backward-compatibility with old C code
 */
#define DISABLE_WARNING(n) \
    __pragma(warning(push)) \
    __pragma(warning(disable : n))

#define END_DISABLE_WARNING(n) __pragma(warning(pop))

/**
 * SQLITE_MSVC_H is a preprocessor directive that checks if this header file 
 * has already been included before. If it has been included before, the 
 * remaining code will be ignored.
 */
#ifndef SQLITE_MSVC_H
#define SQLITE_MSVC_H

#if defined(_MSC_VER)
    /**
     * Apply DISABLE_WARNING macro to temporarily disable specific 
     * warning messages
     */
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
    /**
     * END_DISABLE_WARNING macro is used to restore warning level back to normal
     */
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

/**
 * SQLITE_4_BYTE_ALIGNED_MALLOC is a preprocessor directive that align the 
 * size of allocated memory to 4 bytes. This is useful in ensuring backward
 * compatibility in 32-bit systems.
 */
#if defined(_MSC_VER) && !defined(_WIN64)
#undef SQLITE_4_BYTE_ALIGNED_MALLOC
#define SQLITE_4_BYTE_ALIGNED_MALLOC
#endif /* defined(_MSC_VER) && !defined(_WIN64) */

/**
 * HAVE_LOG2 is a preprocessor directive that checks if the log2() function is
 * already defined. If it is not defined, check if the version of MSVC being 
 * used is older than version 18. If it is an older version, then it is 
 * assumed that the log2() function is not available and should not be used.
 */
#if !defined(HAVE_LOG2) && defined(_MSC_VER) && _MSC_VER<1800
#define HAVE_LOG2 0
#endif /* !defined(HAVE_LOG2) && defined(_MSC_VER) && _MSC_VER<1800 */

/**
 * This ends the inclusion protection directive for this header file.
 */
#endif /* SQLITE_MSVC_H */
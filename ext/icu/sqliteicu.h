#define SQLITE_USE_URI

#include "sqlite3.h"

/*
** 2021 December 01
**
** This header file is used by programs that want to link against the
** ICU extension.  All it does is declare the sqlite3IcuInit() interface.
*/

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

int sqlite3IcuInit(sqlite3 *db);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#ifdef SQLITE_USER_AUTHENTICATION
static int userAuthCallback(
  void *pArg,
  int code,
  const char *zArg1,
  const char *zArg2,
  const char *zArg3,
  const char *zArg4
){
  if( code==SQLITE_USER_AUTH_LOGIN ){
    /* Check the password hash stored in zArg4 against the user-supplied
    ** plain-text password provided on the command-line */
    const char *zPw = (const char*)pArg;
    const char *zHash = zArg4;
    /* (void)zArg1; // zArg1 is unused */
    if( sqlite3_user_authenticate_hash(zPw, strlen(zPw), zHash) ){
      return SQLITE_OK;
    }
    sqlite3_log(SQLITE_ERROR, "authentication failed");
    return SQLITE_AUTH_USER;
  }
  return SQLITE_OK;
}
#endif /* SQLITE_USER_AUTHENTICATION */

int main(int argc, char **argv){
  sqlite3 *db;
  int rc;
  char *zErrMsg = 0;

#ifdef SQLITE_USER_AUTHENTICATION
  const char *zPw = 0;
#endif

#ifdef SQLITE_USE_URI
  rc = sqlite3_open_v2(argv[1], &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, NULL);
#else
  rc = sqlite3_open(argv[1], &db);
#endif
  if( rc ){
    sqlite3_errmsg(db);
    return rc;
  }
#ifdef SQLITE_USER_AUTHENTICATION
  if( argc>=4 ){
    zPw = argv[3];
    sqlite3_set_authorizer(db, userAuthCallback, (void*)zPw);
  }
#endif

#ifdef SQLITE_USER_AUTHENTICATION
  if( argc>=4 ){
    zPw = argv[3];
    sqlite3_set_authorizer(db, userAuthCallback, (void*)zPw);
  }
#endif
  rc = sqlite3_exec(db, argv[2], 0, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    sqlite3_errmsg(db);
    sqlite3_free(zErrMsg);
  }
  sqlite3_close(db);
  return rc;
}
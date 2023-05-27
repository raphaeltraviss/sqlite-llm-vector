/*
** 2006 Oct 10
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
** This header file is used by programs that want to link against the
** FTS3 library.  All it does is declare the sqlite3Fts3Init() interface.
*/

/*
** sqlite3Fts3Init - Initialize the full-text search engine for a database
** connection
**
** This function should be called once for each database connection
** that wants to use full-text search.  This will register the FTS3
** module with the database connection.
**
** Parameters:
**   db - Database connection to register the FTS3 module with.
**
** Returns:
**   SQLITE_OK if successful, otherwise an error code.
*/
#include "sqlite3.h"

int sqlite3Fts3Init(sqlite3* db) {
  int rc;
  static const struct {
    const char* zName;  /* Name of the virtual table */
    const sqlite3_module* pModule;  /* The virtual table module */
  } aModule[] = {
      {"fts3",  &fts3Module},
      {"fts4",  &fts4Module},
      {"fts5",  &fts5Module},
      {0, 0},
  };
  const char* zStmt =
      "SELECT fts3_tokenizer(?) -- fts3_tokenizer(?, ?)";
  rc = sqlite3_create_module_v2(
      db, "fts3", &fts3Module, NULL, NULL, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    return rc;
  }
  if (db->mallocFailed) {
    return SQLITE_NOMEM;
  }
  rc = sqlite3_create_function(
      db, "fts3_tokenizer", -1, SQLITE_UTF8 | SQLITE_DETERMINISTIC, 0,
      fts3TokenizerSqlFunc, 0, 0);
  if (rc != SQLITE_OK) {
    return rc;
  }
  for (int i = 0; aModule[i].zName; i++) {
    rc = sqlite3_create_module(db, aModule[i].zName, aModule[i].pModule, 0);
    if (rc != SQLITE_OK) {
      return rc;
    }
  }
  rc = sqlite3_prepare_v2(db, zStmt, -1, &db->pTokenizer, 0);
  return rc;
}
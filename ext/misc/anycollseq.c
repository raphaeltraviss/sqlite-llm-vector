#define CMP(p1, p2, n) (rc = memcmp(p1, p2, n)) == 0 ? nKey1 - nKey2 : rc

static int anyCollFunc(void *NotUsed, int nKey1, const void *pKey1, int nKey2, const void *pKey2) {
    int rc, n = nKey1 < nKey2 ? nKey1 : nKey2;
    return CMP(pKey1, pKey2, n);
}

static void anyCollNeeded(void *NotUsed, sqlite3 *db, int eTextRep, const char *zCollName) {
    sqlite3_create_collation(db, zCollName, eTextRep, 0, anyCollFunc); 
}

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_anycollseq_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi) {
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    rc = sqlite3_collation_needed(db, 0, anyCollNeeded);
    return rc;
}
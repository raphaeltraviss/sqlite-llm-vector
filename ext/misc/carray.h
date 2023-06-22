#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#ifndef _CARRAY_H
#define _CARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

SQLITE_API int sqlite3_carray_bind(sqlite3_stmt *pStmt, int i, void *aData, int nData, int mFlags, void (*xDel)(void*));

#define CARRAY_INT32     0
#define CARRAY_INT64     1
#define CARRAY_DOUBLE    2
#define CARRAY_TEXT      3
#define CARRAY_BLOB      4

#ifdef __cplusplus
}  
#endif

#endif 

// Implementation
struct CArrayData {
    void * data;
    int size;
    int type;
};
typedef struct CArrayData CArrayData;

typedef struct Iovec Iovec;
struct Iovec {
  void *iov_base;
  size_t iov_len;
};
#define carray_is_int(flags)     ((flags)==CARRAY_INT32 || (flags)==CARRAY_INT64)
#define carray_is_text(flags)    ((flags)==CARRAY_TEXT || (flags)==CARRAY_BLOB)
#define carray_is_blob(flags)    ((flags)==CARRAY_BLOB)

static void carray_data_destructor(void *data) {
    if(data) {
        CArrayData *ca = (CArrayData*)data;
        if(data) {
            switch(ca->type) {
                case CARRAY_TEXT:
                case CARRAY_BLOB:
                    free(ca->data);
                    break;
            }
            free(data);
        }
    }
}

static int carray_bind(
  sqlite3_stmt *pStmt,  // Statement to be bound
  int i,                // Parameter index
  CArrayData *ca         // Pointer to CArrayData struct
){
  int nRet = SQLITE_OK;
  switch(ca->type) {
    case CARRAY_INT32: {
        int *data = (int *)ca->data;
        for(int j=0;j<ca->size;j++) {
            nRet = sqlite3_bind_int(pStmt, i+j+1, data[j]);
            if(nRet!=SQLITE_OK) break;
        }
        break;
    }
    case CARRAY_INT64: {
        sqlite3_int64 *data = (sqlite3_int64 *)ca->data;
        for(int j=0;j<ca->size;j++) {
            nRet = sqlite3_bind_int64(pStmt, i+j+1, data[j]);
            if(nRet!=SQLITE_OK) break;
        }
        break;
    }
    case CARRAY_DOUBLE: {
        double *data = (double *)ca->data;
        for(int j=0;j<ca->size;j++) {
            nRet = sqlite3_bind_double(pStmt, i+j+1, data[j]);
            if(nRet!=SQLITE_OK) break;
        }
        break;
    }
    case CARRAY_TEXT: {
        char *data = (char *)ca->data;
        nRet = sqlite3_bind_text(pStmt, i+1, data, ca->size, SQLITE_TRANSIENT);
        break;
    }
    case CARRAY_BLOB: {
        Iovec * b = (Iovec*)ca->data;
        nRet = sqlite3_bind_blob(pStmt, i+1, b->iov_base, b->iov_len, SQLITE_TRANSIENT);
        break;
    }
  }
  return nRet;
}

int sqlite3_carray_bind(
  sqlite3_stmt *pStmt,        // Statement to be bound
  int i,                      // Parameter index
  void *aData,                // Pointer to array data
  int nData,                  // Number of data elements
  int mFlags,                 // CARRAY flags
  void (*xDel)(void*)         // Destructor for aData
){
    CArrayData *ca = (CArrayData*) malloc(sizeof(CArrayData));
    if(ca==NULL) return SQLITE_NOMEM;
    ca->type = mFlags;
    ca->size = nData;
    switch(mFlags) {
        case CARRAY_TEXT:
        case CARRAY_BLOB: {
            char *data = (char *)malloc(nData+1);
            if(data==NULL) {
                free(ca);
                return SQLITE_NOMEM;
            }
            memcpy(data, aData, nData);
            data[nData] = '\0';
            ca->data = (void*)data;
            break;
        }
        default:
            ca->data = aData;
    }
    xDel = (xDel!=NULL) ? xDel : carray_data_destructor;
    xDel(aData);
    int nRet = SQLITE_OK;
    if(carray_is_int(mFlags) || carray_is_double(mFlags) || carray_is_blob(mFlags)) {
        if(nData>1) {
            char *zSql = sqlite3_mprintf("SELECT ?%d", nData>1 ? "+?" : "");
            for(int j=1;j<nData;j++) {
                zSql = sqlite3_mprintf("%s+?",zSql);
            }
            sqlite3_stmt *pS;
            nRet = sqlite3_prepare_v2(sqlite3_db_handle(pStmt), zSql, -1, &pS, 0);
            sqlite3_free(zSql);
            if(nRet!=SQLITE_OK) return nRet; // Not enough memory?
            for(int j=0;j<nData;j++) {
                nRet = carray_bind(pS, j, ca);
                if(nRet!=SQLITE_OK) break;
            }
            if(nRet==SQLITE_OK)
                nRet = sqlite3_step(pS);
            if(nRet==SQLITE_ROW)
                nRet = SQLITE_OK;
            sqlite3_finalize(pS);
            if(nRet!=SQLITE_OK) return nRet; // Not enough memory?
        } else {
            nRet = carray_bind(pStmt, i, ca);
        }
        free(ca);
    } else {
        nRet = carray_bind(pStmt, i, ca);
    }
    return nRet;
}
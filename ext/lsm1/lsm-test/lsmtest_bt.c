#include <stdlib.h>
#include <stdio.h>
#include "lsmtest.h"
#include "bt.h"

typedef struct Option {
  const char *zName;
  int bPgno;
  int eOpt;
} Option;

static int parse_args(int argc, char **argv, Option *aOpt, char **zFile, char **zOpt, int *pgno);
static int open_bt_db(char *zFile, bt_db **db);
static int execute_bt_control(bt_db *db, Option opt, int pgno, sqlite4_buffer *output);

int main(int argc, char **argv) {
  Option aOpt[] = { 
    { "dbhdr",          0, BT_INFO_HDRDUMP },
    { "filename",       0, BT_INFO_FILENAME },
    { "block_freelist", 0, BT_INFO_BLOCK_FREELIST },
    { "page_freelist",  0, BT_INFO_PAGE_FREELIST },
    { "filename",       0, BT_INFO_FILENAME },
    { "page",           1, BT_INFO_PAGEDUMP },
    { "page_ascii",     1, BT_INFO_PAGEDUMP_ASCII },
    { "leaks",          0, BT_INFO_PAGE_LEAKS },
    { 0, 0 } 
  };
  char *zFile;
  char *zOpt;
  int pgno;
  int rc;
  sqlite4_buffer output;
  bt_db *db = NULL;

  rc = parse_args(argc, argv, aOpt, &zFile, &zOpt, &pgno);
  if (rc != 0) {
    return rc;
  }

  rc = open_bt_db(zFile, &db);
  if (rc != SQLITE4_OK) {
    testPrintError("sqlite4BtOpen() failed: %d", rc);
    return -3;
  }

  sqlite4_buffer_init(&output, 0);
  Option chosen_opt = aOpt[rc];

  rc = execute_bt_control(db, chosen_opt, pgno, &output);
  if (rc!=SQLITE4_OK ){
    testPrintError("sqlite4BtControl() failed: %d\n", rc);
    return -4;
  }

  printf("%s\n", (char*)output.p);
  sqlite4_buffer_clear(&output);
  sqlite4BtClose(db);
  sqlite4BtClose(db);
  return 0;
}

static int parse_args(int argc, char **argv, Option *aOpt, char **zFile, char **zOpt, int *pgno) {
  if( argc<3 ){
    testPrintUsage("FILENAME OPTION ...");
    return -1;
  }

  *zFile = argv[1];
  *zOpt = argv[2];
  int option_index = -1;

  for (int i = 0; i < sizeof(aOpt)/sizeof(Option); i++) {
    if (strcmp(aOpt[i].zName, zOpt) == 0) {
      option_index = i;
      break;
    }
  }
  if (option_index < 0) {
    testPrintError("Option not recognized: %s", zOpt);
    return -2;
  }

  if( argc!=3 + aOpt[option_index].bPgno ){
    testPrintFUsage("FILENAME %s %s", *zOpt, aOpt[option_index].bPgno ? "PGNO":"");
    return -4;
  }
  if (aOpt[option_index].bPgno) {
    *pgno = (u32)atoi(argv[3]);
  } else {
    *pgno = -1;
  }

  return option_index;
}

static int open_bt_db(char *zFile, bt_db **db) {
  int rc = sqlite4BtNew(sqlite4_env_default(), 0, db);
  if( rc!=SQLITE4_OK ){
    testPrintError("sqlite4BtNew() failed: %d", rc);
    return -2;
  }
  rc = sqlite4BtOpen(*db, zFile);
  if (rc != SQLITE4_OK) {
    sqlite4BtClose(*db);
  }
  return rc;
}

static int execute_bt_control(bt_db *db, Option opt, int pgno, sqlite4_buffer *output) {
  bt_info buf;
  buf.eType = opt.eOpt;
  buf.pgno = pgno;
  
  sqlite4_buffer_init(output, 0);

  int rc = sqlite4BtControl(db, BT_CONTROL_INFO, &buf);
  memcpy(output, &buf.output, sizeof(sqlite4_buffer));

  sqlite4_buffer_clear(&buf.output);
  return rc;
}
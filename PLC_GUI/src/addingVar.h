#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

extern sqlite3* db;

void AddVariable(
    const char* name,
    const char* type,
    const char* address,
    int value
);

#endif

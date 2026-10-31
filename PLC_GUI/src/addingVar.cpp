#include "addingVar.h"
#include <iostream>

sqlite3* db = nullptr;

// AddVariable() implementation

void AddVariable(const char* name,
                 const char* type,
                 const char* address,
                 int value)
{
    const char* sql =
        "INSERT INTO variables (name, type, address, value) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, type, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, address, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, value);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Variable added!\n";
    } else {
        std::cout << "Insert failed: " << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(stmt);
}

#include "addingVar.h"
#include <iostream>
#include <vector>
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


std::vector<Variable> GetVariables()
{
    std::vector<Variable> variables;

    const char* sql = "SELECT * FROM variables";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Variable var;

        var.name = (const char*)sqlite3_column_text(stmt, 1);
        var.value = sqlite3_column_int(stmt, 4);

        variables.push_back(var);
    }

    sqlite3_finalize(stmt);

    return variables;
}

#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>

struct Variable {
    int id;
    std::string name;
    char* type;
    std::string address;
    int value;
};

extern sqlite3* db;



void AddVariable(
    const char* name,
    const char* type,
    const char* address,
    int value
);

std::vector<Variable> GetVariables();

#endif

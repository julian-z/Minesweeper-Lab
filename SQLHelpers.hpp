// SQLHelpers.hpp -- Julian Zulfikar, 2023
// ------------------------------------------------------
// Helper functions for SQL commands.

#ifndef SQLHELP_HPP
#define SQLHELP_HPP

#include <SQLite/sqlite3.h>
#include <iostream>

struct Result
{
    int resultingCall;
};

int callback(void *data, int argc, char **argv, char **azColName);

#endif

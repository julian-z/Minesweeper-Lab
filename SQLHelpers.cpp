// SQLHelpers.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// Helper functions for SQL commands.

#include "SQLHelpers.hpp"


// Will only be selecting one column at a time
int callback(void *data, int argc, char **argv, char **azColName)
{
    Result* result = (Result*) data;
    result->resultingCall = std::atoi(argv[0]); // Store result from select

    // For debugging purposes
    // printf("%s = %s\n", azColName[0], argv[0] ? argv[0] : "NULL");
    
    return 0;
}


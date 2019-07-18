
//#include <sqlite3.h>
//#include <stdio.h>
#include <stdlib.h>
#include "include/db.hpp"

int main()
{
    printf("%s\n", sqlite3_libversion());
    char* db_name = "db/sample.db";
    int result = 0;
    sqlite3_demo(db_name);
    printf("sqlite2 exe return code: %d\n", result);
    system("pause");
    return 0;
}
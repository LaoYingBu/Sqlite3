
//#include <sqlite3.h>
//#include <stdio.h>
#include <stdlib.h>
#include "include/db.hpp"
#include "include/cv.hpp"

int main()
{
    printf("%s\n", sqlite3_libversion());
    char* db_name = "db/sample.db";
    int result = 0;
    result = sqlite3_demo(db_name);
    printf("sqlite2 exe return code: %d\n", result);

    char* filename = "C:/digits_tsne-generated.png";
    cv_demo(filename);
    system("pause");
    return 0;
}
// main.cpp
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("%s\n", sqlite3_libversion());
    system("pause");
    return 0;
}
#include "include/db.hpp"

int callback(void* pv, int argc, char** argv, char** col)
{
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < argc; cnt_i++)
    {
        printf("%s\t%s\n", col[cnt_i], argv[cnt_i]);/* code */
    }
    printf("\n");
    return 0;
}

int sqlite3_demo(char* db_name)
{
    sqlite3* db;
    int result = 0;
    char* sql = NULL;
    char* rerrmsg = NULL;
    char* data = "callback";

    result = sqlite3_open(db_name, &db);
    if(result > 0)
    {
        printf("open database err: %s\n", db_name);
        return -1;
    }
    else
    {
        printf("open database successfully!\n");
        
        sql = "CREATE TABLE STUDENT(" \
        "NUM INT PRIMARY KEY NOT NULL," \
        "NAME TEXT NOT NULL," \
        "AGE INT NOT NULL," \
        "SCORE REAL);";
        result = sqlite3_exec(db, sql, callback, NULL, &rerrmsg);
        if(result > 0)
        {
            printf("create tabel err: %s.\n", rerrmsg);
            sqlite3_free(rerrmsg);
            return -2;
        }
        else
        {
            printf("create tabel successfully!\n");

            sql = "INSERT INTO STUDENT(NUM, NAME, AGE, SCORE)" \
            "VALUES(1, 'PAUL', 21, 121.2);" \
            "INSERT INTO STUDENT(NUM, NAME, AGE, SCORE)" \
            "VALUES(2, 'JOB', 22, 111.2);" \
            "INSERT INTO STUDENT(NUM, NAME, AGE, SCORE)" \
            "VALUES(3, 'LILY', 19, 142.2);" \
            "INSERT INTO STUDENT(NUM, NAME, AGE, SCORE)" \
            "VALUES(4, 'MAX', 22, 111.2);" \
            "INSERT INTO STUDENT(NUM, NAME, AGE, SCORE)" \
            "VALUES(5, 'NEO', 19, 142.2);";
            result = sqlite3_exec(db, sql, callback, NULL, &rerrmsg);
            if(result > 0)
            {
                printf("insert data err: %s\n", rerrmsg);
                sqlite3_free(rerrmsg);
                return -3;
            }
            else
            {
                printf("insert data successfully!\n");
                sql = "SELECT * FROM STUDENT";
                result = sqlite3_exec(db, sql, callback, (void*)data, &rerrmsg);
                if(result > 0)
                {
                    printf("select data err: %s\n", rerrmsg);
                    sqlite3_free(rerrmsg);
                    return -4;
                }
                else
                {
                    printf("select data successfully!\n");

                }
                
            }
            
        }
    }
    sqlite3_close(db);
    return 0;   
    
}
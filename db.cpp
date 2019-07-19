#include "include/db.hpp"

int callback(void* pv, int argc, char** argv, char** col)
{
    int cnt_i = 0;
    fprintf(stdout, "%s\n", (char*)pv);
    for (cnt_i = 0; cnt_i < argc; cnt_i++)
    {
        printf("%s\t%s\n", col[cnt_i], argv[cnt_i]);
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
            printf("create table successfully!\n");

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
                sql = "SELECT * FROM STUDENT;";
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
            /*select if condition avaliable */
            fprintf(stdout, "begin...\n");
            data = "select callback function call!\n";
            sql = "select * from STUDENT where 'NAME=MAX';";
            result = sqlite3_exec(db, sql, callback, data, &rerrmsg);
            if(result != 0)
            {
                fprintf(stderr, "select data err: %s\n", rerrmsg);
                sqlite3_free(rerrmsg);
                return -4;
            }
            else
            {
                fprintf(stdout, "select data successfully!\n");
                fprintf(stdout, "new data: %s\n", data);
            }
            
            /*update */
            sql = "update STUDENT set SCORE=100 where NAME=='NEO';" \
            "select * from STUDENT where NAME=='NEO';";
            result = sqlite3_exec(db, sql, NULL, NULL, &rerrmsg);
            if(result != 0)
            {
                printf("update data err: %s\n", rerrmsg);
                sqlite3_free(rerrmsg);
                return -5;
            }
            else
            {
                printf("update data successfully!\n");
            }
            /*drop table */
            sql = "drop table STUDENT;";
            result = sqlite3_exec(db, sql, NULL, NULL, &rerrmsg);
            if(result != 0)
            {
                printf("drop table err: %s\n", rerrmsg);
                sqlite3_free(rerrmsg);
                return -6;
            }
            else
            {
                printf("drop table successfully!\n");
            }
            /*create new tabel if condition avaliable */
            char* tname = "lzw";
            char csql[256] = {'0'};
            sprintf(csql, "create table if not exists %s(" \
            "id int not null," \
            "name text not null);", tname);
            printf("%s\n", csql);
            result = sqlite3_exec(db, csql, NULL, NULL, &rerrmsg);
            if(result != 0)
            {
                printf("create new table err: %s\n", rerrmsg);
                sqlite3_free(rerrmsg);
                return -7;
            }
            else
            {
                printf("create new table successfully!\n");
                sql = "insert into lzw(id, name)" \
                "values(10, 'zyz');" \
                "insert into lzw(id, name)" \
                "values(11, 'lsm');";
                sqlite3_exec(db, sql, NULL, NULL, &rerrmsg);
                if(result != 0)
                {
                    fprintf(stderr, "insert data err:%s\n", rerrmsg);
                    sqlite3_free(rerrmsg);
                    return -8;
                }
                else
                {
                    sql = "select * from lzw where id=10;";
                    result = sqlite3_exec(db, sql, callback, data, &rerrmsg);
                    fprintf(stdout, "LZW: %s\n", data);
                }
            }
            
            
        }
    }
    sqlite3_close(db);
    return 0;   
    
}
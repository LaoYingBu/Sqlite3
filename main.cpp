
//#include <sqlite3.h>
//#include <stdio.h>
#include <stdlib.h>
#include "include/db.hpp"
#include "include/cv.hpp"
#include "include/ncnn.hpp"

// int main()
// {
//     printf("%s\n", sqlite3_libversion());
//     char* db_name = "db/sample.db";
//     int result = 0;
//     result = sqlite3_demo(db_name);
//     printf("sqlite2 exe return code: %d\n", result);
//     fprintf(stdout, "sqlite3 dll loaded successfully!\n");
//     char* filename = "finger.bmp";
//     cv_demo(filename);
//     fprintf(stdout, "cv dll loaded successfully!\n");
//     ncnn_demo(filename);
//     fprintf(stdout, "ncnn dll loaded successfully!\n");
//     system("pause");
//     return 0;
// }

int main(int argc, char** argv)
{
    char* ifilename = "in.txt";
    char* ofilename = "out.txt";
    if(argc == 3){
        ifilename = argv[1];
        ofilename = argv[2];
    }
    ncnn_demo(ifilename, ofilename);
    fprintf(stdout, "ncnn dll loaded successfully!\n");
    system("pause");
    return 0;
}
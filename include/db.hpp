#ifndef __DB_HPP__
#define __DB_HPP__

#include <stdio.h>
#include <sqlite3.h>

//#define SQLITE3_EXPORT

#ifdef _MSC_VER
#ifdef SQLITE3_EXPORT
#define SQLITE3_API __declspec(dllexport)
#else
#define SQLITE3_API __declspec(dllimport)
#endif
#else
#define SQLITE3_API
#endif

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#ifdef __cplusplus
extern "C" {
#endif

SQLITE3_API int callback(void* pv, int argc, char** argv, char** col);
SQLITE3_API int sqlite3_demo(char* db_name);

#ifdef __cplusplus
}
#endif

// int callback(void* pv, int argc, char** argv, char** col);
// int sqlite3_demo(char* db_name);
#endif // __DB_HPP__
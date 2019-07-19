#ifndef __CV_HPP__
#define __CV_HPP__

#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

#ifdef _MSC_VER
#ifdef CV_EXPORT
#define CV_API __declspec(dllexport)
#else
#define CV_API __declspec(dllimport)
#endif
#else
#define CV_API
#endif

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif


#ifdef __cplusplus
extern "C" {
#endif

CV_API int cv_demo(char* filename);

#ifdef __cplusplus
}
#endif

#endif //__CV_HPP__
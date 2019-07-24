#ifndef __NCNN_HPP__
#define __NCNN_HPP__

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <vector>
#include <net.h>
#include <platform.h>

#if NCNN_VULKAN
#include <gpu.h>
#endif

using namespace cv;

#ifdef _MSC_VER
#ifdef NCNN_EXPORT
#define NCNN_API __declspec(dllexport)
#else
#define NCNN_API __declspec(dllimport)
#endif
#else
#define NCNN_API
#endif

#ifdef _MSC_VER
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#endif


#ifdef __cplusplus
extern "C" {
#endif

NCNN_API int ncnn_demo(char* filename);

#ifdef __cplusplus
}
#endif

#endif //__NCNN_HPP__
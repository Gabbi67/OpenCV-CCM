#include <iostream>

#pragma once
#ifdef OpenDLL_EXPORTS
#define OpenDLL_API __declspec(dllexport)
#else
#define OpenDLL_API __declspec(dllimport)
#endif

using namespace std;

extern "C" OpenDLL_API void ImageChart(string ImageLink, bool show);

extern "C" OpenDLL_API void PrintCCM();

extern "C" OpenDLL_API void CCM(string ImageLink, string SaveLink, bool show);

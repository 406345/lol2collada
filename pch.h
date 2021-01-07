#ifndef PCH_H
#define PCH_H


#include "framework.h"

#endif

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <vector>

using namespace std;

#include "ImporterException.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

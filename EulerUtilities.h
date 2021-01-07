#pragma once

#ifndef _EULER_UTILITIES_H_
#define _EULER_UTILITIES_H_

#include "Euler.h"

class EulerUtilities
{
public:
	static const float PI;
	static const float degree;

	static void radiansToDegrees(Euler& r);
	EulerUtilities();
	~EulerUtilities();
};


#endif

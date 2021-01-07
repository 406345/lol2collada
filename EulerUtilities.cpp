#include "pch.h"
#include "EulerUtilities.h"

const float EulerUtilities::PI = 3.14159265358979323846f;
const float EulerUtilities::degree = 180.0f / PI;

void EulerUtilities::radiansToDegrees(Euler& r)
{
	r.x *= degree;
	r.y *= degree;
	r.z *= degree;
}

EulerUtilities::EulerUtilities()
{
}

EulerUtilities::~EulerUtilities()
{
}

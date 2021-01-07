#include "pch.h"
#include "Vector3f.h"

float&
Vector3f::operator [](int i)
{
	return (&x)[i];
}


const float&
Vector3f::operator [](int i) const
{
	return (&x)[i];
}


Vector3f::Vector3f()
{
}


Vector3f::Vector3f(float a)
{
	x = y = z = a;
}


Vector3f::Vector3f(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}


Vector3f::Vector3f(const Vector3f& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}


const Vector3f&
Vector3f::operator =(const Vector3f& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector3f::~Vector3f()
{
}


bool
Vector3f::operator ==(const Vector3f& v) const
{
	return x == v.x && y == v.y && z == v.z;
}


bool
Vector3f::operator !=(const Vector3f& v) const
{
	return x != v.x || y != v.y || z != v.z;
}


bool
Vector3f::equalWithAbsError(const Vector3f& v, float e) const
{
	auto x1 = x;
	auto x2 = v.x;

	if (!(((x1 > x2) ? x1 - x2 : x2 - x1) <= e))
	{
		return false;
	}

	x1 = y;
	x2 = v.y;

	if (!(((x1 > x2) ? x1 - x2 : x2 - x1) <= e))
	{
		return false;
	}

	x1 = z;
	x2 = v.z;

	if (!(((x1 > x2) ? x1 - x2 : x2 - x1) <= e))
	{
		return false;
	}


	return true;
}


bool
Vector3f::equalWithRelError(const Vector3f& v, float e) const
{
	auto x1 = x;
	auto x2 = v.x;

	if (!(((x1 > x2) ? x1 - x2 : x2 - x1) <= e * ((x1 > 0) ? x1 : -x1)))
	{
		return false;
	}

	x1 = y;
	x2 = v.y;

	if (!(((x1 > x2) ? x1 - x2 : x2 - x1) <= e * ((x1 > 0) ? x1 : -x1)))
	{
		return false;
	}

	x1 = z;
	x2 = v.z;

	if (!(((x1 > x2) ? x1 - x2 : x2 - x1) <= e * ((x1 > 0) ? x1 : -x1)))
	{
		return false;
	}


	return true;
}


float
Vector3f::dot(const Vector3f& v) const
{
	return x * v.x + y * v.y + z * v.z;
}


float
Vector3f::operator ^(const Vector3f& v) const
{
	return dot(v);
}


Vector3f
Vector3f::cross(const Vector3f& v) const
{
	return Vector3f(y * v.z - z * v.y,
	                z * v.x - x * v.z,
	                x * v.y - y * v.x);
}


const Vector3f&
Vector3f::operator %=(const Vector3f& v)
{
	float a = y * v.z - z * v.y;
	float b = z * v.x - x * v.z;
	float c = x * v.y - y * v.x;
	x = a;
	y = b;
	z = c;
	return *this;
}


Vector3f
Vector3f::operator %(const Vector3f& v) const
{
	return Vector3f(y * v.z - z * v.y,
	                z * v.x - x * v.z,
	                x * v.y - y * v.x);
}


const Vector3f&
Vector3f::operator +=(const Vector3f& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}


Vector3f
Vector3f::operator +(const Vector3f& v) const
{
	return Vector3f(x + v.x, y + v.y, z + v.z);
}


const Vector3f&
Vector3f::operator -=(const Vector3f& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}


Vector3f
Vector3f::operator -(const Vector3f& v) const
{
	return Vector3f(x - v.x, y - v.y, z - v.z);
}


Vector3f
Vector3f::operator -() const
{
	return Vector3f(-x, -y, -z);
}


const Vector3f&
Vector3f::negate()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}


const Vector3f&
Vector3f::operator *=(const Vector3f& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}


const Vector3f&
Vector3f::operator *=(float a)
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}


Vector3f
Vector3f::operator *(const Vector3f& v) const
{
	return Vector3f(x * v.x, y * v.y, z * v.z);
}


Vector3f
Vector3f::operator *(float a) const
{
	return Vector3f(x * a, y * a, z * a);
}


const Vector3f&
Vector3f::operator /=(const Vector3f& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}


const Vector3f&
Vector3f::operator /=(float a)
{
	x /= a;
	y /= a;
	z /= a;
	return *this;
}


Vector3f
Vector3f::operator /(const Vector3f& v) const
{
	return Vector3f(x / v.x, y / v.y, z / v.z);
}


Vector3f
Vector3f::operator /(float a) const
{
	return Vector3f(x / a, y / a, z / a);
}


float
Vector3f::lengthfloatiny() const
{
	float absX = (x >= float(0)) ? x : -x;
	float absY = (y >= float(0)) ? y : -y;
	float absZ = (z >= float(0)) ? z : -z;

	float max = absX;

	if (max < absY)
		max = absY;

	if (max < absZ)
		max = absZ;

	if (max == float(0))
		return float(0);


	absX /= max;
	absY /= max;
	absZ /= max;

	return max * sqrtf(absX * absX + absY * absY + absZ * absZ);
}


float
Vector3f::length() const
{
	float length2 = dot(*this);

	if (length2 < float(2) * FLT_MIN)
		return lengthfloatiny();

	return sqrtf(length2);
}


float
Vector3f::length2() const
{
	return dot(*this);
}


const Vector3f&
Vector3f::normalize()
{
	float l = length();

	if (l != float(0))
	{
		x /= l;
		y /= l;
		z /= l;
	}

	return *this;
}


const Vector3f&
Vector3f::normalizeExc()
{
	float l = length();

	if (l == float(0))
	{
	}

	x /= l;
	y /= l;
	z /= l;
	return *this;
}


const Vector3f&
Vector3f::normalizeNonNull()
{
	float l = length();
	x /= l;
	y /= l;
	z /= l;
	return *this;
}


Vector3f
Vector3f::normalized() const
{
	float l = length();

	if (l == float(0))
		return Vector3f(float(0));

	return Vector3f(x / l, y / l, z / l);
}


Vector3f
Vector3f::normalizedExc() const
{
	float l = length();

	if (l == float(0))
	{
	}

	return Vector3f(x / l, y / l, z / l);
}


Vector3f
Vector3f::normalizedNonNull() const
{
	float l = length();
	return Vector3f(x / l, y / l, z / l);
}

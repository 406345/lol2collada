#pragma once

#ifndef _VECTOR_3F_H_
#define _VECTOR_3F_H_

class Vector3f
{
public:
	float x, y, z;

	float& operator [](int i);
	const float& operator [](int i) const;

	Vector3f();
	explicit Vector3f(float a);
	Vector3f(float a, float b, float c);


	Vector3f(const Vector3f& v);


	const Vector3f& operator =(const Vector3f& v);


	~Vector3f();

	bool operator ==(const Vector3f& v) const;
	bool operator !=(const Vector3f& v) const;

	bool equalWithAbsError(const Vector3f& v, float e) const;
	bool equalWithRelError(const Vector3f& v, float e) const;


	float dot(const Vector3f& v) const;
	float operator ^(const Vector3f& v) const;


	Vector3f cross(const Vector3f& v) const;
	const Vector3f& operator %=(const Vector3f& v);
	Vector3f operator %(const Vector3f& v) const;


	const Vector3f& operator +=(const Vector3f& v);
	Vector3f operator +(const Vector3f& v) const;


	const Vector3f& operator -=(const Vector3f& v);
	Vector3f operator -(const Vector3f& v) const;


	Vector3f operator -() const;
	const Vector3f& negate();


	const Vector3f& operator *=(const Vector3f& v);
	const Vector3f& operator *=(float a);
	Vector3f operator *(const Vector3f& v) const;
	Vector3f operator *(float a) const;


	const Vector3f& operator /=(const Vector3f& v);
	const Vector3f& operator /=(float a);
	Vector3f operator /(const Vector3f& v) const;
	Vector3f operator /(float a) const;


	float length() const;
	float length2() const;

	const Vector3f& normalize();
	const Vector3f& normalizeExc();
	const Vector3f& normalizeNonNull();

	Vector3f normalized() const;
	Vector3f normalizedExc() const;
	Vector3f normalizedNonNull() const;


private:

	float lengthfloatiny() const;
};

Vector3f operator *(float a, const Vector3f& v);

inline Vector3f
operator *(float a, const Vector3f& v)
{
	return Vector3f(a * v.x, a * v.y, a * v.z);
}

#endif

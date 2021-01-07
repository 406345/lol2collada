#pragma once

#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "Vector3f.h"
#include "Matrix4x4.h"

class Quaternion
{
public:

	float r;
	Vector3f v;

	Quaternion();
	Quaternion(float s, float i, float j, float k);
	Quaternion(float s, Vector3f d);
	static Quaternion identity();
	Quaternion(const Quaternion& q);
	~Quaternion();

	const Quaternion& operator =(const Quaternion& q);
	const Quaternion& operator *=(const Quaternion& q);
	const Quaternion& operator *=(float t);
	const Quaternion& operator /=(const Quaternion& q);
	const Quaternion& operator /=(float t);
	const Quaternion& operator +=(const Quaternion& q);
	const Quaternion& operator -=(const Quaternion& q);
	float& operator [](int index);
	float operator [](int index) const;

	bool operator ==(const Quaternion& q) const;
	bool operator !=(const Quaternion& q) const;

	Quaternion& invert();
	Quaternion inverse() const;
	Quaternion& normalize();
	Quaternion normalized() const;
	float length() const;
	Vector3f rotateVector(const Vector3f& original) const;
	float euclideanInnerProduct(const Quaternion& q) const;

	Quaternion& setAxisAngle(const Vector3f& axis, float radians);

	Quaternion& setRotation(const Vector3f& fromDirection,
	                        const Vector3f& toDirection);

	float angle() const;
	Vector3f axis() const;

	Matrix4x4 toMatrix44() const;

	Quaternion log() const;
	Quaternion exp() const;


private:

	void setRotationInternal(const Vector3f& f0,
	                         const Vector3f& t0,
	                         Quaternion& q);
};

inline Quaternion
operator *(const Quaternion& q1, const Quaternion& q2)
{
	return Quaternion(q1.r * q2.r - (q1.v ^ q2.v),
	                  q1.r * q2.v + q1.v * q2.r + q1.v % q2.v);
}

#endif

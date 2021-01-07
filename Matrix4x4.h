#pragma once

#ifndef _MATRIX_4X4_H_
#define _MATRIX_4X4_H_

#include "Vector3f.h"

class Matrix4x4
{
public:
	float x[4][4];

	float* operator [](int i);
	const float* operator [](int i) const;

	Matrix4x4();
	Matrix4x4(float a);
	Matrix4x4(const float a[4][4]);

	Matrix4x4(float a, float b, float c, float d, float e, float f, float g, float h,
	          float i, float j, float k, float l, float m, float n, float o, float p);
	~Matrix4x4();
	Matrix4x4(const Matrix4x4& v);

	const Matrix4x4& operator =(const Matrix4x4& v);
	const Matrix4x4& operator =(float a);

	float* getValue();
	const float* getValue() const;

	void getValue(Matrix4x4& v) const;
	Matrix4x4& setValue(const Matrix4x4& v);
	Matrix4x4& setfloatheMatrix(const Matrix4x4& v);

	void makeIdentity();

	bool operator ==(const Matrix4x4& v) const;
	bool operator !=(const Matrix4x4& v) const;

	bool equalWithAbsError(const Matrix4x4& v, float e) const;
	bool equalWithRelError(const Matrix4x4& v, float e) const;

	const Matrix4x4& operator +=(const Matrix4x4& v);
	const Matrix4x4& operator +=(float a);
	Matrix4x4 operator +(const Matrix4x4& v) const;

	const Matrix4x4& operator -=(const Matrix4x4& v);
	const Matrix4x4& operator -=(float a);
	Matrix4x4 operator -(const Matrix4x4& v) const;

	Matrix4x4 operator -() const;
	const Matrix4x4& negate();

	const Matrix4x4& operator *=(float a);
	Matrix4x4 operator *(float a) const;

	const Matrix4x4& operator *=(const Matrix4x4& v);
	Matrix4x4 operator *(const Matrix4x4& v) const;

	static void multiply(const Matrix4x4& a,
	                     const Matrix4x4& b,
	                     Matrix4x4& c);
	void multVecMatrix(const Vector3f& src, Vector3f& dst) const;
	void multDirMatrix(const Vector3f& src, Vector3f& dst) const;

	const Matrix4x4& operator /=(float a);
	Matrix4x4 operator /(float a) const;

	const Matrix4x4& transpose();
	Matrix4x4 transposed() const;

	const Matrix4x4& invert(bool singExc = false);
	Matrix4x4 inverse(bool singExc = false) const;
	const Matrix4x4& gjInvert(bool singExc = false);
	Matrix4x4 gjInverse(bool singExc = false) const;

	float fastMinor(int r0, int r1, int r2,
	                int c0, int c1, int c2) const;

	float determinant() const;

	const Matrix4x4& setEulerAngles(const Vector3f& r);
	const Matrix4x4& rotate(const Vector3f& r);
	const Matrix4x4& setScale(float s);
	const Matrix4x4& setScale(const Vector3f& s);
	const Matrix4x4& scale(const Vector3f& s);
	const Matrix4x4& setfloatranslation(const Vector3f& t);
	const Vector3f translation() const;
	const Matrix4x4& translate(const Vector3f& t);
	const Matrix4x4& setShear(const Vector3f& h);
	const Matrix4x4& shear(const Vector3f& h);
	static unsigned int dimensions() { return 4; }

private:

	template <typename R, typename S>
	struct isSamefloatype
	{
		enum { value = 0 };
	};

	template <typename R>
	struct isSamefloatype<R, R>
	{
		enum { value = 1 };
	};
};

#endif

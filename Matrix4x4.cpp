#include "pch.h"
#include "Matrix4x4.h"

/* inline */
float*
Matrix4x4::operator [](int i)
{
	return x[i];
}


/* inline */
const float*
Matrix4x4::operator [](int i) const
{
	return x[i];
}


/* inline */
Matrix4x4::Matrix4x4()
{
	memset(x, 0, sizeof(x));
	x[0][0] = 1;
	x[1][1] = 1;
	x[2][2] = 1;
	x[3][3] = 1;
}


/* inline */
Matrix4x4::Matrix4x4(float a)
{
	x[0][0] = a;
	x[0][1] = a;
	x[0][2] = a;
	x[0][3] = a;
	x[1][0] = a;
	x[1][1] = a;
	x[1][2] = a;
	x[1][3] = a;
	x[2][0] = a;
	x[2][1] = a;
	x[2][2] = a;
	x[2][3] = a;
	x[3][0] = a;
	x[3][1] = a;
	x[3][2] = a;
	x[3][3] = a;
}


/* inline */
Matrix4x4::Matrix4x4(const float a[4][4])
{
	memcpy(x, a, sizeof(x));
}


/* inline */
Matrix4x4::Matrix4x4(float a, float b, float c, float d, float e, float f, float g, float h,
                     float i, float j, float k, float l, float m, float n, float o, float p)
{
	x[0][0] = a;
	x[0][1] = b;
	x[0][2] = c;
	x[0][3] = d;
	x[1][0] = e;
	x[1][1] = f;
	x[1][2] = g;
	x[1][3] = h;
	x[2][0] = i;
	x[2][1] = j;
	x[2][2] = k;
	x[2][3] = l;
	x[3][0] = m;
	x[3][1] = n;
	x[3][2] = o;
	x[3][3] = p;
}

Matrix4x4::~Matrix4x4()
{
}


/* inline */
Matrix4x4::Matrix4x4(const Matrix4x4& v)
{
	x[0][0] = v.x[0][0];
	x[0][1] = v.x[0][1];
	x[0][2] = v.x[0][2];
	x[0][3] = v.x[0][3];
	x[1][0] = v.x[1][0];
	x[1][1] = v.x[1][1];
	x[1][2] = v.x[1][2];
	x[1][3] = v.x[1][3];
	x[2][0] = v.x[2][0];
	x[2][1] = v.x[2][1];
	x[2][2] = v.x[2][2];
	x[2][3] = v.x[2][3];
	x[3][0] = v.x[3][0];
	x[3][1] = v.x[3][1];
	x[3][2] = v.x[3][2];
	x[3][3] = v.x[3][3];
}


/* inline */
const Matrix4x4&
Matrix4x4::operator =(const Matrix4x4& v)
{
	x[0][0] = v.x[0][0];
	x[0][1] = v.x[0][1];
	x[0][2] = v.x[0][2];
	x[0][3] = v.x[0][3];
	x[1][0] = v.x[1][0];
	x[1][1] = v.x[1][1];
	x[1][2] = v.x[1][2];
	x[1][3] = v.x[1][3];
	x[2][0] = v.x[2][0];
	x[2][1] = v.x[2][1];
	x[2][2] = v.x[2][2];
	x[2][3] = v.x[2][3];
	x[3][0] = v.x[3][0];
	x[3][1] = v.x[3][1];
	x[3][2] = v.x[3][2];
	x[3][3] = v.x[3][3];
	return *this;
}


/* inline */
const Matrix4x4&
Matrix4x4::operator =(float a)
{
	x[0][0] = a;
	x[0][1] = a;
	x[0][2] = a;
	x[0][3] = a;
	x[1][0] = a;
	x[1][1] = a;
	x[1][2] = a;
	x[1][3] = a;
	x[2][0] = a;
	x[2][1] = a;
	x[2][2] = a;
	x[2][3] = a;
	x[3][0] = a;
	x[3][1] = a;
	x[3][2] = a;
	x[3][3] = a;
	return *this;
}


/* inline */
float*
Matrix4x4::getValue()
{
	return static_cast<float*>(&x[0][0]);
}


/* inline */
const float*
Matrix4x4::getValue() const
{
	return static_cast<const float*>(&x[0][0]);
}


/* inline */
void
Matrix4x4::getValue(Matrix4x4& v) const
{
	if constexpr (isSamefloatype<float, float>::value)
	{
		memcpy(v.x, x, sizeof(x));
	}
	else
	{
		v.x[0][0] = x[0][0];
		v.x[0][1] = x[0][1];
		v.x[0][2] = x[0][2];
		v.x[0][3] = x[0][3];
		v.x[1][0] = x[1][0];
		v.x[1][1] = x[1][1];
		v.x[1][2] = x[1][2];
		v.x[1][3] = x[1][3];
		v.x[2][0] = x[2][0];
		v.x[2][1] = x[2][1];
		v.x[2][2] = x[2][2];
		v.x[2][3] = x[2][3];
		v.x[3][0] = x[3][0];
		v.x[3][1] = x[3][1];
		v.x[3][2] = x[3][2];
		v.x[3][3] = x[3][3];
	}
}


/* inline */
Matrix4x4&
Matrix4x4::setValue(const Matrix4x4& v)
{
	if constexpr (isSamefloatype<float, float>::value)
	{
		memcpy(x, v.x, sizeof(x));
	}
	else
	{
		x[0][0] = v.x[0][0];
		x[0][1] = v.x[0][1];
		x[0][2] = v.x[0][2];
		x[0][3] = v.x[0][3];
		x[1][0] = v.x[1][0];
		x[1][1] = v.x[1][1];
		x[1][2] = v.x[1][2];
		x[1][3] = v.x[1][3];
		x[2][0] = v.x[2][0];
		x[2][1] = v.x[2][1];
		x[2][2] = v.x[2][2];
		x[2][3] = v.x[2][3];
		x[3][0] = v.x[3][0];
		x[3][1] = v.x[3][1];
		x[3][2] = v.x[3][2];
		x[3][3] = v.x[3][3];
	}

	return *this;
}


/* inline */
Matrix4x4&
Matrix4x4::setfloatheMatrix(const Matrix4x4& v)
{
	if constexpr (isSamefloatype<float, float>::value)
	{
		memcpy(x, v.x, sizeof(x));
	}
	else
	{
		x[0][0] = v.x[0][0];
		x[0][1] = v.x[0][1];
		x[0][2] = v.x[0][2];
		x[0][3] = v.x[0][3];
		x[1][0] = v.x[1][0];
		x[1][1] = v.x[1][1];
		x[1][2] = v.x[1][2];
		x[1][3] = v.x[1][3];
		x[2][0] = v.x[2][0];
		x[2][1] = v.x[2][1];
		x[2][2] = v.x[2][2];
		x[2][3] = v.x[2][3];
		x[3][0] = v.x[3][0];
		x[3][1] = v.x[3][1];
		x[3][2] = v.x[3][2];
		x[3][3] = v.x[3][3];
	}

	return *this;
}


/* inline */
void
Matrix4x4::makeIdentity()
{
	memset(x, 0, sizeof(x));
	x[0][0] = 1;
	x[1][1] = 1;
	x[2][2] = 1;
	x[3][3] = 1;
}


bool
Matrix4x4::operator ==(const Matrix4x4& v) const
{
	return x[0][0] == v.x[0][0] &&
		x[0][1] == v.x[0][1] &&
		x[0][2] == v.x[0][2] &&
		x[0][3] == v.x[0][3] &&
		x[1][0] == v.x[1][0] &&
		x[1][1] == v.x[1][1] &&
		x[1][2] == v.x[1][2] &&
		x[1][3] == v.x[1][3] &&
		x[2][0] == v.x[2][0] &&
		x[2][1] == v.x[2][1] &&
		x[2][2] == v.x[2][2] &&
		x[2][3] == v.x[2][3] &&
		x[3][0] == v.x[3][0] &&
		x[3][1] == v.x[3][1] &&
		x[3][2] == v.x[3][2] &&
		x[3][3] == v.x[3][3];
}


bool
Matrix4x4::operator !=(const Matrix4x4& v) const
{
	return x[0][0] != v.x[0][0] ||
		x[0][1] != v.x[0][1] ||
		x[0][2] != v.x[0][2] ||
		x[0][3] != v.x[0][3] ||
		x[1][0] != v.x[1][0] ||
		x[1][1] != v.x[1][1] ||
		x[1][2] != v.x[1][2] ||
		x[1][3] != v.x[1][3] ||
		x[2][0] != v.x[2][0] ||
		x[2][1] != v.x[2][1] ||
		x[2][2] != v.x[2][2] ||
		x[2][3] != v.x[2][3] ||
		x[3][0] != v.x[3][0] ||
		x[3][1] != v.x[3][1] ||
		x[3][2] != v.x[3][2] ||
		x[3][3] != v.x[3][3];
}


bool
Matrix4x4::equalWithAbsError(const Matrix4x4& m, float e) const
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			auto x1 = (*this)[i][j];
			auto x2 = m[i][j];

			if (!(((x1 > x2) ? x1 - x2 : x2 - x1) <= e))
			{
				return false;
			}
		}

	return true;
}


bool
Matrix4x4::equalWithRelError(const Matrix4x4& m, float e) const
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			auto x1 = (*this)[i][j];
			auto x2 = m[i][j];

			if (!(((x1 > x2) ? x1 - x2 : x2 - x1) <= e * ((x1 > 0) ? x1 : -x1)))
			{
				return false;
			}
		}

	return true;
}


const Matrix4x4&
Matrix4x4::operator +=(const Matrix4x4& v)
{
	x[0][0] += v.x[0][0];
	x[0][1] += v.x[0][1];
	x[0][2] += v.x[0][2];
	x[0][3] += v.x[0][3];
	x[1][0] += v.x[1][0];
	x[1][1] += v.x[1][1];
	x[1][2] += v.x[1][2];
	x[1][3] += v.x[1][3];
	x[2][0] += v.x[2][0];
	x[2][1] += v.x[2][1];
	x[2][2] += v.x[2][2];
	x[2][3] += v.x[2][3];
	x[3][0] += v.x[3][0];
	x[3][1] += v.x[3][1];
	x[3][2] += v.x[3][2];
	x[3][3] += v.x[3][3];

	return *this;
}


const Matrix4x4&
Matrix4x4::operator +=(float a)
{
	x[0][0] += a;
	x[0][1] += a;
	x[0][2] += a;
	x[0][3] += a;
	x[1][0] += a;
	x[1][1] += a;
	x[1][2] += a;
	x[1][3] += a;
	x[2][0] += a;
	x[2][1] += a;
	x[2][2] += a;
	x[2][3] += a;
	x[3][0] += a;
	x[3][1] += a;
	x[3][2] += a;
	x[3][3] += a;

	return *this;
}


Matrix4x4
Matrix4x4::operator +(const Matrix4x4& v) const
{
	return Matrix4x4(x[0][0] + v.x[0][0],
	                 x[0][1] + v.x[0][1],
	                 x[0][2] + v.x[0][2],
	                 x[0][3] + v.x[0][3],
	                 x[1][0] + v.x[1][0],
	                 x[1][1] + v.x[1][1],
	                 x[1][2] + v.x[1][2],
	                 x[1][3] + v.x[1][3],
	                 x[2][0] + v.x[2][0],
	                 x[2][1] + v.x[2][1],
	                 x[2][2] + v.x[2][2],
	                 x[2][3] + v.x[2][3],
	                 x[3][0] + v.x[3][0],
	                 x[3][1] + v.x[3][1],
	                 x[3][2] + v.x[3][2],
	                 x[3][3] + v.x[3][3]);
}


const Matrix4x4&
Matrix4x4::operator -=(const Matrix4x4& v)
{
	x[0][0] -= v.x[0][0];
	x[0][1] -= v.x[0][1];
	x[0][2] -= v.x[0][2];
	x[0][3] -= v.x[0][3];
	x[1][0] -= v.x[1][0];
	x[1][1] -= v.x[1][1];
	x[1][2] -= v.x[1][2];
	x[1][3] -= v.x[1][3];
	x[2][0] -= v.x[2][0];
	x[2][1] -= v.x[2][1];
	x[2][2] -= v.x[2][2];
	x[2][3] -= v.x[2][3];
	x[3][0] -= v.x[3][0];
	x[3][1] -= v.x[3][1];
	x[3][2] -= v.x[3][2];
	x[3][3] -= v.x[3][3];

	return *this;
}


const Matrix4x4&
Matrix4x4::operator -=(float a)
{
	x[0][0] -= a;
	x[0][1] -= a;
	x[0][2] -= a;
	x[0][3] -= a;
	x[1][0] -= a;
	x[1][1] -= a;
	x[1][2] -= a;
	x[1][3] -= a;
	x[2][0] -= a;
	x[2][1] -= a;
	x[2][2] -= a;
	x[2][3] -= a;
	x[3][0] -= a;
	x[3][1] -= a;
	x[3][2] -= a;
	x[3][3] -= a;

	return *this;
}


Matrix4x4
Matrix4x4::operator -(const Matrix4x4& v) const
{
	return Matrix4x4(x[0][0] - v.x[0][0],
	                 x[0][1] - v.x[0][1],
	                 x[0][2] - v.x[0][2],
	                 x[0][3] - v.x[0][3],
	                 x[1][0] - v.x[1][0],
	                 x[1][1] - v.x[1][1],
	                 x[1][2] - v.x[1][2],
	                 x[1][3] - v.x[1][3],
	                 x[2][0] - v.x[2][0],
	                 x[2][1] - v.x[2][1],
	                 x[2][2] - v.x[2][2],
	                 x[2][3] - v.x[2][3],
	                 x[3][0] - v.x[3][0],
	                 x[3][1] - v.x[3][1],
	                 x[3][2] - v.x[3][2],
	                 x[3][3] - v.x[3][3]);
}


Matrix4x4
Matrix4x4::operator -() const
{
	return Matrix4x4(-x[0][0],
	                 -x[0][1],
	                 -x[0][2],
	                 -x[0][3],
	                 -x[1][0],
	                 -x[1][1],
	                 -x[1][2],
	                 -x[1][3],
	                 -x[2][0],
	                 -x[2][1],
	                 -x[2][2],
	                 -x[2][3],
	                 -x[3][0],
	                 -x[3][1],
	                 -x[3][2],
	                 -x[3][3]);
}


const Matrix4x4&
Matrix4x4::negate()
{
	x[0][0] = -x[0][0];
	x[0][1] = -x[0][1];
	x[0][2] = -x[0][2];
	x[0][3] = -x[0][3];
	x[1][0] = -x[1][0];
	x[1][1] = -x[1][1];
	x[1][2] = -x[1][2];
	x[1][3] = -x[1][3];
	x[2][0] = -x[2][0];
	x[2][1] = -x[2][1];
	x[2][2] = -x[2][2];
	x[2][3] = -x[2][3];
	x[3][0] = -x[3][0];
	x[3][1] = -x[3][1];
	x[3][2] = -x[3][2];
	x[3][3] = -x[3][3];

	return *this;
}


const Matrix4x4&
Matrix4x4::operator *=(float a)
{
	x[0][0] *= a;
	x[0][1] *= a;
	x[0][2] *= a;
	x[0][3] *= a;
	x[1][0] *= a;
	x[1][1] *= a;
	x[1][2] *= a;
	x[1][3] *= a;
	x[2][0] *= a;
	x[2][1] *= a;
	x[2][2] *= a;
	x[2][3] *= a;
	x[3][0] *= a;
	x[3][1] *= a;
	x[3][2] *= a;
	x[3][3] *= a;

	return *this;
}


Matrix4x4
Matrix4x4::operator *(float a) const
{
	return Matrix4x4(x[0][0] * a,
	                 x[0][1] * a,
	                 x[0][2] * a,
	                 x[0][3] * a,
	                 x[1][0] * a,
	                 x[1][1] * a,
	                 x[1][2] * a,
	                 x[1][3] * a,
	                 x[2][0] * a,
	                 x[2][1] * a,
	                 x[2][2] * a,
	                 x[2][3] * a,
	                 x[3][0] * a,
	                 x[3][1] * a,
	                 x[3][2] * a,
	                 x[3][3] * a);
}


/* inline */
Matrix4x4
operator *(float a, const Matrix4x4& v)
{
	return v * a;
}


/* inline */
const Matrix4x4&
Matrix4x4::operator *=(const Matrix4x4& v)
{
	Matrix4x4 tmp(float(0));

	multiply(*this, v, tmp);
	*this = tmp;
	return *this;
}


/* inline */
Matrix4x4
Matrix4x4::operator *(const Matrix4x4& v) const
{
	Matrix4x4 tmp(float(0));

	multiply(*this, v, tmp);
	return tmp;
}


void
Matrix4x4::multiply(const Matrix4x4& a, const Matrix4x4& b, Matrix4x4& c)
{
	const float* ap = &a.x[0][0];
	const float* bp = &b.x[0][0];
	float* cp = &c.x[0][0];

	float a0, a1, a2, a3;

	a0 = ap[0];
	a1 = ap[1];
	a2 = ap[2];
	a3 = ap[3];

	cp[0] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
	cp[1] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
	cp[2] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
	cp[3] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

	a0 = ap[4];
	a1 = ap[5];
	a2 = ap[6];
	a3 = ap[7];

	cp[4] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
	cp[5] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
	cp[6] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
	cp[7] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

	a0 = ap[8];
	a1 = ap[9];
	a2 = ap[10];
	a3 = ap[11];

	cp[8] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
	cp[9] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
	cp[10] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
	cp[11] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

	a0 = ap[12];
	a1 = ap[13];
	a2 = ap[14];
	a3 = ap[15];

	cp[12] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
	cp[13] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
	cp[14] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
	cp[15] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];
}


void
Matrix4x4::multVecMatrix(const Vector3f& src, Vector3f& dst) const
{
	float a, b, c, w;

	a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0];
	b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1];
	c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2];
	w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3];

	dst.x = a / w;
	dst.y = b / w;
	dst.z = c / w;
}


void
Matrix4x4::multDirMatrix(const Vector3f& src, Vector3f& dst) const
{
	float a, b, c;

	a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0];
	b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1];
	c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2];

	dst.x = a;
	dst.y = b;
	dst.z = c;
}


const Matrix4x4&
Matrix4x4::operator /=(float a)
{
	x[0][0] /= a;
	x[0][1] /= a;
	x[0][2] /= a;
	x[0][3] /= a;
	x[1][0] /= a;
	x[1][1] /= a;
	x[1][2] /= a;
	x[1][3] /= a;
	x[2][0] /= a;
	x[2][1] /= a;
	x[2][2] /= a;
	x[2][3] /= a;
	x[3][0] /= a;
	x[3][1] /= a;
	x[3][2] /= a;
	x[3][3] /= a;

	return *this;
}


Matrix4x4
Matrix4x4::operator /(float a) const
{
	return Matrix4x4(x[0][0] / a,
	                 x[0][1] / a,
	                 x[0][2] / a,
	                 x[0][3] / a,
	                 x[1][0] / a,
	                 x[1][1] / a,
	                 x[1][2] / a,
	                 x[1][3] / a,
	                 x[2][0] / a,
	                 x[2][1] / a,
	                 x[2][2] / a,
	                 x[2][3] / a,
	                 x[3][0] / a,
	                 x[3][1] / a,
	                 x[3][2] / a,
	                 x[3][3] / a);
}


const Matrix4x4&
Matrix4x4::transpose()
{
	Matrix4x4 tmp(x[0][0],
	              x[1][0],
	              x[2][0],
	              x[3][0],
	              x[0][1],
	              x[1][1],
	              x[2][1],
	              x[3][1],
	              x[0][2],
	              x[1][2],
	              x[2][2],
	              x[3][2],
	              x[0][3],
	              x[1][3],
	              x[2][3],
	              x[3][3]);
	*this = tmp;
	return *this;
}


Matrix4x4
Matrix4x4::transposed() const
{
	return Matrix4x4(x[0][0],
	                 x[1][0],
	                 x[2][0],
	                 x[3][0],
	                 x[0][1],
	                 x[1][1],
	                 x[2][1],
	                 x[3][1],
	                 x[0][2],
	                 x[1][2],
	                 x[2][2],
	                 x[3][2],
	                 x[0][3],
	                 x[1][3],
	                 x[2][3],
	                 x[3][3]);
}


const Matrix4x4&
Matrix4x4::gjInvert(bool singExc)
{
	*this = gjInverse(singExc);
	return *this;
}


Matrix4x4
Matrix4x4::gjInverse(bool singExc) const
{
	int i, j, k;
	Matrix4x4 s;
	Matrix4x4 t(*this);


	for (i = 0; i < 3; i++)
	{
		int pivot = i;

		float pivotsize = t[i][i];

		if (pivotsize < 0)
			pivotsize = -pivotsize;

		for (j = i + 1; j < 4; j++)
		{
			float tmp = t[j][i];

			if (tmp < 0)
				tmp = -tmp;

			if (tmp > pivotsize)
			{
				pivot = j;
				pivotsize = tmp;
			}
		}

		if (pivotsize == 0)
		{
			if (singExc)
			{
			}

			return Matrix4x4();
		}

		if (pivot != i)
		{
			for (j = 0; j < 4; j++)
			{
				float tmp;

				tmp = t[i][j];
				t[i][j] = t[pivot][j];
				t[pivot][j] = tmp;

				tmp = s[i][j];
				s[i][j] = s[pivot][j];
				s[pivot][j] = tmp;
			}
		}

		for (j = i + 1; j < 4; j++)
		{
			float f = t[j][i] / t[i][i];

			for (k = 0; k < 4; k++)
			{
				t[j][k] -= f * t[i][k];
				s[j][k] -= f * s[i][k];
			}
		}
	}


	for (i = 3; i >= 0; --i)
	{
		float f;

		if ((f = t[i][i]) == 0)
		{
			if (singExc)
			{
			}

			return Matrix4x4();
		}

		for (j = 0; j < 4; j++)
		{
			t[i][j] /= f;
			s[i][j] /= f;
		}

		for (j = 0; j < i; j++)
		{
			f = t[j][i];

			for (k = 0; k < 4; k++)
			{
				t[j][k] -= f * t[i][k];
				s[j][k] -= f * s[i][k];
			}
		}
	}

	return s;
}


const Matrix4x4&
Matrix4x4::invert(bool singExc)
{
	*this = inverse(singExc);
	return *this;
}

Matrix4x4
Matrix4x4::inverse(bool singExc) const
{
	if (x[0][3] != 0 || x[1][3] != 0 || x[2][3] != 0 || x[3][3] != 1)
		return gjInverse(singExc);

	Matrix4x4 s(x[1][1] * x[2][2] - x[2][1] * x[1][2],
	            x[2][1] * x[0][2] - x[0][1] * x[2][2],
	            x[0][1] * x[1][2] - x[1][1] * x[0][2],
	            0,

	            x[2][0] * x[1][2] - x[1][0] * x[2][2],
	            x[0][0] * x[2][2] - x[2][0] * x[0][2],
	            x[1][0] * x[0][2] - x[0][0] * x[1][2],
	            0,

	            x[1][0] * x[2][1] - x[2][0] * x[1][1],
	            x[2][0] * x[0][1] - x[0][0] * x[2][1],
	            x[0][0] * x[1][1] - x[1][0] * x[0][1],
	            0,

	            0,
	            0,
	            0,
	            1);

	float r = x[0][0] * s[0][0] + x[0][1] * s[1][0] + x[0][2] * s[2][0];

	if (abs(r) >= 1)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				s[i][j] /= r;
			}
		}
	}
	else
	{
		float mr = abs(r) / FLT_MIN;

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (mr > abs(s[i][j]))
				{
					s[i][j] /= r;
				}
				else
				{
					if (singExc)
					{
					}

					return Matrix4x4();
				}
			}
		}
	}

	s[3][0] = -x[3][0] * s[0][0] - x[3][1] * s[1][0] - x[3][2] * s[2][0];
	s[3][1] = -x[3][0] * s[0][1] - x[3][1] * s[1][1] - x[3][2] * s[2][1];
	s[3][2] = -x[3][0] * s[0][2] - x[3][1] * s[1][2] - x[3][2] * s[2][2];

	return s;
}


/* inline */
float
Matrix4x4::fastMinor(const int r0, const int r1, const int r2,
                     const int c0, const int c1, const int c2) const
{
	return x[r0][c0] * (x[r1][c1] * x[r2][c2] - x[r1][c2] * x[r2][c1])
		+ x[r0][c1] * (x[r1][c2] * x[r2][c0] - x[r1][c0] * x[r2][c2])
		+ x[r0][c2] * (x[r1][c0] * x[r2][c1] - x[r1][c1] * x[r2][c0]);
}


/* inline */
float
Matrix4x4::determinant() const
{
	float sum = static_cast<float>(0);

	if (x[0][3] != 0.) sum -= x[0][3] * fastMinor(1, 2, 3, 0, 1, 2);
	if (x[1][3] != 0.) sum += x[1][3] * fastMinor(0, 2, 3, 0, 1, 2);
	if (x[2][3] != 0.) sum -= x[2][3] * fastMinor(0, 1, 3, 0, 1, 2);
	if (x[3][3] != 0.) sum += x[3][3] * fastMinor(0, 1, 2, 0, 1, 2);

	return sum;
}


const Matrix4x4&
Matrix4x4::setEulerAngles(const Vector3f& r)
{
	float cos_rz, sin_rz, cos_ry, sin_ry, cos_rx, sin_rx;

	cos_rz = cosf(r[2]);
	cos_ry = cosf(r[1]);
	cos_rx = cosf(r[0]);

	sin_rz = sinf(r[2]);
	sin_ry = sinf(r[1]);
	sin_rx = sinf(r[0]);

	x[0][0] = cos_rz * cos_ry;
	x[0][1] = sin_rz * cos_ry;
	x[0][2] = -sin_ry;
	x[0][3] = 0;

	x[1][0] = -sin_rz * cos_rx + cos_rz * sin_ry * sin_rx;
	x[1][1] = cos_rz * cos_rx + sin_rz * sin_ry * sin_rx;
	x[1][2] = cos_ry * sin_rx;
	x[1][3] = 0;

	x[2][0] = sin_rz * sin_rx + cos_rz * sin_ry * cos_rx;
	x[2][1] = -cos_rz * sin_rx + sin_rz * sin_ry * cos_rx;
	x[2][2] = cos_ry * cos_rx;
	x[2][3] = 0;

	x[3][0] = 0;
	x[3][1] = 0;
	x[3][2] = 0;
	x[3][3] = 1;

	return *this;
}


const Matrix4x4&
Matrix4x4::rotate(const Vector3f& r)
{
	float cos_rz, sin_rz, cos_ry, sin_ry, cos_rx, sin_rx;
	float m00, m01, m02;
	float m10, m11, m12;
	float m20, m21, m22;

	cos_rz = cosf(r[2]);
	cos_ry = cosf(r[1]);
	cos_rx = cosf(r[0]);

	sin_rz = sinf(r[2]);
	sin_ry = sinf(r[1]);
	sin_rx = sinf(r[0]);

	m00 = cos_rz * cos_ry;
	m01 = sin_rz * cos_ry;
	m02 = -sin_ry;
	m10 = -sin_rz * cos_rx + cos_rz * sin_ry * sin_rx;
	m11 = cos_rz * cos_rx + sin_rz * sin_ry * sin_rx;
	m12 = cos_ry * sin_rx;
	m20 = -sin_rz * -sin_rx + cos_rz * sin_ry * cos_rx;
	m21 = cos_rz * -sin_rx + sin_rz * sin_ry * cos_rx;
	m22 = cos_ry * cos_rx;

	Matrix4x4 P(*this);

	x[0][0] = P[0][0] * m00 + P[1][0] * m01 + P[2][0] * m02;
	x[0][1] = P[0][1] * m00 + P[1][1] * m01 + P[2][1] * m02;
	x[0][2] = P[0][2] * m00 + P[1][2] * m01 + P[2][2] * m02;
	x[0][3] = P[0][3] * m00 + P[1][3] * m01 + P[2][3] * m02;

	x[1][0] = P[0][0] * m10 + P[1][0] * m11 + P[2][0] * m12;
	x[1][1] = P[0][1] * m10 + P[1][1] * m11 + P[2][1] * m12;
	x[1][2] = P[0][2] * m10 + P[1][2] * m11 + P[2][2] * m12;
	x[1][3] = P[0][3] * m10 + P[1][3] * m11 + P[2][3] * m12;

	x[2][0] = P[0][0] * m20 + P[1][0] * m21 + P[2][0] * m22;
	x[2][1] = P[0][1] * m20 + P[1][1] * m21 + P[2][1] * m22;
	x[2][2] = P[0][2] * m20 + P[1][2] * m21 + P[2][2] * m22;
	x[2][3] = P[0][3] * m20 + P[1][3] * m21 + P[2][3] * m22;

	return *this;
}


const Matrix4x4&
Matrix4x4::setScale(float s)
{
	memset(x, 0, sizeof(x));
	x[0][0] = s;
	x[1][1] = s;
	x[2][2] = s;
	x[3][3] = 1;

	return *this;
}


const Matrix4x4&
Matrix4x4::setScale(const Vector3f& s)
{
	memset(x, 0, sizeof(x));
	x[0][0] = s[0];
	x[1][1] = s[1];
	x[2][2] = s[2];
	x[3][3] = 1;

	return *this;
}


const Matrix4x4&
Matrix4x4::scale(const Vector3f& s)
{
	x[0][0] *= s[0];
	x[0][1] *= s[0];
	x[0][2] *= s[0];
	x[0][3] *= s[0];

	x[1][0] *= s[1];
	x[1][1] *= s[1];
	x[1][2] *= s[1];
	x[1][3] *= s[1];

	x[2][0] *= s[2];
	x[2][1] *= s[2];
	x[2][2] *= s[2];
	x[2][3] *= s[2];

	return *this;
}


const Matrix4x4&
Matrix4x4::setfloatranslation(const Vector3f& t)
{
	x[0][0] = 1;
	x[0][1] = 0;
	x[0][2] = 0;
	x[0][3] = 0;

	x[1][0] = 0;
	x[1][1] = 1;
	x[1][2] = 0;
	x[1][3] = 0;

	x[2][0] = 0;
	x[2][1] = 0;
	x[2][2] = 1;
	x[2][3] = 0;

	x[3][0] = t[0];
	x[3][1] = t[1];
	x[3][2] = t[2];
	x[3][3] = 1;

	return *this;
}


/* inline */
const Vector3f
Matrix4x4::translation() const
{
	return Vector3f(x[3][0], x[3][1], x[3][2]);
}


const Matrix4x4&
Matrix4x4::translate(const Vector3f& t)
{
	x[3][0] += t[0] * x[0][0] + t[1] * x[1][0] + t[2] * x[2][0];
	x[3][1] += t[0] * x[0][1] + t[1] * x[1][1] + t[2] * x[2][1];
	x[3][2] += t[0] * x[0][2] + t[1] * x[1][2] + t[2] * x[2][2];
	x[3][3] += t[0] * x[0][3] + t[1] * x[1][3] + t[2] * x[2][3];

	return *this;
}


const Matrix4x4&
Matrix4x4::setShear(const Vector3f& h)
{
	x[0][0] = 1;
	x[0][1] = 0;
	x[0][2] = 0;
	x[0][3] = 0;

	x[1][0] = h[0];
	x[1][1] = 1;
	x[1][2] = 0;
	x[1][3] = 0;

	x[2][0] = h[1];
	x[2][1] = h[2];
	x[2][2] = 1;
	x[2][3] = 0;

	x[3][0] = 0;
	x[3][1] = 0;
	x[3][2] = 0;
	x[3][3] = 1;

	return *this;
}


const Matrix4x4&
Matrix4x4::shear(const Vector3f& h)
{
	for (int i = 0; i < 4; i++)
	{
		x[2][i] += h[1] * x[0][i] + h[2] * x[1][i];
		x[1][i] += h[0] * x[0][i];
	}

	return *this;
}

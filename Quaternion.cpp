#include "pch.h"
#include "Quaternion.h"
#include <algorithm>


/*  inline */
Quaternion::Quaternion() : r(1), v(0, 0, 0)
{
}


/*  inline */
Quaternion::Quaternion(const Quaternion& q) : r(q.r), v(q.v)
{
}

Quaternion::~Quaternion()
{
}


/*  inline */
Quaternion::Quaternion(float s, float i, float j, float k) : r(s), v(i, j, k)
{
}


/*  inline */
Quaternion::Quaternion(float s, Vector3f d) : r(s), v(d)
{
}


/* inline */
Quaternion
Quaternion::identity()
{
	return Quaternion();
}


/* inline */
const Quaternion&
Quaternion::operator =(const Quaternion& q)
{
	r = q.r;
	v = q.v;
	return *this;
}


/* inline */
const Quaternion&
Quaternion::operator *=(const Quaternion& q)
{
	float rtmp = r * q.r - (v ^ q.v);
	v = r * q.v + v * q.r + v % q.v;
	r = rtmp;
	return *this;
}


/* inline */
const Quaternion&
Quaternion::operator *=(float t)
{
	r *= t;
	v *= t;
	return *this;
}


/* inline */
const Quaternion&
Quaternion::operator /=(const Quaternion& q)
{
	*this = *this * q.inverse();
	return *this;
}


/* inline */
const Quaternion&
Quaternion::operator /=(float t)
{
	r /= t;
	v /= t;
	return *this;
}


/* inline */
const Quaternion&
Quaternion::operator +=(const Quaternion& q)
{
	r += q.r;
	v += q.v;
	return *this;
}


/* inline */
const Quaternion&
Quaternion::operator -=(const Quaternion& q)
{
	r -= q.r;
	v -= q.v;
	return *this;
}


/* inline */
float&
Quaternion::operator [](int index)
{
	return index ? v[index - 1] : r;
}


/* inline */
float
Quaternion::operator [](int index) const
{
	return index ? v[index - 1] : r;
}


/* inline */
bool
Quaternion::operator ==(const Quaternion& q) const
{
	return r == q.r && v == q.v;
}


/* inline */
bool
Quaternion::operator !=(const Quaternion& q) const
{
	return r != q.r || v != q.v;
}


/* inline */
float
operator ^(const Quaternion& q1, const Quaternion& q2)
{
	return q1.r * q2.r + (q1.v ^ q2.v);
}


/* inline */
float
Quaternion::length() const
{
	return sqrtf(r * r + (v ^ v));
}


/* inline */
Quaternion&
Quaternion::normalize()
{
	if (float l = length())
	{
		r /= l;
		v /= l;
	}
	else
	{
		r = 1;
		v = Vector3f(0);
	}

	return *this;
}


/* inline */
Quaternion
Quaternion::normalized() const
{
	if (float l = length())
		return Quaternion(r / l, v / l);

	return Quaternion();
}


/* inline */
Quaternion
Quaternion::inverse() const
{
	float qdot = *this ^ *this;
	return Quaternion(r / qdot, -v / qdot);
}


/* inline */
Quaternion&
Quaternion::invert()
{
	float qdot = (*this) ^ (*this);
	r /= qdot;
	v = -v / qdot;
	return *this;
}


/* inline */
Vector3f
Quaternion::rotateVector(const Vector3f& original) const
{
	Quaternion vec(0, original);
	Quaternion inv(*this);
	inv.v *= -1;
	Quaternion result = *this * vec * inv;
	return result.v;
}


/* inline */
float
Quaternion::euclideanInnerProduct(const Quaternion& q) const
{
	return r * q.r + v.x * q.v.x + v.y * q.v.y + v.z * q.v.z;
}


/* inline */
Quaternion
Quaternion::log() const
{
	float theta = acosf((std::min)(r, static_cast<float>(1.0)));

	if (theta == 0)
		return Quaternion(0, v);

	float sintheta = sinf(theta);

	float k;
	if (abs(sintheta) < 1 && abs(theta) >= FLT_MAX * abs(sintheta))
		k = 1;
	else
		k = theta / sintheta;

	return Quaternion(static_cast<float>(0), v.x * k, v.y * k, v.z * k);
}


/* inline */
Quaternion
Quaternion::exp() const
{
	float theta = v.length();
	float sintheta = sinf(theta);

	float k;
	if (abs(theta) < 1 && abs(sintheta) >= FLT_MAX * abs(theta))
		k = 1;
	else
		k = sintheta / theta;

	float costheta = cosf(theta);

	return Quaternion(costheta, v.x * k, v.y * k, v.z * k);
}


/* inline */
float
Quaternion::angle() const
{
	return 2 * atan2f(v.length(), r);
}


/* inline */
Vector3f
Quaternion::axis() const
{
	return v.normalized();
}


/* inline */
Quaternion&
Quaternion::setAxisAngle(const Vector3f& axis, float radians)
{
	r = cosf(radians / 2);
	v = axis.normalized() * sinf(radians / 2);
	return *this;
}


Quaternion&
Quaternion::setRotation(const Vector3f& from, const Vector3f& to)
{
	Vector3f f0 = from.normalized();
	Vector3f t0 = to.normalized();

	if ((f0 ^ t0) >= 0)
	{
		setRotationInternal(f0, t0, *this);
	}
	else
	{
		Vector3f h0 = (f0 + t0).normalized();

		if ((h0 ^ h0) != 0)
		{
			setRotationInternal(f0, h0, *this);

			Quaternion q;
			setRotationInternal(h0, t0, q);

			*this *= q;
		}
		else
		{
			r = float(0);

			Vector3f f02 = f0 * f0;

			if (f02.x <= f02.y && f02.x <= f02.z)
				v = (f0 % Vector3f(1, 0, 0)).normalized();
			else if (f02.y <= f02.z)
				v = (f0 % Vector3f(0, 1, 0)).normalized();
			else
				v = (f0 % Vector3f(0, 0, 1)).normalized();
		}
	}

	return *this;
}


void
Quaternion::setRotationInternal(const Vector3f& f0, const Vector3f& t0, Quaternion& q)
{
	Vector3f h0 = (f0 + t0).normalized();


	q.r = f0 ^ h0;
	q.v = f0 % h0;
}


Matrix4x4
Quaternion::toMatrix44() const
{
	return Matrix4x4(1 - 2 * (v.y * v.y + v.z * v.z),
	                 2 * (v.x * v.y + v.z * r),
	                 2 * (v.z * v.x - v.y * r),
	                 0,
	                 2 * (v.x * v.y - v.z * r),
	                 1 - 2 * (v.z * v.z + v.x * v.x),
	                 2 * (v.y * v.z + v.x * r),
	                 0,
	                 2 * (v.z * v.x + v.y * r),
	                 2 * (v.y * v.z - v.x * r),
	                 1 - 2 * (v.y * v.y + v.x * v.x),
	                 0,
	                 0,
	                 0,
	                 0,
	                 1);
}

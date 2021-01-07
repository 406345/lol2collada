#include "pch.h"
#include "Euler.h"

#include <ostream>

/* inline */
void
Euler::angleOrder(int& i, int& j, int& k) const
{
	i = _initialAxis;
	j = _parityEven ? (i + 1) % 3 : (i > 0 ? i - 1 : 2);
	k = _parityEven ? (i > 0 ? i - 1 : 2) : (i + 1) % 3;
}


/* inline */
void
Euler::angleMapping(int& i, int& j, int& k) const
{
	int m[3];

	m[_initialAxis] = 0;
	m[(_initialAxis + 1) % 3] = _parityEven ? 1 : 2;
	m[(_initialAxis + 2) % 3] = _parityEven ? 2 : 1;
	i = m[0];
	j = m[1];
	k = m[2];
}


/* inline */
void
Euler::setXYZVector(const Vector3f& v)
{
	int i, j, k;
	angleMapping(i, j, k);

	if (i == 0)
	{
		(*this).x = v.x;
	}
	else if (i == 1)
	{
		(*this).y = v.x;
	}
	else if (i == 2)
	{
		(*this).z = v.x;
	}

	if (j == 0)
	{
		(*this).x = v.y;
	}
	else if (j == 1)
	{
		(*this).y = v.y;
	}
	else if (j == 2)
	{
		(*this).z = v.y;
	}

	if (k == 0)
	{
		(*this).x = v.z;
	}
	else if (k == 1)
	{
		(*this).y = v.z;
	}
	else if (k == 2)
	{
		(*this).z = v.z;
	}
}


/* inline */
Vector3f
Euler::toXYZVector() const
{
	int i, j, k;
	angleMapping(i, j, k);

	Vector3f v;

	if (i == 0)
	{
		v.x = (*this).x;
	}
	else if (i == 1)
	{
		v.x = (*this).y;
	}
	else if (i == 2)
	{
		v.x = (*this).z;
	}

	if (j == 0)
	{
		v.y = (*this).x;
	}
	else if (j == 1)
	{
		v.y = (*this).y;
	}
	else if (j == 2)
	{
		v.y = (*this).z;
	}

	if (k == 0)
	{
		v.z = (*this).x;
	}
	else if (k == 1)
	{
		v.z = (*this).y;
	}
	else if (k == 2)
	{
		v.z = (*this).z;
	}

	return v;
}


Euler::Euler() :
	Vector3f(0, 0, 0),
	_frameStatic(true),
	_initialRepeated(false),
	_parityEven(true),
	_initialAxis(X)
{
}


Euler::Euler(Order p) :
	Vector3f(0, 0, 0),
	_frameStatic(true),
	_initialRepeated(false),
	_parityEven(true),
	_initialAxis(X)
{
	setOrder(p);
}


/* inline */
Euler::Euler(const Vector3f& v,
             Order p,
             InputLayout l)
{
	setOrder(p);
	if (l == XYZLayout) setXYZVector(v);
	else
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
}


/* inline */
Euler::Euler(const Euler& euler)
{
	operator=(euler);
}


/* inline */
Euler::Euler(float xi, float yi, float zi,
             Order p,
             InputLayout l)
{
	setOrder(p);
	if (l == XYZLayout) setXYZVector(Vector3f(xi, yi, zi));
	else
	{
		x = xi;
		y = yi;
		z = zi;
	}
}


/* inline */
Euler::Euler(const Matrix4x4& M, Order p)
{
	setOrder(p);
	extract(M);
}

Euler::~Euler()
{
}


/* inline */
void Euler::extract(const Quaternion& q)
{
	extract(q.toMatrix44());
}


void Euler::extract(const Matrix4x4& M)
{
	int i, j, k;
	angleOrder(i, j, k);

	if (_initialRepeated)
	{
		x = atan2f(M[j][i], M[k][i]);


		Vector3f r(0, 0, 0);

		if (i == 0)
		{
			r.x = (_parityEven ? -x : x);
		}
		else if (i == 1)
		{
			r.y = (_parityEven ? -x : x);
		}
		else if (i == 2)
		{
			r.z = (_parityEven ? -x : x);
		}
		/*r[i] = (_parityEven ? -x : x);*/

		Matrix4x4 N;
		N.rotate(r);
		N = N * M;


		float sy = sqrtf(N[j][i] * N[j][i] + N[k][i] * N[k][i]);
		y = atan2f(sy, N[i][i]);
		z = atan2f(N[j][k], N[j][j]);
	}
	else
	{
		x = atan2f(M[j][k], M[k][k]);


		Vector3f r(0, 0, 0);

		if (i == 0)
		{
			r.x = (_parityEven ? -x : x);
		}
		else if (i == 1)
		{
			r.y = (_parityEven ? -x : x);
		}
		else if (i == 2)
		{
			r.z = (_parityEven ? -x : x);
		}


		Matrix4x4 N;
		N.rotate(r);
		N = N * M;


		float cy = sqrtf(N[i][i] * N[i][i] + N[i][j] * N[i][j]);
		y = atan2f(-N[i][k], cy);
		z = atan2f(-N[j][i], N[j][j]);
	}

	if (!_parityEven)
		*this *= -1;

	if (!_frameStatic)
	{
		float t = x;
		x = z;
		z = t;
	}
}


Matrix4x4 Euler::toMatrix44() const
{
	int i, j, k;
	angleOrder(i, j, k);

	Vector3f angles;

	if (_frameStatic) angles = (*this);
	else angles = Vector3f(z, y, x);

	if (!_parityEven) angles *= -1.0;

	float ci = cos(angles.x);
	float cj = cos(angles.y);
	float ch = cos(angles.z);
	float si = sin(angles.x);
	float sj = sin(angles.y);
	float sh = sin(angles.z);

	float cc = ci * ch;
	float cs = ci * sh;
	float sc = si * ch;
	float ss = si * sh;

	Matrix4x4 M;

	if (_initialRepeated)
	{
		M[i][i] = cj;
		M[j][i] = sj * si;
		M[k][i] = sj * ci;
		M[i][j] = sj * sh;
		M[j][j] = -cj * ss + cc;
		M[k][j] = -cj * cs - sc;
		M[i][k] = -sj * ch;
		M[j][k] = cj * sc + cs;
		M[k][k] = cj * cc - ss;
	}
	else
	{
		M[i][i] = cj * ch;
		M[j][i] = sj * sc - cs;
		M[k][i] = sj * cc + ss;
		M[i][j] = cj * sh;
		M[j][j] = sj * ss + cc;
		M[k][j] = sj * cs - sc;
		M[i][k] = -sj;
		M[j][k] = cj * si;
		M[k][k] = cj * ci;
	}

	return M;
}


Quaternion Euler::toQuat() const
{
	Vector3f angles;
	int i, j, k;
	angleOrder(i, j, k);

	if (_frameStatic) angles = (*this);
	else angles = Vector3f(z, y, x);

	if (!_parityEven) angles.y = -angles.y;

	float ti = angles.x * 0.5;
	float tj = angles.y * 0.5;
	float th = angles.z * 0.5;
	float ci = cos(ti);
	float cj = cos(tj);
	float ch = cos(th);
	float si = sin(ti);
	float sj = sin(tj);
	float sh = sin(th);
	float cc = ci * ch;
	float cs = ci * sh;
	float sc = si * ch;
	float ss = si * sh;

	float parity = _parityEven ? 1.0 : -1.0;

	Quaternion q;
	Vector3f a;

	if (_initialRepeated)
	{
		a[i] = cj * (cs + sc);
		a[j] = sj * (cc + ss) * parity,
			a[k] = sj * (cs - sc);
		q.r = cj * (cc - ss);
	}
	else
	{
		a[i] = cj * sc - sj * cs,
			a[j] = (cj * ss + sj * cc) * parity,
			a[k] = cj * cs - sj * sc;
		q.r = cj * cc + sj * ss;
	}

	q.v = a;

	return q;
}


/* inline */
bool
Euler::legal(Order order)
{
	return (order & ~Legal) ? false : true;
}


Euler::Order
Euler::order() const
{
	int foo = (_initialAxis == Z ? 0x2000 : (_initialAxis == Y ? 0x1000 : 0));

	if (_parityEven) foo |= 0x0100;
	if (_initialRepeated) foo |= 0x0010;
	if (_frameStatic) foo++;

	return static_cast<Order>(foo);
}


/* inline */
void Euler::setOrder(Order p)
{
	set(p & 0x2000 ? Z : (p & 0x1000 ? Y : X),
	    !(p & 0x1),
	    !!(p & 0x100),
	    !!(p & 0x10));
}


void Euler::set(Axis axis,
                bool relative,
                bool parityEven,
                bool firstRepeats)
{
	_initialAxis = axis;
	_frameStatic = !relative;
	_parityEven = parityEven;
	_initialRepeated = firstRepeats;
}


const Euler& Euler::operator=(const Euler& euler)
{
	x = euler.x;
	y = euler.y;
	z = euler.z;
	_initialAxis = euler._initialAxis;
	_frameStatic = euler._frameStatic;
	_parityEven = euler._parityEven;
	_initialRepeated = euler._initialRepeated;
	return *this;
}


const Euler& Euler::operator=(const Vector3f& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}


std::ostream& operator <<(std::ostream& o, const Euler& euler)
{
	char a[3] = {'X', 'Y', 'Z'};

	const char* r = euler.frameStatic() ? "" : "r";
	int i, j, k;
	euler.angleOrder(i, j, k);

	if (euler.initialRepeated()) k = i;

	return o << "("
		<< euler.x << " "
		<< euler.y << " "
		<< euler.z << " "
		<< a[i] << a[j] << a[k] << r << ")";
}


float
Euler::angleMod(float angle)
{
	const float pi = static_cast<float>(M_PI);
	angle = fmod(float(angle), float(2 * pi));

	if (angle < -pi) angle += 2 * pi;
	if (angle > +pi) angle -= 2 * pi;

	return angle;
}


void
Euler::simpleXYZRotation(Vector3f& xyzRot, const Vector3f& targetXyzRot)
{
	Vector3f d = xyzRot - targetXyzRot;
	xyzRot[0] = targetXyzRot.x + angleMod(d[0]);
	xyzRot[1] = targetXyzRot.y + angleMod(d[1]);
	xyzRot[2] = targetXyzRot.z + angleMod(d[2]);
}


void
Euler::nearestRotation(Vector3f& xyzRot, const Vector3f& targetXyzRot,
                       Order order)
{
	int i, j, k;
	Euler e(0, 0, 0, order);
	e.angleOrder(i, j, k);

	simpleXYZRotation(xyzRot, targetXyzRot);

	Vector3f otherXyzRot;
	otherXyzRot[i] = M_PI + xyzRot[i];
	otherXyzRot[j] = M_PI - xyzRot[j];
	otherXyzRot[k] = M_PI + xyzRot[k];

	simpleXYZRotation(otherXyzRot, targetXyzRot);

	Vector3f d = xyzRot - targetXyzRot;
	Vector3f od = otherXyzRot - targetXyzRot;
	float dMag = d.dot(d);
	float odMag = od.dot(od);

	if (odMag < dMag)
	{
		xyzRot = otherXyzRot;
	}
}


void
Euler::makeNear(const Euler& target)
{
	Vector3f xyzRot = toXYZVector();
	Vector3f targetXyz;
	if (order() != target.order())
	{
		Euler targetSameOrder = Euler(target, order());
		targetXyz = targetSameOrder.toXYZVector();
	}
	else
	{
		targetXyz = target.toXYZVector();
	}

	nearestRotation(xyzRot, targetXyz, order());

	setXYZVector(xyzRot);
}

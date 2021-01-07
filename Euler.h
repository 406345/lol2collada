#pragma once

#ifndef _EULER_H_
#define _EULER_H_

#include "Vector3f.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

class Euler : public Vector3f
{
public:

	using Vector3f::x;
	using Vector3f::y;
	using Vector3f::z;

	enum Order
	{
		XYZ = 0x0101,
		XZY = 0x0001,
		YZX = 0x1101,
		YXZ = 0x1001,
		ZXY = 0x2101,
		ZYX = 0x2001,

		XZX = 0x0011,

		XYX = 0x0111,
		YXY = 0x1011,
		YZY = 0x1111,
		ZYZ = 0x2011,
		ZXZ = 0x2111,

		XYZr = 0x2000,

		XZYr = 0x2100,
		YZXr = 0x1000,
		YXZr = 0x1100,
		ZXYr = 0x0000,
		ZYXr = 0x0100,

		XZXr = 0x2110,

		XYXr = 0x2010,
		YXYr = 0x1110,
		YZYr = 0x1010,
		ZYZr = 0x0110,
		ZXZr = 0x0010,

		Legal = XYZ | XZY | YZX | YXZ | ZXY | ZYX |
		XZX | XYX | YXY | YZY | ZYZ | ZXZ |
		XYZr | XZYr | YZXr | YXZr | ZXYr | ZYXr |
		XZXr | XYXr | YXYr | YZYr | ZYZr | ZXZr,

		Min = 0x0000,
		Max = 0x2111,
		Default = XYZ
	};

	enum Axis { X = 0, Y = 1, Z = 2 };

	enum InputLayout { XYZLayout, IJKLayout };

	Euler();
	Euler(const Euler&);
	Euler(Order p);
	Euler(const Vector3f& v, Order o = Default, InputLayout l = IJKLayout);
	Euler(float i, float j, float k, Order o = Default, InputLayout l = IJKLayout);
	Euler(const Matrix4x4&, Order o = Default);

	~Euler();

	const Euler& operator=(const Euler&);
	const Euler& operator=(const Vector3f&);

	static bool legal(Order);

	void setXYZVector(const Vector3f&);

	Order order() const;
	void setOrder(Order);

	void set(Axis initial,
	         bool relative,
	         bool parityEven,
	         bool firstRepeats);

	void extract(const Matrix4x4&);
	void extract(const Quaternion&);

	Matrix4x4 toMatrix44() const;
	Quaternion toQuat() const;
	Vector3f toXYZVector() const;

	void angleOrder(int& i, int& j, int& k) const;
	void angleMapping(int& i, int& j, int& k) const;

	static float angleMod(float angle);
	static void simpleXYZRotation(Vector3f& xyzRot,
	                              const Vector3f& targetXyzRot);
	static void nearestRotation(Vector3f& xyzRot,
	                            const Vector3f& targetXyzRot,
	                            Order order = XYZ);

	void makeNear(const Euler& target);

	bool frameStatic() const { return _frameStatic; }
	bool initialRepeated() const { return _initialRepeated; }
	bool parityEven() const { return _parityEven; }
	Axis initialAxis() const { return _initialAxis; }

protected:

	bool _frameStatic : 1;
	bool _initialRepeated : 1;
	bool _parityEven : 1;
#if defined _WIN32 || defined _WIN64
	Axis _initialAxis;
#else
    Axis		_initialAxis : 2;
#endif
};

#endif

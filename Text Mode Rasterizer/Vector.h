#ifndef VECTOR_H
#define VECTOR_H

#include "main.h"

class Vector
{
	friend class Matrix4x4;
public:
	Vector(float fx = 0.0f, float fy = 0.0f, float fz = 0.0f)
	{
		x = fx;
		y = fy;
		z = fz;
	}
	Vector(Vector& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	void Set(float fx, float fy, float fz)
	{
		x = fx;
		y = fy;
		z = fz;
	}
	inline float Length(void)
	{
		return sqrtf(x * x + y * y + z * z);
	}
	inline Vector& Normalize(void)
	{
		float l = invsqrtf(x * x + y * y + z * z);
		x *= l;
		y *= l;
		z *= l;

		return *this;
	}
	inline void Clamp(void)
	{
		if(r > 1.0f)
			r = 1.0f;
		if(g > 1.0f)
			g = 1.0f;
		if(b > 1.0f)
			b = 1.0f;
	}
	inline float SqrLength(void)
	{
		return (x * x + y * y + z * z);
	}
	inline void AdjustPrecision(float mult=1000.0f)
	{
		r = (ltof(lrintf(r * mult)))/mult;
		g = (ltof(lrintf(g * mult)))/mult;
		b = (ltof(lrintf(b * mult)))/mult;
	}
	float Dot(Vector v)
	{
		return (x * v.x) + (y * v.y) + (z * v.z);
	}
	Vector Cross(Vector v)
	{
		return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	void operator-=(const Vector &v);
	void operator+=(const Vector &v);
	Vector operator+(const Vector &v) const;
	Vector operator-(const Vector &v) const;
	void operator*=(float f);
	Vector operator*(float f) const;
	void operator/=(float f);
	Vector operator/(float f) const;
	Vector operator*(const Vector &v) const;
	void operator*=(const Vector &v);
	Vector operator*(const Matrix4x4 &mat) const;
	void operator*=(const Matrix4x4 &mat);
	bool operator==(const Vector &v) const;
	bool operator!=(const Vector &v) const;

	void WriteOut(FILE *fp);
	void ReadFrom(FILE *fp);
	void Print();

	void Translate(float dx, float dy, float dz);
	void Translate(Vector d);

	void RotateAboutPoint(Vector rot, Vector point);

	void RotateAboutXAboutPoint(float angle, Vector point);
	void RotateAboutYAboutPoint(float angle, Vector point);
	void RotateAboutZAboutPoint(float angle, Vector point);

	void ScaleXAboutPoint(float s, Vector point);
	void ScaleYAboutPoint(float s, Vector point);
	void ScaleZAboutPoint(float s, Vector point);

	union
	{
		struct { float r, g, b; };
		struct { float x, y, z; };
		struct { float cell[3]; };
	};
};

typedef Vector Color;

class Vertex
{
public:
	Vector pos;
	Vector normal;
	Color col;
	Vertex operator*(const Matrix4x4 &mat) const;
	void operator*=(const Matrix4x4 &mat);

	void WriteOut(FILE *fp);
	void ReadFrom(FILE *fp);
	void Print();

	void Translate(float dx, float dy, float dz);
	void Translate(Vector d);

	void RotateAboutPoint(Vector rot, Vector point);

	void RotateAboutXAboutPoint(float angle, Vector point);
	void RotateAboutYAboutPoint(float angle, Vector point);
	void RotateAboutZAboutPoint(float angle, Vector point);

	void ScaleXAboutPoint(float s, Vector point);
	void ScaleYAboutPoint(float s, Vector point);
	void ScaleZAboutPoint(float s, Vector point);
};

class Ray
{
public:
	Ray(void) : Origin(Vector(0,0,0)), Direction(Vector(0,0,0)) { }
	Ray(Vector theOrigin, Vector theDirection)
	{
		Origin = theOrigin;
		Direction = theDirection;
	}
	void SetDirection(Vector theDirection)
	{
		Direction = theDirection;
	}
	void SetOrigin(Vector theOrigin)
	{
		Origin = theOrigin;
	}
	Vector GetOrigin(void)
	{
		return Origin;
	}
	Vector GetDirection(void)
	{
		return Direction;
	}
private:
	Vector Origin, Direction;
};

class MathPlane
{
public:
	MathPlane(void) : N(0, 0, 0), D(0)
	{
	}
	MathPlane(Vector normal, float dVal) : N(normal), D(dVal)
	{
	}
	union
	{
		struct
		{
			Vector N;
			float D;
		};
		float Cell[4];
	};
};

#endif
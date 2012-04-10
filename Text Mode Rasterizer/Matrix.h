#ifndef MATRIX_H
#define MATRIX_H

#include "main.h"

class Matrix4x4
{
	friend class Vector;
public:
	Matrix4x4(void);
	Matrix4x4(const Matrix4x4 &mat);
	Matrix4x4(float b11, float b12, float b13, float b14, float b21, float b22, float b23, float b24, float b31, float b32, float b33, float b34, float b41, float b42, float b43, float b44);

	static Matrix4x4 Identity(void);
	static Matrix4x4 Zero(void);
	static Matrix4x4 One(void);

	Matrix4x4& operator=(const Matrix4x4 &mat);
	bool operator==(const Matrix4x4 &mat) const;
	bool operator!=(const Matrix4x4 &mat) const;

	Vector operator*(const Vector &v) const;
	Vertex operator*(const Vertex &v) const;

	Matrix4x4 operator+(const Matrix4x4 &mat) const;
	Matrix4x4 operator-(const Matrix4x4 &mat) const;
	Matrix4x4 operator*(const Matrix4x4 &mat) const;
	Matrix4x4 operator*(const float num) const;
	Matrix4x4 operator/(const float num) const;

	void operator+=(const Matrix4x4 &mat);
	void operator-=(const Matrix4x4 &mat);
	void operator*=(const Matrix4x4 &mat);
	void operator*=(const float num);
	void operator/=(const float num);

	union
	{
		struct
		{
			float a11, a12, a13, a14;
			float a21, a22, a23, a24;
			float a31, a32, a33, a34;
			float a41, a42, a43, a44;
		};
		struct
		{
			float cell[4][4];
		};
	};
};

#endif
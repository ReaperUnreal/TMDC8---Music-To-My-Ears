#include "main.h"

Matrix4x4::Matrix4x4(void)
{
	for(int i = 0; i < 4; i++)
	{
		cell[i][0] = 0;
		cell[i][1] = 0;
		cell[i][2] = 0;
		cell[i][3] = 0;
	}
}

Matrix4x4::Matrix4x4(const Matrix4x4 &mat)
{
	for(int i = 0; i < 4; i++)
	{
		cell[i][0] = mat.cell[i][0];
		cell[i][1] = mat.cell[i][1];
		cell[i][2] = mat.cell[i][2];
		cell[i][3] = mat.cell[i][3];
	}
}

Matrix4x4::Matrix4x4(float b11, float b12, float b13, float b14, float b21, float b22, float b23, float b24, float b31, float b32, float b33, float b34, float b41, float b42, float b43, float b44)
{
	a11 = b11;
	a12 = b12;
	a13 = b13;
	a14 = b14;
	a21 = b21;
	a22 = b22;
	a23 = b23;
	a24 = b24;
	a31 = b31;
	a32 = b32;
	a33 = b33;
	a34 = b34;
	a41 = b41;
	a42 = b42;
	a43 = b43;
	a44 = b44;
}

Matrix4x4 Matrix4x4::Identity(void)
{
	return Matrix4x4(1, 0, 0, 0,
					 0, 1, 0, 0,
					 0, 0, 1, 0,
					 0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Zero(void)
{
	return Matrix4x4(0, 0, 0, 0,
					 0, 0, 0, 0,
					 0, 0, 0, 0,
					 0, 0, 0, 0);
}

Matrix4x4 Matrix4x4::One(void)
{
	return Matrix4x4(1, 1, 1, 1,
					 1, 1, 1, 1,
					 1, 1, 1, 1,
					 1, 1, 1, 1);
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4 &mat)
{
	for(int i = 0; i < 4; i++)
	{
		cell[i][0] = mat.cell[i][0];
		cell[i][1] = mat.cell[i][1];
		cell[i][2] = mat.cell[i][2];
		cell[i][3] = mat.cell[i][3];
	}
	return *this;
}

bool Matrix4x4::operator==(const Matrix4x4 &mat) const
{
	bool equal = true;
	int i = 0;
	while((equal == true) && (i < 4))
	{
		if(cell[i][0] != cell[i][0])
			equal = false;
		if(cell[i][1] != cell[i][1])
			equal = false;
		if(cell[i][2] != cell[i][2])
			equal = false;
		if(cell[i][3] != cell[i][3])
			equal = false;
		i++;
	}
	return equal;
}

bool Matrix4x4::operator!=(const Matrix4x4 &mat) const
{
	bool equal = true;
	int i = 0;
	while((equal == true) && (i < 4))
	{
		if(cell[i][0] != cell[i][0])
			equal = false;
		if(cell[i][1] != cell[i][1])
			equal = false;
		if(cell[i][2] != cell[i][2])
			equal = false;
		if(cell[i][3] != cell[i][3])
			equal = false;
		i++;
	}
	return !equal;
}

Vector Matrix4x4::operator*(const Vector &v) const
{
	return Vector(v.x * a11 + v.y * a21 + v.z * a31 + a41,
				  v.x * a12 + v.y * a22 + v.z * a32 + a42,
				  v.x * a13 + v.y * a23 + v.z * a33 + a43);
}

Vertex Matrix4x4::operator*(const Vertex &v) const
{
	Vertex vert;
	vert.col = v.col;
	vert.pos = (*this) * vert.pos;
	vert.normal = (*this) * vert.normal;
	return vert;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 &mat) const
{
	return Matrix4x4(a11 + mat.a11, a12 + mat.a12, a13 + mat.a13, a14 + mat.a14,
					 a21 + mat.a21, a22 + mat.a22, a23 + mat.a23, a24 + mat.a24,
					 a31 + mat.a31, a32 + mat.a32, a33 + mat.a33, a34 + mat.a34,
					 a41 + mat.a41, a42 + mat.a42, a43 + mat.a43, a44 + mat.a44);
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4 &mat) const
{
	return Matrix4x4(a11 - mat.a11, a12 - mat.a12, a13 - mat.a13, a14 - mat.a14,
					 a21 - mat.a21, a22 - mat.a22, a23 - mat.a23, a24 - mat.a24,
					 a31 - mat.a31, a32 - mat.a32, a33 - mat.a33, a34 - mat.a34,
					 a41 - mat.a41, a42 - mat.a42, a43 - mat.a43, a44 - mat.a44);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &mat) const
{
	Matrix4x4 ret;
	for(int i = 0; i < 4; i++)
	{
		ret.cell[i][0] = cell[i][0] * mat.a11 + cell[i][1] * mat.a21 + cell[1][2] * mat.a31 + cell[i][3] * mat.a41;
		ret.cell[i][1] = cell[i][0] * mat.a12 + cell[i][1] * mat.a22 + cell[1][2] * mat.a32 + cell[i][3] * mat.a42;
		ret.cell[i][2] = cell[i][0] * mat.a13 + cell[i][1] * mat.a23 + cell[1][2] * mat.a33 + cell[i][3] * mat.a43;
		ret.cell[i][3] = cell[i][0] * mat.a14 + cell[i][1] * mat.a24 + cell[1][2] * mat.a34 + cell[i][3] * mat.a44;
	}

	return ret;
}

Matrix4x4 Matrix4x4::operator*(const float num) const
{
	return Matrix4x4(a11 * num, a12 * num, a13 * num, a14 * num,
					 a21 * num, a22 * num, a23 * num, a24 * num,
					 a31 * num, a32 * num, a33 * num, a34 * num,
					 a41 * num, a42 * num, a43 * num, a44 * num);
}

Matrix4x4 Matrix4x4::operator/(const float num) const
{
	return Matrix4x4(a11 / num, a12 / num, a13 / num, a14 / num,
					 a21 / num, a22 / num, a23 / num, a24 / num,
					 a31 / num, a32 / num, a33 / num, a34 / num,
					 a41 / num, a42 / num, a43 / num, a44 / num);
}

void Matrix4x4::operator+=(const Matrix4x4 &mat)
{
	a11 += mat.a11; a12 += mat.a12; a13 += mat.a13; a14 += mat.a14;
	a21 += mat.a21; a22 += mat.a22; a23 += mat.a23; a24 += mat.a24;
	a31 += mat.a31; a32 += mat.a32; a33 += mat.a33; a34 += mat.a34;
	a41 += mat.a41; a42 += mat.a42; a43 += mat.a43; a44 += mat.a44;
}

void Matrix4x4::operator-=(const Matrix4x4 &mat)
{
	a11 -= mat.a11; a12 -= mat.a12; a13 -= mat.a13; a14 -= mat.a14;
	a21 -= mat.a21; a22 -= mat.a22; a23 -= mat.a23; a24 -= mat.a24;
	a31 -= mat.a31; a32 -= mat.a32; a33 -= mat.a33; a34 -= mat.a34;
	a41 -= mat.a41; a42 -= mat.a42; a43 -= mat.a43; a44 -= mat.a44;
}

void Matrix4x4::operator*=(const Matrix4x4 &mat)
{
	float a, b, c, d;

	for(int i = 0; i < 4; i++)
	{
		a = cell[i][0];
		b = cell[i][1];
		c = cell[i][2];
		d = cell[i][3];

		cell[i][0] = a * mat.a11 + b * mat.a21 + c * mat.a31 + d * mat.a41;
		cell[i][1] = a * mat.a12 + b * mat.a22 + c * mat.a32 + d * mat.a42;
		cell[i][2] = a * mat.a13 + b * mat.a23 + c * mat.a33 + d * mat.a43;
		cell[i][3] = a * mat.a14 + b * mat.a24 + c * mat.a34 + d * mat.a44;
	}
}

void Matrix4x4::operator*=(const float num)
{
	a11 *= num; a12 *= num; a13 *= num; a14 *= num;
	a21 *= num; a22 *= num; a23 *= num; a24 *= num;
	a31 *= num; a32 *= num; a33 *= num; a34 *= num;
	a41 *= num; a42 *= num; a43 *= num; a44 *= num;
}

void Matrix4x4::operator/=(const float num)
{
	a11 /= num; a12 /= num; a13 /= num; a14 /= num;
	a21 /= num; a22 /= num; a23 /= num; a24 /= num;
	a31 /= num; a32 /= num; a33 /= num; a34 /= num;
	a41 /= num; a42 /= num; a43 /= num; a44 /= num;
}
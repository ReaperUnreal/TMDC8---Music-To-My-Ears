#include "main.h"

bool Vector::operator ==(const Vector &v) const
{
	return ((x >= v.x - epsilon && x <= v.x + epsilon) &&
			(y >= v.y - epsilon && y <= v.y + epsilon) &&
			(z >= v.z - epsilon && z <= v.z + epsilon));
}

bool Vector::operator !=(const Vector &v) const
{
	return ((x < v.x - epsilon || x > v.x + epsilon) ||
		    (y < v.y - epsilon || y > v.y + epsilon) ||
			(z < v.z - epsilon || z > v.z + epsilon));
}

void Vector::operator +=(const Vector &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vector::operator -=(const Vector &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vector::operator *=(float f)
{
	x *= f;
	y *= f;
	z *= f;
}

void Vector::operator /=(float f)
{
	x /= f;
	y /= f;
	z /= f;
}

void Vector::operator *=(const Vector &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void Vector::operator *=(const Matrix4x4 &mat)
{
	float vx = x, vy = y, vz = z;

	x = vx * mat.a11 + vy * mat.a21 + vz * mat.a31 + mat.a41;
	y = vx * mat.a12 + vy * mat.a22 + vz * mat.a32 + mat.a42;
	z = vx * mat.a13 + vy * mat.a23 + vz * mat.a33 + mat.a43;
}

Vector Vector::operator +(const Vector &v) const
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator -(const Vector &v) const
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator *(float f) const
{
	return Vector(x * f, y * f, z * f);
}

Vector Vector::operator /(float f) const
{
	return Vector(x / f, y / f, z / f);
}

Vector Vector::operator *(const Vector &v) const
{
	return Vector(x * v.x, y * v.y, z * v.z);
}

Vector Vector::operator *(const Matrix4x4 &mat) const
{
	return Vector(x * mat.a11 + y * mat.a21 + z * mat.a31 + mat.a41,
				  x * mat.a12 + y * mat.a22 + z * mat.a32 + mat.a42,
				  x * mat.a13 + y * mat.a23 + z * mat.a33 + mat.a43);
}

void Vector::WriteOut(FILE *fp)
{
	fwrite(cell, sizeof(float), 3, fp);
}

void Vector::ReadFrom(FILE *fp)
{
	fread(cell, sizeof(float), 3, fp);
}

void Vector::Print()
{
	cout << "(" << x << ", " << y << ", " << z << ")\n";
}

void Vector::Translate(Vector d)
{
	x += d.x;
	y += d.y;
	z += d.z;
}

void Vector::Translate(float dx, float dy, float dz)
{
	x += dx;
	y += dy;
	z += dz;
}

void Vector::RotateAboutPoint(Vector rot, Vector point)
{
	float dx = x - point.x;
	float dy = y - point.y;
	float dz = z - point.z;
	float cx = cosf(rot.x);
	float cy = cosf(rot.y);
	float cz = cosf(rot.z);
	float sx = sinf(rot.x);
	float sy = sinf(rot.y);
	float sz = sinf(rot.z);
	x = cy * cz * dx - sz * cy * dy - sy * dz + point.x;
	y = (cx * sz - sx * sy * cz) * dx + (sx * sy * sz + cx * cz) * dy - sx * cy * dz + point.y;
	z = (cx * sy * cz + sx * sz) * dx + (sx * cz - cx * sy * sz) * dy + cx * cy * dz + point.z;
}

void Vector::RotateAboutXAboutPoint(float angle, Vector point)
{
	float dy = y - point.y;
	float dz = z - point.z;
	float ca = cosf(angle);
	float sa = sinf(angle);
	float ny = dy * ca - dz * sa;
	float nz = dy * sa + dz * ca;
	y = point.y + ny;
	z = point.z + nz;
}

void Vector::RotateAboutYAboutPoint(float angle, Vector point)
{
	float dx = x - point.x;
	float dz = z - point.z;
	float ca = cosf(angle);
	float sa = sinf(angle);
	float nx = dx * ca - dz * sa;
	float nz = dx * sa + dz * ca;
	x = point.x + nx;
	z = point.z + nz;
}

void Vector::RotateAboutZAboutPoint(float angle, Vector point)
{
	float dx = x - point.x;
	float dy = y - point.y;
	float ca = cosf(angle);
	float sa = sinf(angle);
	float nx = dx * ca - dy * sa;
	float ny = dx * sa + dy * ca;
	x = point.x + nx;
	y = point.y + ny;
}

void Vector::ScaleXAboutPoint(float s, Vector point)
{
	x = s * (x - point.x) + point.x;
}

void Vector::ScaleYAboutPoint(float s, Vector point)
{
	y = s * (y - point.y) + point.y;
}

void Vector::ScaleZAboutPoint(float s, Vector point)
{
	z = s * (z - point.z) + point.z;
}

Vertex Vertex::operator *(const Matrix4x4 &mat) const
{
	Vertex vert;
	vert.col = col;
	vert.pos = pos * mat;
	vert.normal = normal * mat;
	return vert;
}

void Vertex::operator *=(const Matrix4x4 &mat)
{
	pos *= mat;
	normal *= mat;
}

void Vertex::WriteOut(FILE *fp)
{
	pos.WriteOut(fp);
	normal.WriteOut(fp);
	col.WriteOut(fp);
}

void Vertex::ReadFrom(FILE *fp)
{
	pos.ReadFrom(fp);
	normal.ReadFrom(fp);
	col.ReadFrom(fp);
}

void Vertex::Print()
{
	cout << "Pos: ";
	pos.Print();
	cout << "Normal: ";
	normal.Print();
	cout << "Colour: ";
	col.Print();
}

void Vertex::Translate(Vector d)
{
	pos.x += d.x;
	pos.y += d.y;
	pos.z += d.z;
}

void Vertex::Translate(float dx, float dy, float dz)
{
	pos.x += dx;
	pos.y += dy;
	pos.z += dz;
}

void Vertex::RotateAboutPoint(Vector rot, Vector point)
{
	float dx = pos.x - point.x;
	float dy = pos.y - point.y;
	float dz = pos.z - point.z;
	float cx = cosf(rot.x);
	float cy = cosf(rot.y);
	float cz = cosf(rot.z);
	float sx = sinf(rot.x);
	float sy = sinf(rot.y);
	float sz = sinf(rot.z);
	pos.x = cy * cz * dx - sz * cy * dy - sy * dz + point.x;
	pos.y = (cx * sz - sx * sy * cz) * dx + (sx * sy * sz + cx * cz) * dy - sx * cy * dz + point.y;
	pos.z = (cx * sy * cz + sx * sz) * dx + (sx * cz - cx * sy * sz) * dy + cx * cy * dz + point.z;
	float nx = normal.x;
	float ny = normal.y;
	float nz = normal.z;
	normal.x = cy * cz * nx - sz * cy * ny - sy * nz;
	normal.y = (cx * sz - sx * sy * cz) * nx + (sx * sy * sz + cx * cz) * ny - sx * cy * nz;
	normal.z = (cx * sy * cz + sx * sz) * nx + (sx * cz - cx * sy * sz) * ny + cx * cy * nz;
}

void Vertex::RotateAboutXAboutPoint(float angle, Vector point)
{
	float dy = pos.y - point.y;
	float dz = pos.z - point.z;
	float ca = cosf(angle);
	float sa = sinf(angle);
	float ny = dy * ca - dz * sa;
	float nz = dy * sa + dz * ca;
	pos.y = point.y + ny;
	pos.z = point.z + nz;
	ny = normal.y;
	nz = normal.z;
	normal.y = ny * ca - nz * sa;
	normal.z = ny * sa + nz * ca;
}

void Vertex::RotateAboutYAboutPoint(float angle, Vector point)
{
	float dx = pos.x - point.x;
	float dz = pos.z - point.z;
	float ca = cosf(angle);
	float sa = sinf(angle);
	float nx = dx * ca - dz * sa;
	float nz = dx * sa + dz * ca;
	pos.x = point.x + nx;
	pos.z = point.z + nz;
	nx = normal.x;
	nz = normal.z;
	normal.x = nx * ca - nz * sa;
	normal.z = nx * sa + nz * ca;
}

void Vertex::RotateAboutZAboutPoint(float angle, Vector point)
{
	float dx = pos.x - point.x;
	float dy = pos.y - point.y;
	float ca = cosf(angle);
	float sa = sinf(angle);
	float nx = dx * ca - dy * sa;
	float ny = dx * sa + dy * ca;
	pos.x = point.x + nx;
	pos.y = point.y + ny;
	nx = normal.x;
	ny = normal.y;
	normal.x = nx * ca - ny * sa;
	normal.y = nx * sa + ny * ca;
}

void Vertex::ScaleXAboutPoint(float s, Vector point)
{
	pos.x = s * (pos.x - point.x) + point.x;
}

void Vertex::ScaleYAboutPoint(float s, Vector point)
{
	pos.y = s * (pos.y - point.y) + point.y;
}

void Vertex::ScaleZAboutPoint(float s, Vector point)
{
	pos.z = s * (pos.z - point.z) + point.z;
}

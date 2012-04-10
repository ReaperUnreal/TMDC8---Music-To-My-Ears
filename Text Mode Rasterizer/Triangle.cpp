#include "main.h"

Triangle::Triangle()
{
	for(int i = 0; i < 3; i++)
	{
		vertices[i].pos.x = 0;
		vertices[i].pos.y = 0;
		vertices[i].pos.z = 0;
		vertices[i].col.r = 0;
		vertices[i].col.g = 0;
		vertices[i].col.b = 0;
	}
	normal.x = 0;
	normal.y = 0;
	normal.z = 0;
	centroidChanged = false;
	normalChanged = false;
}

Triangle::Triangle(const Triangle &tri)
{
	for(int i = 0; i < 3; i++)
	{
		vertices[i] = tri.vertices[i];
	}
	normal = tri.normal;
	centroid = tri.centroid;
	centroidChanged = true;
	normalChanged = true;
}

Triangle& Triangle::operator =(const Triangle &tri)
{
	for(int i = 0; i < 3; i++)
	{
		vertices[i] = tri.vertices[i];
	}
	normal = tri.normal;
	centroid = tri.centroid;
	centroidChanged = true;
	normalChanged = true;

	return *this;
}

void Triangle::ComputeNormal()
{
	if(normalChanged)
	{
		Vector d1 = vertices[1].pos - vertices[0].pos;
		Vector d2 = vertices[2].pos - vertices[1].pos;
		normal = d2.Cross(d1);
		normal.Normalize();
		normalChanged = false;
	}
}

void Triangle::ComputeCentroid()
{
	if(centroidChanged)
	{
		centroid = (vertices[0].pos + vertices[1].pos + vertices[2].pos)/3.0f;
		centroidChanged = false;
	}
}

void Triangle::ReColour(Triangle *tris, int numTris, Color col)
{
	for(int i = 0; i < numTris; i++)
	{
		tris[i].vertices[0].col = col;
		tris[i].vertices[1].col = col;
		tris[i].vertices[2].col = col;
	}
}

void Triangle::WriteOut(FILE *fp)
{
	for(int i = 0; i < 3; i++)
	{
		vertices[i].WriteOut(fp);
	}
	normal.WriteOut(fp);
	centroid.WriteOut(fp);
}

void Triangle::ReadFrom(FILE *fp)
{
	for(int i = 0; i < 3; i++)
	{
		vertices[i].ReadFrom(fp);
		vertices[i].normal *= -1.0f;
	}
	normal.ReadFrom(fp);
	centroid.ReadFrom(fp);
	centroidChanged = true;
	normalChanged = true;
}

void Triangle::Print()
{
	for(int i = 0; i < 3; i++)
	{
		cout << "Vertex " << i + 1 << ":\n";
		vertices[i].Print();
	}
	cout << "Normal: ";
	normal.Print();
	cout << "Centroid: ";
	centroid.Print();
}

void Triangle::RecalcNormal(void)
{
	normalChanged = true;
}

void Triangle::Translate(Vector d)
{
	vertices[0].Translate(d);
	vertices[1].Translate(d);
	vertices[2].Translate(d);
	centroidChanged = true;
}

void Triangle::Translate(float dx, float dy, float dz)
{
	vertices[0].Translate(dx, dy, dz);
	vertices[1].Translate(dx, dy, dz);
	vertices[2].Translate(dx, dy, dz);
	centroidChanged = true;
}

void Triangle::Rotate(Vector rot)
{
	ComputeCentroid();
	vertices[0].RotateAboutPoint(rot, centroid);
	vertices[1].RotateAboutPoint(rot, centroid);
	vertices[2].RotateAboutPoint(rot, centroid);
	float cx = cosf(rot.x);
	float cy = cosf(rot.y);
	float cz = cosf(rot.z);
	float sx = sinf(rot.x);
	float sy = sinf(rot.y);
	float sz = sinf(rot.z);
	float nx = normal.x;
	float ny = normal.y;
	float nz = normal.z;
	normal.x = cy * cz * nx - sz * cy * ny - sy * nz;
	normal.y = (cx * sz - sx * sy * cz) * nx + (sx * sy * sz + cx * cz) * ny - sx * cy * nz;
	normal.z = (cx * sy * cz + sx * sz) * nx + (sx * cz - cx * sy * sz) * ny + cx * cy * nz;
}

void Triangle::RotateAboutPoint(Vector rot, Vector point)
{
	vertices[0].RotateAboutPoint(rot, point);
	vertices[1].RotateAboutPoint(rot, point);
	vertices[2].RotateAboutPoint(rot, point);
	float cx = cosf(rot.x);
	float cy = cosf(rot.y);
	float cz = cosf(rot.z);
	float sx = sinf(rot.x);
	float sy = sinf(rot.y);
	float sz = sinf(rot.z);
	float nx = normal.x;
	float ny = normal.y;
	float nz = normal.z;
	normal.x = cy * cz * nx - sz * cy * ny - sy * nz;
	normal.y = (cx * sz - sx * sy * cz) * nx + (sx * sy * sz + cx * cz) * ny - sx * cy * nz;
	normal.z = (cx * sy * cz + sx * sz) * nx + (sx * cz - cx * sy * sz) * ny + cx * cy * nz;
}

void Triangle::RotateAboutXAboutPoint(float angle, Vector point)
{
	vertices[0].RotateAboutXAboutPoint(angle, point);
	vertices[1].RotateAboutXAboutPoint(angle, point);
	vertices[2].RotateAboutXAboutPoint(angle, point);
	float sa = sinf(angle);
	float ca = cosf(angle);
	float ny = normal.y;
	float nz = normal.z;
	normal.y = ca * ny - sa * nz;
	normal.z = sa * ny + ca * nz;
	centroidChanged = true;
}

void Triangle::RotateAboutYAboutPoint(float angle, Vector point)
{
	vertices[0].RotateAboutYAboutPoint(angle, point);
	vertices[1].RotateAboutYAboutPoint(angle, point);
	vertices[2].RotateAboutYAboutPoint(angle, point);
	float sa = sinf(angle);
	float ca = cosf(angle);
	float nx = normal.x;
	float nz = normal.z;
	normal.x = ca * nx - sa * nz;
	normal.z = sa * nx + ca * nz;
	centroidChanged = true;
}

void Triangle::RotateAboutZAboutPoint(float angle, Vector point)
{
	vertices[0].RotateAboutZAboutPoint(angle, point);
	vertices[1].RotateAboutZAboutPoint(angle, point);
	vertices[2].RotateAboutZAboutPoint(angle, point);
	float sa = sinf(angle);
	float ca = cosf(angle);
	float nx = normal.x;
	float ny = normal.y;
	normal.x = ca * nx - sa * ny;
	normal.y = sa * nx + ca * ny;
	centroidChanged = true;
}

void Triangle::RotateAboutX(float angle)
{
	ComputeCentroid();
	vertices[0].RotateAboutXAboutPoint(angle, centroid);
	vertices[1].RotateAboutXAboutPoint(angle, centroid);
	vertices[2].RotateAboutXAboutPoint(angle, centroid);
	float sa = sinf(angle);
	float ca = cosf(angle);
	float ny = normal.y;
	float nz = normal.z;
	normal.y = ca * ny - sa * nz;
	normal.z = sa * ny + ca * nz;
}

void Triangle::RotateAboutY(float angle)
{
	ComputeCentroid();
	vertices[0].RotateAboutYAboutPoint(angle, centroid);
	vertices[1].RotateAboutYAboutPoint(angle, centroid);
	vertices[2].RotateAboutYAboutPoint(angle, centroid);
	float sa = sinf(angle);
	float ca = cosf(angle);
	float nx = normal.x;
	float nz = normal.z;
	normal.x = ca * nx - sa * nz;
	normal.z = sa * nx + ca * nz;
}

void Triangle::RotateAboutZ(float angle)
{
	ComputeCentroid();
	vertices[0].RotateAboutZAboutPoint(angle, centroid);
	vertices[1].RotateAboutZAboutPoint(angle, centroid);
	vertices[2].RotateAboutZAboutPoint(angle, centroid);
	float sa = sinf(angle);
	float ca = cosf(angle);
	float nx = normal.x;
	float ny = normal.y;
	normal.x = ca * nx - sa * ny;
	normal.y = sa * nx + ca * ny;
}

void Triangle::ScaleXAboutPoint(float s, Vector point)
{
	vertices[0].ScaleXAboutPoint(s, point);
	vertices[1].ScaleXAboutPoint(s, point);
	vertices[2].ScaleXAboutPoint(s, point);
	centroidChanged = true;
}

void Triangle::ScaleYAboutPoint(float s, Vector point)
{
	vertices[0].ScaleYAboutPoint(s, point);
	vertices[1].ScaleYAboutPoint(s, point);
	vertices[2].ScaleYAboutPoint(s, point);
	centroidChanged = true;
}

void Triangle::ScaleZAboutPoint(float s, Vector point)
{
	vertices[0].ScaleZAboutPoint(s, point);
	vertices[1].ScaleZAboutPoint(s, point);
	vertices[2].ScaleZAboutPoint(s, point);
	centroidChanged = true;
}

void Triangle::ScaleX(float s)
{
	vertices[0].ScaleXAboutPoint(s, centroid);
	vertices[1].ScaleXAboutPoint(s, centroid);
	vertices[2].ScaleXAboutPoint(s, centroid);
}

void Triangle::ScaleY(float s)
{
	vertices[0].ScaleYAboutPoint(s, centroid);
	vertices[1].ScaleYAboutPoint(s, centroid);
	vertices[2].ScaleYAboutPoint(s, centroid);
}

void Triangle::ScaleZ(float s)
{
	vertices[0].ScaleZAboutPoint(s, centroid);
	vertices[1].ScaleZAboutPoint(s, centroid);
	vertices[2].ScaleZAboutPoint(s, centroid);
}
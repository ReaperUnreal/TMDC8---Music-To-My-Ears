#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "main.h"

class Triangle
{
public:
	Triangle();
	Triangle(const Triangle &tri);
	Triangle& operator=(const Triangle &tri);
	Vertex vertices[3];
	Vector normal, centroid;
	void ComputeNormal();
	void ComputeCentroid();
	static void ReColour(Triangle *tris, int numTris, Color col);
	void WriteOut(FILE *fp);
	void ReadFrom(FILE *fp);
	void Print();

	void RecalcNormal(void);

	void Translate(float dx, float dy, float dz);
	void Translate(Vector d);

	void RotateAboutXAboutPoint(float angle, Vector point);
	void RotateAboutYAboutPoint(float angle, Vector point);
	void RotateAboutZAboutPoint(float angle, Vector point);
	void RotateAboutX(float angle);
	void RotateAboutY(float angle);
	void RotateAboutZ(float angle);
	void Rotate(Vector rot);
	void RotateAboutPoint(Vector rot, Vector point);

	void ScaleXAboutPoint(float s, Vector point);
	void ScaleYAboutPoint(float s, Vector point);
	void ScaleZAboutPoint(float s, Vector point);
	void ScaleX(float s);
	void ScaleY(float s);
	void ScaleZ(float s);
private:
	bool centroidChanged;
	bool normalChanged;
};

#endif
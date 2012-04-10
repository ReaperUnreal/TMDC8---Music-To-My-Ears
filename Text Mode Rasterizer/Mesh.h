#ifndef MESH_H
#define MESH_H

class Mesh
{
	friend class Rasterizer;
public:
	Mesh(void);
	virtual ~Mesh(void);

	void WriteOut(FILE *fp);
	void WriteOut(char *filename);
	void ReadFrom(FILE *fp);
	void ReadFrom(char *filename);

	void ReColour(Color col);

	Vector ComputeCentroid();
	
	void Translate(Vector d);
	void Translate(float dx, float dy, float dz);

	void MoveTo(Vector d);
	void MoveTo(float x, float y, float z);

	void RotateAboutXAboutPoint(float angle, Vector point);
	void RotateAboutYAboutPoint(float angle, Vector point);
	void RotateAboutZAboutPoint(float angle, Vector point);
	void RotateAboutX(float angle);
	void RotateAboutY(float angle);
	void RotateAboutZ(float angle);

	void ScaleXAboutPoint(float s, Vector point);
	void ScaleYAboutPoint(float s, Vector point);
	void ScaleZAboutPoint(float s, Vector point);
	void ScaleX(float s);
	void ScaleY(float s);
	void ScaleZ(float s);

	void SetRotation(Vector r);
	void SetRotation(float x, float y, float z);

	void Explode(float dist, float variance);

	int GetNumTris(void);
private:
	Vector centroid;
	bool centroidChanged;
	Triangle *tris;
	Triangle *originalTris;
	int numTris;
	Vector rotation;
};

#endif
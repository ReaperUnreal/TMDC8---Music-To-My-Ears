#include "main.h"

Mesh::Mesh()
{
	tris = NULL;
	originalTris = NULL;
	numTris = 0;
	centroidChanged = false;
}

Mesh::~Mesh()
{
	if(originalTris)
	{
		delete[] originalTris;
		originalTris = NULL;
	}
	if(tris)
	{
		delete[] tris;
		tris = NULL;
	}
}

int Mesh::GetNumTris()
{
	return numTris;
}

void Mesh::WriteOut(char *filename)
{
	FILE *out = fopen(filename, "w+b");
	if(!out)
	{
		cout << "ERROR: Couldn't create file " << filename << " to write to.\n";
		return;
	}
	WriteOut(out);
	fclose(out);
}

void Mesh::WriteOut(FILE *fp)
{
	//write how many triangles there are
	fwrite(&numTris, sizeof(int), 1, fp);
	
	//write the triangles
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->WriteOut(fp);
	}
}

void Mesh::ReadFrom(char *filename)
{
	FILE *in = fopen(filename, "rb");
	if(!in)
	{
		cout << "ERROR: Couldn't open file " << filename << " to read from.\n";
		return;
	}
	ReadFrom(in);
	fclose(in);
}

void Mesh::ReadFrom(FILE *fp)
{
	//clear the current tris
	if(tris)
	{
		delete[] tris;
		tris = NULL;
		numTris = 0;
	}

	//get the number of triangles
	fread(&numTris, sizeof(int), 1, fp);
	tris = new Triangle[numTris];
	originalTris = new Triangle[numTris];

	//get all of the triangles
	Triangle *tri = tris, *ot = originalTris;
	for(int i = 0; i < numTris; i++, tri++, ot++)
	{
		tri->ReadFrom(fp);
		*ot = *tri;
	}
	centroid = ComputeCentroid();
}

void Mesh::ReColour(Color col)
{
	Triangle::ReColour(tris, numTris, col);
	Triangle::ReColour(originalTris, numTris, col);
}

void Mesh::Translate(Vector d)
{
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->Translate(d);
	}
	centroidChanged = true;
}

void Mesh::Translate(float dx, float dy, float dz)
{
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->Translate(dx, dy, dz);
	}
	centroidChanged = true;
}

void Mesh::MoveTo(Vector d)
{
	Triangle *tri = tris, *ot = originalTris;
	Vector c;
	for(int i = 0; i < numTris; i++, tri++, ot++)
	{
		*tri = *ot;
		tri->RotateAboutPoint(rotation, c);
		tri->Translate(d);
		tri->RecalcNormal();
	}
	centroidChanged = true;
}

void Mesh::MoveTo(float x, float y, float z)
{
	Triangle *tri = tris, *ot = originalTris;
	Vector c;
	for(int i = 0; i < numTris; i++, tri++, ot++)
	{
		*tri = *ot;
		tri->RotateAboutPoint(rotation, c);
		tri->Translate(x, y, z);
		tri->RecalcNormal();
	}
	centroidChanged = true;
}

void Mesh::SetRotation(Vector r)
{
	Triangle *tri = tris, *ot = originalTris;
	Vector c = ComputeCentroid();
	Vector o;
	for(int i = 0; i < numTris; i++, tri++, ot++)
	{
		*tri = *ot;
		tri->RotateAboutPoint(rotation + r, o);
		tri->Translate(c);
		tri->RecalcNormal();
	}
}

void Mesh::SetRotation(float x, float y, float z)
{
	Triangle *tri = tris, *ot = originalTris;
	Vector c = ComputeCentroid();
	Vector o;
	Vector r(x, y, z);
	r += rotation;
	for(int i = 0; i < numTris; i++, tri++, ot++)
	{
		*tri = *ot;
		tri->RotateAboutPoint(r, o);
		tri->Translate(c);
		tri->RecalcNormal();
	}
}

Vector Mesh::ComputeCentroid()
{
	if(centroidChanged)
	{
		Vector centre;
		Triangle *tri = tris;
		for(int i = 0; i < numTris; i++, tri++)
		{
			centre += (*tri).vertices[0].pos + (*tri).vertices[1].pos + (*tri).vertices[2].pos;
		}
		if(numTris > 0)
		{
			centre /= ltof(numTris * 3);
		}
		centroid = centre;
		centroidChanged = false;
	}
	return centroid;
}

void Mesh::RotateAboutX(float angle)
{
	Vector c = ComputeCentroid();
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->RotateAboutXAboutPoint(angle, c);
	}
	rotation.x += angle;
}

void Mesh::RotateAboutY(float angle)
{
	Vector c = ComputeCentroid();
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->RotateAboutYAboutPoint(angle, c);
	}
	rotation.y += angle;
}

void Mesh::RotateAboutZ(float angle)
{
	Vector c = ComputeCentroid();
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->RotateAboutZAboutPoint(angle, c);
	}
	rotation.z += angle;
}

void Mesh::RotateAboutXAboutPoint(float angle, Vector point)
{
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->RotateAboutXAboutPoint(angle, point);
	}
	centroidChanged = true;
	rotation.x += angle;
}

void Mesh::RotateAboutYAboutPoint(float angle, Vector point)
{
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->RotateAboutYAboutPoint(angle, point);
	}
	centroidChanged = true;
	rotation.y += angle;
}

void Mesh::RotateAboutZAboutPoint(float angle, Vector point)
{
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->RotateAboutZAboutPoint(angle, point);
	}
	centroidChanged = true;
	rotation.z += angle;
}

void Mesh::ScaleXAboutPoint(float s, Vector point)
{
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->ScaleXAboutPoint(s, point);
	}
	centroidChanged = true;
}

void Mesh::ScaleYAboutPoint(float s, Vector point)
{
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->ScaleYAboutPoint(s, point);
	}
	centroidChanged = true;
}

void Mesh::ScaleZAboutPoint(float s, Vector point)
{
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->ScaleZAboutPoint(s, point);
	}
	centroidChanged = true;
}

void Mesh::ScaleX(float s)
{
	Vector c = ComputeCentroid();
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->ScaleXAboutPoint(s, c);
	}
}

void Mesh::ScaleY(float s)
{
	Vector c = ComputeCentroid();
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->ScaleYAboutPoint(s, c);
	}
}

void Mesh::ScaleZ(float s)
{
	Vector c = ComputeCentroid();
	Triangle *tri = tris;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->ScaleZAboutPoint(s, c);
	}
}

void Mesh::Explode(float dist, float variance)
{
	Vector c = ComputeCentroid();
	Vector d;
	Triangle *tri = tris;
	float rand;
	variance *= 2;
	for(int i = 0; i < numTris; i++, tri++)
	{
		tri->ComputeCentroid();
		d = tri->centroid - c;
		rand = (Random::randf() - 0.5f) * variance;
		d = d.Normalize() * (dist + rand);
		tri->Translate(d);
	}
}
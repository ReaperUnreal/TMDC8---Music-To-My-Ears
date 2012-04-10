#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "main.h"

enum RenderMode
{
	WIREFRAME = 0,
	LAMBERT,
	GOURAD,
	PHONG
};

class Rasterizer
{
public:
	Rasterizer(Graphics *gr, int initial_size = 20);
	virtual ~Rasterizer();
	
	void SetFOV(float angle);
	void SetAmbientLight(float intensity);
	void SetRenderMode(RenderMode rMode);
	void AddToRenderList(Triangle *triList, int numTris);
	void AddToRenderList(Mesh *meshObj);
	void AddToRenderList(Light *light);
	void ClearRenderList();
	void Render();
	void SortTriangles();
	bool BackfaceCull(Triangle &tri);
	bool Cull3D(Triangle &tri);
	bool ScreenCull(Triangle &tri);
private:
	float dx, dy;
	Graphics *graphics;
	Triangle *renderList;
	int numTris;
	int listSize;
	int mode;
	Light* lightList;
	int numLights;
	float ambient_light;
	Color white, black;

	void RenderTriangle(Triangle &tri);
	void LightFace(Triangle &tri);
	void LightVertices(Triangle &tri);
};

#endif
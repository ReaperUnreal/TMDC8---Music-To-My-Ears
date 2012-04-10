#include "main.h"

Rasterizer::Rasterizer(Graphics *gr, int initial_size) : black(0, 0, 0), white(1, 1, 1)
{
	graphics = gr;
	dx = 0;
	dy = 0;
	numTris = 0;
	mode = WIREFRAME;
	listSize = initial_size;
	renderList = new Triangle[listSize];
	numLights = 0;
	lightList = new Light[10];
	ambient_light = 0.0f;
}

Rasterizer::~Rasterizer()
{
	if(renderList)
	{
		delete[] renderList;
		renderList = NULL;
	}
}

void Rasterizer::SetFOV(float angle)
{
	float d = 1 / (tanf(angle * PI / 180.0f));
	dx = (d / (4.0f / 3.0f) + 1.0f) * 40.0f;
	dy = (d + 1.0f) * -25.0f;
}

void Rasterizer::SetAmbientLight(float intensity)
{
	if(intensity < 0.0f)
		intensity = 0.0f;
	ambient_light = intensity;
}

void Rasterizer::SetRenderMode(RenderMode rMode)
{
	mode = rMode;
}

void Rasterizer::AddToRenderList(Mesh *meshObj)
{
	AddToRenderList(meshObj->tris, meshObj->numTris);
}

void Rasterizer::AddToRenderList(Triangle *triList, int num)
{
	if((num + numTris) > listSize)
	{
		Triangle *temp = new Triangle[listSize];
		for(int i = 0; i < listSize; i++)
		{
			temp[i] = renderList[i];
		}
		delete[] renderList;
		renderList = new Triangle[(num + numTris) * 2];
		for(i = 0; i < listSize; i++)
		{
			renderList[i] = temp[i];
		}
		delete[] temp;
		listSize = (num + numTris) * 2;
	}
	for(int i = 0; i < num; i++)
	{
		renderList[numTris + i] = triList[i];
	}
	numTris += num;
}

void Rasterizer::AddToRenderList(Light *light)
{
	if(numLights < 10)
	{
		lightList[numLights] = *light;
		numLights++;
	}
}

void Rasterizer::ClearRenderList()
{
	delete[] renderList;
	renderList = new Triangle[listSize];
	numTris = 0;
	delete[] lightList;
	lightList = new Light[10];
	numLights = 0;
}

void Rasterizer::Render()
{
	//sort the triangles by z
	SortTriangles();

	for(int i = 0; i < numTris; i++)
	{
		RenderTriangle(renderList[i]);
	}
}

void Rasterizer::SortTriangles()
{
	int i;
	Triangle temp, *iter = NULL;

	for(i = 0, iter = renderList; i < numTris; i++, iter++)
	{
		iter->ComputeNormal();
		iter->normal.Normalize();
		iter->vertices[0].normal.Normalize();
		iter->vertices[1].normal.Normalize();
		iter->vertices[2].normal.Normalize();
		iter->ComputeCentroid();
	}

	//heapsort!
	if(numTris > 1)
	{
		int n = numTris, j, w;
		i = n>>1;
		for(;;)
		{
			if(i > 0)
			{
				temp = renderList[--i];
			}
			else
			{
				if(--n <= 0)
					return;
				temp = renderList[n];
				renderList[n] = renderList[0];
			}

			j = i;
			w = (i << 1) + 1;

			while (w < n)
			{
				if((w + 1 < n) && (renderList[w + 1].centroid.z > renderList[w].centroid.z))
				{
					++w;
				}
				if(renderList[w].centroid.z > temp.centroid.z)
				{
					renderList[j] = renderList[w];
					j = w;
					w = (j << 1) + 1;
				}
				else
					break;
			}
			renderList[j] = temp;
		}
	}
}

/*void Rasterizer::SortTriangles()
{
	//I'm too lazy to code up a merge, heap, or quicksort,
	//so it looks like it's going to be a shell sort,
	//the fastest of the n^2 sorts (Quicksort not included)
	int increment, i, j, k;
	bool end_now;
	Triangle temp, *iter = NULL;

	for(i = 0, iter = renderList; i < numTris; i++, iter++)
	{
		iter->ComputeNormal();
		iter->normal.Normalize();
		iter->vertices[0].normal.Normalize();
		iter->vertices[1].normal.Normalize();
		iter->vertices[2].normal.Normalize();
		iter->ComputeCentroid();
	}

	increment = 3;
	while(increment > 0)
	{
		for(i = 0; i < numTris; i++)
		{
			end_now = false;
			j = i;
			k = j - increment;
			temp = renderList[i];
			while((j >= increment) && (!end_now))
			{
				if(renderList[k].centroid.z > temp.centroid.z)
				{
					renderList[j] = renderList[k];
					j = k;
					k -= increment;
				}
				else
				{
					end_now = true;
				}
			}
			renderList[j] = temp;
		}
		if(increment / 2 != 0)
		{
			increment /= 2;
		}
		else if(increment == 1)
		{
			increment = 0;
		}
		else
		{
			increment = 1;
		}
	}
}*/

void Rasterizer::RenderTriangle(Triangle &tri)
{
	//3D Culling
	if(!Cull3D(tri))
	{
		return;
	}

	//Backface Culling
	/*if(!BackfaceCull(tri))
	{
		return;
	}*/

	//do the lighting
	if(mode == WIREFRAME)
	{
		LightVertices(tri);
	}
	else if(mode == LAMBERT)
	{
		LightFace(tri);
	}
	else if(mode == GOURAD)
	{
		LightVertices(tri);
	}

	//perspective divide
	for(int i = 0; i < 3; i++)
	{
		tri.vertices[i].pos.x = dx * (tri.vertices[i].pos.x / tri.vertices[i].pos.z) + 40;
		tri.vertices[i].pos.y = dy * (tri.vertices[i].pos.y / tri.vertices[i].pos.z) + 25;
	}

	//culling
	if(!ScreenCull(tri))
	{
		//return;
	}

	//draw the triangle
	if(mode == WIREFRAME)
	{
		graphics->DrawTriangle(&tri);
	}
	else if(mode == LAMBERT)
	{
		graphics->FloodTriangle(&tri);
	}
	else if(mode == GOURAD)
	{
		graphics->FillTriangle(&tri);
	}
}

void Rasterizer::LightFace(Triangle &tri)
{
	if(numLights <= 0)
		return;
	Color col = (tri.vertices[0].col + tri.vertices[1].col + tri.vertices[2].col) / 3.0f;
	Color accumulator = col * ambient_light;

	Vector norm, dir;
	float shade;
	norm = tri.normal;

	for(int i = 0; i < numLights; i++)
	{
		dir = tri.centroid - lightList[i].pos;
		dir.Normalize();
		shade = dir.Dot(norm);
		if(shade < 0.0f)
			shade = 0.0f;
		accumulator += col * shade * lightList[i].intensity;
	}

	accumulator.Clamp();

	tri.vertices[0].col = accumulator;
	tri.vertices[1].col = accumulator;
	tri.vertices[2].col = accumulator;
}

void Rasterizer::LightVertices(Triangle &tri)
{
	if(numLights <= 0)
		return;
	Color v1c = tri.vertices[0].col;
	Color v2c = tri.vertices[1].col;
	Color v3c = tri.vertices[2].col;
	tri.vertices[0].col = v1c * ambient_light;
	tri.vertices[1].col = v2c * ambient_light;
	tri.vertices[2].col = v3c * ambient_light;

	Vector norm, dir;
	float shade;

	for(int i = 0; i < numLights; i++)
	{
		dir = tri.vertices[0].pos - lightList[i].pos;
		dir.Normalize();
		norm = tri.vertices[0].normal;
		shade = dir.Dot(norm);
		if(shade < 0.0f)
			shade = 0.0f;
		tri.vertices[0].col += v1c * (shade * lightList[i].intensity);

		dir = tri.vertices[1].pos - lightList[i].pos;
		dir.Normalize();
		norm = tri.vertices[1].normal;
		shade = dir.Dot(norm);
		if(shade < 0.0f)
			shade = 0.0f;
		tri.vertices[1].col += v2c * (shade * lightList[i].intensity);

		dir = tri.vertices[2].pos - lightList[i].pos;
		dir.Normalize();
		norm = tri.vertices[2].normal;
		shade = dir.Dot(norm);
		if(shade < 0.0f)
			shade = 0.0f;
		tri.vertices[2].col += v3c * (shade * lightList[i].intensity);
	}

	tri.vertices[0].col.Clamp();
	tri.vertices[1].col.Clamp();
	tri.vertices[2].col.Clamp();
}

bool Rasterizer::ScreenCull(Triangle &tri)
{
	//get the vertices
	Vector v[3];
	v[0] = tri.vertices[0].pos;
	v[1] = tri.vertices[1].pos;
	v[2] = tri.vertices[2].pos;

	//check left
	if((v[0].x < 0) && (v[1].x < 0) && (v[2].x < 0))
		return false;

	//check top
	if((v[0].y < 0) && (v[1].y < 0) && (v[2].y < 0))
		return false;

	//check right
	if((v[0].x > 79) && (v[1].x > 79) && (v[2].x > 79))
		return false;

	//check bottom
	if((v[0].y > 49) && (v[1].y > 49) && (v[2].y > 49))
		return false;

	return true;
}

bool Rasterizer::Cull3D(Triangle &tri)
{
	//get the vertices
	Vector v[3];
	v[0] = tri.vertices[0].pos;
	v[1] = tri.vertices[1].pos;
	v[2] = tri.vertices[2].pos;

	//check close clipping plane
	if((v[0].z <= epsilon) || (v[1].z <= epsilon) || (v[2].z <= epsilon))
		return false;

	//check for a vertex that would be divided by 0
	if(((v[0].z <= epsilon) && (v[0].z >= -epsilon)) ||
	   ((v[1].z <= epsilon) && (v[1].z >= -epsilon)) ||
	   ((v[2].z <= epsilon) && (v[2].z >= -epsilon)))
		return false;

	return true;
}

bool Rasterizer::BackfaceCull(Triangle &tri)
{
	tri.ComputeCentroid();
	tri.ComputeNormal();
	Vector view = tri.centroid;
	if(view.Dot(tri.normal) < 0)
		return false;
	return true;
}
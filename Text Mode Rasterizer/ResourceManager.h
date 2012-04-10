#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_h

#include "main.h"

class ResourceManager
{
public:
	ResourceManager(int initialCount = 8);
	virtual ~ResourceManager(void);

	int GetNumMeshes(void);
	Mesh* GetMesh(int meshNum);
	int AddMesh(Mesh *mesh);
	void DeleteMeshList(void);

	int GetNumFloats(void);
	float* GetFloat(int floatNum);
	int AddFloat(float *f);
	void DeleteFloatList(void);

	int GetNumBackFills(void);
	BufferFiller* GetBackFill(int bFillNum);
	int AddBackFill(BufferFiller *bf);
	void DeleteBackFillList(void);

	void DeleteAll(void);
private:
	Mesh **meshList;
	int numMeshes;
	int meshCapacity;

	float **floatList;
	int numFloats;
	int floatCapacity;

	BufferFiller **bFillList;
	int numBFills;
	int bFillCapacity;
};

#endif
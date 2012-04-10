#include "main.h"

ResourceManager::ResourceManager(int initialCount) : meshCapacity(initialCount), floatCapacity(initialCount), bFillCapacity(initialCount)
{
	meshList = new Mesh*[meshCapacity];
	numMeshes = 0;

	floatList = new float*[floatCapacity];
	numFloats = 0;

	bFillList = new BufferFiller*[bFillCapacity];
	numBFills = 0;
}

ResourceManager::~ResourceManager(void)
{
	if(meshList)
	{
		delete[] meshList;
		meshList = NULL;
	}

	if(floatList)
	{
		delete[] floatList;
		floatList = NULL;
	}

	if(bFillList)
	{
		delete[] bFillList;
		bFillList = NULL;
	}
}

int ResourceManager::GetNumMeshes(void)
{
	return numMeshes;
}

Mesh* ResourceManager::GetMesh(int meshNum)
{
	if((meshNum < 0) || (meshNum >= numMeshes))
	{
		return NULL;
	}
	else
	{
		return meshList[meshNum];
	}
}

int ResourceManager::AddMesh(Mesh *mesh)
{
	if(numMeshes == meshCapacity)
	{
		Mesh **temp = new Mesh*[numMeshes];
		for(int i = 0; i < numMeshes; i++)
		{
			*(temp + i) = *(meshList + i);
		}
		delete[] meshList;
		meshCapacity *= 2;
		meshList = new Mesh*[meshCapacity];
		for(int i = 0; i < numMeshes; i++)
		{
			*(meshList + i) = *(temp + i);
		}
		delete[] temp;
	}
	*(meshList + numMeshes) = mesh;
	numMeshes++;

	return numMeshes - 1;
}

void ResourceManager::DeleteMeshList(void)
{
	for(int i = 0; i < numMeshes; i++)
	{
		delete *(meshList + i);
		*(meshList + i) = NULL;
	}
	numMeshes = 0;
}

int ResourceManager::GetNumFloats(void)
{
	return numFloats;
}

float* ResourceManager::GetFloat(int floatNum)
{
	if((floatNum < 0) || (floatNum >= numFloats))
	{
		return NULL;
	}
	else
	{
		return floatList[floatNum];
	}
}

int ResourceManager::AddFloat(float *f)
{
	if(numFloats == floatCapacity)
	{
		float **temp = new float*[numFloats];
		for(int i = 0; i < numFloats; i++)
		{
			*(temp + i) = *(floatList + i);
		}
		delete[] floatList;
		floatCapacity *= 2;
		floatList = new float*[floatCapacity];
		for(int i = 0; i < numFloats; i++)
		{
			*(floatList + i) = *(temp + i);
		}
		delete[] temp;
	}
	*(floatList + numFloats) = f;
	numFloats++;

	return numFloats - 1;
}

void ResourceManager::DeleteFloatList(void)
{
	for(int i = 0; i < numFloats; i++)
	{
		delete *(floatList + i);
		*(floatList + i) = NULL;
	}
	numFloats = 0;
}

int ResourceManager::GetNumBackFills(void)
{
	return numBFills;
}

BufferFiller* ResourceManager::GetBackFill(int bFillNum)
{
	if((bFillNum < 0) || (bFillNum >= numBFills))
	{
		return NULL;
	}
	else
	{
		return bFillList[bFillNum];
	}
}

int ResourceManager::AddBackFill(BufferFiller *bf)
{
	if(numBFills == bFillCapacity)
	{
		BufferFiller **temp = new BufferFiller*[numBFills];
		for(int i = 0; i < numBFills; i++)
		{
			*(temp + i) = *(bFillList + i);
		}
		delete[] bFillList;
		bFillCapacity *= 2;
		bFillList = new BufferFiller*[bFillCapacity];
		for(int i = 0; i < numBFills; i++)
		{
			*(bFillList + i) = *(temp + i);
		}
		delete[] temp;
	}
	*(bFillList + numBFills) = bf;
	numBFills++;

	return numBFills - 1;
}

void ResourceManager::DeleteBackFillList(void)
{
	for(int i = 0; i < numBFills; i++)
	{
		delete *(bFillList + i);
		*(bFillList + i) = NULL;
	}
	numBFills = 0;
}

void ResourceManager::DeleteAll(void)
{
	DeleteMeshList();
	DeleteFloatList();
	DeleteBackFillList();
}
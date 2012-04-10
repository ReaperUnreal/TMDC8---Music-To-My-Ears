#ifndef MAIN_H
#define MAIN_H

#define WIN_32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

__inline long lrint(double flt)
{
	int intgr;
	__asm
	{
		fld flt
		fistp intgr
	}
	return intgr;
}

__inline long lrintf(float flt)
{
	int intgr;
	__asm
	{
		fld flt
		fistp intgr
	}
	return intgr;
}

__inline float ltof(long intgr)
{
	float flt;
	__asm
	{
		fild intgr
		fstp flt
	}
	return flt;
}

__inline double ltod(long intgr)
{
	double flt;
	__asm
	{
		fild intgr
		fstp flt
	}
	return flt;
}

inline void BreakIf(bool condition)
{
	if(condition)
	{
		__asm
		{
			int 3
		}
	}
}

inline int ConvertColor(float rgb)
{
	int temp = lrintf(rgb * 63.0f);
	if(temp > 63)
		temp = 63;
	if(temp < 0)
		temp = 0;
	return temp;
}

inline long iRand(int low, int high)
{
	return low + (rand() % (high - low));
}

inline float fRand(float low, float high)
{
	float rNum = ltof(rand()), diff = high - low;
	return low + rNum - floorf(rNum / diff) * diff;
}

inline float invsqrtf(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int *)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float *)&i;
	x = x * (1.5f - xhalf * x * x);
	//x = x * (1.5f - xhalf * x * x);
	return x;
}

#include "fmod.h"
#include "Random.h"
#include "Complex.h"
#include "Vector.h"
#include "Matrix.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Light.h"
#include "ColorMap.h"
#include "Console.h"
#include "BufferFiller.h"
#include "Graphics.h"
#include "Rasterizer.h"
#include "Font.h"
#include "ResourceManager.h"

#define KeyDown(key) ((GetAsyncKeyState(key) & 0x8000) ? 1 : 0)
#define KeyUp(key)   ((GetAsyncKeyState(key) & 0x8000) ? 0 : 1)
#define MIN(a, b)	 (((a) < (b)) ? (a) : (b))
#define MAX(a, b)	 (((a) > (b)) ? (a) : (b))
const float PI = 3.14159265358979f;
const float TWOPI = PI * 2.0f;
const float FOURPI = PI * 4.0f;
const float EIGHTPI = PI * 8.0f;
const float SIXTEENPI = PI * 16.0f;
const float INVPI = 1.0f / PI;
const float INVTWOPI = 0.5f / PI;
const float PIBY2 = PI / 2.0f;
const float PIBY3 = PI / 3.0f;
const float PIBY6 = PI / 6.0f;
const float ROOT3 = sqrtf(3.0f);
const float ROOT3BY2 = sqrtf(3.0f)/2.0f;
const float THIRD = 1.0f / 3.0f;
const float TWOTHIRD = 2.0f / 3.0f;
const float epsilon = 0.0001f;

inline float lerp(float start, float end, float path)
{
	float diff = end - start;
	return start + path * diff;
}

inline int lerp(int start, int end, float path)
{
	float diff = ltof(end - start);
	return start + lrintf(path * diff + 0.5f);
}

inline float slerp(float start, float end, float path)
{
	float diff = end - start;
	if(path <= 0)
		path = 0;
	else if(path >= 1)
		path = 1;
	else
		path = 0.5f - 0.5f * cosf(path * PI);
	return start + path * diff;
}

inline int slerp(int start, int end, float path)
{
	float diff = ltof(end - start);
	if(path <= 0)
		path = 0;
	else if(path >= 1)
		path = 1;
	else
		path = 0.5f - 0.5f * cosf(path * PI);
	return start + lrintf(path * diff + 0.5f);
}

inline float Rand(float range, float start = 0.0f) { return start + (ltof(rand()) / RAND_MAX) * range; }

#endif
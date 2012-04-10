#include "main.h"
//note: will be using 64x64x64 color map, so, 2^6 colors per channel

//constructor and destructor
ColorMap::ColorMap()
{
	initGood = false;
	BlockMap = NULL;
}

ColorMap::~ColorMap()
{
	if(BlockMap)
		delete[] BlockMap;
}

//general ColorMap utility functions
bool ColorMap::isInit()
{
	return initGood;
}

//graphics functions
short int ColorMap::CalculateColor(int red, int green, int blue)
{
	#define square(num) ((num) * (num))
	int a, b, c, d, ch = 0, co = 0, i;
	int lastdist, dist;
	float charVal, invCharVal;

	lastdist = 0x7fffffff;

	for(c = 0, d = 0; c < 16; c++, d += 3)
	{
		dist = square(palette[d] - red) + square(palette[d + 1] - green) + square(palette[d + 2] - blue);
		if(dist < lastdist)
		{
			lastdist = dist;
			co = c;
			ch = 219; //100% ascii block
		}
	}
	c = co;
	d = c * 3;
	for(b = 0, a = 0; b < 16; b++, a += 3)
	{
		dist = lrintf(square(palette[a] + (palette[d] * 0.7f) - red) + square(palette[a + 1] + (palette[d + 1] * 0.7f) - green) + square(palette[a + 2] + (palette[d + 2] * 0.7f) - blue));
		if(dist < lastdist)
		{
			lastdist = dist;
			if(b > c)
				co = b + (c << 4);
			else
				co = c + (b << 4);
			ch = 177; //50% ascii block
		}
		dist = lrintf(square((palette[a] * 0.8f) + (palette[d] * 0.2f) - red) + square((palette[a + 1] * 0.8f) + (palette[d + 1] * 0.2f) - green) + square((palette[a + 2] * 0.8f) + (palette[d + 2] * 0.2f) - blue));
		if(dist < lastdist)
		{
			lastdist = dist;
			if(b > c)
			{
				co = b + (c << 4);
				ch = 178; //75% block in ascii
			}
			else
			{
				co = c + (b << 4);
				ch = 176; //25% block in ascii
			}
		}
		dist = lrintf(square((palette[a] * 0.2f) + (palette[d] * 0.8f) - red) + square((palette[a + 1] * 0.2f) + (palette[d + 1] * 0.8f) - green) + square((palette[a + 2] * 0.2f) + (palette[d + 2] * 0.8f) - blue));
		if(dist < lastdist)
		{
			lastdist = dist;
			if(c > b)
			{
				co = c + (b << 4);
				ch = 178; //75% block in ascii
			}
			else
			{
				co = b + (c << 4);
				ch = 176; //25% block in ascii
			}
		}

		//more colour map
		for(i = 0; i < NUM_COLOR_CHARS; i++)
		{
			charVal = charValMap[i];
			invCharVal = 1.0f - charVal;
			dist = lrintf(square((palette[a] * charVal) + (palette[d] * invCharVal) - red) + square((palette[a + 1] * charVal) + (palette[d + 1] * invCharVal) - green) + square((palette[a + 2] * charVal) + (palette[d + 2] * invCharVal) - blue));
			if(dist < lastdist)
			{
				lastdist = dist;
				if(c < b)
					co = c + (b << 4);
				else
					co = b + (c << 4);
				ch = charMap[i];
			}
		}
	}
	return (short)((co << 8) + ch);
}

void ColorMap::BuildLUT(void)
{
	int r, g, b;
	if(BlockMap)
		delete[] BlockMap;
	BlockMap = new short[64*64*64];
	for(r = 0; r < 64; r++)
	{
		for(g = 0; g < 64; g++)
		{
			for(b = 0; b < 64; b++)
			{
				*(BlockMap + (r << 12) + (g << 6) + b) = CalculateColor(r, g, b);
			}
		}
	}
	initGood = true;
}

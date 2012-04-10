//note: a colour table of 64x64x64 will be used, so 2^6 colours per channel

#ifndef COLORMAP_H
#define COLORMAP_H

#include "main.h"

#define NUM_COLOR_CHARS 14

static const int palette[48]= {0, 0, 0,  0, 0,32,  0,32, 0,  0,32,32, 32, 0, 0, 32, 0,32, 32,32, 0, 32,32,32, 9999,9999,9999,  0, 0,63,  0,63, 0,  0,63,63, 63, 0, 0, 63, 0,63, 63,63, 0, 63,63,63};

static const unsigned char charMap[NUM_COLOR_CHARS] = {'#',   '@',   '&',  '$',  '5',  'Z',   'O',    'I',  '!',  ';',    ':',    '-',  ',',    '.'};
static const float charValMap[NUM_COLOR_CHARS] =      {0.75f, 0.65f, 0.6f, 0.5f, 0.4f, 0.375f, 0.35f, 0.3f, 0.2f, 0.175f, 0.165f, 0.1f, 0.08f, 0.07f};

class ColorMap
{
public:
	//constructor and destructor
	ColorMap(void);
	virtual ~ColorMap(void);

	//general ColorMap utility functions
	bool isInit(void);

	//graphics functions
	short int CalculateColor(int red, int green, int blue);
	void BuildLUT(void);
	inline short __fastcall ColorMap::GetColor(int r, int g, int b)
	{
		return *(BlockMap + (r << 12) + (g << 6) + b);
	}
private:
	short int* BlockMap;
	bool initGood;
};

#endif
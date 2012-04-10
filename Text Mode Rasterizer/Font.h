#ifndef FONT_H
#define FONT_H
#include "main.h"

extern Graphics *graphics;

class Letter
{
public:
	Letter()
	{
		character = NULL;
		data = NULL;
		width = height = 0;
	}
	~Letter()
	{
		if(data)
		{
			delete[] data;
			data = NULL;
		}
	}
	char character;
	unsigned short *data;
	int width, height;
};

class Font
{
public:
	Font();
	~Font();
	bool LoadFont(char *filename);
	void PrintCharacter(char character, Color col, int x, int y);
	void PrintPhrase(char *phrase, Color col, int x, int y);
private:
	Letter *letters;
	int numLetters;
};

#endif
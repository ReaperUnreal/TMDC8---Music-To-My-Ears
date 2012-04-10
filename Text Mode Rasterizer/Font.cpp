#include "main.h"

Font::Font()
{
	numLetters = 46;
	letters = NULL;
}

Font::~Font()
{
	if(letters)
	{
		delete[] letters;
		letters = NULL;
	}
}

bool Font::LoadFont(char *filename)
{
	letters = new Letter[numLetters];
	FILE *f = fopen(filename, "r");

	if(!f)
		return false;

	//calculate the colors
	ColorMap *colMap = graphics->GetColorMap();
	unsigned short white = colMap->GetColor(63, 63, 63);
	unsigned short black = colMap->GetColor(0, 0, 0);

	int width = 0;
	int pixel;
	for(int i = 0; i < numLetters; i++)
	{
		//initializations
		pixel = 0;

		//get the width
		fscanf(f, "%d", &width);

		//fill out some of the letter's info
		letters[i].data = new unsigned short[5 * width];
		letters[i].height = 5;
		letters[i].width = width;
		if(i < 26)
			letters[i].character = 'A' + i;
		else if(i < 36)
			letters[i].character = '0' + (i - 26);
		else if(i == 36)
			letters[i].character = '.';
		else if(i == 37)
			letters[i].character = ',';
		else if(i == 38)
			letters[i].character = '!';
		else if(i == 39)
			letters[i].character = '?';
		else if(i == 40)
			letters[i].character = ':';
		else if(i == 41)
			letters[i].character = '_';
		else if(i == 42)
			letters[i].character = '\'';
		else if(i == 43)
			letters[i].character = '-';
		else if(i == 44)
			letters[i].character = '(';
		else if(i == 45)
			letters[i].character = ')';

		//get the pixels
		char *str = new char[width + 1];
		for(int j = 0; j < 5; j++)
		{
			fscanf(f, "%s", str);
			for(int k = 0; k < width; k++)
			{
				if(str[k] == '0')
					letters[i].data[pixel] = white;
				else
					letters[i].data[pixel] = black;
				pixel++;
			}
			
		}
		delete[] str;
	}

	fclose(f);
	return true;
}

void Font::PrintCharacter(char character, Color col, int x, int y)
{
	//uppercase
	if((character >= 97) && (character <= 122))
		character -= 32;

	//filter out the bad characters
	if(((character < 65) || (character > 90)) && ((character < 48) || (character > 57)) && (character != 46) && (character != 44) && (character != 33) && (character != 63) && (character != 58) && (character != 95) && (character != 39) && (character != 45) && (character != 40) && (character != 41))
		return;

	//get rid of bad cases
	if((x >= 80) || (y >= 50))
		return;

	//find the letter
	Letter *theLetter = NULL;
	for(int i = 0; i < numLetters; i++)
	{
		if(letters[i].character == character)
		{
			theLetter = letters + i;
			break;
		}
	}

	if(theLetter == NULL)
		return;

	//get rid of more bad cases
	if((x + theLetter->width - 1 < 0) || (y + 4 < 0))
		return;

	//get the needed colours
	ColorMap *colMap = graphics->GetColorMap();
	short black = colMap->GetColor(0, 0, 0);
	int r = lrintf(col.r * 63);
	int g = lrintf(col.g * 63);
	int b = lrintf(col.b * 63);
	if(r > 63) r = 63;
	if(g > 63) g = 63;
	if(b > 63) b = 63;
	short textCol = colMap->GetColor(r, g, b);

	//plot the letter
	for(int py = y, l = 0, ly = 0; ly < theLetter->height; ly++, py++)
	{
		if(py < 0)
		{
			l+= theLetter->width;
			continue;
		}
		if(py > 49)
			break;
		for(int px = x, lx = 0; lx < theLetter->width; px++, l++, lx++)
		{
			if((px < 0) || (px > 79))
				continue;
			if(theLetter->data[l] == black)
				graphics->SetEntry(px + py * 80, textCol);
		}
	}
}

void Font::PrintPhrase(char *phrase, Color col, int x, int y)
{
	Letter *theLetter = NULL;
	char currChar;
	for(unsigned int i = 0; i < strlen(phrase); i++)
	{
		//uppercase
		if((phrase[i] >= 97) && (phrase[i] <= 122))
			currChar = phrase[i] - 32;
		else
			currChar = phrase[i];

		//check for space
		if(currChar == ' ')
		{
			x += 4;
			continue;
		}
		//find the letter
		for(int j = 0; j < numLetters; j++)
		{
			if(letters[j].character == currChar)
			{
				theLetter = letters + j;
				break;
			}
		}

		if(theLetter == NULL)
			continue;

		PrintCharacter(phrase[i], col, x, y);

		x += theLetter->width;
		x++;
	}
}
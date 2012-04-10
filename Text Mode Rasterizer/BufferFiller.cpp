#include "main.h"

BufferFiller::BufferFiller(void)
{
}

BufferFiller::~BufferFiller(void)
{
}

void BufferFiller::FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap)
{
}


ColorFill::ColorFill(Color c) : col(c)
{
}

ColorFill::~ColorFill(void)
{
}

void ColorFill::SetColor(Color c)
{
	col = c;
}

void ColorFill::FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap)
{
	short fillColor = cMap->GetColor(ConvertColor(col.r), ConvertColor(col.g), ConvertColor(col.b));
	for(int i = 0; i < 4000; i++, buffer++)
	{
		*buffer = fillColor;
	}
}

AnimatedCircle::AnimatedCircle(ColorMap *cMap)
{
	phase = 0.0f;
	for(int i = 0; i < 20; i++)
	{
		palette[i] = cMap->GetColor(lrintf((sinf(ltof(i)) + 1) * 31.5f), lrintf((sinf(ltof(i)) + 1) * 31.5f), lrintf((sinf(ltof(i)) + 1) * 31.5f));
	}
}

AnimatedCircle::~AnimatedCircle(void)
{
}

void AnimatedCircle::FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap)
{
	float shade1, shade2, shade3, shade;
	for(int y = 0; y < 50; y++)
	{
		for(int x = 0; x < 80; x++, buffer++)
		{
			shade1 = (sinf(ltof((40 - x) * (40 - x) + (25 - y) * (25 - y)) * 0.0025f + phase) + 1.0f) * 0.5f;
			shade2 = (sinf(ltof(x + y) * 0.1f + 0.233f + phase * 0.33f) + 1.0f) * 0.5f;
			shade3 = (sinf(ltof(y) * 0.25f + 1.174f - phase * 0.77f) + 1.0f) * 0.5f;
			shade = (2.0f * shade1 + shade2 + shade3) / 4.0f;
			*buffer = palette[lrintf(shade * 19.0f)];
		}
	}
	phase += 0.1f;
}

Tunnel::Tunnel(ColorMap *cMap, Color c1, Color c2)
{
	phase = 0.0f;
	uphase = 0.0f;
	vphase = 0.0f;
	float t;
	Color c;
	for(int i = 0; i < 20; i++)
	{
		t = ltof(i) / 19.0f;
		c = c1 * t + c2 * (1 - t);
		palette[i] = cMap->GetColor(lrintf(c.r * 63.0f), lrintf(c.g * 63.0f), lrintf(c.b * 63.0f));
	}
}

Tunnel::~Tunnel(void)
{
}

void Tunnel::SetColors(ColorMap *cMap, Color c1, Color c2)
{
	float t;
	Color c;
	for(int i = 0; i < 20; i++)
	{
		t = ltof(i) / 19.0f;
		c = c1 * t + c2 * (1 - t);
		palette[i] = cMap->GetColor(lrintf(c.r * 63.0f), lrintf(c.g * 63.0f), lrintf(c.b * 63.0f));
	}
}

void Tunnel::SetPhase(float p)
{
	phase = p;
}

void Tunnel::SetUPhase(float up)
{
	uphase = up;
}

void Tunnel::SetVPhase(float vp)
{
	vphase = vp;
}

void Tunnel::FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap)
{
	float dx, dy, t, u, v, shade, shade1, shade2, shade3;
	for(int y = 0; y < 50; y++)
	{
		dy = 1.0f - (ltof(y) * 0.04f);
		for(int x = 0; x < 80; x++, buffer++)
		{
			//calculate tunnel
			dx = (ltof(x) * 0.025f) - 1.0f;
			t = 2.0f * invsqrtf(dx * dx + dy * dy);
			u = (atan2f(dx * t, dy * t) + PI) * INVPI;
			v = t * 0.2f + vphase;
			u += uphase;
			u = u - floorf(u);
			v = v - floorf(v);

			//plasma!
			shade1 = (sinf(9.0f * (0.5f - u) * (0.5f - u) + 6.25f * (0.5f - v) * (0.5f - v) + phase) + 1.0f) * 0.5f;
			shade2 = (sinf(8.0f * u + 5.0f * v + 0.233f + phase * 0.33f) + 1.0f) * 0.5f;
			shade3 = (sinf(12.5f * v + 1.174f - phase * 0.77f) + 1.0f) * 0.5f;
			shade = (2.0f * shade1 + shade2 + shade3)  * 0.25f;
			*buffer = palette[lrintf(shade * 19.0f)];
		}
	}
}

Metaballs::Metaballs(Color inner, Color back) : phase(0.0f), ball(inner), background(back)
{
}

Metaballs::~Metaballs(void)
{
}

void Metaballs::SetBackColor(Color c)
{
	background = c;
}

void Metaballs::SetBallColor(Color c)
{
	ball = c;
}

void Metaballs::SetPhase(float p)
{
	phase = p;
}

void Metaballs::FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap)
{
	float r = 20.0f * (1.25f - fabsf(0.7f * cosf(0.2f * phase)));//15.0f * cosf(0.3f * (phase + 0.1f * phase));
	int xpos[4] = {40 + lrintf(cosf(phase * 0.23f) * 10.0f), 40 + lrintf(cosf(phase * -0.22f) * 10.0f), 40 + lrintf(r * cosf(phase * 0.1f)), 40 + lrintf(40.0f * sinf(0.08f * phase))};
	int ypos[4] = {20 + lrintf(sinf(phase * 0.23f) * 10.0f), 30 + lrintf(sinf(phase * -0.22f) * 10.0f), 25 + lrintf(r * sinf(phase * 0.1f)), 25 + lrintf(25.0f * cosf(0.1f * phase))};
	float pointVal, dist;
	int dy, dx;
	short white = cMap->GetColor(lrintf(ball.r * 63.0f), lrintf(ball.g * 63.0f), lrintf(ball.b * 63.0f));
	short black = cMap->GetColor(lrintf(background.r * 63.0f), lrintf(background.g * 63.0f), lrintf(background.b * 63.0f));
	for(int y = 0; y < 50; y++)
	{
		for(int x = 0; x < 80; x++, buffer++)
		{
			pointVal = 0.0f;
			for(int i = 0; i < 4; i++)
			{
				dx = x - xpos[i];
				dy = y - ypos[i];
				dist = invsqrtf(ltof(dx * dx + dy * dy));
				if(dist > 1)
					dist = 1;
				pointVal += dist * 2.0f;
			}
			if(pointVal > 0.6f)
				*buffer = white;
			else
				*buffer = black;
		}
	}
}

RadialFill::RadialFill(Color outerCol, Color innerCol, float r) : c1(outerCol), c2(innerCol), radius(r)
{
}

RadialFill::~RadialFill(void)
{
}

void RadialFill::SetRadius(float r)
{
	radius = r;
}

void RadialFill::SetInnerColor(Color c)
{
	c2 = c;
}

void RadialFill::SetOuterColor(Color c)
{
	c1 = c;
}

void RadialFill::FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap)
{
	int dx, dy;
	float i;
	Color c;
	for(int y = 0; y < 50; y++)
	{
		dy = 25 - y;
		for(int x = 0; x < 80; x++, buffer++)
		{
			dx = 40 - x;
			i = sqrtf(ltof(dx * dx + dy * dy)) - radius + 1.0f;
			if(i < 1.0f)
				i = 1.0f;
			i = 1.0f / i;
			if(i > 1.0f)
				i = 1.0f;
			if(i < 0.0f)
				i = 0.0f;
			c = c2 * i + c1 * (1 - i);
			c *= 63.0f;
			*buffer = cMap->GetColor(lrintf(c.r), lrintf(c.g), lrintf(c.b));
		}
	}
}

CircleFill::CircleFill(float r, Color col) : radius(r), c(col)
{
}

CircleFill::~CircleFill(void)
{
}

void CircleFill::SetRadius(float r)
{
	radius = r;
}

void CircleFill::FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap)
{
	short col = cMap->GetColor(lrintf(c.r * 63.0f), lrintf(c.g * 63.0f), lrintf(c.b * 63.0f));
	float dist;
	float rs = radius * radius;
	int dx, dy;
	for(int y = 0; y < 50; y++)
	{
		dy = y - 25;
		for(int x = 0; x < 80; x++, buffer++)
		{
			dx = x - 40;
			dist = ltof(dx * dx + dy * dy);
			if(dist < rs)
				*buffer = col;
		}
	}
}

Fire::Fire(ColorMap *cMap) : phase(0.0f), source(true)
{
	//zero out both heat buffers
	for(int i = 0; i < 4000; i++)
	{
		fireBuffer1[i] = 0;
		fireBuffer2[i] = 0;
	}

	//create the palette
	for(int i = 0; i < 255; i++)
	{
		if(i < 71)
		{
			palette[i] = cMap->GetColor(0, 0, 0);
		}
		else if(i < 81)
		{
			palette[i] = cMap->GetColor(((i - 70) * 63) / 10, ((i - 70) * 63) / 10, 0);
		}
		else if(i < 128)
		{
			palette[i] = cMap->GetColor(63, 63 - ((i - 80) * 63) / 47, 0);
		}
		else if(i < 190)
		{
			palette[i] = cMap->GetColor(190 - i, 0, i - 127);
		}
		else if(i < 230)
		{
			palette[i] = cMap->GetColor(((i - 190) * 63) / 40, ((i - 190) * 63) / 40, 63);
		}
		else
		{
			palette[i] = cMap->GetColor(63, 63, 63);
		}
	}

	//load up the note
	FILE *noteFile = fopen("data/note.dat", "r");
	if(noteFile)
	{
		fscanf(noteFile, "%d", &notew);
		fscanf(noteFile, "%d", &noteh);
		note = new char[notew * noteh];
		char *currPix = note;
		char line[255];
		for(int row = 0; row < noteh; row++)
		{
			fscanf(noteFile, "%s", line);
			for(int col = 0; col < notew; col++, currPix++)
			{
				if(line[col] == '0')
				{
					*currPix = 0;
				}
				else
				{
					*currPix = 1;
				}
			}
		}
		fclose(noteFile);
	}
}

Fire::~Fire(void)
{
	if(note)
	{
		delete[] note;
		note = NULL;
	}
}

void Fire::SetPhase(float p)
{
	phase = p;
}

void Fire::SetSource(bool src)
{
	source = src;
}

void Fire::FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap)
{
	short *bufferLoc = buffer;

	//draw the two heat lines at the bottom of the screen
	unsigned char *point = fireBuffer1 + 3840;
	if(source)
	{
		for(int i = 0; i < 160; i++, point++)
		{
			int d = (Random::rand())%3;
			if(d == 0)
			{
				*point = 0;
			}
			else if(d == 1)
			{
				*point = 127;
			}
			else
			{
				*point = 255;
			}
		}
	}

	//draw the note heat
	int top = 25 - noteh / 2 + lrintf(10.0f * sinf(phase + PIBY2));
	int left = 40 - notew / 2 + lrintf(10.0f * cosf(phase + PIBY2));
	unsigned char *row = fireBuffer1 + (top << 6) + (top << 4) + left;
	char *noteRow = note;
	if(source)
	{
		for(int y = 0; y < noteh; y++, row += 80, noteRow += notew)
		{
			for(int x = 0; x < notew; x++)
			{
				if(*(noteRow + x) == 1)
				{
					*(row + x) = Random::rand() % 100 + 100;
				}
			}
		}
	}

	//apply the fire filter
	point = fireBuffer1;
	unsigned char *result = fireBuffer2;
	int sum;
	for(int y = 0; y < 50; y++)
	{
		for(int x = 0; x < 80; x++, point++, result++, buffer++)
		{
			//sum the filter
			sum = 0;
			if(x > 0)
			{
				sum += *(point - 1);
				if(y < 49)
				{
					sum += *(point + 79);
					if(y < 48)
					{
						sum += *(point + 159);
					}
				}
			}
			if(x < 79)
			{
				sum += *(point + 1);
				if(y < 49)
				{
					sum += *(point + 81);
					if(y < 48)
					{
						sum += *(point + 161);
					}
				}
			}
			if(y < 49)
			{
				sum += *(point + 80);
				if(y < 48)
				{
					sum += *(point + 160);
				}
			}

			//apply the divide
			*result = MAX((sum >> 3) - 3, 0);

			//set the appropriate color
			*buffer = palette[*result];
		} //end for x
	} //end for y

	//draw the note in black
	buffer = bufferLoc + (top << 6) + (top << 4) + left;
	noteRow = note;
	short black = cMap->GetColor(0, 0, 0);
	for(int y = 0; y < noteh; y++, buffer += 80, noteRow += notew)
	{
		for(int x = 0; x < notew; x++)
		{
			if(*(noteRow + x) == 1)
			{
				*(buffer + x) = black;
			}
		}
	}

	//copy the results buffer back
	memcpy(fireBuffer1, fireBuffer2, 4000 * sizeof(char));
}
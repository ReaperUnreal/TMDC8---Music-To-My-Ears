#ifndef BUFFERFILLER_H
#define BUFFERFILLER_H

#include "main.h"

class BufferFiller
{
public:
	BufferFiller(void);
	virtual ~BufferFiller(void);

	virtual void FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap);
};

class ColorFill : public BufferFiller
{
public:
	ColorFill(Color c);
	virtual ~ColorFill(void);

	void SetColor(Color c);

	void FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap);
private:
	Color col;
};

class AnimatedCircle : public BufferFiller
{
public:
	AnimatedCircle(ColorMap *cMap);
	virtual ~AnimatedCircle(void);

	void FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap);
private:
	float phase;
	short palette[20];
};

class Tunnel : public BufferFiller
{
public:
	Tunnel(ColorMap *cMap, Color c1, Color c2);
	virtual ~Tunnel(void);

	void FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap);

	void SetColors(ColorMap *cMap, Color c1, Color c2);

	void SetPhase(float p);
	void SetUPhase(float up);
	void SetVPhase(float vp);
private:
	float uphase;
	float vphase;
	float phase;
	short palette[20];
};

class Metaballs : public BufferFiller
{
public:
	Metaballs(Color inner, Color back);
	virtual ~Metaballs(void);

	void SetBallColor(Color c);
	void SetBackColor(Color c);

	void SetPhase(float p);

	void FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap);
private:
	float phase;
	Color ball, background;
};

class RadialFill : public BufferFiller
{
public:
	RadialFill(Color outerCol, Color innerCol, float r);
	virtual ~RadialFill(void);

	void SetRadius(float r);
	void SetOuterColor(Color c);
	void SetInnerColor(Color c);

	void FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap);
private:
	float radius;
	Color c1, c2;
};

class CircleFill : public BufferFiller
{
public:
	CircleFill(float r, Color col);
	virtual ~CircleFill(void);

	void SetRadius(float r);

	void FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap);
private:
	float radius;
	Color c;
};

class Fire : public BufferFiller
{
public:
	Fire(ColorMap *cMap);
	virtual ~Fire(void);

	void SetPhase(float p);
	void SetSource(bool src);

	void FillBuffer(short *buffer, float *invZBuffer, ColorMap *cMap);
private:
	short palette[255];
	unsigned char fireBuffer1[4000], fireBuffer2[4000];
	char *note;
	int notew, noteh;
	float phase;
	bool source;
};

#endif
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "main.h"

class Scanline
{
public:
	long y;
	float x[2];
	float invz[2];
	Color c[2];
};

class Graphics
{
public:
	Graphics(Console &cons);
	virtual ~Graphics(void);
	void __fastcall PlotPixel(int x, int y, int r, int g, int b);
	void __fastcall DrawLine(int x1, int y1, int x2, int y2, Color startCol, Color endCol);
	void __fastcall DrawTriangle(Triangle *tri, bool recolour = false, Color newCol = Color(0, 1, 0));
	void __fastcall FloodTriangle(Triangle *tri, bool recolour = false, Color newCol = Color(1, 1, 1));
	void __fastcall FillTriangle(Triangle *tri, bool recolour = false, Color newCol = Color(1, 1, 1));
	void WriteText(char* text, int len, int x, int y, float r, float g, float b);
	void __fastcall Clear(Color col = Color(0, 0, 0));
	void Present(void);
	int GetXSize(void);
	int GetYSize(void);
	ColorMap* GetColorMap(void);
	inline void __fastcall SetEntry(int i, short col)
	{
		FrameBuffer[i] = col;
	}
	short *GetBuffer(void)
	{
		return FrameBuffer;
	}
	void DumpBuffer(char* filename = "temp.txt")
	{
		ofstream fileout(filename, ios::out);
		for(int y = 0; y < 50; y++)
		{
			for(int x = 0; x < 80; x++)
			{
				fileout << setw(6) << FrameBuffer[y*80 + x] << "  ";
			}
			fileout << "\n";
		}
		fileout.close();
	}
	void FillBuffer(BufferFiller *bf);
private:
	Console &console;
	ColorMap colorMap;
	short *FrameBuffer;
	float *InvZBuffer;
	const int size_x, size_y;
	Scanline scanline;

	void __fastcall RenderShadedScanline(Scanline *data);
	void __fastcall RenderScanline(Scanline *data);
	void __fastcall ClearZ();
};

#endif
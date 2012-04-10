#include "main.h"

Graphics::Graphics(Console &cons): console(cons), size_x(cons.console_width), size_y(cons.console_height)
{
	FrameBuffer = cons.FrameBuffer;
	InvZBuffer = new float[size_x * size_y];
	colorMap.BuildLUT();
}

Graphics::~Graphics(void)
{
	delete[] InvZBuffer;
}

void __fastcall Graphics::PlotPixel(int x, int y, int r, int g, int b)
{
	unsigned int pos = x + (y << 6) + (y << 4);
	unsigned short col = colorMap.GetColor(r, g, b);
	FrameBuffer[pos] = col;
}

void __fastcall Graphics::DrawLine(int x1, int y1, int x2, int y2, Color startCol, Color endCol)
{
	//calculate colour stepping
	Color col = startCol;
	Color colStep;

	//draw the line
	int dy = y2 - y1;
	int dx = x2 - x1;
	int stepx, stepy;
	//BreakIf((col.r < 0) || (col.g < 0) || (col.b < 0));
	unsigned short color = colorMap.GetColor(lrintf(col.r * 63), lrintf(col.g * 63), lrintf(col.b * 63));
	
	if(dx == 0)
	{
		if(y2 < y1)
		{
			int temp = y1;
			y1 = y2;
			y2 = temp;
			Color tempCol = startCol;
			startCol = endCol;
			endCol = tempCol;
		}
		col = startCol;
		colStep = (endCol - startCol) / fabsf(ltof(y2 - y1));
		y1 *= 80;
		y2 *= 80;
		for(; y1 <= y2; y1 += 80)
		{
			color = colorMap.GetColor(lrintf(col.r * 63), lrintf(col.g * 63), lrintf(col.b * 63));
			FrameBuffer[y1 + x1] = color;
			col += colStep;
			//BreakIf((col.r < 0) || (col.g < 0) || (col.b < 0));
		}
		return;
	}
	if(dy == 0)
	{
		if(x2 < x1)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			Color tempCol = startCol;
			startCol = endCol;
			endCol = tempCol;
		}
		col = startCol;
		colStep = (endCol - startCol) / fabsf(ltof(x2 - x1));
		y1 *= 80;
		for(; x1 <= x2; x1++)
		{
			color = colorMap.GetColor(lrintf(col.r * 63), lrintf(col.g * 63), lrintf(col.b * 63));
			FrameBuffer[y1 + x1] = color;
			col += colStep;
			//BreakIf((col.r < 0) || (col.g < 0) || (col.b < 0));
		}
		return;
	}
	
	if(dy < 0)
	{
		dy = -dy;
		stepy = -80;
	}
	else
	{
		stepy = 80;
	}
	if(dx < 0)
	{
		dx = -dx;
		stepx = -1;
	}
	else
	{
		stepx = 1;
	}
	dy <<= 1;
	dx <<= 1;

	y1 *= 80;
	y2 *= 80;
	FrameBuffer[x1 + y1] = color;
	if(dx > dy)
	{
		col = startCol;
		colStep = (endCol - startCol) / ltof(dy);
		int fraction = dy - (dx >> 1);
		while(x1 != x2)
		{
			if(fraction >= 0)
			{
				y1 += stepy;
				fraction -= dx;
				col += colStep;
				//BreakIf((col.r < 0) || (col.g < 0) || (col.b < 0));
				color = colorMap.GetColor(lrintf(col.r * 63), lrintf(col.g * 63), lrintf(col.b * 63));
			}
			x1 += stepx;
			fraction += dy;
			FrameBuffer[x1 + y1] = color;
		}
	}
	else
	{
		col = startCol;
		colStep = (endCol - startCol) / ltof(dx);
		int fraction = dx - (dy >> 1);
		while(y1 != y2)
		{
			if(fraction >= 0)
			{
				x1 += stepx;
				fraction -= dy;
				col += colStep;
				//BreakIf((col.r < 0) || (col.g < 0) || (col.b < 0));
				color = colorMap.GetColor(lrintf(col.r * 63), lrintf(col.g * 63), lrintf(col.b * 63));
			}
			y1 += stepy;
			fraction += dx;
			FrameBuffer[x1 + y1] = color;
		}
	}
}

void __fastcall Graphics::DrawTriangle(Triangle *tri, bool recolour, Color newCol)
{
	Vector vertex1, vertex2, vertex3;
	vertex1 = tri->vertices[0].pos;
	vertex2 = tri->vertices[1].pos;
	vertex3 = tri->vertices[2].pos;
	Color color1, color2, color3;

	if(recolour)
	{
		color1 = color2 = color3 = newCol;
	}
	else
	{
		color1 = tri->vertices[0].col;
		color2 = tri->vertices[1].col;
		color3 = tri->vertices[2].col;
	}
	DrawLine(lrintf(vertex1.x + 0.5f), lrintf(vertex1.y + 0.5f), lrintf(vertex2.x + 0.5f), lrintf(vertex2.y + 0.5f), color1, color2);
	DrawLine(lrintf(vertex2.x + 0.5f), lrintf(vertex2.y + 0.5f), lrintf(vertex3.x + 0.5f), lrintf(vertex3.y + 0.5f), color2, color3);
	DrawLine(lrintf(vertex3.x + 0.5f), lrintf(vertex3.y + 0.5f), lrintf(vertex1.x + 0.5f), lrintf(vertex1.y + 0.5f), color3, color1);

}

void __fastcall Graphics::RenderShadedScanline(Scanline *data)
{
	Color slopeInvC;
	float invDeltaX, sub, invzs, invze, currZ, slopeInvZ;
	int xe, xs, i, r, g, b;
	short *bitsFrame = NULL;
	float *invzbuff = NULL;

	//apply top-left fill convention to scanline
	xs = lrintf(ceilf(data->x[0]));
	xe = lrintf(ceilf(data->x[1]) - 1.0f);

	if(xe < xs)
		return;

	//set up z correction
	invzs = data->invz[0];
	invze = data->invz[1];

	//calculate slopes
	invDeltaX = data->x[1] - data->x[0];

	if(invDeltaX < epsilon)
	{
		invDeltaX = 1.0f;
	}
	else
	{
		invDeltaX = 1.0f / invDeltaX;
	}

	sub = ltof(xs) - data->x[0];

	slopeInvZ = (invze - invzs) * invDeltaX;
	invzs += slopeInvZ * sub;

	slopeInvC = (data->c[1] - data->c[0]) * invDeltaX;
	data->c[0] += slopeInvC * sub;

	if(xs > 79)
	{
		return;
	}
	if(xs < 0)
	{
		invzs += slopeInvZ * ltof(-xs);
		data->c[0] += slopeInvC * ltof(-xs);
		xs = 0;
	}
	if(xe > 79)
	{
		xe = 79;
	}
	if(xe < 0)
	{
		return;
	}

	bitsFrame = FrameBuffer + (data->y * 80) + xs;
	invzbuff = InvZBuffer + (data->y * 80) + xs;

	//rasterize the line
	for(i = xs; i <= xe; i++)
	{
		if((invzs >= *invzbuff) && (invzs > epsilon))
		{
			currZ = 1.0f / invzs;
			r = lrintf(data->c[0].r * currZ * 63.0f);
			g = lrintf(data->c[0].g * currZ * 63.0f);
			b = lrintf(data->c[0].b * currZ * 63.0f);

			*bitsFrame = colorMap.GetColor(r, g, b);
			*invzbuff = currZ;
		}
		bitsFrame++;
		invzbuff++;

		data->c[0] += slopeInvC;
		invzs += slopeInvZ;
	}
}

void __fastcall Graphics::FillTriangle(Triangle *tri, bool recolour, Color newCol)
{
	Vector sp[3], temp;
	Color col[3], slopeC[2], c[2];
	float invDeltaY[3], v1[2], v2[2], slopeX[2], x[2], sub, invz[2], slopeInvZ[2];
	int left, yStart, yEnd, i;

	//get the vertices
	sp[0] = tri->vertices[0].pos;
	sp[1] = tri->vertices[1].pos;
	sp[2] = tri->vertices[2].pos;

	//get the colours right
	if(recolour)
	{
		col[0] = col[1] = col[2] = newCol;
	}
	else
	{
		col[0] = tri->vertices[0].col;
		col[1] = tri->vertices[1].col;
		col[2] = tri->vertices[2].col;
	}

	//sort them
	if(sp[0].y > sp[1].y)
	{
		temp = sp[0];
		sp[0] = sp[1];
		sp[1] = temp;
		temp = col[0];
		col[0] = col[1];
		col[1] = temp;
	}
	if(sp[0].y > sp[2].y)
	{
		temp = sp[0];
		sp[0] = sp[2];
		sp[2] = temp;
		temp = col[0];
		col[0] = col[2];
		col[2] = temp;
	}
	if(sp[1].y > sp[2].y)
	{
		temp = sp[1];
		sp[1] = sp[2];
		sp[2] = temp;
		temp = col[1];
		col[1] = col[2];
		col[2] = temp;
	}

	invDeltaY[0] = sp[2].y - sp[0].y;
	if(invDeltaY[0] < epsilon)
	{
		return;
	}

	invDeltaY[1] = sp[1].y - sp[0].y;
	invDeltaY[2] = sp[2].y - sp[1].y;

	invDeltaY[0] = 1.0f / invDeltaY[0];
	if(invDeltaY[1] >= epsilon) invDeltaY[1] = 1.0f / invDeltaY[1];
	if(invDeltaY[2] >= epsilon) invDeltaY[2] = 1.0f / invDeltaY[2];

	//find if the major edge is left or right aligned
	v1[0] = sp[0].x - sp[2].x;
	v1[1] = sp[0].y - sp[2].y;
	v2[0] = sp[1].x - sp[0].x;
	v2[1] = sp[1].y - sp[0].y;

	if((v1[0] * v2[1] - v1[1] * v2[0]) > 0)
		left = 0;
	else
		left = 1;

	//do perspective correction
	sp[0].z = 1.0f / sp[0].z;
	sp[1].z = 1.0f / sp[1].z;
	sp[2].z = 1.0f / sp[2].z;
	col[0] *= sp[0].z;
	col[1] *= sp[1].z;
	col[2] *= sp[2].z;


	//calculate the slopes for the major edge
	slopeX[0] = (sp[2].x - sp[0].x) * invDeltaY[0];
	x[0] = sp[0].x;

	slopeInvZ[0] = (sp[2].z - sp[0].z) * invDeltaY[0];
	invz[0] = sp[0].z;

	slopeC[0] = (col[2] - col[0]) * invDeltaY[0];
	c[0] = col[0];

	//rasterize upper sub-triangle
	if(invDeltaY[1] >= epsilon)
	{
		//calculate the slopes for the top edge
		slopeX[1] = (sp[1].x - sp[0].x) * invDeltaY[1];
		x[1] = sp[0].x;

		slopeInvZ[1] = (sp[1].z - sp[0].z) * invDeltaY[1];
		invz[1] = sp[0].z;

		slopeC[1] = (col[1] - col[0]) * invDeltaY[1];
		c[1] = col[0];

		yStart = lrintf(ceilf(sp[0].y));
		yEnd = lrintf(ceilf(sp[1].y) - 1.0f);

		sub = ltof(yStart) - sp[0].y;

		x[0] += slopeX[0] * sub;
		invz[0] += slopeInvZ[0] * sub;
		c[0] += slopeC[0] * sub;

		x[1] += slopeX[1] * sub;
		invz[1] += slopeInvZ[1] * sub;
		c[1] += slopeC[1] * sub;

		if(yEnd > size_y - 1)
			yEnd = size_y - 1;

		//rasterize the scanlines
		for(i = yStart; i <= yEnd; i++)
		{
			if(i >= 0)
			{
				scanline.y = i;
				scanline.x[left] = x[0];
				scanline.x[!left] = x[1];
				scanline.invz[left] = invz[0];
				scanline.invz[!left] = invz[1];
				scanline.c[left] = c[0];
				scanline.c[!left] = c[1];
		
				//draw a scanline
				RenderShadedScanline(&scanline);
			}

			x[0] += slopeX[0];
			invz[0] += slopeInvZ[0];
			c[0] += slopeC[0];

			x[1] += slopeX[1];
			invz[1] += slopeInvZ[1];
			c[1] += slopeC[1];
		}
	}

	//rasterize the lower sub triangle
	if(invDeltaY[2] >= epsilon)
	{
		//advance middle edge attributes to middle point (if there is another edge)
		if(invDeltaY[1] >= epsilon)
		{
			float dy = sp[1].y - sp[0].y;
			x[0] = sp[0].x + slopeX[0] * dy;
			invz[0] = sp[0].z + slopeInvZ[0] * dy;
			c[0] = col[0] + slopeC[0] * dy;
		}

		//calculate slopes for the bottom edge
		slopeX[1] = (sp[2].x - sp[1].x) * invDeltaY[2];
		x[1] = sp[1].x;

		slopeInvZ[1] = (sp[2].z - sp[1].z) * invDeltaY[2];
		invz[1] = sp[1].z;

		slopeC[1] = (col[2] - col[1]) * invDeltaY[2];
		c[1] = col[1];

		yStart = lrintf(ceilf(sp[1].y));
		yEnd = lrintf(ceilf(sp[2].y) - 1.0f);

		sub = ltof(yStart) - sp[1].y;

		x[0] += slopeX[0] * sub;
		invz[0] += slopeInvZ[0] * sub;
		c[0] += slopeC[0] * sub;

		x[1] += slopeX[1] * sub;
		invz[1] += slopeInvZ[1] * sub;
		c[1] += slopeC[1] * sub;

		if(yEnd > size_y - 1)
			yEnd = size_y - 1;

		//rasterize the scanlines
		for(i = yStart; i <= yEnd; i++)
		{
			if(i >= 0)
			{
				scanline.y = i;
				scanline.x[left] = x[0];
				scanline.x[!left] = x[1];
				scanline.invz[left] = invz[0];
				scanline.invz[!left] = invz[1];
				scanline.c[left] = c[0];
				scanline.c[!left] = c[1];

				RenderShadedScanline(&scanline);
			}

			x[0] += slopeX[0];
			invz[0] += slopeInvZ[0];
			c[0] += slopeC[0];
			
			x[1] += slopeX[1];
			invz[1] += slopeInvZ[1];
			c[1] += slopeC[1];
		}
	}
}

void __fastcall Graphics::RenderScanline(Scanline *data)
{
	float invDeltaX, sub, invzs, invze, currZ, slopeInvZ;
	int xe, xs, i, r, g, b;
	short *bitsFrame = NULL, col;
	float *invzbuff = NULL;

	//apply top-left fill convention to scanline
	xs = lrintf(ceilf(data->x[0]));
	xe = lrintf(ceilf(data->x[1]) - 1.0f);

	if(xe < xs)
		return;

	//set up z correction
	invzs = data->invz[0];
	invze = data->invz[1];

	//calculate slopes
	invDeltaX = data->x[1] - data->x[0];

	if(invDeltaX < epsilon)
	{
		invDeltaX = 1.0f;
	}
	else
	{
		invDeltaX = 1.0f / invDeltaX;
	}

	sub = ltof(xs) - data->x[0];

	slopeInvZ = (invze - invzs) * invDeltaX;
	invzs += slopeInvZ * sub;

	if(xs > 79)
	{
		return;
	}
	if(xs < 0)
	{
		invzs += slopeInvZ * ltof(-xs);
		xs = 0;
	}
	if(xe > 79)
	{
		xe = 79;
	}
	if(xe < 0)
	{
		return;
	}

	//calculate the colour
	r = lrintf(data->c[0].r * 63.0f);
	g = lrintf(data->c[0].g * 63.0f);
	b = lrintf(data->c[0].b * 63.0f);
	col = colorMap.GetColor(r, g, b);

	bitsFrame = FrameBuffer + (data->y * 80) + xs;
	invzbuff = InvZBuffer + (data->y * 80) + xs;

	//rasterize the line
	for(i = xs; i <= xe; i++)
	{
		if((invzs >= *invzbuff) && (invzs > epsilon))
		{
			currZ = 1.0f / invzs;

			*bitsFrame = col;
			*invzbuff = currZ;
		}
		bitsFrame++;
		invzbuff++;

		invzs += slopeInvZ;
	}
}

void __fastcall Graphics::FloodTriangle(Triangle *tri, bool recolour, Color newCol)
{
	//the needed memory
	Vector sp[3], temp;
	Color col;
	float invDeltaY[3], v1[2], v2[2], slopeX[2], slopeInvZ[2], x[2], invz[2], sub;
	int left, yStart, yEnd, i;

	//get the vertices
	sp[0] = tri->vertices[0].pos;
	sp[1] = tri->vertices[1].pos;
	sp[2] = tri->vertices[2].pos;

	//calculate the colours
	if(recolour)
	{
		col = newCol;
	}
	else
	{
		col = (tri->vertices[0].col + tri->vertices[1].col + tri->vertices[2].col) / 3;
	}

	//sort them
	if(sp[0].y > sp[1].y)
	{
		temp = sp[0];
		sp[0] = sp[1];
		sp[1] = temp;
	}
	if(sp[0].y > sp[2].y)
	{
		temp = sp[0];
		sp[0] = sp[2];
		sp[2] = temp;
	}
	if(sp[1].y > sp[2].y)
	{
		temp = sp[1];
		sp[1] = sp[2];
		sp[2] = temp;
	}

	//computer the deltas
	invDeltaY[0] = sp[2].y - sp[0].y;
	if(invDeltaY[0] < epsilon)
	{
		return;
	}

	invDeltaY[1] = sp[1].y - sp[0].y;
	invDeltaY[2] = sp[2].y - sp[1].y;

	invDeltaY[0] = 1.0f / invDeltaY[0];
	if(invDeltaY[1] >= epsilon) invDeltaY[1] = 1.0f / invDeltaY[1];
	if(invDeltaY[2] >= epsilon) invDeltaY[2] = 1.0f / invDeltaY[2];

	//find if the major edge is left or right aligned
	v1[0] = sp[0].x - sp[2].x;
	v1[1] = sp[0].y - sp[2].y;
	v2[0] = sp[1].x - sp[0].x;
	v2[1] = sp[1].y - sp[0].y;

	if((v1[0] * v2[1] - v1[1] * v2[0]) > 0)
		left = 0;
	else
		left = 1;

	//do perspective correction
	sp[0].z = 1.0f / sp[0].z;
	sp[1].z = 1.0f / sp[1].z;
	sp[2].z = 1.0f / sp[2].z;

	//calculate the slopes for the major edge
	slopeX[0] = (sp[2].x - sp[0].x) * invDeltaY[0];
	x[0] = sp[0].x;

	slopeInvZ[0] = (sp[2].z - sp[0].z) * invDeltaY[0];
	invz[0] = sp[0].z;

	//rasterize upper sub-triangle
	if(invDeltaY[1] >= epsilon)
	{
		//calculate the slopes for the top edge
		slopeX[1] = (sp[1].x - sp[0].x) * invDeltaY[1];
		x[1] = sp[0].x;

		slopeInvZ[1] = (sp[1].z - sp[0].z) * invDeltaY[1];
		invz[1] = sp[0].z;

		yStart = lrintf(ceilf(sp[0].y));
		yEnd = lrintf(ceilf(sp[1].y) - 1.0f);

		sub = ltof(yStart) - sp[0].y;

		x[0] += slopeX[0] * sub;
		invz[0] += slopeInvZ[0] * sub;

		x[1] += slopeX[1] * sub;
		invz[1] += slopeInvZ[1] * sub;

		//rasterize the scanlines
		if(yEnd > size_y - 1)
			yEnd = size_y - 1;
		for(i = yStart; i <= yEnd; i++)
		{
			if(i >= 0)
			{
				scanline.y = i;
				scanline.x[left] = x[0];
				scanline.x[!left] = x[1];
				scanline.invz[left] = invz[0];
				scanline.invz[!left] = invz[1];
				scanline.c[0] = col;

				//draw a scanline
				RenderScanline(&scanline);
			}

			x[0] += slopeX[0];
			invz[0] += slopeInvZ[0];

			x[1] += slopeX[1];
			invz[1] += slopeInvZ[1];
		}
	}

	//rasterize the lower sub triangle
	if(invDeltaY[2] >= epsilon)
	{
		//advance middle edge attributes to middle point (if there is another edge)
		if(invDeltaY[1] >= epsilon)
		{
			float dy = sp[1].y - sp[0].y;
			x[0] = sp[0].x + slopeX[0] * dy;
			invz[0] = sp[0].z + slopeInvZ[0] * dy;
		}

		//calculate slopes for the bottom edge
		slopeX[1] = (sp[2].x - sp[1].x) * invDeltaY[2];
		x[1] = sp[1].x;

		slopeInvZ[1] = (sp[2].z - sp[1].z) * invDeltaY[2];
		invz[1] = sp[1].z;

		yStart = lrintf(ceilf(sp[1].y));
		yEnd = lrintf(ceilf(sp[2].y) - 1.0f);

		sub = ltof(yStart) - sp[1].y;

		x[0] += slopeX[0] * sub;
		invz[0] += slopeInvZ[0] * sub;

		x[1] += slopeX[1] * sub;
		invz[1] += slopeInvZ[1] * sub;

		//rasterize the scanlines
		if(yEnd > size_y - 1)
			yEnd = size_y - 1;
		for(i = yStart; i <= yEnd; i++)
		{
			if(i >= 0)
			{
				scanline.y = i;
				scanline.x[left] = x[0];
				scanline.x[!left] = x[1];
				scanline.invz[left] = invz[0];
				scanline.invz[!left] = invz[1];
				scanline.c[0] = col;

				RenderScanline(&scanline);
			}

			x[0] += slopeX[0];
			invz[0] += slopeInvZ[0];
			
			x[1] += slopeX[1];
			invz[1] += slopeInvZ[1];
		}
	}
}

void __fastcall Graphics::Clear(Color col)
{
	int red = lrintf(col.r * 63);
	int green = lrintf(col.g * 63);
	int blue = lrintf(col.b * 63);
	if(red > 63) red = 63;
	if(green > 63) green = 63;
	if(blue > 63) blue = 63;
	//BreakIf(blue != 0);
	short color = colorMap.GetColor(red, green, blue);
	console.clear(color);
	ClearZ();
}

void Graphics::Present()
{
	console.present();
}

void Graphics::WriteText(char *text, int len, int x, int y, float r, float g, float b)
{
	if((x < 0) || (x >= size_x))
		return;
	if((y < 0) || (y >= size_y))
		return;
	if(text[len - 1] == 0)
		len--;
	int startpos = x + (y << 6) + (y << 4);
	if((x + len) >= size_x)
		len = size_x - 1;
	int endpos = startpos + len;
	unsigned short col = colorMap.GetColor(lrintf(r * 63), lrintf(g * 63), lrintf(b * 63));
	for(int pos = startpos, i = 0; pos < endpos; pos++, i++)
	{
		FrameBuffer[pos] = ((col & 0xff00) | text[i]);
	}
}

int Graphics::GetXSize()
{
	return size_x;
}

int Graphics::GetYSize()
{
	return size_y;
}

ColorMap* Graphics::GetColorMap(void)
{
	return &colorMap;
}

void Graphics::ClearZ()
{
	memset(InvZBuffer, 0, size_x * size_y * sizeof(float));
	/*for(int i = 0; i < num; i++)
	{
		ZBuffer[i] = z;
	}*/
}

void Graphics::FillBuffer(BufferFiller *bf)
{
	bf->FillBuffer(FrameBuffer, InvZBuffer, &colorMap);
}
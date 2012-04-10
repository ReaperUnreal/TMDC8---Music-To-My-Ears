#include "main.h"

//constructor and destructor

Console::Console(char *caption, int width, int height) : console_width(width), console_height(height)
{
	title = caption;
	ConsoleWindowHandle = NULL;
	ScreenBuffer = NULL;
	FrameBuffer = NULL;
	initGood = false;
	if(AllocConsole())
	{
		FrameBuffer = new short[console_width * console_height];
		ScreenBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(ScreenBuffer);
		COORD size = {console_width, console_height};
		SetConsoleScreenBufferSize(ScreenBuffer, size);
		SetConsoleMode(ScreenBuffer, 0);
		srand(GetTickCount());
		char temp[81];
		for(int i = 0; i < 80; i++)
		{
			temp[i] = (char)((rand()&0x31)+'A');
		}
		temp[80] = 0;
		SetConsoleTitle(temp);
		ConsoleWindowHandle = FindWindow(NULL, temp);
		SetConsoleTitle(title);
		ShowWindow(ConsoleWindowHandle, SW_MAXIMIZE);
		initGood = true;
	}
}

Console::~Console()
{
	if(FrameBuffer)
		delete[] FrameBuffer;
}

//general console utility functions

HWND Console::getConsoleHandle()
{
	return ConsoleWindowHandle;
}

void Console::setTitle(char *name)
{
	if(initGood && name)
	{
		title = name;
		SetConsoleTitle(title);
	}
}

bool Console::isInit(void)
{
	return initGood;
}

//graphics functions

void Console::present()
{
	if(!initGood)
		return;
	int bufferLength = console_width * console_height;
	CHAR_INFO *ci = new CHAR_INFO[bufferLength];
	for(int i = 0; i < bufferLength; i++)
	{
		ci[i].Char.AsciiChar = (char)(FrameBuffer[i] & 0xff);
		ci[i].Attributes = (unsigned short)((FrameBuffer[i] >> 8) & 0xff);
	}
	COORD max = {console_width, console_height}, src = {0, 0};
	SMALL_RECT outrect;
	outrect.Top = 0;
	outrect.Left = 0;
	outrect.Bottom = console_height - 1;
	outrect.Right = console_width - 1;
	WriteConsoleOutput(ScreenBuffer, ci, max, src, &outrect);
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 1;
	SetConsoleCursorInfo(ScreenBuffer, &cci);
	delete[] ci;
}

void Console::clear(short col)
{
	if(!initGood)
		return;
	int bufferLength = console_width * console_height;
	for(int i = 0; i < bufferLength; i++)
	{
		FrameBuffer[i] = col;
	}
	//memset(FrameBuffer, col, bufferLength * sizeof(short));
}

short* Console::getFrameBuffer()
{
	return FrameBuffer;
}
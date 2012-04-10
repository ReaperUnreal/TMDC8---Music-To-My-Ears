#ifndef CONSOLE_H
#define CONSOLE_H

#include "main.h"

class Console
{
public:
	friend class Graphics;

	//constructor and destructor
	Console(char *caption, int width = 80, int height = 50);
	virtual ~Console(void);

	//general console utility functions
	HWND getConsoleHandle(void);
	void setTitle(char *name);
	bool isInit(void);

	//graphics functions
	void present(void);
	void clear(short col = 0);
	short* getFrameBuffer(void);

	const int console_width, console_height;
private:
	HWND ConsoleWindowHandle;
	bool initGood;
	short *FrameBuffer;
	HANDLE ScreenBuffer;
	char *title;
};

#endif
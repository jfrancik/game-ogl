#include "stdafx.h"
#include "MyGame.h"

CMyGame game;
CGameApp app;

int main(int argc, char* argv[])
{
	//app.DebugMemoryLeaks();
	app.OpenConsole();
	app.OpenWindow(1280, 720, "OpenGL Test (limited)", SDL_OPENGL);
	//app.OpenFullScreen(800, 600, 24);
	app.SetClearColor(CColor::White());
	app.Run(&game);
	return(0);
}

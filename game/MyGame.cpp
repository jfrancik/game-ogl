#include "stdafx.h"
#include "MyGame.h"

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")

CMyGame::CMyGame(void) : 
	m_sprite(400, 40, "rocket.bmp", CColor::Blue(), 0)	
	// to initialise more sprites here use a comma-separated list
{
	// TODO: add initialisation here
}

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	Uint32 t = GetTime();

	// TODO: add the game update code here
	m_sprite.Update(t);	// this will update the sample rocket sprite
}

void CMyGame::OnDraw(CGraphics* g)
{
	// clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render a red triangle
	glBegin(GL_TRIANGLES);
	glColor3ub(255, 0, 0); // Red color
	glVertex2d(100, 100);
	glVertex2d(700, 100);
	glVertex2d(400, 500);
	glEnd();

	// flush and swap
	glFlush();
	SDL_GL_SwapBuffers();
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	// GLEW initialisation
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << "GLEW Error: " << (const char*)glewGetErrorString(err) << endl;
		return;
	}
	cout << "Using GLEW " << (const char*)glewGetString(GLEW_VERSION) << endl;

	// Some additional diagnostic information
	cout << "Vendor: " << (const char*)glGetString(GL_VENDOR) << endl;
	cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << endl;
	cout << "Version: OpenGL" << (const char*)glGetString(GL_VERSION) << endl;

	// Setting up the projection to match window resolution
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, GetWidth(), 0, GetHeight(), -1, 1);
}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	StartGame();	// exits the menu mode and starts the game mode
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
}

// called when the game is over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}

#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame(void)  :
	m_back("back.jpg"),
	m_rocket(CVector(50, 0), "rocket.bmp", CColor::Blue(), 0)

	//m_sprite(400, 40, "rocket.bmp", CColor::Blue(), 0)	
	// to initialise more sprites here use a comma-separated list
{
	// TODO: add initialisation here
}

CMyGame::~CMyGame(void)
{
	for (auto blob : m_blobs)
		delete blob;
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	Uint32 t = GetTime();

	// TODO: add the game update code here
	//m_sprite.Update(t);	// this will update the sample rocket sprite
}

void CMyGame::OnDraw(CGraphics* g)
{
	// TODO: add drawing code here
	//m_sprite.Draw(g);	// this will draw the sample rocket sprite

	g->Blit(CVector(0, 0), m_back);
	g->Blit(CVector(0, 0), *m_pRocket2);

	m_rocket.Draw(g);

	for (auto blob : m_blobs)
		blob->Draw(g);
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	m_pRocket2 = new CGraphics("rocket.bmp", CColor::Blue());
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
	m_rocket.SetPos(x, y);
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
	m_blobs.push_back(new CSprite(CVector(x, y), "goo.png", GetTime()));
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

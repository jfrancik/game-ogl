/****************************************************************************
GFC - Games Fundamental Classes version 2.70
Copyright (C) 2009-2022 Jarek Francik, Kingston University London

This work is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License:
http://creativecommons.org/licenses/by-sa/4.0/

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Created by Jarek Francik
jarek@kingston.ac.uk
****************************************************************************/
#include "pch.h"
#include "GameApp.h"
#include "Game.h"
#include "Graphics.h"

#include "sdl/include/SDL.h"
#include "sdl/include/SDL_ttf.h"

// Some windows-specific features...
#include "WindowsTools.h"

//////////////////////////////////////
// Constructor & Destructor

CGameApp::CGameApp(CGame *pGame) : m_pGame(pGame), m_pGraphics(NULL), m_colorClear(0, 0, 0), m_FPS(60)
{
	srand((unsigned)time(NULL));
	while (rand() < 2000);
	if (m_pGame) m_pGame->SetApp(this);
}

CGameApp::~CGameApp()
{
	if (m_pGraphics) delete m_pGraphics;
}

//////////////////////////////////////
// Attribute Getters and Setters

void CGameApp::SetGame(CGame *pGame)
{
	m_pGame = pGame;
	if (!m_pGame) return;
	m_pGame->SetApp(this);
	m_pGame->SetSize(GetWidth(), GetHeight());
}

CVectorI CGameApp::GetSize()	{ return CVectorI(GetWidth(), GetHeight()); }
int CGameApp::GetWidth()		{ return GetGraphics() ? GetGraphics()->GetWidth() : 0; }
int CGameApp::GetHeight()		{ return GetGraphics() ? GetGraphics()->GetHeight() : 0; }

//////////////////////////////////////
// The Clock

uint32_t CGameApp::GetSystemTime()
{
	return SDL_GetTicks();
}

uint32_t CGameApp::GetGameTime()
{
	uint32_t t = GetSystemTime();
	if (m_bRunning)
		return m_nTimePaused + t - m_nTimeStarted;
	else
		return m_nTimePaused;
}

void CGameApp::ResetClock()
{
	m_nTimeStarted = GetSystemTime();
	m_nTimePaused = 0;
	m_bRunning = true;
}

void CGameApp::SuspendClock()
{
	if (!m_bRunning) return;
	m_nTimePaused = GetGameTime();
	m_bRunning = false;
}

void CGameApp::ResumeClock()
{
	if (m_bRunning) return;
	m_nTimeStarted = GetSystemTime();
	m_bRunning = true;
}

bool CGameApp::IsClockRunning()
{
	return m_bRunning;
}

//////////////////////////////////////
// Application Operations

bool CGameApp::OpenWindow(int nWidth, int nHeight, std::string strTitle, unsigned SDLflags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	if (TTF_Init() == -1)
		return false;

	if (m_pGame) m_pGame->SetSize(nWidth, nHeight);
	
	atexit(SDL_Quit);
	m_pGraphics = new CGraphics(nWidth, nHeight, 0, SDLflags ? SDLflags : SDL_ANYFORMAT | SDL_SWSURFACE);
	if (!m_pGraphics)
		return false;

	SDL_WM_SetCaption(strTitle.c_str(), strTitle.c_str());

	return true;
}

bool CGameApp::OpenFullScreen(int nWidth, int nHeight, int nBpp)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;
	
	if (TTF_Init() == -1)
		return false;

	if (m_pGame) m_pGame->SetSize(nWidth, nHeight);

	atexit(SDL_Quit);
	m_pGraphics = new CGraphics(nWidth, nHeight, nBpp, SDL_FULLSCREEN);
	if (!m_pGraphics)
		return false;

	return true;
}

bool CGameApp::OpenConsole()
{
	OpenWindowsConsole();
	printf("Games Foundation Classes ver. 3.00.\nCopyright (C) 2009-2024 Jarek Francik, Kingston University London\n");
	return true;
}

void CGameApp::DebugMemoryLeaks(long nBreakAlloc)
{
#ifndef NDEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
	if (nBreakAlloc > 0) _crtBreakAlloc = nBreakAlloc;
#endif
}

unsigned CGameApp::_GetSDLVersion()
{
	SDL_version ver;
	SDL_VERSION(&ver);
	return min(ver.patch, 99) + 100 * min(ver.minor, 99) + 10000 * min(ver.major, 99);
}

unsigned long long CGameApp::_GetWindowHandle()
{
	return ::GetWindowHandle();
}

bool CGameApp::Run(CGame *pGame)
{
	if (pGame) SetGame(pGame);
	if (!m_pGame) return false;
	if (!m_pGraphics) return false;

	SDL_Event anEvent;
	int idGame = 0;

	ResetClock();
	uint32_t timeStamp = GetSystemTime();
	OnInitialize();
	m_pGame->OnInitialize();
//	m_pGame->OnDisplayMenu();
	uint32_t nFrames;

	// again..
	ResetClock();
	timeStamp = GetSystemTime();

	// The Main Animation Loop
	while (m_pGame->IsRunning())
	{
		uint32_t period = 1000 / m_FPS;

		while (m_pGame->IsRunning() && SDL_PollEvent(&anEvent))
			if (!OnDispatchEvent(&anEvent))
			{
				m_pGame->SetTime(GetGameTime());
				if (anEvent.type == SDL_QUIT)
					m_pGame->StopGame();
				m_pGame->DispatchEvents(&anEvent);
			}
		
		if (!m_pGame->IsRunning()) break;
		if (!OnUpdate())
		{
			// calculate timing (considering pausing)
			if (m_pGame->IsPaused())
				SuspendClock();
			else
				ResumeClock();

			// handle the game mode requests
			m_pGame->HandleGameState();

			// update the game
			if (!m_pGame->IsPaused())
			{
				uint32_t T = GetGameTime();
				
				// check the number of frames to cover
				m_pGame->SetTime(T);
				nFrames = max(1, m_pGame->GetDeltaTime() / period);

				for (int iFrame = 1 - nFrames; iFrame < 1; iFrame++)
				{
					m_pGame->SetTime(T + iFrame * period);
					m_pGame->OnUpdate();
					m_pGame->CatchDeltaTime();
				}
			}

			if (!m_pGame->IsRunning()) break;

			// draw the game
			m_pGraphics->Clear(m_colorClear);
			m_pGame->OnDraw(m_pGraphics);

			//time_t theTime = time(NULL);
			//struct tm *aTime = localtime(&theTime);
			//if (aTime->tm_year > 113)
			//{
			//	*m_pGraphics << font(32) << vcenter << center << color(CColor::Red()) << "GFC VERSION YOU ARE USING" << endl << "IS OUTDATED" << endl;
			//	*m_pGraphics << font(22) << endl << "PLEASE DOWNLOAD & INSTALL THE CURRENT VERSION" << endl;
			//}

			// flush any remaining text
			m_pGraphics->Flush();

			// flip buffers
			m_pGraphics->Flip();
		}
		// Controlling the Time
		Sint32 sleepTime = period * nFrames - (GetSystemTime() - timeStamp);
		timeStamp += period * nFrames;

		if (sleepTime > 0)
			SDL_Delay(sleepTime);
	}
	m_pGame->OnTerminate();
	OnTerminate();
	if (m_pGraphics) delete m_pGraphics;
	m_pGraphics = NULL;
	TTF_Quit();
	SDL_Quit();
	return true;
}


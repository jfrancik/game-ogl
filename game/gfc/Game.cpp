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
#include "Game.h"
#include "GameApp.h"
#include "Graphics.h"

#include "sdl/include/SDL.h"

CGame::CGame() : m_dim(0, 0)
{
	m_bMouseFocus = false;
	m_time = m_timePrev = m_timeGameOver = 0;
	m_flagRunning = true;
	m_flagPaused = false;
	m_flagModeReq = MODE_MENU;
	m_flagMode = MODE_MENU;
	m_level = 0;
	m_levelReq = 0;
}

CGame::~CGame(void)
{
}

///////////////////////////////////////////////
// Keyboard and Mouse State Functions

bool CGame::IsKeyDown(SDLKey sym)
{
	return (m_kbarray[sym] == 1); 
}

CVectorI CGame::GetMouseCoords()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	y = GetHeight() - y;
	return CVectorI(x, y);
}

void CGame::GetMouseCoords(uint16_t&x, uint16_t&y)
{
	int _x, _y;
	SDL_GetMouseState(&_x, &_y);
	x = _x;
	y = GetHeight() - _y;
}

bool CGame::IsMouseFocus()
{ 
	return m_bMouseFocus; 
}

bool CGame::IsLButtonDown()
{
	return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) != 0;
}

bool CGame::IsRButtonDown()
{
	return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK) != 0;
}

bool CGame::IsMButtonDown()
{
	return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MMASK) != 0;
}

void CGame::HideMouse()
{
	SDL_ShowCursor(0);
}
void CGame::ShowMouse()
{
	SDL_ShowCursor(1);
}

///////////////////////////////////////////////
// Force Game Draw

void CGame::ForceDraw()
{
	auto pApp = GetApp();
	auto pGraphics = pApp->GetGraphics();
	pGraphics->Clear(pApp->GetClearColor());
	OnDraw(pGraphics);
	pGraphics->Flush();
	pGraphics->Flip();
}


///////////////////////////////////////////////
// Game State & Events

void CGame::HandleGameState()
{
	if (m_flagModeReq != MODE_NONE)
	{
		m_flagMode = m_flagModeReq; 
		m_flagModeReq = MODE_NONE;
		switch (GetMode())
		{
		case CGame::MODE_MENU:
			m_pApp->ResetClock();
			this->ResetTime();
			m_level = m_levelReq = 0;
			OnDisplayMenu();
			OnStartLevel(m_level);
			break;
		case CGame::MODE_GAME:
			m_pApp->ResetClock();
			this->ResetTime();
			m_level = m_levelReq = 1;
			OnStartGame();
			OnStartLevel(m_level);
			break;
		case CGame::MODE_GAMEOVER:
			SetTimeGameOver(GetTime());
			m_pApp->ResetClock();
			this->ResetTime();
			m_levelReq = m_level;
			OnGameOver();
			break;
		}
	}
	if (m_level != m_levelReq)
	{
		m_level = m_levelReq; 
		OnStartLevel(m_level);
	}
}

void CGame::DispatchEvents(SDL_Event* pEvent)
{
	//filter event
	if (OnEvent(pEvent)) return;

	m_kbarray = SDL_GetKeyState(NULL);   // get keyboard states 


	//dispatch event
	switch(pEvent->type)
	{
		case SDL_ACTIVEEVENT:		//activation event
			//what was gained or lost?
			switch(pEvent->active.state)
			{
				case SDL_APPMOUSEFOCUS:		//mouse focus
					//gain or loss?
					m_bMouseFocus = pEvent->active.gain != 0;
					if (m_bMouseFocus)
						OnMouseFocus();
					else
						OnMouseBlur();
					break;
				case SDL_APPINPUTFOCUS:		//input focus
					if(pEvent->active.gain)
						OnInputFocus();
					else
						OnInputBlur();
					break;
				case SDL_APPACTIVE:			//application activation
					if(pEvent->active.gain)
						OnRestore();
					else
						OnMinimize();
					break;
			}
			break;
		case SDL_KEYDOWN:			//key press
			if (pEvent->key.keysym.sym >= SDLK_1 && pEvent->key.keysym.sym <= SDLK_9 && pEvent->key.keysym.mod == (KMOD_LSHIFT | KMOD_LCTRL))
				SetLevel(pEvent->key.keysym.sym - SDLK_0);
			OnKeyDown(pEvent->key.keysym.sym,pEvent->key.keysym.mod,pEvent->key.keysym.unicode);
			break;
		case SDL_KEYUP:				//key release
			OnKeyUp(pEvent->key.keysym.sym,pEvent->key.keysym.mod,pEvent->key.keysym.unicode);
			break;
		case SDL_MOUSEMOTION:		//mouse motion
			OnMouseMove(pEvent->motion.x,GetHeight() - pEvent->motion.y,pEvent->motion.xrel,pEvent->motion.yrel,(pEvent->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,(pEvent->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,(pEvent->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
			break;
		case SDL_MOUSEBUTTONDOWN:	//mouse button press
			//which button?
			switch(pEvent->button.button)
			{
				case SDL_BUTTON_LEFT:	OnLButtonDown(pEvent->button.x,GetHeight() - pEvent->button.y); break;
				case SDL_BUTTON_RIGHT:	OnRButtonDown(pEvent->button.x,GetHeight() - pEvent->button.y); break;
				case SDL_BUTTON_MIDDLE:	OnMButtonDown(pEvent->button.x,GetHeight() - pEvent->button.y); break;
			}
			break;
		case SDL_MOUSEBUTTONUP:		//mouse button release
			//which button?
			switch(pEvent->button.button)
			{
				case SDL_BUTTON_LEFT:	OnLButtonUp(pEvent->button.x,GetHeight() - pEvent->button.y); break;
				case SDL_BUTTON_RIGHT:	OnRButtonUp(pEvent->button.x,GetHeight() - pEvent->button.y); break;
				case SDL_BUTTON_MIDDLE:	OnMButtonUp(pEvent->button.x,GetHeight() - pEvent->button.y); break;
			}
			break;
		case SDL_JOYAXISMOTION:		//joystick axis motion
			OnJoyAxis(pEvent->jaxis.which,pEvent->jaxis.axis,pEvent->jaxis.value);
			break;
		case SDL_JOYBALLMOTION:		//joystick ball motion
			OnJoyBall(pEvent->jball.which,pEvent->jball.ball,pEvent->jball.xrel,pEvent->jball.yrel);
			break;
		case SDL_JOYHATMOTION:		//joystick hat motion
			OnJoyHat(pEvent->jhat.which,pEvent->jhat.hat,pEvent->jhat.value);
			break;
		case SDL_JOYBUTTONDOWN:		//joystick button press
			OnJoyButtonDown(pEvent->jbutton.which,pEvent->jbutton.button);
			break;
		case SDL_JOYBUTTONUP:		//joystick button release
			OnJoyButtonUp(pEvent->jbutton.which,pEvent->jbutton.button);
			break;
		case SDL_QUIT:				//quit event
			//ignore it!
			break;
		case SDL_SYSWMEVENT:		//window manager event
			//ignore as well...
			break;
		case SDL_VIDEORESIZE:		//resize video
			OnResize(pEvent->resize.w,pEvent->resize.h);
			break;
		case SDL_VIDEOEXPOSE:		//video expose
			OnExpose();
			break;
		default:					//user defined
			OnUserEvent(pEvent->user.type,pEvent->user.code,pEvent->user.data1,pEvent->user.data2);
			break;
	}
}

///////////////////////////////////////////////////////
// Generic Implementation of Events Handlers

// Generic Event
bool CGame::OnEvent(SDL_Event* pEvent)									{ return false; }
// Keyboard events
void CGame::OnKeyDown(SDLKey sym,SDLMod mod, uint16_t unicode)			{ }
void CGame::OnKeyUp(SDLKey sym,SDLMod mod, uint16_t unicode)			{ }
// Mouse events
void CGame::OnMouseMove(uint16_t x, uint16_t y, int16_t relx, int16_t rely,bool bLeft,bool bRight,bool bMiddle)	{ }
void CGame::OnLButtonDown(uint16_t x, uint16_t y)						{ }
void CGame::OnLButtonUp(uint16_t x, uint16_t y)							{ }
void CGame::OnRButtonDown(uint16_t x, uint16_t y)						{ }
void CGame::OnRButtonUp(uint16_t x, uint16_t y)							{ }
void CGame::OnMButtonDown(uint16_t x, uint16_t y)						{ }
void CGame::OnMButtonUp(uint16_t x, uint16_t y)							{ }
// Joystick events
void CGame::OnJoyAxis(uint8_t which, uint8_t axis, int16_t value)			{ }
void CGame::OnJoyButtonDown(uint8_t which, uint8_t button)					{ }
void CGame::OnJoyButtonUp(uint8_t which, uint8_t button)						{ }
void CGame::OnJoyHat(uint8_t which, uint8_t hat, uint8_t value)					{ }
void CGame::OnJoyBall(uint8_t which, uint8_t ball, int16_t xrel, int16_t yrel)	{ }
// Active Events (keyboard)
void CGame::OnInputFocus()												{ }
void CGame::OnInputBlur()												{ }
// Active Events (mouse)
void CGame::OnMouseFocus()												{ }
void CGame::OnMouseBlur()												{ }
// Active Events (application)
void CGame::OnMinimize()												{ PauseGame(true); }
void CGame::OnRestore()													{ PauseGame(false); }
// Video Event
void CGame::OnResize(int w,int h)										{ }
void CGame::OnExpose()													{ }
//User Event
void CGame::OnUserEvent(uint8_t type,int code,void* data1,void* data2)	{ }


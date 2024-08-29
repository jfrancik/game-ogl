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
#pragma once

#include "Vector.h"
#include "Rectangle.h"
#ifdef _USRDLL
#include <SDL_keysym.h>
#else
#include "gfc/sdl/include/SDL_keysym.h"
#endif
class CGameApp;
class CGraphics;

union SDL_Event;

class EXT_DECL CGame
{
public:
	CGame();
	virtual ~CGame();

	// Game Modes
	enum GAMEMODE { MODE_MENU, MODE_GAME, MODE_GAMEOVER, MODE_NONE };

private:
	// Attributes:
	///////////////////////////////////////////////

	CVectorI m_dim;						// playfield size

	CGameApp *m_pApp;					// the Game Application object

	///////////////////////////////////////////////
	// Game State Flags
	volatile bool m_flagRunning;		// used to stop the animation thread
	volatile bool m_flagPaused;			// used to pause the animation thread
	volatile GAMEMODE m_flagMode;		// current game mode - either MODE_MENU, MODE_GAME or MODE_GAMEOVER
	volatile GAMEMODE m_flagModeReq;	// requested game mode

	///////////////////////////////////////////////
	// Game Level
	volatile int16_t m_level;			// current game level
	volatile int16_t m_levelReq;		// requested game level

	///////////////////////////////////////////////
	// Key States
	uint8_t *m_kbarray;					// pointer to keyboard states array
	bool m_bMouseFocus;

	///////////////////////////////////////////////
	// Game Timing
	volatile uint32_t m_time;			// set automatically by the framework (CGameApp)
	volatile uint32_t m_timePrev;		// time of the previous update (used to calculate DeltaTime)
	volatile uint32_t m_timeGameOver;	// set automatically by the framework (CGameApp)

public:
	// Accessor Functions
	
	///////////////////////////////////////////////
	// The application object

	CGameApp *GetApp()					{ return m_pApp; }

	///////////////////////////////////////////////
	// Game Geometry: simple but usable implementation

	CVectorI GetSize()					{ return m_dim; }
	int16_t GetWidth()					{ return m_dim.X(); }
	int16_t GetHeight()					{ return m_dim.Y(); }

	///////////////////////////////////////////////
	// Time Utilities
	
	uint32_t GetTime()					{ return m_time; }
	uint32_t GetDeltaTime()				{ return m_time - m_timePrev; }
	uint32_t GetTimeGameOver()			{ return m_timeGameOver; }

	///////////////////////////////////////////////
	// Keyboard and Mouse State Functions
	
	// Checks if a particular key is pressed
	bool IsKeyDown(SDLKey sym);

	CVectorI GetMouseCoords();
	void GetMouseCoords(uint16_t &x, uint16_t&y);
	bool IsMouseFocus();
	bool IsLButtonDown();
	bool IsRButtonDown();
	bool IsMButtonDown();

	void HideMouse();
	void ShowMouse();


	///////////////////////////////////////////////
	// Game State

	bool IsRunning()					{ return m_flagRunning; }
	bool IsPaused()						{ return m_flagPaused; }
	bool IsMenuMode()					{ return m_flagMode == MODE_MENU; }
	bool IsGameMode()					{ return m_flagMode == MODE_GAME; }
	bool IsGameOverMode()				{ return m_flagMode == MODE_GAMEOVER; }
	bool IsGameOver()					{ return IsGameOverMode(); }
	
	GAMEMODE GetMode()					{ return m_flagMode; }
	GAMEMODE GetGameMode()				{ return m_flagMode; }
	void ChangeMode(GAMEMODE mode)		{ m_flagModeReq = mode; }	// will only act from the next animation loop iteration!
	bool IsModeChanging()				{ return m_flagModeReq != MODE_NONE; }

	void StartGame()					{ ChangeMode(MODE_GAME); }
	void GameOver() 					{ ChangeMode(MODE_GAMEOVER); }
	void NewGame()						{ ChangeMode(MODE_MENU); }
	void StopGame()						{ m_flagRunning = false; }
	void StopApp()						{ m_flagRunning = false; }
	void PauseGame()					{ m_flagPaused = !m_flagPaused; }
	void PauseGame(bool b)				{ m_flagPaused = b; }

	///////////////////////////////////////////////
	// Game Level

	int16_t GetLevel()					{ return m_level; }
	void SetLevel(int16_t level)		{ m_levelReq = level; }
	void NewLevel()						{ SetLevel(GetLevel() + 1); }

	///////////////////////////////////////////////
	// Force Game Draw

	void ForceDraw();

	///////////////////////////////////////////////
	// Life Cycle

	virtual void OnInitialize()			{ }
	virtual void OnDisplayMenu()		{ }
	virtual void OnStartGame()			{ }
	virtual void OnStartLevel(int16_t nLevel)	{ }
	virtual void OnGameOver()			{ }
	virtual void OnUpdate()				{ }
	virtual void OnDraw(CGraphics* g)	{ }
	virtual void OnTerminate()			{ }


	///////////////////////////////////////////////
	// Game State Handlers
	void HandleGameState();

	///////////////////////////////////////////////
	// Event Dispatcher
	void DispatchEvents(SDL_Event* pEvent);

	///////////////////////////////////////////////
	// Event Handlers

	// Generic Event
	virtual bool OnEvent(SDL_Event* pEvent);

	// Keyboard events
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, uint16_t unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, uint16_t unicode);

	// Mouse events
	virtual void OnMouseMove(uint16_t x, uint16_t y, int16_t relx, int16_t rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(uint16_t x, uint16_t y);
	virtual void OnLButtonUp(uint16_t x, uint16_t y);
	virtual void OnRButtonDown(uint16_t x, uint16_t y);
	virtual void OnRButtonUp(uint16_t x, uint16_t y);
	virtual void OnMButtonDown(uint16_t x, uint16_t y);
	virtual void OnMButtonUp(uint16_t x, uint16_t y);

	// Joystick events
	virtual void OnJoyAxis(uint8_t which, uint8_t axis, int16_t value);
	virtual void OnJoyButtonDown(uint8_t which, uint8_t button);
	virtual void OnJoyButtonUp(uint8_t which, uint8_t button);
	virtual void OnJoyHat(uint8_t which, uint8_t hat, uint8_t value);
	virtual void OnJoyBall(uint8_t which, uint8_t ball, int16_t xrel, int16_t yrel);

	// Active Events (keyboard)
	virtual void OnInputFocus();
	virtual void OnInputBlur();

	// Active Events (mouse)
	virtual void OnMouseFocus();
	virtual void OnMouseBlur();

	// Active Events (application)
	virtual void OnMinimize();
	virtual void OnRestore();

	// Video Event
	virtual void OnResize(int w,int h);
	virtual void OnExpose();

	// User Event
	virtual void OnUserEvent(uint8_t type,int code,void* data1,void* data2);

//////////////////////////////////////////////////////////////////
// These functions are provided strictly for CGameApp internal use
private:
	void SetSize(int16_t x, int16_t y)	{ m_dim = CVectorI(x, y); }
	void SetApp(CGameApp *p)			{ m_pApp = p; }
	void ResetTime(long t = 0)			{ m_time = m_timePrev = t; }
	void SetTime(long t)				{ m_time = t; }
	void CatchDeltaTime()				{ m_timePrev = m_time; }
	void SetTimeGameOver(long t)		{ m_timeGameOver = t; }
	friend class CGameApp;
};

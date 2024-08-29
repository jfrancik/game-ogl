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
#include "WindowsTools.h"
#include <windows.h>
#include "sdl/include/SDL.h"


// Returns complete exe file pathname
std::string GetExecutablePath()
{
	char mfn[256];
	GetModuleFileNameA(0, mfn, 256);
	return mfn;
}

// returns executable file path only (drive + dir)
std::string GetBasePath()
{
	char mfn[256];
	GetModuleFileNameA(0, mfn, 256);

	char drive[_MAX_DRIVE];
	char dir[_MAX_PATH];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath_s(mfn, drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
	std::string baseDir = std::string(drive) + std::string(dir);
	return baseDir;
}

// Opens a text console window
void OpenWindowsConsole()
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "conin$", "r", stdin);
	freopen_s(&f, "conout$", "w", stdout);
	freopen_s(&f, "conout$", "w", stderr);
}

// returns the game main window Windows-specific handle (HWND)
unsigned long long GetWindowHandle()
{
	char* title, * icon;
	SDL_WM_GetCaption(&title, &icon);
	char* title2 = _strdup(title), * icon2 = _strdup(icon);
	char buf[64], strrand[6], strtime[32];
	_itoa_s(rand(), strrand, 10);
	time_t t = time(NULL);
	ctime_s(strtime, 32, &t);
	strcpy_s(buf, "GFCTestWnd");
	strcat_s(buf, strrand);
	strcat_s(buf, strtime);
	SDL_WM_SetCaption(buf, buf);
	HWND hWnd = FindWindowA(NULL, buf);
	SDL_WM_SetCaption(title2, icon2);
	return (unsigned long long)hWnd;
}
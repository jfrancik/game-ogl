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

// CVector - a fast inline class

#ifndef __WINDOWS_TOOLS_H__
#define __WINDOWS_TOOLS_H__

#include <string>

// Windows-specific tools

// There is no portable possibility to determine the executable path
// main argc/argv values arrive too late - this function is typically called
// from within constructors of static objects!
// GetExecutablePath returns entire exe file pathname
// GetBasePath only provides drive+dir
EXT_DECL std::string GetExecutablePath();
EXT_DECL std::string GetBasePath();

// Opens a text console window
EXT_DECL void OpenWindowsConsole();

// returns the game main window Windows-specific handle (HWND)
EXT_DECL unsigned long long GetWindowHandle();

#endif

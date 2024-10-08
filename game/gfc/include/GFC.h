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

#ifndef GFC_VERSION
#define GFC_VERSION	160
#endif

#ifndef max
#define max(x, y)	((x) > (y) ? (x) : (y))
#endif

#ifndef min
#define min(x, y)	((x) < (y) ? (x) : (y))
#endif

#ifndef RAD2DEG
#define RAD2DEG(x)	((float)(x) * 180.0f / (float)M_PI)
#endif

#ifndef DEG2RAD
#define DEG2RAD(x)  ((float)(x) * (float)M_PI / 180.0f)
#endif

#include <sstream>

#ifdef _USRDLL
   #define EXT_DECL		__declspec(dllexport)
#else
   #define EXT_DECL		__declspec(dllimport)
#endif


#include "Color.h"
#include "Vector.h"
#include "Rectangle.h"
#include "GameApp.h"
#include "Game.h"
#include "Graphics.h"
#include "RendererFactory.h"
#include "Sprite.h"
#include "SpriteRect.h"
#include "SpriteOval.h"
#include "SpriteText.h"
#include "SpriteContainer.h"
#include "Sound.h"
//#include "Font.h"

///////////////////////////////////////
// WINDOWS SPECIFIC
// Allows for client's programs to use main function instead of WinMain

#include <windows.h>
#undef LoadImage
int main(int argc, char* argv[]);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return main(0, NULL);
}

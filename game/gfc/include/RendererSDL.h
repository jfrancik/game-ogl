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
#ifndef __RENDERER_SDL_H__
#define __RENDERER_SDL_H__

#include "Renderer.h"
#include "FileMgr.h"

struct SDL_Surface;
struct _TTF_Font;

// CRendererSDL class
class EXT_DECL CRendererSDL : public IRenderer
{
private:
	// Pointer to an SDL_Surface
	SDL_Surface* m_pSurface;

	// Font-related data
	_TTF_Font* m_pFont;							// current font
	std::string m_strFontFace = "arial.ttf";	// font face name
	int m_nPtSize;								// font point size
	CColor m_textColor;							// text color

	// File Loader (path resolving and cached loading)
	static CFileMgr<SDL_Surface> c_filemgr;
	static CFileMgr<_TTF_Font> c_fontmgr;

public:
	// Default Constructor
	CRendererSDL();

	// Copy Constructor
	CRendererSDL(CRendererSDL& g);

	// Virtual Destructor
	~CRendererSDL() override;


	// Type Identification - returns a type id string, for example "SDL", "SDL2", "OGL"
	std::string GetType() override { return "SDL"; }


	// Virtual Initialisers:
	
	// from another IRenderer - checks if they are of compatible type
	void Create(IRenderer*) override;

	// from a video mode initialisation
	void Create(int width, int height, int depth, uint32_t flagsCreation) override;

	// from memory canvas
	void Create(int width, int height, int depth, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) override;
	// from memory canvas compatible with the current display
	void Create(int width, int height) override;

	// from a loaded bitmap
	void Create(std::string sFileName) override;


	// Rectangular Fragment
	// Creates a renderer attached to a rectangular fragment of another image.
	void Create(IRenderer*, CRectangle rect) override;
	void Create(std::string sFileName, CRectangle rect) override;
	

	// Tiled Fragment
	// Creates a renderer attached to a rectangular fragment of another image.
	// The image is considered to be divided into numCols x numRows regular rectangular tiles,
	// of which the one is taken iCol's column and iRow's row
	void Create(IRenderer*, short numCols, short numRows, short iCol, short iRow) override;
	void Create(std::string sFileName, short numCols, short numRows, short iCol, short iRow) override;


	// Low-level surface access and tester
	void CreateFromSurface(void*) override;
	void SetSurface(void* pSurface) override;
	void* GetSurface() override;
	bool HasPixels();


	
	// Static functions for standard path finding
	// change the default graphics file path (standard is "%;%images\\;.\\;images\\")
	static void SetDefaultFilePath(std::string new_path);
	// find a full pathname from the filename and default graphics file paths
	static std::string FindPathStr(std::string filename);

	// Rotozoom: create a clone object, rotated and zoomed
	CRendererSDL* CreateRotozoom(double angle, double zoomx, double zoomy, bool smooth = true) override;

	// Width & Height
	int GetWidth() override;
	int GetHeight() override;

	// Color functions
	CColor MatchColor(CColor color) override;				// Match color with the closest

	// Color Key (Trasnparent Color)
	void SetColorKey(CColor& color) override;
	bool IsColorKeySet() override;
	CColor GetColorKey() override;
	void ClearColorKey() override;

	// Pixel collision function
	// Collision with another renderer based object object - with pixel precision
	// nSkip skips pixels between test, high values increase efficiency but decrease accuracy, 1 for maximum accuracy, 0 to switch pixel precision off
	// Based on SDL_Collide by genjix & robloach (http://sdl-collide.sourceforge.net/)
	bool HitTest(int ax, int ay, IRenderer*, int bx, int by, int nSkip = 4) override;

	// Flip Function
	void Flip() override;

	// lock and unlock (for direct pixel access)
	bool Lock() override;
	void Unlock() override;

	// Drawing Functions
	CColor GetPixel(int x, int y) override;
	void SetPixel(int x, int y, CColor& color) override;

	void Clear(CColor& clr) override;
	void Blit(CRectangle& rectDest, IRenderer& Src, CRectangle& rectSrc) override;

	void DrawHLine(CVectorI pt1, int16_t x2, CColor& color) override;
	void DrawVLine(CVectorI pt1, int16_t y2, CColor& color) override;
	void DrawLine(CVectorI pt1, CVectorI pt2, CColor& color) override;
	void DrawLine(CVectorI pt1, CVectorI pt2, uint8_t width, CColor& color) override;	// thick line
	void DrawRect(CRectangle& rect, CColor& color) override;
	void FillRect(CRectangle& rect, CColor& color) override;
	void DrawRect(CRectangle& rect, CColor& color, int16_t rad) override;				// rounded rectangle
	void FillRect(CRectangle& rect, CColor& color, int16_t rad) override;				// rounded rectangle
	void DrawOval(CRectangle& rect, CColor& color) override;
	void FillOval(CRectangle& rect, CColor& color) override;
	void DrawCircle(CVectorI pt, uint16_t radius, CColor& color) override;
	void FillCircle(CVectorI pt, uint16_t radius, CColor& color) override;
	void DrawPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color) override;
	void FillPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color) override;
	void DrawTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color) override;
	void FillTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color) override;
	void DrawPolyLine(CVectorI pts[], uint16_t num, CColor& color) override;
	void DrawPolygon(CVectorI pts[], uint16_t num, CColor& color) override;
	void FillPolygon(CVectorI pts[], uint16_t num, CColor& color) override;
	void DrawBezierLine(CVectorI pts[], uint16_t num, uint16_t nSteps, CColor& color) override;

	// Text Functions (low level)
	bool SetFont(std::string fontFace, int nPtSize = 0) override;
	bool SetFont(int nPtSize) override;

	void SetTextColor(CColor textColor) override		{ m_textColor = textColor;  }
	CColor GetTextColor() override						{ return m_textColor; }

	std::string GetFontFace() override;
	int GetFontSize() override;
	void GetFontSize(int& size, int& height, int& width, int& ascent, int& descent, int& leading, int& baseline) override;

	IRenderer* GetTextGraphics(std::string pText) override;
	IRenderer* GetTextGraphics(std::string fontFace, int nPtSize, CColor color, std::string pText) override;
private:
	IRenderer* CRendererSDL::GetTextGraphics(void* pFont, CColor textColor, std::string pText);
};

#endif

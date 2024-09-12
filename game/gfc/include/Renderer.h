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
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Color.h"
#include "Vector.h"
#include "Rectangle.h"

// CRenderer class
class EXT_DECL IRenderer
{
public:
	// Default Constructor
	IRenderer() = default;

	// Virtual Destructor
	virtual ~IRenderer() = default;


	// Type Identification - returns a type id string, for example "SDL", "SDL2", "OGL"
	virtual std::string GetType() = 0;


	// Virtual Initialisers:
	
	// system-wide initialisation
	virtual void Initialise(int width, int height, int depth, uint32_t flagsCreation) = 0;

	// clones another Renderer - checks if they are of compatible type. Produces a separate object
	virtual void Clone(IRenderer&) = 0;

	// create from another Renderer - checks if they are of compatible type.
	// May be identical to Clone, or share the same underlying object - imlementation dependent.
	virtual void Create(IRenderer&) = 0;

	// craete from memory canvas
	virtual void Create(int width, int height, int depth, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) = 0;
	// craete from memory canvas compatible with the current display
	virtual void Create(int width, int height) = 0;

	// craete from a loaded bitmap
	virtual void Create(std::string sFileName) = 0;

	// Rectangular Fragment
	// Creates a renderer attached to a rectangular fragment of another image.
	virtual void Create(std::shared_ptr<IRenderer>, CRectangle) = 0;
	virtual void Create(std::string sFileName, CRectangle rect) = 0;
	

	// Tiled Fragment
	// Creates a renderer attached to a rectangular fragment of another image.
	// The image is considered to be divided into numCols x numRows regular rectangular tiles,
	// of which the one is taken iCol's column and iRow's row
	virtual void Create(std::shared_ptr<IRenderer>, short numCols, short numRows, short iCol, short iRow) = 0;
	virtual void Create(std::string sFileName, short numCols, short numRows, short iCol, short iRow) = 0;


	// Low-level surface access and tester
	virtual void CreateFromSurface(void*) = 0;
	virtual void SetSurface(void*) = 0;
	virtual void* GetSurface() = 0;
	virtual bool HasPixels() = 0;


		
	// Static functions for standard path finding
	// change the default graphics file path (standard is "%;%images\\;.\\;images\\")
//	static void SetDefaultFilePath(std::string new_path);

	// Rotozoom: create a clone object, rotated and zoomed
	virtual std::shared_ptr<IRenderer> CreateRotozoom(double angle, double zoomx, double zoomy, bool smooth = true) = 0;


	// Width & Height
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;

	// Color functions
	virtual CColor MatchColor(CColor color) = 0;				// Match color with the closest

	// Color Key (Trasnparent Color)
	virtual void SetColorKey(CColor color) = 0;
	virtual bool IsColorKeySet() = 0;
	virtual CColor GetColorKey() = 0;
	virtual void ClearColorKey() = 0;

	// Pixel collision function
	// Collision with another renderer based object object - with pixel precision
	// nSkip skips pixels between test, high values increase efficiency but decrease accuracy, 1 for maximum accuracy, 0 to switch pixel precision off
	// Based on SDL_Collide by genjix & robloach (http://sdl-collide.sourceforge.net/)
	virtual bool HitTest(int ax, int ay, std::shared_ptr<IRenderer>, int bx, int by, int nSkip = 4) = 0;

	// Flip Function
	virtual void Flip() = 0;

	// Lock and unlock (for direct pixel access)
	virtual bool Lock() = 0;
	virtual void Unlock() = 0;

	// Drawing Functions
	virtual CColor GetPixel(int x, int y) = 0;
	virtual void SetPixel(int x, int y, CColor& color) = 0;

	virtual void Clear(CColor& clr) = 0;
	virtual void Blit(CRectangle& rectDest, IRenderer& Src, CRectangle& rectSrc) = 0;

	virtual void DrawHLine(CVectorI pt1, int16_t x2, CColor& color) = 0;
	virtual void DrawVLine(CVectorI pt1, int16_t y2, CColor& color) = 0;
	virtual void DrawLine(CVectorI pt1, CVectorI pt2, CColor& color) = 0;
	virtual void DrawLine(CVectorI pt1, CVectorI pt2, uint8_t width, CColor& color) = 0;	// thick line
	virtual void DrawRect(CRectangle& rect, CColor& color) = 0;
	virtual void FillRect(CRectangle& rect, CColor& color) = 0;
	virtual void DrawRect(CRectangle& rect, CColor& color, int16_t rad) = 0;				// rounded rectangle
	virtual void FillRect(CRectangle& rect, CColor& color, int16_t rad) = 0;				// rounded rectangle
	virtual void DrawOval(CRectangle& rect, CColor& color) = 0;
	virtual void FillOval(CRectangle& rect, CColor& color) = 0;
	virtual void DrawCircle(CVectorI pt, uint16_t radius, CColor& color) = 0;
	virtual void FillCircle(CVectorI pt, uint16_t radius, CColor& color) = 0;
	virtual void DrawPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color) = 0;
	virtual void FillPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color) = 0;
	virtual void DrawTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color) = 0;
	virtual void FillTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color) = 0;
	virtual void DrawPolyLine(CVectorI pts[], uint16_t num, CColor& color) = 0;
	virtual void DrawPolygon(CVectorI pts[], uint16_t num, CColor& color) = 0;
	virtual void FillPolygon(CVectorI pts[], uint16_t num, CColor& color) = 0;
	virtual void DrawBezierLine(CVectorI pts[], uint16_t num, uint16_t nSteps, CColor& color) = 0;

	// Text Functions (low level)
	virtual bool SetFont(std::string fontFace, int nPtSize = 0) = 0;
	virtual bool SetFont(int nPtSize) = 0;

	virtual void SetTextColor(CColor) = 0;
	virtual CColor GetTextColor() = 0;

	virtual std::string GetFontFace() = 0;
	virtual int GetFontSize() = 0;
	virtual void GetFontSize(int& size, int& height, int& width, int& ascent, int& descent, int& leading, int& baseline) = 0;

	virtual std::shared_ptr<IRenderer> GetTextGraphics(std::string pText) = 0;
	virtual std::shared_ptr<IRenderer> GetTextGraphics(std::string fontFace, int nPtSize, CColor color, std::string pText) = 0;
};

#endif

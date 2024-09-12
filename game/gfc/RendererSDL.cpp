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
#include "RendererSDL.h"

#include "sdl/include/SDL_image.h"
#include "sdl/include/SDL_ttf.h"
#include "sdl/include/SDL_gfxPrimitives.h"
#include "sdl/include/SDL_rotozoom.h"
#include "sdl/include/SDL_collide.h"

using namespace std;

unsigned char NO_IMAGE[] = { 
66,77,246,0,0,0,0,0,0,0,118,0,0,0,40,0,0,0,16,0,0,0,16,0,0,0,1,0,4,0,0,0,0,0,128,
0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,16,0,0,0,0,0,0,0,0,0,128,0,0,128,0,0,0,128,128,0,
128,0,0,0,128,0,128,0,128,128,0,0,192,192,192,0,128,128,128,0,0,0,255,0,0,255,0,
0,0,255,255,0,255,0,0,0,255,0,255,0,255,255,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,153,153,153,153,153,153,0,0,159,249,153,153,159,249,0,0,159,255,
153,153,255,249,0,0,153,255,249,159,255,153,0,0,153,159,255,255,249,153,0,0,153,
153,255,255,153,153,0,0,153,153,255,255,153,153,0,0,153,159,255,255,249,153,0,0,
153,255,249,159,255,153,0,0,159,255,153,153,255,249,0,0,159,249,153,153,159,249,
0,0,153,153,153,153,153,153,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
SDL_Surface* NoImage() { return SDL_LoadBMP_RW(SDL_RWFromMem(NO_IMAGE, sizeof(NO_IMAGE)), 1); }


CFileMgr<SDL_Surface> CRendererSDL::c_filemgr("%;%images\\;.\\;images\\",
											[](string filename) 
											{ 
												return IMG_Load(filename.c_str()); 
											},
											[](SDL_Surface *pSurface) 
											{ 
												SDL_FreeSurface(pSurface); 
											});

CFileMgr<_TTF_Font> CRendererSDL::c_fontmgr("%;%images\\;.\\;images\\",
	[](string filename, int nSize)
	{
		return TTF_OpenFont(CRendererSDL::FindPathStr(filename).c_str(), nSize);
	},
	[](_TTF_Font* pFont)
	{
		// TTF_CloseFont(pFont);   //// should be called here, but causes "Access violation reading location 0x0000000000000054"
	});

/////////////////////////////////////////////////////////
// Constructors & Destructors

CRendererSDL::CRendererSDL() : IRenderer()
{
	m_pSurface = NULL;
}

// copy constructor
CRendererSDL::CRendererSDL(CRendererSDL& renderer) : IRenderer()
{
	Clone(renderer);
}

CRendererSDL::~CRendererSDL()
{
	if (m_pSurface)
	{
		SDL_FreeSurface(m_pSurface);
		m_pSurface = NULL;
	}
}

/////////////////////////////////////////////////////////
// Virtual Initialisers

// System-wide initialisation
void CRendererSDL::Initialise(int width, int height, int depth, uint32_t flagsCreation)
{
	m_pSurface = SDL_SetVideoMode(width, height, depth, flagsCreation);
}

// Clone another Renderer
void CRendererSDL::Clone(IRenderer& renderer)
{
	if (renderer.GetSurface() && GetType() == renderer.GetType())
	{
		SDL_Surface* pSurface = static_cast<SDL_Surface*>(renderer.GetSurface());
		m_pSurface = SDL_ConvertSurface(pSurface, pSurface->format, pSurface->flags);
	}
	else
		m_pSurface = NoImage();
}

// Create from another Renderer - identical to Clone
void CRendererSDL::Create(IRenderer& renderer)
{
	Clone(renderer);
	//m_pSurface = static_cast<SDL_Surface*>(renderer.GetSurface());
}

// Create from memory canvas
void CRendererSDL::Create(int width, int height, int depth, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask)
{
	m_pSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, depth, Rmask, Gmask, Bmask, Amask);
}

	// memory canvas compatible with the current display
	static SDL_PixelFormat *_getVideoSurfaceFormat()
	{
		static SDL_PixelFormat stdformat = { NULL, 32, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0xff0000, 0xff00, 0xff, 0, 0, 0 };
		SDL_Surface *pVideo = SDL_GetVideoSurface();
		if (pVideo)
			return pVideo->format;
		else
			return &stdformat;
	}

// Create from memory canvas compatible with the current display
void CRendererSDL::Create(int width, int height)
{
	SDL_PixelFormat* pVideoFormat = _getVideoSurfaceFormat();
	m_pSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, pVideoFormat->BitsPerPixel, pVideoFormat->Rmask, pVideoFormat->Gmask, pVideoFormat->Bmask, pVideoFormat->Amask);
}

// Create from a loaded bitmap
void CRendererSDL::Create(string sFileName)
{
	SDL_Surface *pSurface = c_filemgr.Load(sFileName);
	if (pSurface)
	{
		m_pSurface = SDL_ConvertSurface(pSurface, pSurface->format, pSurface->flags);
		if (m_pSurface->format->BitsPerPixel == 24)
		{
			SDL_Surface *pBlit = SDL_CreateRGBSurface(SDL_SWSURFACE, m_pSurface->w, m_pSurface->h, 32, m_pSurface->format->Rmask, m_pSurface->format->Gmask, m_pSurface->format->Bmask, m_pSurface->format->Amask /*| 0xFF000000*/);
			SDL_BlitSurface(m_pSurface, NULL, pBlit, NULL);
			SDL_FreeSurface(m_pSurface);
			m_pSurface = pBlit;
		}
	}
	else
		m_pSurface = NoImage();
}

// Rectangular Fragment
void CRendererSDL::Create(shared_ptr<IRenderer> pRenderer, CRectangle rect)
{
	if (pRenderer && pRenderer->GetSurface() && GetType() == pRenderer->GetType())
	{
		SDL_Surface* pSurface = static_cast<SDL_Surface*>(pRenderer->GetSurface());
		SDL_PixelFormat* pVideoFormat = _getVideoSurfaceFormat();
		m_pSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, rect.w, rect.h, pVideoFormat->BitsPerPixel, pVideoFormat->Rmask, pVideoFormat->Gmask, pVideoFormat->Bmask, pVideoFormat->Amask);
		SDL_BlitSurface(pSurface, (SDL_Rect*)&CRectangle(rect.x, rect.y, rect.w, rect.h).YInv(pRenderer->GetHeight()), m_pSurface, (SDL_Rect*)&CRectangle(0, 0, rect.w, rect.h).YInv(GetHeight()));
	}
	else
		m_pSurface = NoImage();
}

void CRendererSDL::Create(string sFileName, CRectangle rect)
{
	SDL_Surface *pFileSurface = c_filemgr.Load(sFileName);
	if (pFileSurface)
	{
		SDL_PixelFormat* pVideoFormat = _getVideoSurfaceFormat();
		m_pSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, rect.w, rect.h, pVideoFormat->BitsPerPixel, pVideoFormat->Rmask, pVideoFormat->Gmask, pVideoFormat->Bmask, pVideoFormat->Amask);
		SDL_BlitSurface(pFileSurface, (SDL_Rect*)&CRectangle(rect.x, rect.y, rect.w, rect.h).YInv(pFileSurface->h), m_pSurface, (SDL_Rect*)&CRectangle(0, 0, rect.w, rect.h).YInv(GetHeight()));
	}
	else
		m_pSurface = NoImage();
}

// Tiled Fragment
void CRendererSDL::Create(shared_ptr<IRenderer> pRenderer, short numCols, short numRows, short iCol, short iRow)
{
	if (pRenderer && pRenderer->GetSurface() && GetType() == pRenderer->GetType())
	{
		int width = pRenderer->GetWidth() / numCols;
		int height = pRenderer->GetHeight() / numRows;

		SDL_Surface* pSurface = static_cast<SDL_Surface*>(pRenderer->GetSurface());
		SDL_PixelFormat* pVideoFormat = _getVideoSurfaceFormat();
		m_pSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, pVideoFormat->BitsPerPixel, pVideoFormat->Rmask, pVideoFormat->Gmask, pVideoFormat->Bmask, pVideoFormat->Amask);
		SDL_BlitSurface(pSurface, (SDL_Rect*)&CRectangle(iCol * width, iRow * height, width, height).YInv(pRenderer->GetHeight()), m_pSurface, (SDL_Rect*)&CRectangle(0, 0, width, height).YInv(GetHeight()));
	}
	else
		m_pSurface = NoImage();
}

void CRendererSDL::Create(string sFileName, short numCols, short numRows, short iCol, short iRow)
{
	SDL_Surface *pFileSurface = c_filemgr.Load(sFileName);

	if (pFileSurface)
	{
		int width = pFileSurface->w / numCols;
		int height = pFileSurface->h / numRows;

		SDL_PixelFormat* pVideoFormat = _getVideoSurfaceFormat();
		m_pSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, pVideoFormat->BitsPerPixel, pVideoFormat->Rmask, pVideoFormat->Gmask, pVideoFormat->Bmask, pVideoFormat->Amask);
		SDL_BlitSurface(pFileSurface, (SDL_Rect*)&CRectangle(iCol * width, iRow * height, width, height).YInv(pFileSurface->h), m_pSurface, (SDL_Rect*)&CRectangle(0, 0, width, height).YInv(GetHeight()));
	}
	else
		m_pSurface = NoImage();
}

/////////////////////////////////////////////////////////
// Low-level surface access and tester

void CRendererSDL::CreateFromSurface(void* pSurface)
{
	if (pSurface)
		m_pSurface = static_cast<SDL_Surface*>(pSurface);
	else
		m_pSurface = NoImage();
}

void CRendererSDL::SetSurface(void* pSurface)
{
	m_pSurface = static_cast<SDL_Surface*>(pSurface);
}

void* CRendererSDL::GetSurface()
{
	return m_pSurface;
}

bool CRendererSDL::HasPixels()
{
	return m_pSurface && m_pSurface->pixels;
}

/////////////////////////////////////////////////////////
// Static functions for standard path finding

void CRendererSDL::SetDefaultFilePath(string new_path) 
{ 
	c_filemgr.SetPathString(new_path); 
}

string CRendererSDL::FindPathStr(string filename)
{
	return c_filemgr.FindPathStr(filename);
}

/////////////////////////////////////////////////////////
// RotoZoom: create a clone object, rotated and zoomed

shared_ptr<IRenderer> CRendererSDL::CreateRotozoom(double angle, double zoomx, double zoomy, bool smooth)
{
	if (!m_pSurface) return NULL;

	SDL_Surface* pRotoZoom = NULL;
	if (zoomx == 1.0 && zoomy == 1.0)
	{
		pRotoZoom = rotozoomSurface(m_pSurface, angle, 1.0, smooth);
	}
	else
	{
		// must be zoomed and rotated in two steps because of a bug in rotozoomSurfaceXY
		SDL_Surface* pZoom = rotozoomSurfaceXY(m_pSurface, 0, zoomx, zoomy, smooth);
		pRotoZoom = rotozoomSurface(pZoom, angle, 1.0, smooth);
		SDL_FreeSurface(pZoom);
	}

	auto p = make_shared<CRendererSDL>();
	p->CreateFromSurface(pRotoZoom);

	return p;
}

/////////////////////////////////////////////////////////
// Width & Height

int CRendererSDL::GetWidth()
{ 
	return m_pSurface ? m_pSurface->w : 0;
}

int CRendererSDL::GetHeight()
{ 
	return m_pSurface ? m_pSurface->h : 0;
}

/////////////////////////////////////////////////////////
// Match color with the closest

CColor CRendererSDL::MatchColor(CColor color)
{
	if (!m_pSurface) return color;

	//convert it to the pixel format
	uint32_t col = SDL_MapRGBA(m_pSurface->format, color.R(), color.G(), color.B(), color.A());

	//convert it from the pixel format
	SDL_GetRGBA(col, m_pSurface->format, &color.R(), &color.G(), &color.B(), &color.A());

	//return the matched color
	return (color);
}

/////////////////////////////////////////////////////////
// Color Key (Trasnparent Color)

void CRendererSDL::SetColorKey(CColor color)
{
	if (!m_pSurface) return;
	m_bColorKey = true;
	m_colorKey = color;
	uint32_t col = SDL_MapRGBA(m_pSurface->format, color.R(), color.G(), color.B(), color.A());
	SDL_SetColorKey(m_pSurface, SDL_SRCCOLORKEY, col);
}

bool CRendererSDL::IsColorKeySet()
{
	return m_bColorKey;
}

CColor CRendererSDL::GetColorKey()
{
	if (!m_bColorKey) return CColor::Black();

	uint32_t col = m_pSurface->format->colorkey;
	CColor color;
	SDL_GetRGBA(col, m_pSurface->format, &color.R(), &color.G(), &color.B(), &color.A());
	return (color);
}

void CRendererSDL::ClearColorKey()
{
	m_bColorKey = false;
	m_colorKey = CColor::Black();
	if (m_pSurface) SDL_SetColorKey(m_pSurface, 0, 0);
}

/////////////////////////////////////////////////////////
// Pixel collision function

bool CRendererSDL::HitTest(int ax, int ay, shared_ptr<IRenderer> pOther, int bx, int by, int nSkip)
{
	if (!m_pSurface || !pOther || !pOther->GetSurface()) return false;
	SDL_Surface* pOtherSurface = static_cast<SDL_Surface*>(pOther->GetSurface());
	return SDL_CollidePixel(m_pSurface, ax, ay, pOtherSurface, bx, by, nSkip) != 0;
}

/////////////////////////////////////////////////////////
// Flip Function

void CRendererSDL::Flip()
{
	if (m_pSurface) SDL_Flip(m_pSurface);
}

/////////////////////////////////////////////////////////
// lock and unlock (for direct pixel access)

bool CRendererSDL::Lock()
{
	if (!m_pSurface) return false;
	if (!SDL_MUSTLOCK(m_pSurface))
		return true;
	return (SDL_LockSurface(m_pSurface) == 0);
}

void CRendererSDL::Unlock()
{
	if (m_pSurface && SDL_MUSTLOCK(m_pSurface))
		SDL_UnlockSurface(m_pSurface);
}


/////////////////////////////////////////////////////////
// Drawing Functions

CColor CRendererSDL::GetPixel(int x, int y)
{
	if (!m_pSurface) return CColor::Black();

	uint32_t color = 0;
	int position = (GetHeight() - y - 1) * m_pSurface->pitch + m_pSurface->format->BytesPerPixel * x;
	char* buffer =(char*)m_pSurface->pixels;
	buffer += position;
	Lock();
	memcpy(&color, buffer, m_pSurface->format->BytesPerPixel);
	Unlock();
	CColor col;
	SDL_GetRGBA(color, m_pSurface->format, &col.R(), &col.G(), &col.B(), &col.A());
	return (col);
}

void CRendererSDL::SetPixel(int x, int y, CColor& color)
{
	if (!m_pSurface) return;
	int position = (GetHeight() - y - 1) * m_pSurface->pitch + m_pSurface->format->BytesPerPixel * x;
	char* buffer =(char*)m_pSurface->pixels;
	buffer += position;
	uint32_t col = SDL_MapRGBA(m_pSurface->format, color.R(), color.G(), color.B(), color.A());
	Lock();
	memcpy(buffer, &col, m_pSurface->format->BytesPerPixel);
	Unlock();
}

void CRendererSDL::Clear(CColor& clr)
{
	if (m_pSurface) SDL_FillRect(m_pSurface, NULL, SDL_MapRGBA(m_pSurface->format, clr.R(), clr.G(), clr.B(), clr.A()));
}

void CRendererSDL::Blit(CRectangle& rectDest, IRenderer& Src, CRectangle& rectSrc)
{
	SDL_Surface* pSurface = static_cast<SDL_Surface*>(Src.GetSurface());
	if (m_pSurface) SDL_BlitSurface(pSurface, (SDL_Rect*)&rectSrc.YInv(Src.GetHeight()), m_pSurface, (SDL_Rect*)&(rectDest).YInv(GetHeight()));
}

void CRendererSDL::DrawHLine(CVectorI pt1, int16_t x2, CColor& color)
{
	if (m_pSurface) hlineRGBA(m_pSurface, pt1.GetX(), x2, GetHeight() - pt1.GetY() - 1, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawVLine(CVectorI pt1, int16_t y2, CColor& color)
{
	if (m_pSurface) vlineRGBA(m_pSurface, pt1.GetX(), GetHeight() - pt1.GetY() - 1, GetHeight() - y2 - 1, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawLine(CVectorI pt1, CVectorI pt2, CColor& color)
{
	if (m_pSurface) lineRGBA(m_pSurface, pt1.GetX(), GetHeight() - pt1.GetY() - 1, pt2.GetX(), GetHeight() - pt2.GetY() - 1, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawLine(CVectorI pt1, CVectorI pt2, uint8_t width, CColor& color)
{
	if (m_pSurface) thickLineRGBA(m_pSurface, pt1.GetX(), GetHeight() - pt1.GetY() - 1, pt2.GetX(), GetHeight() - pt2.GetY() - 1, width, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawRect(CRectangle& rect, CColor& color)
{
	if (m_pSurface) rectangleRGBA(m_pSurface, rect.Left(), GetHeight() - rect.Top(), rect.Right(), GetHeight() - rect.Bottom(), color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::FillRect(CRectangle& rect, CColor& color)
{
	if (m_pSurface) SDL_FillRect(m_pSurface, (SDL_Rect*)&(rect).YInv(GetHeight()), SDL_MapRGBA(m_pSurface->format, color.R(), color.G(), color.B(), color.A()));	// this is the only function here to use main SDL rather than SDL_gfxPrimitives 
}

void CRendererSDL::DrawRect(CRectangle& rect, CColor& color, int16_t rad)
{
	if (m_pSurface) roundedRectangleRGBA(m_pSurface, rect.Left(), GetHeight() - rect.Top(), rect.Right(), GetHeight() - rect.Bottom(), rad, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::FillRect(CRectangle& rect, CColor& color, int16_t rad)
{
	if (m_pSurface) roundedBoxRGBA(m_pSurface, rect.Left(), GetHeight() - rect.Top(), rect.Right(), GetHeight() - rect.Bottom(), rad, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawOval(CRectangle& rect, CColor& color)
{
	if (m_pSurface) ellipseRGBA(m_pSurface, rect.GetCenterX(), GetHeight() - rect.GetCenterY(), rect.w/2, rect.h/2, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::FillOval(CRectangle& rect, CColor& color)
{
	if (m_pSurface) filledEllipseRGBA(m_pSurface, rect.GetCenterX(), GetHeight() - rect.GetCenterY(), rect.w/2, rect.h/2, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawCircle(CVectorI pt, uint16_t radius, CColor& color)
{
	if (m_pSurface) circleRGBA(m_pSurface, pt.GetX(), GetHeight() - pt.GetY(), radius, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::FillCircle(CVectorI pt, uint16_t radius, CColor& color)
{
	if (m_pSurface) filledCircleRGBA(m_pSurface, pt.GetX(), GetHeight() - pt.GetY(), radius, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color)
{
	if (m_pSurface) pieRGBA(m_pSurface, pt.GetX(), GetHeight() - pt.GetY(), radius, angleStart-90, angleEnd-90, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::FillPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color)
{
	if (m_pSurface) filledPieRGBA(m_pSurface, pt.GetX(), GetHeight() - pt.GetY(), radius, angleStart-90, angleEnd-90, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color)
{
	if (m_pSurface) trigonRGBA(m_pSurface, pt1.GetX(), GetHeight() - pt1.GetY(), pt2.GetX(), GetHeight() - pt2.GetY(), pt3.GetX(), GetHeight() - pt3.GetY(), color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::FillTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color)
{
	if (m_pSurface) filledTrigonRGBA(m_pSurface, pt1.GetX(), GetHeight() - pt1.GetY(), pt2.GetX(), GetHeight() - pt2.GetY(), pt3.GetX(), GetHeight() - pt3.GetY(), color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawPolyLine(CVectorI pts[], uint16_t num, CColor& color)
{
	if (!m_pSurface) return;
	if (num >= 2)
		for (int i = 0; i < num-1; i++)
			lineRGBA(m_pSurface, pts[i].GetX(), GetHeight() - pts[i].GetY() - 1, pts[i + 1].GetX(), GetHeight() - pts[i + 1].GetY() - 1, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void CRendererSDL::DrawPolygon(CVectorI pts[], uint16_t num, CColor& color)
{
	if (!m_pSurface) return;
	int16_t *xs = new int16_t[num];
	int16_t *ys = new int16_t[num];
	for (int i = 0; i < num; i++)
	{;
		xs[i] = pts[i].GetX();
		ys[i] = GetHeight() - pts[i].GetY();
	}
	polygonRGBA(m_pSurface, xs, ys, num, color.GetR(), color.GetG(), color.GetB(), color.GetA());
	delete [] xs;
	delete [] ys;
}

void CRendererSDL::FillPolygon(CVectorI pts[], uint16_t num, CColor& color)
{
	if (!m_pSurface) return;
	int16_t *xs = new int16_t[num];
	int16_t *ys = new int16_t[num];
	for (int i = 0; i < num; i++)
	{
		xs[i] = pts[i].GetX();
		ys[i] = GetHeight() - pts[i].GetY();
	}
	filledPolygonRGBA(m_pSurface, xs, ys, num, color.GetR(), color.GetG(), color.GetB(), color.GetA());
	delete [] xs;
	delete [] ys;
}

void CRendererSDL::DrawBezierLine(CVectorI pts[], uint16_t num, uint16_t nSteps, CColor& color)
{
	if (!m_pSurface) return;
	int16_t *xs = new int16_t[num];
	int16_t *ys = new int16_t[num];
	for (int i = 0; i < num; i++)
	{
		xs[i] = pts[i].GetX();
		ys[i] = GetHeight() - pts[i].GetY();
	}
	bezierRGBA(m_pSurface, xs, ys, num, nSteps, color.GetR(), color.GetG(), color.GetB(), color.GetA());
	delete [] xs;
	delete [] ys;
}

/////////////////////////////////////////////////////////
// Text Drawing Functions

bool CRendererSDL::SetFont(string fontFace, int nPtSize)
{
	m_strFontFace = fontFace;
	if (nPtSize != 0) m_nPtSize = nPtSize;
	if (m_nPtSize == 0) m_nPtSize = 18;
	
	m_pFont = CRendererSDL::c_fontmgr.Load(m_strFontFace, m_nPtSize);
	return m_pFont != NULL;
}

bool CRendererSDL::SetFont(int nPtSize)
{
	if (nPtSize != 0) m_nPtSize = nPtSize;
	if (m_nPtSize == 0) m_nPtSize = 18;

	m_pFont = CRendererSDL::c_fontmgr.Load(m_strFontFace, m_nPtSize);
	return m_pFont != NULL;
}

string CRendererSDL::GetFontFace()
{
	return m_strFontFace;
}

int CRendererSDL::GetFontSize()
{
	return m_nPtSize;
}

void CRendererSDL::GetFontSize(int& size, int& height, int& width, int& ascent, int& descent, int& leading, int& baseline)
{
	size = m_nPtSize;
	height = TTF_FontHeight(m_pFont);
	width = height;
	ascent = TTF_FontAscent(m_pFont);
	descent = TTF_FontDescent(m_pFont);
	leading = max(TTF_FontLineSkip(m_pFont), height);
	baseline = leading - height - descent;
}

shared_ptr<IRenderer> CRendererSDL::GetTextGraphics(string pText)
{
	if (!m_pFont)
		SetFont(0);
	return GetTextGraphics(m_pFont, m_colorText, pText);
}

shared_ptr<IRenderer> CRendererSDL::GetTextGraphics(string fontFace, int nPtSize, CColor color, string pText)
{
	if (nPtSize == 0) nPtSize = m_nPtSize;
	if (m_nPtSize == 0) nPtSize = 18;
	return GetTextGraphics(c_fontmgr.Load(fontFace, nPtSize), color, pText);
}


shared_ptr<IRenderer> CRendererSDL::GetTextGraphics(void* pFont, CColor textColor, string pText)
{
	if (!pFont) return NULL;
	SDL_Surface* pSurface = TTF_RenderText_Blended(static_cast<_TTF_Font*>(pFont), pText.c_str(), *(SDL_Color*)&textColor.r);
	auto pRenderer = make_shared<CRendererSDL>();
	pRenderer->CreateFromSurface(pSurface);
	return pRenderer;
}

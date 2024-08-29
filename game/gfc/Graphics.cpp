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
#include "Graphics.h"
#include "Rectangle.h"

#include "RendererFactory.h"

using namespace std;

/////////////////////////////////////////////////////////
// constructor

CGraphics::CGraphics(int width, int height, int depth, uint32_t flagsCreation)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(width, height, depth, flagsCreation);
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}


// from a SDL Surface
//CGraphics::CGraphics(void* pSurface)
//{
//	m_pRenderer = CRendererFactory::Instance().Create();
//	m_pRenderer->CreateFromSurface(pSurface);
//	m_ml = m_mr = 5; m_mu = m_mb = 2;
//	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
//}

// from a CSurface object
CGraphics::CGraphics(IRenderer* pRenderer)
{
	m_pRenderer = pRenderer;
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

// memory canvas
CGraphics::CGraphics(int width, int height, int depth, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(width, height, depth, Rmask, Gmask, Bmask, Amask);
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

CGraphics::CGraphics(int width, int height, int depth, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask, CColor colorKey)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(width, height, depth, Rmask, Gmask, Bmask, Amask);
	SetColorKey(colorKey);
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

// memory canvas compatible with the current display
CGraphics::CGraphics(int width, int height)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(width, height);
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

CGraphics::CGraphics(int width, int height, CColor colorKey)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(width, height);
	SetColorKey(colorKey);
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

// from a loaded bitmap
CGraphics::CGraphics(string sFileName)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(sFileName);
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

CGraphics::CGraphics(string sFileName, CColor colorKey)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(sFileName);
	SetColorKey(colorKey);
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

// copy constructor
CGraphics::CGraphics(CGraphics &g)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(g.GetRenderer());

	if (g.IsColorKeySet()) SetColorKey(g.GetColorKey());
	m_ml = g.m_ml; m_mr = g.m_mr; m_mu = g.m_mu; m_mb = g.m_mb;
	m_fontSize = g.m_fontSize; m_fontHeight = g.m_fontHeight; m_fontWidth = g.m_fontWidth; m_fontAscent = g.m_fontAscent; m_fontDescent = g.m_fontDescent; m_fontLeading = g.m_fontLeading; m_fontBaseline = g.m_fontBaseline;
}

// copy from a pointer
CGraphics::CGraphics(CGraphics *pG)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(pG ? pG->GetRenderer() : NULL);

	if (pG)
	{
		if (pG->IsColorKeySet()) SetColorKey(pG->GetColorKey());
		m_ml = pG->m_ml; m_mr = pG->m_mr; m_mu = pG->m_mu; m_mb = pG->m_mb;
		m_fontSize = pG->m_fontSize; m_fontHeight = pG->m_fontHeight; m_fontWidth = pG->m_fontWidth; m_fontAscent = pG->m_fontAscent; m_fontDescent = pG->m_fontDescent; m_fontLeading = pG->m_fontLeading; m_fontBaseline = pG->m_fontBaseline;
	}
	else
	{
		m_ml = m_mr = 5; m_mu = m_mb = 2;
		m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
	}
}

// Rectangular Fragment
CGraphics::CGraphics(CGraphics *pG, CRectangle rect)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(pG ? pG->GetRenderer() : NULL, rect);

	if (pG)
	{
		if (pG->IsColorKeySet()) SetColorKey(pG->GetColorKey());
		m_ml = pG->m_ml; m_mr = pG->m_mr; m_mu = pG->m_mu; m_mb = pG->m_mb;
		m_fontSize = pG->m_fontSize; m_fontHeight = pG->m_fontHeight; m_fontWidth = pG->m_fontWidth; m_fontAscent = pG->m_fontAscent; m_fontDescent = pG->m_fontDescent; m_fontLeading = pG->m_fontLeading; m_fontBaseline = pG->m_fontBaseline;
	}
	else
	{
		m_ml = m_mr = 5; m_mu = m_mb = 2;
		m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
	}
}

CGraphics::CGraphics(string sFileName, CRectangle rect)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(sFileName, rect);

	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

CGraphics::CGraphics(string sFileName, CRectangle rect, CColor colorKey)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(sFileName, rect);

	SetColorKey(colorKey);
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

// Tiled Fragment
CGraphics::CGraphics(CGraphics *pG, short numCols, short numRows, short iCol, short iRow)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(pG ? pG->GetRenderer() : NULL, numCols, numRows, iCol, iRow);
	
	if (pG)
	{
		if (pG->IsColorKeySet()) SetColorKey(pG->GetColorKey());
		m_ml = pG->m_ml; m_mr = pG->m_mr; m_mu = pG->m_mu; m_mb = pG->m_mb;
		m_fontSize = pG->m_fontSize; m_fontHeight = pG->m_fontHeight; m_fontWidth = pG->m_fontWidth; m_fontAscent = pG->m_fontAscent; m_fontDescent = pG->m_fontDescent; m_fontLeading = pG->m_fontLeading; m_fontBaseline = pG->m_fontBaseline;
	}
	else
	{
		m_ml = m_mr = 5; m_mu = m_mb = 2;
		m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
	}
}

CGraphics::CGraphics(string sFileName, short numCols, short numRows, short iCol, short iRow)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(sFileName, numCols, numRows, iCol, iRow);

	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

CGraphics::CGraphics(string sFileName, short numCols, short numRows, short iCol, short iRow, CColor colorKey)
{
	m_pRenderer = CRendererFactory::Instance().Create();
	m_pRenderer->Create(sFileName, numCols, numRows, iCol, iRow);

	SetColorKey(colorKey);
	m_ml = m_mr = 5; m_mu = m_mb = 2;
	m_fontSize = m_fontHeight = m_fontWidth = m_fontAscent = m_fontDescent = m_fontLeading = m_fontBaseline = 0;
}

/////////////////////////////////////////////////////////
// CGraphicsFragment

CGraphics::~CGraphics() 
{
	if (m_pRenderer)
	{
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

/////////////////////////////////////////////////////////
// RotoZoom: create a clone object, rotated and zoomed
CGraphics* CGraphics::CreateRotozoom(double angle, double zoomx, double zoomy, bool smooth)
{
	return m_pRenderer ? new CGraphics(m_pRenderer->CreateRotozoom(angle, zoomx, zoomy, smooth)) : NULL;
}

/////////////////////////////////////////////////////////
// Drawing Functions

CColor CGraphics::GetPixel(int x, int y) 
{
	x += GetScrollPos().m_x;
	y += GetScrollPos().m_y;
	return m_pRenderer ? m_pRenderer->GetPixel(x, y) : CColor::Black();
}

void CGraphics::SetPixel(int x, int y, CColor& color) 
{
	x += GetScrollPos().m_x;
	y += GetScrollPos().m_y;
	if (m_pRenderer) m_pRenderer->SetPixel(x, y, color);
}

void CGraphics::Clear(CColor& clr) 
{
	ResetScrollPos();
	m_ss.flags(ios::dec | ios::skipws | ios::left);
	m_ss.precision(6);
	m_ss.width(0);
	m_ss.str("");

	*this << margins() << font("arial.ttf", 18) << color(CColor::Black()) << top << left;

	if (m_pRenderer) m_pRenderer->Clear(clr);
}


void CGraphics::Blit(CRectangle& rectDest, CGraphics& Src, CRectangle& rectSrc)
{
	if (m_pRenderer && Src.GetRenderer()) m_pRenderer->Blit(rectDest + CVector(GetScrollPos()), *Src.GetRenderer(), rectSrc);
}

void CGraphics::Blit(CRectangle& rectDest, CGraphics& Src)
{
	Blit(rectDest, Src, CRectangle(0, 0, Src.GetWidth(), Src.GetHeight()));
}

void CGraphics::Blit(CVectorI ptDest, CGraphics& Src, CRectangle& rectSrc)
{
	CRectangle rect(ptDest.GetX(), ptDest.GetY(), rectSrc.w, rectSrc.h);
	Blit(rect, Src, rectSrc);
}

void CGraphics::Blit(CVectorI ptDest, CGraphics& Src)
{
	Blit(CRectangle(ptDest.GetX(), ptDest.GetY(), Src.GetWidth(), Src.GetHeight()), Src, CRectangle(0, 0, Src.GetWidth(), Src.GetHeight()));
}

void CGraphics::DrawHLine(CVectorI pt1, int16_t x2, CColor& color)
{
	pt1 += GetScrollPos();
	x2 += GetScrollPos().m_x;
	if (m_pRenderer) m_pRenderer->DrawHLine(pt1, x2, color);
}

void CGraphics::DrawVLine(CVectorI pt1, int16_t y2, CColor& color)
{
	pt1 += GetScrollPos();
	y2 += GetScrollPos().m_y;
	if (m_pRenderer) m_pRenderer->DrawVLine(pt1, y2, color);
}

void CGraphics::DrawLine(CVectorI pt1, CVectorI pt2, CColor& color)
{
	pt1 += GetScrollPos();
	pt2 += GetScrollPos();
	if (m_pRenderer) m_pRenderer->DrawLine(pt1, pt2, color);
}

void CGraphics::DrawLine(CVectorI pt1, CVectorI pt2, uint8_t width, CColor& color)
{
	pt1 += GetScrollPos();
	pt2 += GetScrollPos();
	if (m_pRenderer) m_pRenderer->DrawLine(pt1, pt2, width, color);
}

void CGraphics::DrawRect(CRectangle& rect, CColor& color)
{
	CRectangle R = rect + (CVector)GetScrollPos();
	if (m_pRenderer) m_pRenderer->DrawRect(R, color);
}

void CGraphics::FillRect(CRectangle& rect, CColor& color)
{
	CRectangle R = rect + (CVector)GetScrollPos();
	if (m_pRenderer) m_pRenderer->FillRect(R, color);
}

void CGraphics::DrawRect(CRectangle& rect, CColor& color, int16_t rad)
{
	CRectangle R = rect + (CVector)GetScrollPos();
	if (m_pRenderer) m_pRenderer->DrawRect(R, color, rad);
}

void CGraphics::FillRect(CRectangle& rect, CColor& color, int16_t rad)
{
	CRectangle R = rect + (CVector)GetScrollPos();
	if (m_pRenderer) m_pRenderer->FillRect(R, color, rad);
}

void CGraphics::DrawOval(CRectangle& rect, CColor& color)
{
	CRectangle R = rect + (CVector)GetScrollPos();
	if (m_pRenderer) m_pRenderer->DrawOval(R, color);
}

void CGraphics::FillOval(CRectangle& rect, CColor& color)
{
	CRectangle R = rect + (CVector)GetScrollPos();
	if (m_pRenderer) m_pRenderer->FillOval(R, color);
}

void CGraphics::DrawCircle(CVectorI pt, uint16_t radius, CColor& color)
{
	pt += GetScrollPos();
	if (m_pRenderer) m_pRenderer->DrawCircle(pt, radius, color);
}

void CGraphics::FillCircle(CVectorI pt, uint16_t radius, CColor& color)
{
	pt += GetScrollPos();
	if (m_pRenderer) m_pRenderer->FillCircle(pt, radius, color);
}

void CGraphics::DrawPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color)
{
	pt += GetScrollPos();
	if (m_pRenderer) m_pRenderer->DrawPie(pt, radius, angleStart, angleEnd, color);
}

void CGraphics::FillPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color)
{
	pt += GetScrollPos();
	if (m_pRenderer) m_pRenderer->FillPie(pt, radius, angleStart, angleEnd, color);
}

void CGraphics::DrawTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color)
{
	pt1 += GetScrollPos();
	pt2 += GetScrollPos();
	pt3 += GetScrollPos();
	if (m_pRenderer) m_pRenderer->DrawTriangle(pt1, pt2, pt3, color);
}

void CGraphics::FillTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color)
{
	pt1 += GetScrollPos();
	pt2 += GetScrollPos();
	pt3 += GetScrollPos();
	if (m_pRenderer) m_pRenderer->FillTriangle(pt1, pt2, pt3, color);
}

void CGraphics::DrawPolyLine(CVectorI pts[], uint16_t num, CColor& color)
{
	if (num >= 2)
		for (int i = 0; i < num-1; i++)
			DrawLine(pts[i], pts[i+1], color);
}

void CGraphics::DrawPolygon(CVectorI pts[], uint16_t num, CColor& color)
{
	if (!m_pRenderer) return;
	CVectorI *spts = new CVectorI[num];
	for (int i = 0; i < num; i++)
		spts[i] = pts[i] + GetScrollPos();
	m_pRenderer->DrawPolyLine(spts, num, color);
	delete[] spts;
}

void CGraphics::FillPolygon(CVectorI pts[], uint16_t num, CColor& color)
{
	if (!m_pRenderer) return;
	CVectorI* spts = new CVectorI[num];
	for (int i = 0; i < num; i++)
		spts[i] = pts[i] + GetScrollPos();
	m_pRenderer->FillPolygon(spts, num, color);
	delete[] spts;
}

void CGraphics::DrawBezierLine(CVectorI pts[], uint16_t num, uint16_t nSteps, CColor& color)
{
	if (!m_pRenderer) return;
	CVectorI* spts = new CVectorI[num];
	for (int i = 0; i < num; i++)
		spts[i] = pts[i] + GetScrollPos();
	m_pRenderer->DrawBezierLine(spts, num, nSteps, color);
	delete[] spts;
}

/////////////////////////////////////////////////////////
// Text Drawing Functions

int CGraphics::DrawText(CVectorI pt, std::string fontFace, int nPtSize, CColor color, std::string pText)
{
	CGraphics *pG;
	if ((pG = GetTextGraphics(fontFace, nPtSize, color, pText)) && pText.length())
	{
		int16_t w = pG->GetWidth();
		Blit(pt + CVectorI(0, m_fontDescent), *pG);
		delete pG;
		return w;
	}
	return pG == NULL ? -1 : 0;
}

void CGraphics::Flush()
{
	if (m_ss.eof()) return;

	string str;

	getline(m_ss, str);
	m_x += drawText(CVectorI(m_x, m_y), str);
	while (!m_ss.eof())
	{
		GotoLn();
		getline(m_ss, str);
		m_x += drawText(CVectorI(m_x, m_y), str);
	}
	m_ss.clear();
	m_ss.str("");
}

/////////////////////////////////////////////////////////
// Private Text Drawing Functions

int16_t CGraphics::drawText(CVectorI pt, string pText)
{
	ios::fmtflags fl = m_ss.flags() & ios::adjustfield;
	if (fl == ios::left)
		return drawTextLt(pt, pText);
	else if (fl == ios::right)
		return drawTextRt(pt, pText);
	else
		return drawTextCt(pt, pText);
}

int16_t CGraphics::drawTextLt(CVectorI pt, string pText)
{
	CGraphics *pG;
	if (pText.length() && (pG = GetTextGraphics(pText)))
	{
		int16_t w = pG->GetWidth();
		Blit(pt + CVectorI(0, m_fontDescent), *pG);
		delete pG;
		return w;
	}
	return 0;
}

int16_t CGraphics::drawTextCt(CVectorI pt, string pText)
{
	CGraphics *pG;
	if (pText.length() && (pG = GetTextGraphics(pText)))
	{
		int16_t w = pG->GetWidth() / 2;
		pt.X() -= w;
		Blit(pt, *pG);
		delete pG;
		return w;
	}
	return 0;
}

int16_t CGraphics::drawTextRt(CVectorI pt, string pText)
{
	CGraphics *pG;
	if (pText.length() && (pG = GetTextGraphics(pText)))
	{
		pt.X() -= pG->GetWidth();
		Blit(pt, *pG);
		delete pG;
	}
	return 0;
}



CGraphics &CGraphics::_DrawText()
{
	ios::fmtflags fl = m_ss.flags() & ios::adjustfield;
	if (fl == ios::left)
		Flush();
	else
	{
		// special flush - up to the most recent eol
		if (m_ss.eof()) return *this;

		string str = m_ss.str();
		if (str.find('\n') == string::npos) return *this;	// no eol

		getline(m_ss, str);
		m_x += drawText(CVectorI(m_x, m_y), str);
		while (!m_ss.eof())
		{
			GotoLn(); 
			getline(m_ss, str);
			
			if (str.size() && m_ss.eof())
			{
				m_ss.clear();
				auto _ = m_ss.str();
				m_ss << str;
				return *this;
			}
			else
				m_x += drawText(CVectorI(m_x, m_y), str);
		}
		m_ss.clear();
		m_ss.str("");
	}
	return *this;
}

CGraphics &CGraphics::_DrawText(ostream&(*f)(ostream&))
{
	m_ss << f;

	//if (f == endl || f == flush)
		Flush();
	return *this;
}

CGraphics &CGraphics::_DrawText(ios_base& (__cdecl *f)(ios_base&))
{
	// output f, monitoring adjustfield flags
	ios::fmtflags fl1 = m_ss.flags() & ios::adjustfield;
	m_ss << f;
	ios::fmtflags fl2 = m_ss.flags() & ios::adjustfield;
	
	// if adjustfield flags changed
	if (fl1 != fl2)
	{	// undo; flush; redo & init
		m_ss.setf(fl1, ios::adjustfield);
		Flush();
		m_ss.setf(fl2, ios::adjustfield);
		GotoCol();
	}
	return *this;
}

///////////////////////////////////////////////////////////
// NOT SUPPORTED CODE

///////////////////////////////////////////////////////////
//// Update Region Functions
//
//// Declared in the class definition:
//// list <SDL_Rect*> m_lstUpdateRects;		// a list of update rectangles
//
//void CGraphics::AddUpdateRect(CRectangle& UpdateRect) 
//{
//	SDL_Rect* pRect = NULL;
//	pRect = new SDL_Rect;
//	*pRect = UpdateRect;
//	m_lstUpdateRects.push_back(pRect);
//}
//
//void CGraphics::ClearUpdateRects() 
//{
//	list < SDL_Rect* >::iterator iter;
//	SDL_Rect* pRect;
//	while(!m_lstUpdateRects.empty())
//	{
//		iter = m_lstUpdateRects.begin();
//		pRect = *iter;
//		m_lstUpdateRects.erase(iter);
//		if (pRect) delete pRect;
//	}
//}
//
//void CGraphics::UpdateRects() 
//{
//	list <SDL_Rect*>::iterator iter;
//	for(iter = m_lstUpdateRects.begin(); iter != m_lstUpdateRects.end(); iter ++)
//		note: Y not yet inverted in this function!
//		SDL_UpdateRect(GetSurface(), (*iter)->x, (*iter)->y, (*iter)->w, (*iter)->h);
//	ClearUpdateRects();
//}
//
///////////////////////////////////////////////////////////
//// Clipping Rectangle
//
//void CGraphics::SetClipRect(CRectangle* pRect) 
//{
//	if (pRect)
//		SDL_SetClipRect(GetSurface(), pRect->YInv(GetHeight()));
//	else
//		SDL_SetClipRect(GetSurface(), NULL);
//}
//
//CRectangle CGraphics::GetClipRect() 
//{
//	SDL_Rect rect;
//	SDL_GetClipRect(GetSurface(), &rect);
//	rect.y = GetHeight() - rect.y - rect.h;
//	return (rect);
//}


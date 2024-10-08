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
#include "Sprite.h"

using namespace std;

/////////////////////////////////////////////////////////////////////
// Constructors and Destructors

CSprite::CSprite() : CSprite(0, 0, 0, 0, 0)
{
}
CSprite::CSprite(CVector v, float w, float h, uint32_t time)
{
	m_pos = v;
	m_pt1 = -CVector(w / 2, h / 2);
	m_pt2 = CVector(w / 2, h / 2);

	m_time = time;

	//Invalidate();
	//ResetColorKey();
	//OnPrepareGraphics();
}
CSprite::CSprite(CVector v, float w, float h, char *pFileBitmap, uint32_t time) : CSprite(v, w, h, time)
{
	LoadImage(pFileBitmap); 
	SetImage(pFileBitmap, false);
}
CSprite::CSprite(CVector v, float w, float h, char *pFileBitmap, CColor colorKey, uint32_t time) : CSprite(v, w, h, time)
{
	LoadImage(pFileBitmap, colorKey); 
	SetImage(pFileBitmap, false);
}
CSprite::CSprite(CVector v, char *pFileBitmap, uint32_t time) : CSprite(v, 0, 0, time)
{
	LoadImage(pFileBitmap); 
	SetImage(pFileBitmap);
}
CSprite::CSprite(CVector v, char *pFileBitmap, CColor colorKey, uint32_t time) : CSprite(v, 0, 0, time)
{
	LoadImage(pFileBitmap, colorKey); 
	SetImage(pFileBitmap);
}
CSprite::CSprite(CVector v, float w, float h, CGraphics *pGraphics, uint32_t time) : CSprite(v, w, h, time)
{
	SetImage(pGraphics, false);
}
CSprite::CSprite(CVector v, float w, float h, CGraphics *pGraphics, CColor colorKey, uint32_t time) : CSprite(v, w, h, time)
{
	SetImage(pGraphics, colorKey, false);
}
CSprite::CSprite(CVector v, CGraphics *pGraphics, uint32_t time) : CSprite(v, 0, 0, time)
{
	SetImage(pGraphics);
}
CSprite::CSprite(CVector v, CGraphics *pGraphics, CColor colorKey, uint32_t time) : CSprite(v, 0, 0, time)
{
	SetImage(pGraphics, colorKey);
}

CSprite::CSprite(float x, float y, float w, float h, uint32_t time) : CSprite(CVector(x, y), w, h, time)
{
}
CSprite::CSprite(float x, float y, float w, float h, char *pFileBitmap, uint32_t time) : CSprite(x, y, w, h, time)
{
	LoadImage(pFileBitmap); 
	SetImage(pFileBitmap, false);
}
CSprite::CSprite(float x, float y, float w, float h, char *pFileBitmap, CColor colorKey, uint32_t time) : CSprite(x, y, w, h, time)
{
	LoadImage(pFileBitmap, colorKey); 
	SetImage(pFileBitmap, false);
}
CSprite::CSprite(float x, float y, char *pFileBitmap, uint32_t time) : CSprite(x, y, 0, 0, time)
{
	LoadImage(pFileBitmap); 
	SetImage(pFileBitmap);
}
CSprite::CSprite(float x, float y, char *pFileBitmap, CColor colorKey, uint32_t time) : CSprite(x, y, 0, 0, time)
{
	LoadImage(pFileBitmap, colorKey); 
	SetImage(pFileBitmap);
}
CSprite::CSprite(float x, float y, float w, float h, CGraphics *pGraphics, uint32_t time) : CSprite(x, y, w, h, time)
{
	SetImage(pGraphics, false);
}
CSprite::CSprite(float x, float y, float w, float h, CGraphics *pGraphics, CColor colorKey, uint32_t time) : CSprite(x, y, w, h, time)
{
	SetImage(pGraphics, colorKey, false);
}
CSprite::CSprite(float x, float y, CGraphics *pGraphics, uint32_t time) : CSprite(x, y, 0, 0, time)
{
	SetImage(pGraphics);
}
CSprite::CSprite(float x, float y, CGraphics *pGraphics, CColor colorKey, uint32_t time) : CSprite(x, y, 0, 0, time)
{
	SetImage(pGraphics, colorKey);
}

CSprite::CSprite(CRectangle r, uint32_t time) : CSprite(r.GetCenterX(), r.GetCenterY(), r.w, r.h, time)
{
	//OnPrepareGraphics();
}

CSprite::CSprite(CRectangle r, char *pFileBitmap, uint32_t time) : CSprite(r.GetCenterX(), r.GetCenterY(), r.w, r.h, time)
{
	LoadImage(pFileBitmap); 
	SetImage(pFileBitmap, false);
}

CSprite::CSprite(CRectangle r, char *pFileBitmap, CColor colorKey, uint32_t time) : CSprite(r.GetCenterX(), r.GetCenterY(), r.w, r.h, time)
{
	LoadImage(pFileBitmap, colorKey); 
	SetImage(pFileBitmap, false);
}

CSprite::CSprite(CRectangle r, CGraphics *pGraphics, uint32_t time) : CSprite(r.GetCenterX(), r.GetCenterY(), r.w, r.h, time)
{
	SetImage(pGraphics, false);
}

CSprite::CSprite(CRectangle r, CGraphics *pGraphics, CColor colorKey, uint32_t time) : CSprite(r.GetCenterX(), r.GetCenterY(), r.w, r.h, time)
{
	SetImage(pGraphics, colorKey, false);
}

CSprite::~CSprite(void)
{
	ClearImage();
	for (map<string, PROPERTY>::iterator i = m_properties.begin(); i != m_properties.end(); i++)
		(*i).second.Erase();
	if (m_pFrames) delete [] m_pFrames;
}

//////////////////////////////////////////
// Cloning

CSprite *CSprite::Clone()
{
	CSprite *p = new CSprite(GetPosition(), GetWidth(), GetHeight(), 0);

	p->m_time = m_time;
	if (p->m_pGraphics) delete p->m_pGraphics;
	p->m_pGraphics = new CGraphics(m_pGraphics);
	if (p->m_pRoto) delete p->m_pRoto;
	p->m_pRoto = NULL;
	p->m_state = m_state;
	p->m_health = m_health;
	p->m_v = m_v;
	p->m_fv = m_fv;
	p->m_rot = m_rot;
	p->m_omega = m_omega;
	p->m_sinrot = m_sinrot;
	p->m_cosrot = m_cosrot;
	p->m_m = m_m;
	p->m_bDeleted = m_bDeleted;
	p->m_timeDeath = m_timeDeath;

	for (map<string, PROPERTY>::iterator i = m_properties.begin(); i != m_properties.end(); i++)
	{
		string label = (*i).first;
		PROPERTY prop = (*i).second;
		prop.CopyFrom((*i).second);
		p->m_properties[label] = prop;
	}

	p->m_nFrames = m_nFrames;
	if (m_nFrames)
	{
		p->m_pFrames = new CGraphics*[m_nFrames];
		for (int i = 0; i < m_nFrames; i++)
			p->m_pFrames[i] = new CGraphics(m_pFrames[i]);
	}
	p->m_nFramePeriod = m_nFramePeriod;
	p->m_nFrameTime = m_nFrameTime;
	p->m_bFrameChangeSize = m_bFrameChangeSize;

	p->Invalidate();

	return p;
}

/////////////////////////////////////////////////////////////////////
// Set Size

void CSprite::SetSize(CVector v)
{
	CVector pivot = GetPivotRel();
	SetBottomLeftLocal(-v * pivot);
	SetTopRightLocal(v * (CVector(1, 1) - pivot));

	if (m_pRoto) delete m_pRoto; 
	m_pRoto = NULL; 
}

/////////////////////////////////////////////////////////////////////
// Coordinates Converters
		
// Converts p from global coordinates to the sprite local coordinates. 
// Local coordinates are centred at the centre of the sprites and axes are rotated as the sprite
void CSprite::GtoL(CVector &p, bool bUseRot)
{
	float x = p.GetX() - GetX();
	float y = p.GetY() - GetY();
	if (!bUseRot || GetRotation() == 0)
		p = CVector(x, y);
	else
		p = CVector((int16_t)(x * m_cosrot + y * m_sinrot), (int16_t)(-x * m_sinrot + y * m_cosrot));
}

// Converts p from sprite local coordinates to the glovbal coordinates. 
// Local coordinates are centred at the centre of the sprites and axes are rotated as the sprite
void CSprite::LtoG(CVector &p, bool bUseRot)
{
	if (!bUseRot || GetRotation() == 0)
		p = CVector(p.GetX() + GetX(), p.GetY()+ GetY());
	else
		p = CVector( (p.GetX() * m_cosrot - p.GetY() * m_sinrot) + GetX(),
					 (p.GetX() * m_sinrot + p.GetY() * m_cosrot) + GetY());
}
	
//////////////////////////////////////////
// Sprite Rectangles

void CSprite::GetBoundingRect(CRectangle &rect)
{
	CVector tl(m_pt1.m_x, m_pt1.m_y);
	CVector bl(m_pt1.m_x, m_pt2.m_y);
	CVector tr(m_pt2.m_x, m_pt1.m_y);
	CVector br(m_pt2.m_x, m_pt2.m_y);
	LtoG(tl); LtoG(bl); LtoG(tr); LtoG(br);
	//float l = min(min(tl.GetX(), bl.GetX()), min(tr.GetX(), br.GetX())); 
	//float t = min(min(tl.GetY(), bl.GetY()), min(tr.GetY(), br.GetY())); 
	//float r = max(max(tl.GetX(), bl.GetX()), max(tr.GetX(), br.GetX())); 
	//float b = max(max(tl.GetY(), bl.GetY()), max(tr.GetY(), br.GetY()));
	//rect.Set((int16_t)(l + 0.5f), (int16_t)(t + 0.5f), (int16_t)(r - l + 0.5f), (int16_t)(b - t + 0.5f));
	int16_t l = (int16_t)floor(min(min(tl.GetX(), bl.GetX()), min(tr.GetX(), br.GetX())));
	int16_t t = (int16_t)floor(min(min(tl.GetY(), bl.GetY()), min(tr.GetY(), br.GetY())));
	int16_t r = (int16_t)floor(max(max(tl.GetX(), bl.GetX()), max(tr.GetX(), br.GetX())));
	int16_t b = (int16_t)floor(max(max(tl.GetY(), bl.GetY()), max(tr.GetY(), br.GetY())));
	rect.Set(l, t, r - l, b - t);
}

/////////////////////////////////////////////////////////////////////
// Pivot Point Functions
		
void CSprite::SetPivotLocal(CVector v)
{
	m_pt1 -= v;
	m_pt2 -= v;
	LtoG(v);
	m_pos = v;
}

void CSprite::GetPivotRel(float &x, float &y)
{
	if (GetRightLocal() == GetLeftLocal())
		x = 0.5;
	else
		x = -(float)GetLeftLocal() / (float)(GetRightLocal() - GetLeftLocal());
	if (GetBottomLocal() == GetTopLocal())
		y = 0.5;
	else
		y = -(float)GetBottomLocal() / (float)(GetTopLocal() - GetBottomLocal());
}

/////////////////////////////////////////////////////////////////////
// Image Functions

void CSprite::ClearImage()
{
	if (m_pFrames)
	{
		delete [] m_pFrames;
		m_pFrames = NULL;
		m_nFrames = 0;
	}
	else
		if (m_pGraphics) delete m_pGraphics;
	m_pGraphics = NULL;
	if (m_pRoto) delete m_pRoto; 
	m_pRoto = NULL; 
}

void CSprite::LoadAnimation(CGraphics *p, char *pAliasName, SHEET grid)
{
	auto _from = grid.m_from;
	auto _rowcol = _from.m_rowcol;
	auto _grid = _rowcol.m_grid;
	if (_rowcol.m_bHorizontally)
		AddImage(p, pAliasName, _grid.m_numCols, _grid.m_numRows, _from.iFrom, _rowcol.m_iRowCol, grid.iTo, _rowcol.m_iRowCol, _rowcol.m_bHorizontally);
	else
		AddImage(p, pAliasName, _grid.m_numCols, _grid.m_numRows, _rowcol.m_iRowCol, _from.iFrom, _rowcol.m_iRowCol, grid.iTo, _rowcol.m_bHorizontally);
}

void CSprite::LoadAnimation(char *pFileName, char *pAliasName, SHEET grid)
{
	auto _from = grid.m_from;
	auto _rowcol = _from.m_rowcol;
	auto _grid = _rowcol.m_grid;
	if (_rowcol.m_bHorizontally)
		AddImage(pFileName, pAliasName, _grid.m_numCols, _grid.m_numRows, _from.iFrom, _rowcol.m_iRowCol, grid.iTo, _rowcol.m_iRowCol, _rowcol.m_bHorizontally);
	else
		AddImage(pFileName, pAliasName, _grid.m_numCols, _grid.m_numRows, _rowcol.m_iRowCol, _from.iFrom, _rowcol.m_iRowCol, grid.iTo, _rowcol.m_bHorizontally);
}

void CSprite::LoadAnimation(CGraphics *p, char *pAliasName, SHEET grid, CColor colorKey)
{
	auto _from = grid.m_from;
	auto _rowcol = _from.m_rowcol;
	auto _grid = _rowcol.m_grid;
	if (_rowcol.m_bHorizontally)
		AddImage(p, pAliasName, _grid.m_numCols, _grid.m_numRows, _from.iFrom, _rowcol.m_iRowCol, grid.iTo, _rowcol.m_iRowCol, colorKey, _rowcol.m_bHorizontally);
	else
		AddImage(p, pAliasName, _grid.m_numCols, _grid.m_numRows, _rowcol.m_iRowCol, _from.iFrom, _rowcol.m_iRowCol, grid.iTo, colorKey, _rowcol.m_bHorizontally);
}

void CSprite::LoadAnimation(char *pFileName, char *pAliasName, SHEET grid, CColor colorKey)
{
	auto _from = grid.m_from;
	auto _rowcol = _from.m_rowcol;
	auto _grid = _rowcol.m_grid;
	if (_rowcol.m_bHorizontally)
		AddImage(pFileName, pAliasName, _grid.m_numCols, _grid.m_numRows, _from.iFrom, _rowcol.m_iRowCol, grid.iTo, _rowcol.m_iRowCol, colorKey, _rowcol.m_bHorizontally);
	else
		AddImage(pFileName, pAliasName, _grid.m_numCols, _grid.m_numRows, _rowcol.m_iRowCol, _from.iFrom, _rowcol.m_iRowCol, grid.iTo, colorKey, _rowcol.m_bHorizontally);
}

void CSprite::AddImage(CGraphics *p, char *pAliasName, short numCols, short numRows, short iColFrom, short iRowFrom, short iColTo, short iRowTo, bool bHorizontally)
{
	if (iColFrom < 0) iColFrom = 0;
	if (iRowFrom < 0) iRowFrom = 0;
	if (iColTo < 0) iColTo = numCols - 1;
	if (iRowTo < 0) iRowTo = numRows - 1;

	if (iColTo >= numCols) iColTo = numCols - 1;
	if (iColFrom >= numCols) iColFrom = iColTo;
	if (iRowTo >= numRows) iRowTo = numRows - 1;
	if (iRowFrom >= numRows) iRowFrom = iRowTo;

	int nRows = abs(iRowTo - iRowFrom) + 1;
	int nCols = abs(iColTo - iColFrom) + 1;
	int iRowStep = iRowTo >= iRowFrom ? 1 : -1;
	int iColStep = iColTo >= iColFrom ? 1 : -1;

	int iRow, iCol, i, j;
	if (bHorizontally)
		for (i = 0, iRow = iRowFrom; i < nRows; i++, iRow += iRowStep)
			for (j = 0, iCol = iColFrom; j < nCols; j++, iCol += iColStep)
				AddProperty(pAliasName, &CGraphics(p, numCols, numRows, iCol, iRow)); 
	else
		for (j = 0, iCol = iColFrom; j < nCols; j++, iCol += iColStep)
			for (i = 0, iRow = iRowFrom; i < nRows; i++, iRow += iRowStep)
				AddProperty(pAliasName, &CGraphics(p, numCols, numRows, iCol, iRow)); 
}

void CSprite::AddImage(char *pFileName, char *pAliasName, short numCols, short numRows, short iColFrom, short iRowFrom, short iColTo, short iRowTo, bool bHorizontally)
{ 
	if (iColFrom < 0) iColFrom = 0;
	if (iRowFrom < 0) iRowFrom = 0;
	if (iColTo < 0) iColTo = numCols - 1;
	if (iRowTo < 0) iRowTo = numRows - 1;

	if (iColTo >= numCols) iColTo = numCols - 1;
	if (iColFrom >= numCols) iColFrom = iColTo;
	if (iRowTo >= numRows) iRowTo = numRows - 1;
	if (iRowFrom >= numRows) iRowFrom = iRowTo;

	int nRows = abs(iRowTo - iRowFrom) + 1;
	int nCols = abs(iColTo - iColFrom) + 1;
	int iRowStep = iRowTo >= iRowFrom ? 1 : -1;
	int iColStep = iColTo >= iColFrom ? 1 : -1;

	int iRow, iCol, i, j;
	if (bHorizontally)
		for (i = 0, iRow = iRowFrom; i < nRows; i++, iRow += iRowStep)
			for (j = 0, iCol = iColFrom; j < nCols; j++, iCol += iColStep)
				AddProperty(pAliasName, &CGraphics(pFileName, numCols, numRows, iCol, iRow)); 
	else
		for (j = 0, iCol = iColFrom; j < nCols; j++, iCol += iColStep)
			for (i = 0, iRow = iRowFrom; i < nRows; i++, iRow += iRowStep)
				AddProperty(pAliasName, &CGraphics(pFileName, numCols, numRows, iCol, iRow)); 
}

void CSprite::AddImage(CGraphics *p, char *pAliasName, short numCols, short numRows, short iColFrom, short iRowFrom, short iColTo, short iRowTo, CColor colorKey, bool bHorizontally)	
{ 
	if (iColFrom < 0) iColFrom = 0;
	if (iRowFrom < 0) iRowFrom = 0;
	if (iColTo < 0) iColTo = numCols - 1;
	if (iRowTo < 0) iRowTo = numRows - 1;

	if (iColTo >= numCols) iColTo = numCols - 1;
	if (iColFrom >= numCols) iColFrom = iColTo;
	if (iRowTo >= numRows) iRowTo = numRows - 1;
	if (iRowFrom >= numRows) iRowFrom = iRowTo;

	int nRows = abs(iRowTo - iRowFrom) + 1;
	int nCols = abs(iColTo - iColFrom) + 1;
	int iRowStep = iRowTo >= iRowFrom ? 1 : -1;
	int iColStep = iColTo >= iColFrom ? 1 : -1;

	int iRow, iCol, i, j;
	if (bHorizontally)
		for (i = 0, iRow = iRowFrom; i < nRows; i++, iRow += iRowStep)
			for (j = 0, iCol = iColFrom; j < nCols; j++, iCol += iColStep)
				AddProperty(pAliasName, &CGraphics(p, numCols, numRows, iCol, iRow, colorKey)); 
	else
		for (j = 0, iCol = iColFrom; j < nCols; j++, iCol += iColStep)
			for (i = 0, iRow = iRowFrom; i < nRows; i++, iRow += iRowStep)
				AddProperty(pAliasName, &CGraphics(p, numCols, numRows, iCol, iRow, colorKey)); 
}

void CSprite::AddImage(char *pFileName, char *pAliasName, short numCols, short numRows, short iColFrom, short iRowFrom, short iColTo, short iRowTo, CColor colorKey, bool bHorizontally)	
{ 
	if (iColFrom < 0) iColFrom = 0;
	if (iRowFrom < 0) iRowFrom = 0;
	if (iColTo < 0) iColTo = numCols - 1;
	if (iRowTo < 0) iRowTo = numRows - 1;

	if (iColTo >= numCols) iColTo = numCols - 1;
	if (iColFrom >= numCols) iColFrom = iColTo;
	if (iRowTo >= numRows) iRowTo = numRows - 1;
	if (iRowFrom >= numRows) iRowFrom = iRowTo;

	int nRows = abs(iRowTo - iRowFrom) + 1;
	int nCols = abs(iColTo - iColFrom) + 1;
	int iRowStep = iRowTo >= iRowFrom ? 1 : -1;
	int iColStep = iColTo >= iColFrom ? 1 : -1;

	int iRow, iCol, i, j;
	if (bHorizontally)
		for (i = 0, iRow = iRowFrom; i < nRows; i++, iRow += iRowStep)
			for (j = 0, iCol = iColFrom; j < nCols; j++, iCol += iColStep)
				AddProperty(pAliasName, &CGraphics(pFileName, numCols, numRows, iCol, iRow, colorKey)); 
	else
		for (j = 0, iCol = iColFrom; j < nCols; j++, iCol += iColStep)
			for (i = 0, iRow = iRowFrom; i < nRows; i++, iRow += iRowStep)
				AddProperty(pAliasName, &CGraphics(pFileName, numCols, numRows, iCol, iRow, colorKey)); 
}

void CSprite::SetImage(CGraphics *p, bool bChangeSize)
{
	ClearImage();
	m_pGraphics = new CGraphics(p);
	if (bChangeSize) SetSize((float)m_pGraphics->GetWidth(), (float)m_pGraphics->GetHeight());
}

void CSprite::SetImage(CGraphics* p, CColor colorKey, bool bChangeSize)
{
	ClearImage();
	m_pGraphics = new CGraphics(p, colorKey);
	if (bChangeSize) SetSize((float)m_pGraphics->GetWidth(), (float)m_pGraphics->GetHeight());
}

void CSprite::SetAnimation(char *pPropName, int fps, int nIndexStart, int numFrames)
{
	static CGraphics *pMissing = NULL;
	ClearImage();
	m_strAnimName = pPropName;
	int nmax = GetPropertyIndexCount(pPropName) - nIndexStart;
	m_nFrames = (m_nFrames > 0) ? min(numFrames, nmax) : nmax;
	if (m_nFrames)
	{
		m_pFrames = new CGraphics*[m_nFrames];
		for (int i = 0; i < m_nFrames; i++)
			m_pFrames[i] = GetProperty(pPropName, i + nIndexStart);
	}
	else
	{
		// animation not found!
		m_nFrames = 1;
		m_pFrames = new CGraphics*[1];
		if (!pMissing) pMissing = new CGraphics((CGraphics*)NULL);
		m_pFrames[0] = pMissing;

	}
	m_nCurFrame = 0;
	m_nFramePeriod = 1000 / fps;
	m_nFrameTime = -1;
	m_bFrameChangeSize = true;
}

void CSprite::SetAnimationKeepSize(char *pPropName, int fps, int nIndexStart, int numFrames)
{
	ClearImage();
	m_strAnimName = pPropName;
	int nmax = GetPropertyIndexCount(pPropName);
	m_nFrames = (m_nFrames > 0) ? min(numFrames, nmax) : nmax;
	m_pFrames = new CGraphics*[m_nFrames];
	for (int i = 0; i < m_nFrames; i++)
		m_pFrames[i] = GetProperty(pPropName, i+nIndexStart);
	m_nFramePeriod = 1000 / fps;
	m_nFrameTime = -1;
	m_bFrameChangeSize = false;
}

bool CSprite::IsAnimationPlaying()
{
	return m_pFrames && m_nFrames;
}

bool CSprite::IsAnimationPlaying(char* pPropName)
{
	if (!IsAnimationPlaying()) return false;
	return string(pPropName) == m_strAnimName;
}

const char* CSprite::GetCurrentAnimation()
{
	if (!IsAnimationPlaying()) return NULL;
	return m_strAnimName.c_str();
}

int CSprite::GetCurrentAnimationFrame()
{
	if (!IsAnimationPlaying()) return -1;
	return m_nCurFrame;
}


// deprecated
void CSprite::SetAnimation(char *pPropName, int nIndexStart, int numFrames, int nPeriod, long nTime, bool bChangeSize)
{
	ClearImage();
	m_strAnimName = pPropName;
	m_nFrames = numFrames;
	m_pFrames = new CGraphics*[m_nFrames];
	for (int i = 0; i < m_nFrames; i++)
		m_pFrames[i] = GetProperty(pPropName, i+nIndexStart);

	m_nFramePeriod = nPeriod;
	m_nFrameTime = nTime;
	m_bFrameChangeSize = bChangeSize;
}

/////////////////////////////////////////////////////////////////////
// Tests

// Checks if the given point pt hits within the sprite bounding rectangle
bool CSprite::HitTest(CVector pt)
{
	GtoL(pt);
	return pt.GetX() >= GetLeftLocal() && pt.GetX() <= GetRightLocal()
		&& pt.GetY() >= GetBottomLocal() && pt.GetY() <= GetTopLocal();
}
	
// Checks if the given circular area with centre at pt and the radius hits within the sprite bounding rectangle
bool CSprite::HitTest(CVector pt, float radius)
{
	GtoL(pt);
	return pt.GetX() >= GetLeftLocal() - radius && pt.GetX() <= GetRightLocal() + radius
		&& pt.GetY() >= GetBottomLocal() - radius && pt.GetY() <= GetTopLocal() + radius;
}
	
// Tests against the collision with the region defined by rect
bool CSprite::HitTest(CRectangle rect)
{ 
	CRectangle myRect;
	GetBoundingRect(myRect);
	return myRect.Intersects(rect); 
}
	
bool CSprite::HitTest(CSprite *pSprite, int nSkip)
{
	// Firstly, test on the bounding box level...

	// p is upper left corner of S
	CVector p, pmin, pmax;
	
	// find min x, y and max x, y within corners of S
	p = CVector(pSprite->GetLeftLocal(), pSprite->GetBottomLocal());
	pSprite->LtoG(p);
	GtoL(p);
	pmin.X() = p.X();
	pmin.Y() = p.Y();
	pmax.X() = p.X();
	pmax.Y() = p.Y();
	p = CVector(pSprite->GetLeftLocal(), pSprite->GetTopLocal());
	pSprite->LtoG(p);
	GtoL(p);
	pmin.X() = min(pmin.X(), p.X());
	pmin.Y() = min(pmin.Y(), p.Y());
	pmax.X() = max(pmax.X(), p.X());
	pmax.Y() = max(pmax.Y(), p.Y());
	p = CVector(pSprite->GetRightLocal(), pSprite->GetBottomLocal());
	pSprite->LtoG(p);
	GtoL(p);
	pmin.X() = min(pmin.X(), p.X());
	pmin.Y() = min(pmin.Y(), p.Y());
	pmax.X() = max(pmax.X(), p.X());
	pmax.Y() = max(pmax.Y(), p.Y());
	p = CVector(pSprite->GetRightLocal(), pSprite->GetTopLocal());
	pSprite->LtoG(p);
	GtoL(p);
	pmin.X() = min(pmin.X(), p.X());
	pmin.Y() = min(pmin.Y(), p.Y());
	pmax.X() = max(pmax.X(), p.X());
	pmax.Y() = max(pmax.Y(), p.Y());

	if (pmax.X() < GetLeftLocal()) return false;	// entire S is on left of me
	if (pmin.X() > GetRightLocal()) return false;	// entire S is on right of me
	if (pmax.Y() < GetBottomLocal()) return false;	// entire S is above me
	if (pmin.Y() > GetTopLocal()) return false;	// entire S is below me

	// find min x, y and max x, y within corners of me (in local coordinates of S)
	p = CVector(GetLeftLocal(), GetBottomLocal());
	LtoG(p);
	pSprite->GtoL(p);
	pmin.X() = p.X();
	pmin.Y() = p.Y();
	pmax.X() = p.X();
	pmax.Y() = p.Y();
	p = CVector(GetLeftLocal(), GetTopLocal());
	LtoG(p);
	pSprite->GtoL(p);
	pmin.X() = min(pmin.X(), p.X());
	pmin.Y() = min(pmin.Y(), p.Y());
	pmax.X() = max(pmax.X(), p.X());
	pmax.Y() = max(pmax.Y(), p.Y());
	p = CVector(GetRightLocal(), GetBottomLocal());
	LtoG(p);
	pSprite->GtoL(p);
	pmin.X() = min(pmin.X(), p.X());
	pmin.Y() = min(pmin.Y(), p.Y());
	pmax.X() = max(pmax.X(), p.X());
	pmax.Y() = max(pmax.Y(), p.Y());
	p = CVector(GetRightLocal(), GetTopLocal());
	LtoG(p);
	pSprite->GtoL(p);
	pmin.X() = min(pmin.X(), p.X());
	pmin.Y() = min(pmin.Y(), p.Y());
	pmax.X() = max(pmax.X(), p.X());
	pmax.Y() = max(pmax.Y(), p.Y());

	if (pmax.X() < pSprite->GetLeftLocal()) return false;	// entire S is on left of me
	if (pmin.X() > pSprite->GetRightLocal()) return false;	// entire S is on right of me
	if (pmax.Y() < pSprite->GetBottomLocal()) return false;	// entire S is above me
	if (pmin.Y() > pSprite->GetTopLocal()) return false;	// entire S is below me

	// There is a collision --- check for the pixel level now

	if (nSkip <= 0)
		return true;	// no pixel-level test...

	CRectangle myRect, hisRect;
	GetBoundingRect(myRect);
	pSprite->GetBoundingRect(hisRect);

	// prepare the roto graphics
	if (m_pRoto && !m_pRoto->HasPixels())
	{
		delete m_pRoto;
		m_pRoto = NULL;
	}
	if (pSprite->m_pRoto && !pSprite->m_pRoto->HasPixels())
	{
		delete pSprite->m_pRoto;
		pSprite->m_pRoto = NULL;
	}

	VoidDraw();
	pSprite->VoidDraw();

	CGraphics *pMySurface = GetRotoGraphics();
	CGraphics *pHisSurface = pSprite->GetRotoGraphics();
	if (!pMySurface || !pHisSurface)
		return true;

	return pMySurface->HitTest(myRect.x, 4192 - myRect.y - myRect.h, 
				   pHisSurface, hisRect.x, 4192 - hisRect.y - hisRect.h, nSkip);
}

//////////////////////////////////////////
// Properties

void CSprite::SetProperty(char *pLabel, PROPERTY prop)
{
	map<string, PROPERTY>::iterator i = m_properties.find(pLabel);
	if (i != m_properties.end())
		(*i).second.Erase();
	m_properties[pLabel] = prop;
}

PROPERTY CSprite::GetProperty(char *pLabel)
{
	return m_properties[pLabel];
}

void CSprite::SetProperty(char *pLabel, unsigned nIndex, PROPERTY prop)
{
	if (nIndex < m_properties[pLabel].m_indexedProperties.size())
		m_properties[pLabel].m_indexedProperties[nIndex].Erase();
	else
		m_properties[pLabel].m_indexedProperties.resize(nIndex + 1);
	
	m_properties[pLabel].m_indexedProperties[nIndex] = prop;
}

void CSprite::AddProperty(char *pLabel, PROPERTY prop)
{
	m_properties[pLabel].m_indexedProperties.push_back(prop);
}

PROPERTY CSprite::GetProperty(char *pLabel, unsigned nIndex)
{
	map<string, PROPERTY>::iterator i = m_properties.find(pLabel);
	if (i != m_properties.end() && nIndex < (*i).second.m_indexedProperties.size())
		return m_properties[pLabel].m_indexedProperties[nIndex];
	else
		return PROPERTY();
}

unsigned CSprite::GetPropertyIndexCount(char *pLabel)
{
	return (unsigned)m_properties[pLabel].m_indexedProperties.size();
}

//////////////////////////////////////////
// Rotation & Automatic Rotational Motion

void CSprite::SetRotation(float newRot)				
{
	if (newRot == m_rot) return;

	m_rot = DEG2RAD(newRot);
	m_sinrot = sin(-m_rot);
	m_cosrot = cos(-m_rot);
	
	if (m_pRoto) delete m_pRoto; 
	m_pRoto = NULL; 
}

//////////////////////////////////////////
// Update & Draw

void CSprite::Update(uint32_t nGameTime)
{
	int32_t _dt = nGameTime - m_time;
	m_time = nGameTime;

	if (_dt < 0) _dt = 0;

	if (IsDead())
		Delete();

	if (!IsDeleted())
		OnUpdate(nGameTime, _dt);
}

void CSprite::Draw(CGraphics *pG)
{
	if (IsDeleted()) return;

	OnPrepareGraphics(pG);
	if (!m_pGraphics)
		return;

	OnDraw(m_pGraphics);

	if (!OnPrepareRotoGraphics(pG))
	{
		CRectangle rect;
		GetNoRotBoundingRect(rect);
		pG->Blit(rect, *m_pGraphics);
	}
	else
	{
		if (!m_pRoto) return;
	
		CRectangle rect;
		GetBoundingRect(rect);
		pG->Blit(rect, *m_pRoto);
	}
	Validate();		// avoid unnecessary redraws
}

// Default update for the sprite.
// uses velocity and direction to change the position
// and rotational velocity to change the rotation
// Also proceeds the animation in case of animated sprites
// In the derived classes you may want to override this function to support
// other types of time-related updates.
// The t param is the current system time (in miliseconds)

void CSprite::OnUpdate(uint32_t time, uint32_t deltaTime)
{
	ProceedVelocity(deltaTime);
	ProceedOmega(deltaTime);

	// update animated image
	if (m_pFrames && m_nFrames)
	{
		if (m_nFrameTime < 0) m_nFrameTime = time;
		m_nCurFrame = ((time - m_nFrameTime) / m_nFramePeriod) % m_nFrames;
		m_pGraphics = m_pFrames[m_nCurFrame];
		if (m_bFrameChangeSize) SetSize((float)m_pGraphics->GetWidth(), (float)m_pGraphics->GetHeight());
	}
}

void CSprite::VoidDraw()
{
	OnPrepareGraphics();
	if (m_pGraphics)
		OnDraw(m_pGraphics);
	OnPrepareRotoGraphics();
}

void CSprite::OnPrepareGraphics(CGraphics*)
{
	if (m_pGraphics && !GetWidth() || !GetHeight())
		SetSize((float)m_pGraphics->GetWidth(), (float)m_pGraphics->GetHeight());

	if (m_pGraphics) return;

	m_pGraphics = new CGraphics((int)GetWidth(), (int)GetHeight(), 32, 0xff0000, 0xff00, 0xff, 0);
}

bool CSprite::OnPrepareRotoGraphics(CGraphics*)
{
	if (m_pGraphics->GetWidth() == 0 || m_pGraphics->GetHeight() == 0) return false;
	float zoomx = (float)GetWidth() / (float)m_pGraphics->GetWidth();
	float zoomy = (float)GetHeight() / (float)m_pGraphics->GetHeight();

	// exit currently suspended
	// if (m_pGraphics->GetSurface()->format->BitsPerPixel == 8 && !m_pGraphics->IsColorKeySet() && zoomx == 1.0 && zoomy == 1.0 && GetRotation() == 0.0f) return false;

	if (!m_pRoto)
		m_pRoto = m_pGraphics->CreateRotozoom(-GetRotation(), zoomx, zoomy);


	return true;
}

void CSprite::OnDraw(CGraphics *g)
{
}


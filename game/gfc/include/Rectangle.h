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
#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

class EXT_DECL CRectangle
{
public:
	int16_t x, y;
	int16_t w, h;

public:
	// Constructors
	CRectangle(int16_t x, int16_t y, int16_t w, int16_t h)	{ Set(x, y, w, h); }
	CRectangle(const CRectangle& rc)			{ Set(rc.x, rc.y, rc.w, rc.h); }
	CRectangle()								{ Set(0, 0, 0, 0); }

	// Destructor
	~CRectangle()	{ }

	// Geometry
	int16_t Left() const						{ return x; }
	int16_t Bottom() const						{ return y; }
	int16_t Right() const						{ return x + w; }
	int16_t Top() const							{ return y + h; }
	int16_t GetCenterX() const					{ return x + w / 2; }
	int16_t GetCenterY() const					{ return y + h / 2; }

	// The following four functions guarantee that w and h (width and height) will always be positive
	// Sets the rectangle from the coordinates of the lower-left corner and size; negative width or height turns the rectangle inside out
	CRectangle& Set(int16_t _x, int16_t _y, int16_t _w, int16_t _h)
												{ if (_w < 0) { _w = -_w; _x -= _w; } 
												  if (_h < 0) { _h = -_h; _y -= _h; } 
												  x = _x; y = _y; w = _w; h = _h; return(*this); }
	// Sets the rectangle from the coordinates of the lower-left corner and size; negative width or height collapse to zero
	CRectangle& SetColl(int16_t _x, int16_t _y, int16_t _w, int16_t _h)
												{ x = _x; y = _y; w = max(_w, 0); h = max(_h, 0); return(*this); }
	// Sets the rectangle from the coordinates of a pair of tops; negative width or height turns the rectangle inside out
	CRectangle& SetTops(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
												{ return Set(x0, y0, x1 - x0, y1 - y0); }
	// Sets the rectangle from the coordinates of a pair of tops; negative width or height collapse to zero
	CRectangle& SetTopsColl(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
												{ if (x1 < x0) x0 = x1 = (x0 + x1) / 2;
												  if (y1 < y0) y0 = y1 = (y0 + y1) / 2;
												  return Set(x0, y0, x1 - x0, y1 - y0); }

	// Conversion Operators
	operator CVector() const					{ return CVector(x, y); }

	// Y-Invert function
	CRectangle &YInv(int16_t H)					{ y = H - y - h; return *this; }
//	friend CRectangle YInv(const CRectangle &r, int16_t H)
//												{ CRectangle rect = r; r.YInv(H); return rect; }

	// Empty - setting & testing
	CRectangle& SetEmpty()						{ return Set(0, 0, 0, 0); }
	bool IsEmpty() const						{ return (w == 0 || h == 0); }

	// Offset and Moving
	CRectangle& Offset(int16_t dx, int16_t dy)	{ x += dx; y += dy; return(*this); }
	CRectangle& Offset(const CVector& pt)		{ x += (int16_t)pt.m_x; y += (int16_t)pt.m_y; return(*this); }
	CRectangle& MoveTo(int16_t _x, int16_t _y)	{ x = _x; y = _y; return(*this); }
	CRectangle& MoveTo(const CVector& pt)		{ x = (int16_t)pt.m_x; y = (int16_t)pt.m_y; return(*this); }

	// Change in size
	CRectangle& Grow(int16_t l, int16_t r, int16_t t, int16_t b)	{ Set(x - l, y - b, w + l + r, h + t + b); return(*this); }
	CRectangle& Grow(int16_t hor, int16_t ver)	{ return Grow(hor, hor, ver, ver); }
	CRectangle& Grow(int16_t amount)				{ return Grow(amount, amount, amount, amount); }

	// Union & Intersection
	CRectangle& Union(const CRectangle& rc)			{ return SetTops  (min(Left(), rc.Left()), min(Bottom(), rc.Bottom()), max(Right(), rc.Right()), max(Top(), rc.Top())); }
	CRectangle& Intersection(const CRectangle& rc)	{ return SetTopsColl(max(Left(), rc.Left()), max(Bottom(), rc.Bottom()), min(Right(), rc.Right()), min(Top(), rc.Top())); }
	CRectangle Union(const CRectangle& rc1, const CRectangle& rc2)			{ CRectangle rc(rc1); rc.Union(rc2); return rc; }
	CRectangle Intersection(const CRectangle& rc1, const CRectangle& rc2)	{ CRectangle rc(rc1); rc.Intersection(rc2); return rc; }

	// Conatinment & Intersection Tests
	bool Contains(int16_t x, int16_t y) const	{ return x >= Left() && x <= Right() && y >= Bottom() && y <= Top(); }
	bool Contains(const CVector& pt) const		{ return pt.m_x >= Left() && pt.m_x <= Right() && pt.m_y >= Bottom() && pt.m_y <= Top(); }

	bool Intersects(const CRectangle &rc) const	{ return min(Right(), rc.Right()) > max(Left(), rc.Left()) && min(Top(), rc.Top()) > max(Bottom(), rc.Bottom()); }

	// Operators
	
	// assignment
	CRectangle& operator +=(const CVector& pt)	{ return Offset(pt); }
	CRectangle& operator -=(const CVector& pt)	{ return Offset(-pt); }

	CRectangle& operator +=(CRectangle& rc)		{ return Union(rc); }
	CRectangle& operator *=(CRectangle& rc)		{ return Intersection(rc); }

	// binary
	CRectangle operator +(const CVector& pt) const		{ CRectangle result =(*this); return result += pt; }
	CRectangle operator -(const CVector& pt) const		{ CRectangle result =(*this); return result -= pt; }
	CRectangle operator +(const CRectangle& rc) const	{ CRectangle result =(*this); return result.Union(rc); }
	CRectangle operator *(const CRectangle& rc) const	{ CRectangle result =(*this); return result.Intersection(rc); }


	// comparisons
	bool operator ==(const CRectangle rc)				{ return x == rc.x && y == rc.y && w == rc.w && h == rc.h; }
	bool operator !=(const CRectangle rc)				{ return x != rc.x || y != rc.y || w != rc.w || h != rc.h; }
};

#endif

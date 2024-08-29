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


#ifndef __COLOR_H__
#define __COLOR_H__

class EXT_DECL CColor  
{
public:
	//actual color representation
	uint8_t r, g, b, a;
public:
	// Constructors
	CColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255)	{ Set(r, g, b, a); }
	CColor(const CColor &Color)			{ r = Color.GetR(); g = Color.GetG(); b = Color.GetB(); a = Color.GetA(); }
	CColor()							{ Set(0, 0, 0, 255); }

	// Destructor
	virtual ~CColor()					{ }

	// Access R, G, B
	uint8_t GetR() const				{ return r; }
	uint8_t GetG() const				{ return g; }
	uint8_t GetB() const				{ return b; }
	uint8_t GetA() const				{ return a; }

	void SetR(uint8_t r)				{ this->r = r; }
	void SetG(uint8_t g)				{ this->g = g; }
	void SetB(uint8_t b)				{ this->b = b; }
	void SetA(uint8_t a)				{ this->a = a; }
	
	void Set(uint8_t r, uint8_t g, uint8_t b, uint8_t a=0)	{ this->r = r; this->g = g; this->b = b; this->a = a; }
	void Set(CColor &color)				{ r = color.R(); g = color.G(); b = color.B(); a = color.A(); }

	uint8_t&R()							{ return r; }
	uint8_t&G()							{ return g; }
	uint8_t&B()							{ return b; }
	uint8_t&A()							{ return a; }

	// Assignment operators
	CColor &operator=(CColor &Color)	{ Set(Color); return *this; }
	CColor &operator+=(CColor &Color)	{ Set(min(R()+Color.R(), 255), min(G()+Color.G(), 255), min(B()+Color.B(), 255), min(A()+Color.A(), 255)); return *this; }
	CColor &operator-=(CColor &Color)	{ Set(max(R()-Color.R(),   0), max(G()-Color.G(),   0), max(B()-Color.B(),   0), max(A()-Color.A(),   0)); return *this; }
	CColor &operator*=(CColor &Color)	{ Set((GetR()*Color.GetR())/255, (GetG()*Color.GetG())/255, (GetB()*Color.GetB())/255, (GetA()*Color.GetA())/255); return *this; }
	CColor &operator*=(int Multiplier)	{ operator *=(CColor(Multiplier, Multiplier, Multiplier, 255)); return *this; }
	CColor &operator|=(CColor &Color)	{ Set(GetR()|Color.GetR(), GetG()|Color.GetG(), GetB()|Color.GetB(), GetA()|Color.GetA()); return(*this); return *this; }
	CColor &operator&=(CColor &Color)	{ Set(GetR()&Color.GetR(), GetG()&Color.GetG(), GetB()&Color.GetB(), GetA()&Color.GetA()); return(*this); return *this; }
	CColor &operator^=(CColor &Color)	{ Set(GetR()^Color.GetR(), GetG()^Color.GetG(), GetB()^Color.GetB(), GetA()^Color.GetA()); return(*this); return *this; }

	// Binary Operators
	CColor operator+(CColor &Color)		{ CColor c1 = *this; return c1 += Color; }
	CColor operator-(CColor &Color)		{ CColor c1 = *this; return c1 -= Color; }
	CColor operator*(CColor &Color)		{ CColor c1 = *this; return c1 *= Color; }
	CColor operator*(int Multiplier)	{ CColor c1 = *this; return c1 *= Multiplier; }
	CColor operator|(CColor &Color)		{ CColor c1 = *this; return c1 |= Color; }
	CColor operator&(CColor &Color)		{ CColor c1 = *this; return c1 &= Color; }
	CColor operator^(CColor &Color)		{ CColor c1 = *this; return c1 ^= Color; }

	// Unary Operator
	CColor operator~()					{ CColor c1 = *this; return c1^=CColor(255,255,255); }

	// Comparison Operators
	bool operator==(CColor Color)		{ return GetR()==Color.GetR() && GetG()==Color.GetG() && GetB()==Color.GetB() && GetA()==Color.GetA(); }
	bool operator!=(CColor Color)		{ return GetR()!=Color.GetR() || GetG()!=Color.GetG() || GetB()!=Color.GetB() || GetA()!=Color.GetA(); }

	//primary colors
	static CColor Red(uint8_t shade=255)				{ return(CColor(shade,0,0)); }
	static CColor Green(uint8_t shade=255)				{ return(CColor(0,shade,0)); }
	static CColor Blue(uint8_t shade=255)				{ return(CColor(0,0,shade)); }

	//secondary colors
	static CColor Yellow(uint8_t shade=255)				{ return(Red(shade)|Green(shade)); }
	static CColor Cyan(uint8_t shade=255)				{ return(Green(shade)|Blue(shade)); }
	static CColor Magenta(uint8_t shade=255)			{ return(Red(shade)|Blue(shade)); }

	// dark colours
	static CColor DarkRed(uint8_t shade=128)			{ return(Red(shade)); }
	static CColor DarkGreen(uint8_t shade=128)			{ return(Green(shade)); }
	static CColor DarkBlue(uint8_t shade=128)			{ return(Blue(shade)); }
	static CColor DarkYellow(uint8_t shade=128)			{ return(Yellow(shade)); }
	static CColor DarkCyan(uint8_t shade=128)			{ return(Cyan(shade)); }
	static CColor DarkMagenta(uint8_t shade=128)		{ return(Magenta(shade)); }

	// light colours	
	static CColor LightRed(uint8_t gray=128, uint8_t shade=255)		{ return(Red(shade)|White(gray)); }
	static CColor LightGreen(uint8_t gray=128, uint8_t shade=255)	{ return(Green(shade)|White(gray)); }
	static CColor LightBlue(uint8_t gray=128, uint8_t shade=255)	{ return(Blue(shade)|White(gray)); }
	static CColor LightYellow(uint8_t gray=128, uint8_t shade=255)	{ return(Yellow(shade)|White(gray)); }
	static CColor LightCyan(uint8_t gray=128, uint8_t shade=255)	{ return(Cyan(shade)|White(gray)); }
	static CColor LightMagenta(uint8_t gray=128, uint8_t shade=255)	{ return(Magenta(shade)|White(gray)); }

	// grayscale
	static CColor White(uint8_t shade=255)						{ return(Red(shade)|Green(shade)|Blue(shade)); }
	static CColor LightGray(uint8_t shade=192)					{ return(White(shade)); }
	static CColor DarkGray(uint8_t shade=128)					{ return(White(shade)); }
	static CColor Black(uint8_t shade=0)						{ return(White(shade)); }

	static CColor AnyBut(CColor c)								{ CColor C = Black(); if (C == c) C = White(); return C; }
	static CColor AnyBut(CColor c1, CColor c2)					{ CColor C = Black(); 
																  if (C == c1 || C == c2) C = White(); 
																  if (C == c1 || C == c2) C = DarkGray(); 
																  return C; }

	static CColor HSB(float hue, float saturation, float brightness)
	{
		int H = (int)(hue / 60.0f) % 6;
		float f = (hue / 60.0f) - (int)(hue / 60.0);
		int p = (int)(255 * brightness * (1 - saturation));
		int q = (int)(255 * brightness * (1 - f * saturation));
		int t = (int)(255 * brightness * (1 - (1 - f) * saturation));
		int v = (int)(brightness * 255);
		switch (H)
		{
		case 0: return CColor(v, t, p);
		case 1: return CColor(q, v, p);
		case 2: return CColor(p, v, t);
		case 3: return CColor(p, q, v);
		case 4: return CColor(t, p, v);
		case 5: return CColor(v, p, q);
		default: return CColor::Black();
		}
	}
};

#endif //#ifndef __COLOR_H__

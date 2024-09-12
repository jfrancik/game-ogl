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
#include "RendererOGL.h"

#include "sdl/include/SDL_image.h"
#include "sdl/include/SDL_ttf.h"
#include "sdl/include/SDL_gfxPrimitives.h"
#include "sdl/include/SDL_rotozoom.h"
#include "sdl/include/SDL_collide.h"

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")

// Include GLM core features
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;

unsigned CRendererOGL::c_idShaders = 0;
unsigned CRendererOGL::c_idBuffer = 0;
unsigned CRendererOGL::c_idSubroutineTexture = 0;
unsigned CRendererOGL::c_idSubroutineTextureColorKey = 0;
unsigned CRendererOGL::c_idSubroutineColorSolid = 0;


SDL_Surface* NoImage();

CFileMgr<unsigned> CRendererOGL::c_filemgr("%;%images\\;.\\;images\\",
	[](string filename)
	{
		// Load the bitmap
		SDL_Surface* pSurface = IMG_Load(filename.c_str());

		if (!pSurface) pSurface = NoImage();

		// Convert the bitmap to the RGBA format
		SDL_PixelFormat rgbaFormat;
		memset(&rgbaFormat, 0, sizeof(rgbaFormat));
		rgbaFormat.palette = NULL;
		rgbaFormat.BitsPerPixel = 32;
		rgbaFormat.BytesPerPixel = 4;
		rgbaFormat.Rmask = 0x000000ff;
		rgbaFormat.Gmask = 0x0000ff00;
		rgbaFormat.Bmask = 0x00ff0000;
		rgbaFormat.Amask = 0xff000000;
		SDL_Surface* pRGBA = SDL_ConvertSurface(pSurface, &rgbaFormat, SDL_SWSURFACE);

		// Create & load OpenGL texture
		unsigned idTexture;
		glGenTextures(1, &idTexture);
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pRGBA->w, pRGBA->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pRGBA->pixels);

		SDL_FreeSurface(pSurface);
		unsigned* pInt = new unsigned;
		*pInt = idTexture;
		return pInt;
	},
	[](unsigned* pidTexture)
	{
		delete pidTexture;
		//SDL_FreeSurface(pSurface); 
	});


CFileMgr<_TTF_Font> CRendererOGL::c_fontmgr("%;%images\\;.\\;images\\",
	[](string filename, int nSize)
	{
		return TTF_OpenFont(CRendererOGL::FindPathStr(filename).c_str(), nSize);
	},
	[](_TTF_Font* pFont)
	{
		// TTF_CloseFont(pFont);   //// should be called here, but causes "Access violation reading location 0x0000000000000054"
	});

/////////////////////////////////////////////////////////
// Constructors & Destructors

CRendererOGL::CRendererOGL() : IRenderer()
{
	m_pSurface = NULL;
}

// copy constructor
CRendererOGL::CRendererOGL(CRendererOGL& renderer) : IRenderer()
{
	Clone(renderer);
}

CRendererOGL::~CRendererOGL()
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
void CRendererOGL::Initialise(int width, int height, int depth, uint32_t flagsCreation)
{
	m_pSurface = SDL_SetVideoMode(width, height, depth, flagsCreation | SDL_OPENGL);

	// OpenGL Standard Initialisation
	Init(width, height, depth);

	// Create screen space texture
	glGenTextures(1, &m_idTexture);
	glBindTexture(GL_TEXTURE_2D, m_idTexture);

	// Texture parameters - to get nice filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// This will allocate an uninitilised texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_nWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_nHeight);
}

void CRendererOGL::Clone(IRenderer& renderer)
{
	if (GetType() == renderer.GetType())
	{
		// Bind the source texture
		GLuint sourceTexture = dynamic_cast<CRendererOGL*>(&renderer)->m_idTexture;
		glBindTexture(GL_TEXTURE_2D, sourceTexture);

		// Get the texture parameters
		GLint width, height, internalFormat;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);

		// Allocate memory and get texture data
		GLubyte* data = new GLubyte[width * height * 4];		// Assuming 4 bytes per pixel (e.g., RGBA8)
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // Assuming format is RGBA

		// Create and bind the new texture
		glGenTextures(1, &m_idTexture);
		glBindTexture(GL_TEXTURE_2D, m_idTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Clean up
		delete[] data;
	}
}

// Clone - from another IRenderer which should be of the same type
void CRendererOGL::Create(IRenderer &renderer)
{
	if (GetType() == renderer.GetType())
	{
		m_idTexture = dynamic_cast<CRendererOGL*>(&renderer)->m_idTexture;
		m_nWidth = dynamic_cast<CRendererOGL*>(&renderer)->m_nWidth;
		m_nHeight = dynamic_cast<CRendererOGL*>(&renderer)->m_nHeight;
		m_fRotate = dynamic_cast<CRendererOGL*>(&renderer)->m_fRotate;
		m_bColorKey = dynamic_cast<CRendererOGL*>(&renderer)->m_bColorKey;
		m_colorKey = dynamic_cast<CRendererOGL*>(&renderer)->m_colorKey;
	}
}



// memory canvas
void CRendererOGL::Create(int width, int height, int depth, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask)
{
	// sorry, we simply ignore depth and masks...
	
	// Create screen space texture
	glGenTextures(1, &m_idTexture);
	glBindTexture(GL_TEXTURE_2D, m_idTexture);

	// Texture parameters - to get nice filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// This will allocate an uninitilised texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_nWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_nHeight);
}

void CRendererOGL::Create(int width, int height)
{
	// Create screen space texture
	glGenTextures(1, &m_idTexture);
	glBindTexture(GL_TEXTURE_2D, m_idTexture);

	// Texture parameters - to get nice filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// This will allocate an uninitilised texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_nWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_nHeight);
}

// from a loaded bitmap
void CRendererOGL::Create(string sFileName)
{
	m_idTexture = *c_filemgr.Load(sFileName);

	glBindTexture(GL_TEXTURE_2D, m_idTexture);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_nWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_nHeight);

	m_fRotate = 0;
}

// Rectangular Fragment
void CRendererOGL::Create(shared_ptr<IRenderer> pRenderer, CRectangle rect)
{
}

void CRendererOGL::Create(string sFileName, CRectangle rect)
{
}

// Tiled Fragment
void CRendererOGL::Create(shared_ptr<IRenderer> pRenderer, short numCols, short numRows, short iCol, short iRow)
{
}

void CRendererOGL::Create(string sFileName, short numCols, short numRows, short iCol, short iRow)
{
}

/////////////////////////////////////////////////////////
// OGL Specific Functions

void CRendererOGL::Init(int width, int height, int depth)
{
	// GLEW initialisation
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << "GLEW Error: " << (const char*)glewGetErrorString(err) << endl;
		return;
	}
	cout << "Using GLEW " << (const char*)glewGetString(GLEW_VERSION) << endl;

	// Some additional diagnostic information
	cout << "Vendor: " << (const char*)glGetString(GL_VENDOR) << endl;
	cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << endl;
	cout << "Version: OpenGL" << (const char*)glGetString(GL_VERSION) << endl;

	// rendering states
	glShadeModel(GL_SMOOTH);	// smooth shading mode is the default one; try GL_FLAT here!
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// this is the default one; try GL_LINE!

	// blending (transparency)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialise Shaders

	// load shader code from files
	ifstream fileVS("shaders/basic.vert"), fileFS("shaders/basic.frag");
	string strVS(std::istreambuf_iterator<char>(fileVS), (std::istreambuf_iterator<char>()));
	string strFS(std::istreambuf_iterator<char>(fileFS), (std::istreambuf_iterator<char>()));
	const GLchar* pVS = static_cast<const GLchar*>(strVS.c_str());
	const GLchar* pFS = static_cast<const GLchar*>(strFS.c_str());

	// create and compile shaders
	GLuint idVS = glCreateShader(GL_VERTEX_SHADER);
	GLuint idFS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(idVS, 1, &pVS, NULL);
	glShaderSource(idFS, 1, &pFS, NULL);
	glCompileShader(idVS);
	glCompileShader(idFS);

	// check compilation status
	GLint result = 0;
	glGetShaderiv(idVS, GL_COMPILE_STATUS, &result);
	glGetShaderiv(idFS, GL_COMPILE_STATUS, &result);

	// create and link the program
	c_idShaders = glCreateProgram();
	glAttachShader(c_idShaders, idVS);
	glAttachShader(c_idShaders, idFS);
	glLinkProgram(c_idShaders);

	// check linking status
	glGetProgramiv(c_idShaders, GL_LINK_STATUS, &result);

	// choose the shader program to use
	glUseProgram(c_idShaders);

	// determine subroutine ids
	c_idSubroutineTexture = glGetSubroutineIndex(c_idShaders, GL_FRAGMENT_SHADER, "renderTexture");
	c_idSubroutineTextureColorKey = glGetSubroutineIndex(c_idShaders, GL_FRAGMENT_SHADER, "renderTextureColorKey");
	c_idSubroutineColorSolid = glGetSubroutineIndex(c_idShaders, GL_FRAGMENT_SHADER, "renderColorSolid");

	// setup the screen background colour
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Common vertex/texture coord data for ALL sprites
	// Same buffer can be reused by various sprites - which will use different textures and transforms
	// Vertex data for a quad
	GLfloat vertices[] = {
		0, 0, 0, 1,
		0, 1, 0, 0,
		1, 1, 1, 0,
		1, 0, 1, 1
	};

	// Generate 1 buffer name
	glGenBuffers(1, &c_idBuffer);
	// Bind (activate) the buffer
	glBindBuffer(GL_ARRAY_BUFFER, c_idBuffer);
	// Send data to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Setting up the projection to match window resolution
	glm::mat4 m = glm::ortho(0.f, (float)width, 0.f, (float)height, -2.f, 2.f);
	auto aa = glGetUniformLocation(c_idShaders, "matrixProjection");
	glUniformMatrix4fv(glGetUniformLocation(c_idShaders, "matrixProjection"), 1, GL_FALSE, &m[0][0]);

	// setup the texture unit in the shaders
	glUniform1i(glGetUniformLocation(c_idShaders, "texture0"), 0);
}

void CRendererOGL::BlitMe(CRectangle& rectDest, CRectangle& rectSrc)
{
	// Bind the texture to use
	glBindTexture(GL_TEXTURE_2D, m_idTexture);

	// Setup OpenGL transformations represented as a 4D(!) matrix
	// 2D graphics shouls only require 3D matrices, but GLM library is optimised for 3D use
	glm::mat4 m = glm::mat4(1);
	m = glm::translate(m, glm::vec3((float)rectDest.x, (float)rectDest.y, 0));
	m = glm::rotate(m, glm::radians(m_fRotate), glm::vec3(0, 0, 1));
	m = glm::scale(m, glm::vec3(GetWidth(), GetHeight(), 1));

	// send the "model-view" matrix to GPU
	glUniformMatrix4fv(glGetUniformLocation(c_idShaders, "matrixModelView"), 1, GL_FALSE, &m[0][0]);

	// choose the subroutine
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, m_bColorKey ? &c_idSubroutineTextureColorKey : &c_idSubroutineTexture);

	// Standard OpenGL buffer rendering
	// enable vertex arrays
	GLuint attribVertex = glGetAttribLocation(c_idShaders, "aVertex");
	GLuint attribText = glGetAttribLocation(c_idShaders, "aTexCoord");
	glEnableVertexAttribArray(attribVertex);
	glEnableVertexAttribArray(attribText);
	glBindBuffer(GL_ARRAY_BUFFER, c_idBuffer);
	glVertexAttribPointer(attribVertex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glVertexAttribPointer(attribText, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// draw triangles - using 4 vertices (starting at 0)
	glDrawArrays(GL_QUADS, 0, 4);

	// tidy up - disable the arrays
	glDisableVertexAttribArray(attribVertex);
	glDisableVertexAttribArray(attribText);

}

/////////////////////////////////////////////////////////
// Low-level surface access and tester

void CRendererOGL::CreateFromSurface(void* pSurface)
{
	//if (pSurface)
	//	m_pSurface = static_cast<SDL_Surface*>(pSurface);
	//else
	//	m_pSurface = NoImage();
}

void CRendererOGL::SetSurface(void* pSurface)
{
//	m_pSurface = static_cast<SDL_Surface*>(pSurface);
}

void* CRendererOGL::GetSurface()
{
//	return m_pSurface;
	return NULL;
}

bool CRendererOGL::HasPixels()
{
	return m_pSurface && m_pSurface->pixels;
}

/////////////////////////////////////////////////////////
// Static functions for standard path finding

void CRendererOGL::SetDefaultFilePath(std::string new_path)
{ 
	c_filemgr.SetPathString(new_path); 
}

std::string CRendererOGL::FindPathStr(std::string filename)
{
	return c_filemgr.FindPathStr(filename);
}

/////////////////////////////////////////////////////////
// RotoZoom: create a clone object, rotated and zoomed

shared_ptr<IRenderer> CRendererOGL::CreateRotozoom(double angle, double zoomx, double zoomy, bool smooth)
{
	auto pRotozoom = make_shared<CRendererOGL>();
	pRotozoom->Create(*this);
	pRotozoom->m_fRotate = (float)angle;
	return pRotozoom;
}

/////////////////////////////////////////////////////////
// Width & Height

int CRendererOGL::GetWidth()
{ 
	return m_nWidth;
}

int CRendererOGL::GetHeight()
{ 
	return m_nHeight;
}

/////////////////////////////////////////////////////////
// Match color with the closest

CColor CRendererOGL::MatchColor(CColor color)
{
	return color;	// we use full 32-bit colour so no need to match
}

/////////////////////////////////////////////////////////
// Color Key (Trasnparent Color)

void CRendererOGL::SetColorKey(CColor color)
{
	m_bColorKey = true;
	m_colorKey = color;

	// additional configuration of the "color key" (transparency)
	glUniform3f(glGetUniformLocation(c_idShaders, "colorKey"), (float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b/255.0f);
	glUniform1f(glGetUniformLocation(c_idShaders, "colorKeyMul"), 1);
}

bool CRendererOGL::IsColorKeySet()
{
	return m_bColorKey;
}

CColor CRendererOGL::GetColorKey()
{
	return m_colorKey;
}

void CRendererOGL::ClearColorKey()
{
	m_bColorKey = false;
	m_colorKey = CColor::Black();
}

/////////////////////////////////////////////////////////
// Pixel collision function

bool CRendererOGL::HitTest(int ax, int ay, shared_ptr<IRenderer> pOther, int bx, int by, int nSkip)
{
	//if (!m_pSurface || !pOther || !pOther->GetSurface()) return false;
	//SDL_Surface* pOtherSurface = static_cast<SDL_Surface*>(pOther->GetSurface());
	//return SDL_CollidePixel(m_pSurface, ax, ay, pOtherSurface, bx, by, nSkip) != 0;
	return false;
}

/////////////////////////////////////////////////////////
// Flip Function

void CRendererOGL::Flip()
{
	// flush and swap
	glFlush();
	SDL_GL_SwapBuffers();
}

/////////////////////////////////////////////////////////
// lock and unlock (for direct pixel access)

bool CRendererOGL::Lock()
{
	//if (!m_pSurface) return false;
	//if (!SDL_MUSTLOCK(m_pSurface))
	//	return true;
	//return (SDL_LockSurface(m_pSurface) == 0);
	return false;
}

void CRendererOGL::Unlock()
{
	//if (m_pSurface && SDL_MUSTLOCK(m_pSurface))
	//	SDL_UnlockSurface(m_pSurface);
}


/////////////////////////////////////////////////////////
// Drawing Functions

CColor CRendererOGL::GetPixel(int x, int y)
{
	//if (!m_pSurface) return CColor::Black();

	//uint32_t color = 0;
	//int position = (GetHeight() - y - 1) * m_pSurface->pitch + m_pSurface->format->BytesPerPixel * x;
	//char* buffer =(char*)m_pSurface->pixels;
	//buffer += position;
	//Lock();
	//memcpy(&color, buffer, m_pSurface->format->BytesPerPixel);
	//Unlock();
	//CColor col;
	//SDL_GetRGBA(color, m_pSurface->format, &col.R(), &col.G(), &col.B(), &col.A());
	//return (col);
	return CColor::Black();
}

void CRendererOGL::SetPixel(int x, int y, CColor& color)
{
	//if (!m_pSurface) return;
	//int position = (GetHeight() - y - 1) * m_pSurface->pitch + m_pSurface->format->BytesPerPixel * x;
	//char* buffer =(char*)m_pSurface->pixels;
	//buffer += position;
	//uint32_t col = SDL_MapRGBA(m_pSurface->format, color.R(), color.G(), color.B(), color.A());
	//Lock();
	//memcpy(buffer, &col, m_pSurface->format->BytesPerPixel);
	//Unlock();
}

void CRendererOGL::Clear(CColor& clr)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CRendererOGL::Blit(CRectangle& rectDest, IRenderer& Src, CRectangle& rectSrc)
{
	//SDL_Surface* pSurface = static_cast<SDL_Surface*>(Src.GetSurface());
	//if (m_pSurface) SDL_BlitSurface(pSurface, (SDL_Rect*)&rectSrc.YInv(Src.GetHeight()), m_pSurface, (SDL_Rect*)&(rectDest).YInv(GetHeight()));

	CRendererOGL* pSource = static_cast<CRendererOGL*>(&Src);

	pSource->BlitMe(rectDest, rectSrc);


}

void CRendererOGL::DrawHLine(CVectorI pt1, int16_t x2, CColor& color)
{
}

void CRendererOGL::DrawVLine(CVectorI pt1, int16_t y2, CColor& color)
{
}

void CRendererOGL::DrawLine(CVectorI pt1, CVectorI pt2, CColor& color)
{
}

void CRendererOGL::DrawLine(CVectorI pt1, CVectorI pt2, uint8_t width, CColor& color)
{
}

void CRendererOGL::DrawRect(CRectangle& rect, CColor& color)
{
}

void CRendererOGL::FillRect(CRectangle& rect, CColor& color)
{
}

void CRendererOGL::DrawRect(CRectangle& rect, CColor& color, int16_t rad)
{
}

void CRendererOGL::FillRect(CRectangle& rect, CColor& color, int16_t rad)
{
}

void CRendererOGL::DrawOval(CRectangle& rect, CColor& color)
{
}

void CRendererOGL::FillOval(CRectangle& rect, CColor& color)
{
}

void CRendererOGL::DrawCircle(CVectorI pt, uint16_t radius, CColor& color)
{
}

void CRendererOGL::FillCircle(CVectorI pt, uint16_t radius, CColor& color)
{
}

void CRendererOGL::DrawPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color)
{
}

void CRendererOGL::FillPie(CVectorI pt, uint16_t radius, uint16_t angleStart, uint16_t angleEnd, CColor& color)
{
}

void CRendererOGL::DrawTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color)
{
}

void CRendererOGL::FillTriangle(CVectorI pt1, CVectorI pt2, CVectorI pt3, CColor& color)
{
}

void CRendererOGL::DrawPolyLine(CVectorI pts[], uint16_t num, CColor& color)
{
}

void CRendererOGL::DrawPolygon(CVectorI pts[], uint16_t num, CColor& color)
{
}

void CRendererOGL::FillPolygon(CVectorI pts[], uint16_t num, CColor& color)
{
}

void CRendererOGL::DrawBezierLine(CVectorI pts[], uint16_t num, uint16_t nSteps, CColor& color)
{
}

/////////////////////////////////////////////////////////
// Text Drawing Functions

bool CRendererOGL::SetFont(std::string fontFace, int nPtSize)
{
	m_strFontFace = fontFace;
	if (nPtSize != 0) m_nPtSize = nPtSize;
	if (m_nPtSize == 0) m_nPtSize = 18;
	
	m_pFont = c_fontmgr.Load(m_strFontFace, m_nPtSize);
	return m_pFont != NULL;
}

bool CRendererOGL::SetFont(int nPtSize)
{
	if (nPtSize != 0) m_nPtSize = nPtSize;
	if (m_nPtSize == 0) m_nPtSize = 18;

	m_pFont = c_fontmgr.Load(m_strFontFace, m_nPtSize);
	return m_pFont != NULL;
}

std::string CRendererOGL::GetFontFace()
{
	return m_strFontFace;
}

int CRendererOGL::GetFontSize()
{
	return m_nPtSize;
}

void CRendererOGL::GetFontSize(int& size, int& height, int& width, int& ascent, int& descent, int& leading, int& baseline)
{
	size = m_nPtSize;
	height = TTF_FontHeight(m_pFont);
	width = height;
	ascent = TTF_FontAscent(m_pFont);
	descent = TTF_FontDescent(m_pFont);
	leading = max(TTF_FontLineSkip(m_pFont), height);
	baseline = leading - height - descent;
}

shared_ptr<IRenderer> CRendererOGL::GetTextGraphics(std::string pText)
{
	if (!m_pFont)
		SetFont(0);
	return GetTextGraphics(m_pFont, m_textColor, pText);
}

shared_ptr<IRenderer> CRendererOGL::GetTextGraphics(std::string fontFace, int nPtSize, CColor color, std::string pText)
{
	if (nPtSize == 0) nPtSize = m_nPtSize;
	if (m_nPtSize == 0) nPtSize = 18;
	return GetTextGraphics(c_fontmgr.Load(fontFace, nPtSize), color, pText);
}


shared_ptr<IRenderer> CRendererOGL::GetTextGraphics(void* pFont, CColor textColor, std::string pText)
{
	if (!pFont) return NULL;
	SDL_Surface* pSurface = TTF_RenderText_Blended(static_cast<_TTF_Font*>(pFont), pText.c_str(), *(SDL_Color*)&textColor.r);
	auto pRenderer = make_shared<CRendererOGL>();
	pRenderer->CreateFromSurface(pSurface);
	return pRenderer;
}

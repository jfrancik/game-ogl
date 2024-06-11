#include "stdafx.h"
#include "MyGame.h"

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

CMyGame::CMyGame(void)
{
}

CMyGame::~CMyGame(void)
{
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	Uint32 t = GetTime();
}

void CMyGame::OnDraw(CGraphics* g)
{
	// EXAMPLE OF OPENGL 2D SPRITE RENDERING

	// clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind the texture to use
	glBindTexture(GL_TEXTURE_2D, m_rocket.idTexture);

	// Setup OpenGL transformations represented as a 4D(!) matrix
	// 2D graphics shouls only require 3D matrices, but GLM library is optimised for 3D use
	glm::mat4 m = glm::mat4(1);
	m = glm::translate(m, glm::vec3(m_rocket.translate.m_x, m_rocket.translate.m_y, 0));
	m = glm::rotate(m, glm::radians(m_rocket.rotate), glm::vec3(0, 0, 1));
	m = glm::scale(m, glm::vec3(m_rocket.scale.m_x, m_rocket.scale.m_y, 1));

	// send the "model-view" matrix to GPU
	glUniformMatrix4fv(glGetUniformLocation(m_idShaders, "matrixModelView"), 1, GL_FALSE, &m[0][0]);

	// additional configuration of the "color key" (transparency)
	glUniform3f(glGetUniformLocation(m_idShaders, "colorKey"), 0, 0, 1);
	glUniform1f(glGetUniformLocation(m_idShaders, "colorKeyMul"), 1);

	// Standard OpenGL buffer rendering
	// enable vertex arrays
	GLuint attribVertex = glGetAttribLocation(m_idShaders, "aVertex");
	GLuint attribText = glGetAttribLocation(m_idShaders, "aTexCoord");
	glEnableVertexAttribArray(attribVertex);
	glEnableVertexAttribArray(attribText);
	glBindBuffer(GL_ARRAY_BUFFER, m_idBuffer);
	glVertexAttribPointer(attribVertex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glVertexAttribPointer(attribText, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// draw triangles - using 4 vertices (starting at 0)
	glDrawArrays(GL_QUADS, 0, 4);

	// tidy up - disable the arrays
	glDisableVertexAttribArray(attribVertex);
	glDisableVertexAttribArray(attribText);



	// flush and swap
	glFlush();
	SDL_GL_SwapBuffers();
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	/*** OpenGL Standard Initialisation ***/

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
	m_idShaders = glCreateProgram();
	glAttachShader(m_idShaders, idVS);
	glAttachShader(m_idShaders, idFS);
	glLinkProgram(m_idShaders);

	// check linking status
	glGetProgramiv(m_idShaders, GL_LINK_STATUS, &result);

	// choose the shader program to use
	glUseProgram(m_idShaders);


	// setup the screen background colour
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Common vertex/texture coord data for ALL sprites
	// Same buffer can be reused by various sprites - which will use different textures and transforms
	// Vertex data for a quad
	GLfloat vertices[] = {
		-0.5, -0.5, 0, 1,
		-0.5, 0.5, 0, 0,
		0.5, 0.5, 1, 0,
		0.5, -0.5, 1, 1
	};

	// Generate 1 buffer name
	glGenBuffers(1, &m_idBuffer);
	// Bind (activate) the buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_idBuffer);
	// Send data to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Setting up the projection to match window resolution
	glm::mat4 m = glm::ortho(0.f, (float)GetWidth(), 0.f, (float)GetHeight(), -2.f, 2.f);
	auto aa = glGetUniformLocation(m_idShaders, "matrixProjection");
	glUniformMatrix4fv(glGetUniformLocation(m_idShaders, "matrixProjection"), 1, GL_FALSE, &m[0][0]);

	
	/*** Initialise the Rocket sprite ***/

	// Load the bitmap
	SDL_Surface* pSurf = SDL_LoadBMP("images/rocket.bmp");

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
	SDL_Surface* pRGBA = SDL_ConvertSurface(pSurf, &rgbaFormat, SDL_SWSURFACE);

	// Create & load OpenGL texture
	glGenTextures(1, &m_rocket.idTexture);
	glBindTexture(GL_TEXTURE_2D, m_rocket.idTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pRGBA->w, pRGBA->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pRGBA->pixels);
	
	// setup the texture unit in the shaders
	glUniform1i(glGetUniformLocation(m_idShaders, "texture0"), 0);

	// setup the rocket transforms
	// note: scale depends on the size of the bitmap. The original size of the sprite rectangle is 1x1
	m_rocket.scale = CVector((float)pRGBA->w, (float)pRGBA->h);
	m_rocket.translate = CVector(640, 40);
	m_rocket.rotate = 0;
}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	StartGame();	// exits the menu mode and starts the game mode
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
}

// called when the game is over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	if (bLeft)
	{
		CVector mv = CVector(x, y) - m_rocket.translate;
		m_rocket.rotate = glm::degrees(atan2(-x + m_rocket.translate.m_x, y - m_rocket.translate.m_y));
		m_rocket.translate = CVector(x, y);
	}
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
	m_rocket.translate = CVector(x, y);
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
	m_rocket.translate = CVector(x, y);
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}

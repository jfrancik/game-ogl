# A very basic test of OpenGL rendering in GFC

## Notable code:
1. **main.cpp**: *app.OpenWindow* function call has an extra parameter: *SDL_OPENGL*
2. **MyGame.cpp**: *CMyGame::OnInitialize* GLEW and OpenGL initialisation code added
3. **MyGame.cpp**: *MyGame::OnDraw()* renders a red triangle on a black background using immediate mode in OpenGL

## Remarks
* No modification to the GFC code was needed
* Only simple OpenGL functionality is available though
* SDL 1.2 is not compatible with OpenGL 3.0 or higher context
* Therefore, an upgrade to SDL 2 is required.

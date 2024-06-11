# A very basic test of OpenGL rendering in GFC
## Version 2

## Notable code:
1. **main.cpp**: *app.OpenWindow* function call has an extra parameter: *SDL_OPENGL*
2. **MyGame.cpp**: *CMyGame::OnInitialize* GLEW and OpenGL initialisation code added
3. **MyGame.cpp**: *MyGame::OnDraw()* renders a rocket sprite

## Remarks
* No modification to the GFC code was needed
* More advanced OpenGL functionality is enabled
* Still SDL 1.2 is used - now working well with OpenGL 4.6!
* Upgrade to SDL 2 is still desired but not immediately required

#ifndef __MODULESCENEHONDA_H__
#define __MODULESCENEHONDA_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleHondaScene : public Module
{
public:
	ModuleHondaScene(bool start_enabled = true);
	~ModuleHondaScene();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	//SDL_Texture* redShip = nullptr;
	SDL_Rect ground;
	SDL_Rect background;
	SDL_Rect foreground;
	Animation flag;
	Animation girl;
	float bounceFrame = 0;
};

#endif // __MODULESCENEHONDA_H__
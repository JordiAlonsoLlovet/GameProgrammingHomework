#pragma once
#include "Module.h"
#include "Globals.h"

#define NUM_MOUSE_BUTTONS 5
#define MAX_KEYS 300

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct iPoint
{
	int x;
	int y;

	iPoint() = default;
};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Get mouse / axis position
	const iPoint& GetMouseMotion() const { return mouse_motion; };
	const iPoint& GetMousePosition() const {
		return mouse;
	};
	const iPoint& GetWheelMotion() const { return wheel_motion; };

	bool KeyPress(int id) const
	{
		return keyboard[id] == KEY_REPEAT || keyboard[id] == KEY_DOWN;
	}

private:
	KeyState *keyboard = NULL;
	KeyState mouse_buttons[NUM_MOUSE_BUTTONS];
	iPoint mouse_motion;
	iPoint mouse;
	iPoint wheel_motion;
};
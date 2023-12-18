#pragma once
#include "Module.h"
#include "Globals.h"
#define NUM_WINDOWS 3
#define LOGS_W "Logs"
#define PROPERTIES_W "Properties"
#define CONFIG_W "Configuration"

typedef unsigned __int8 Uint8;

struct MyWindow {
	const char* title = "";
	bool show = true;
	int w;
	int h;
	int x;
	int y;
};

class ModuleEditor : public Module
{
public:

	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	bool ShowWindow(const char* name) const;

private:
	MyWindow myWindows[NUM_WINDOWS];
};
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "SDL.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include <string>

ModuleInput::ModuleInput() : Module(), mouse(), mouse_motion{}
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        if (SDL_GetWindowID(App->GetWindow()->window) == sdlEvent.window.windowID) {
            switch (sdlEvent.type)
            {
                case SDL_QUIT:
                    return UPDATE_STOP;
                case SDL_WINDOWEVENT:
                    if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    {
                        App->GetOpenGL()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                        App->GetCamera()->SetAspectRatio(sdlEvent.window.data2 / (double)sdlEvent.window.data1);
                    }
                    if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
                        return UPDATE_STOP;
                                
                    break;
            }

			//mouse_motion = { 0, 0 };
	//memset(windowEvents, false, WE_COUNT * sizeof(bool));

			const Uint8* keys = SDL_GetKeyboardState(NULL);
			for (int i = 0; i < MAX_KEYS; ++i)
			{
				if (keys[i] == 1)
				{
					if (keyboard[i] == KEY_IDLE) {
						LOG("DOWN");
						keyboard[i] = KEY_DOWN;
					}
						
					else
					{
						LOG("REPEAT");
						keyboard[i] = KEY_REPEAT;
					}
				}
				else
				{
					if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
						keyboard[i] = KEY_UP;
					else
						keyboard[i] = KEY_IDLE;
				}
	
			}
			for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
			{
				if (mouse_buttons[i] == KEY_DOWN)
					mouse_buttons[i] = KEY_REPEAT;

				if (mouse_buttons[i] == KEY_UP)
					mouse_buttons[i] = KEY_IDLE;
			}
        }
    }

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}


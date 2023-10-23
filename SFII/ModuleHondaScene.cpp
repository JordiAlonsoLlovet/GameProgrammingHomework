#include "Globals.h"
#include "Application.h"
#include "ModuleSceneKen.h"
#include "ModuleHondaScene.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleHondaScene::ModuleHondaScene(bool start_enabled) : Module(start_enabled)
{
	// ground
	ground.x = 8;
	ground.y = 391;
	ground.w = 896;
	ground.h = 72;

	// TODO 2 : setup the foreground (red ship) with
	// coordinates x,y,w,h from ken_stage.png
	foreground.x = 8;
	foreground.y = 24;
	foreground.w = 520;
	foreground.h = 180;

	// Background / sky
	background.x = 72;
	background.y = 208;
	background.w = 768;
	background.h = 176;

	// flag animation
	flag.frames.push_back({ 848, 208, 40, 40 });
	flag.frames.push_back({ 848, 256, 40, 40 });
	flag.frames.push_back({ 848, 304, 40, 40 });
	flag.speed = 0.08f;

	// TODO 4: Setup Girl Animation from coordinates from ken_stage.png
	girl.frames.push_back({ 624, 16, 31, 55 });
	girl.frames.push_back({ 624, 80, 31, 55 });
	girl.frames.push_back({ 624, 144, 31, 55 });
	girl.speed = 0.08f;
}

ModuleHondaScene::~ModuleHondaScene()
{}

// Load assets
bool ModuleHondaScene::Start()
{
	LOG("Loading ken scene");

	graphics = App->textures->Load("ken_stage.png");

	// TODO 7: Enable the player module
	App->player->Enable();
	// TODO 0: trigger background music
	App->audio->PlayMusic("honda.ogg");

	return true;
}

// UnLoad assets
bool ModuleHondaScene::CleanUp()
{
	LOG("Unloading ken scene");

	App->textures->Unload(graphics);
	App->player->Disable();

	return true;
}

// Update: draw background
update_status ModuleHondaScene::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_REPEAT)
		App->fade->FadeToBlack(App->scene_ken, this, 10.0f);

	// TODO 5: make sure the ship goes up and down
	bounceFrame += 0.15;
	if (bounceFrame == 6.3f) {
		bounceFrame = 0;
	}
	float boatHeight = sin(bounceFrame) * 2;
	// Draw everything --------------------------------------
	// TODO 1: Tweak the movement speed of the sea&sky + flag to your taste
	const float bg_speed = 5.0f;
	const float fg_speed = 10.0f;
	App->renderer->Blit(graphics, 0, 0, &background, bg_speed); // sea and sky
	App->renderer->Blit(graphics, 560, 8, &(flag.GetCurrentFrame()), bg_speed); // flag animation

	// TODO 3: Draw the ship. Be sure to tweak the speed.
	App->renderer->Blit(graphics, 0, boatHeight - 2, &foreground, fg_speed);

	// TODO 6: Draw the girl. Make sure it follows the ship movement!
	App->renderer->Blit(graphics, 192, 103 + boatHeight, &(girl.GetCurrentFrame()), fg_speed); // girl animation

	App->renderer->Blit(graphics, 0, 170, &ground);

	// TODO 10: Build an entire new scene "honda", you can find its
	// and music in the Game/ folder

	// TODO 11: Make that pressing space triggers a switch to honda logic module
	// using FadeToBlack module

	return UPDATE_CONTINUE;
}
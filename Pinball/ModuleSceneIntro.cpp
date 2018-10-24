#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	sensed = false;
	ball = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	App->renderer->camera.x = App->renderer->camera.y = 0;

	ball = App->textures->Load("pinball/pinball_ball.png");
	
	rick = App->textures->Load("pinball/rick_head.png");
	background = App->textures->Load("pinball/background2.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	App->physics->CreateLFlipper();
	App->physics->CreateRFlipper();
	App->physics->CreateUpperFlipper();

	backgroundrect.h = 907;
	backgroundrect.w = 609;

	//lives = 3;
	//defining the phisical body of the map
	// Pivot 0, 0
	
	SpawnBall();

	return ret;
}



// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(background, 0, 0, &backgroundrect);

	// Debug spawn ball
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 15));
		circles.getLast()->data->listener = this;
	}

	// Flipper inputs
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->physics->fbody->ApplyTorque(250.0, true);
	}
	else
	{
		if (App->physics->fbody->IsAwake())
		{
			App->physics->fbody->ApplyTorque(-250.0, false);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		App->physics->fbody2->ApplyTorque(-250.0, true);
		App->physics->fbody3->ApplyTorque(-250.0, true);
	}
	else
	{
		if (App->physics->fbody2->IsAwake() && App->physics->fbody3->IsAwake())
		{
			App->physics->fbody2->ApplyTorque(250.0, false);
			App->physics->fbody3->ApplyTorque(250.0, false);
		}
	}


	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	if (ballpos.y > 907) {
		PlayerDeath();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	//App->audio->PlayFx(bonus_fx);

}



void ModuleSceneIntro::SpawnBall()
{
	balls = App->physics->CreateCircle(44, 791, 15);

}



void ModuleSceneIntro::PlayerDeath() {

	App->physics->world->DestroyBody(balls->body);

	lives = lives - 1;

	SpawnBall();

	/*if (lives <= 0) {
		defeat = true;
	}
	
	if (defeat == true) {
		//GameOver();
	}*/


}

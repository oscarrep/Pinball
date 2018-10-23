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
	background = App->textures->Load("pinball/background.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	backgroundrect.h = 907;
	backgroundrect.w = 609;

	lives = 3;
	//defining the phisical body of the map
	// Pivot 0, 0
	int outline[72] = {
		18, 896,
		18, 813,
		18, 648,
		17, 484,
		22, 372,
		50, 271,
		94, 179,
		161, 107,
		212, 74,
		270, 54,
		318, 42,
		362, 10,
		409, 0,
		445, 4,
		482, 22,
		511, 47,
		538, 82,
		563, 125,
		581, 167,
		590, 204,
		597, 257,
		595, 324,
		591, 369,
		575, 418,
		556, 455,
		542, 487,
		530, 528,
		536, 543,
		552, 567,
		556, 601,
		556, 624,
		558, 857,
		520, 862,
		515, 802,
		380, 875,
		378, 904
	};

	out.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), outline, 72));

	int right_inline[54] = {
		446, 434,
		491, 457,
		537, 386,
		557, 324,
		560, 271,
		554, 213,
		528, 165,
		505, 127,
		464, 85,
		433, 67,
		410, 60,
		395, 71,
		381, 98,
		384, 117,
		385, 146,
		398, 161,
		424, 168,
		453, 177,
		470, 183,
		482, 202,
		474, 248,
		459, 295,
		425, 351,
		390, 387,
		399, 393,
		407, 403,
		447, 434
	};
	r_inline.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), right_inline, 54));

	int left_inline[66] = {
		65, 906,
		62, 510,
		74, 358,
		99, 261,
		134, 204,
		171, 160,
		201, 132,
		231, 118,
		247, 111,
		256, 130,
		250, 152,
		233, 167,
		187, 182,
		154, 192,
		140, 198,
		94, 278,
		81, 354,
		128, 385,
		134, 407,
		133, 441,
		133, 490,
		133, 514,
		125, 525,
		109, 538,
		95, 551,
		83, 579,
		79, 609,
		81, 624,
		84, 854,
		125, 854,
		126, 802,
		265, 878,
		265, 905
	};

	l_inline.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), left_inline, 66));

	int left_triangle[8] = {
		175, 626,
		175, 708,
		221, 730,
		178, 625
	};
	l_triangle.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), left_triangle, 8));

	int right_triangle[8] = {
		469, 625,
		470, 711,
		423, 732,
		465, 626
	};
	r_triangle.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), right_triangle, 8));

	int top_triangle[8] = {
		237, 320,
		280, 309,
		318, 346,
		237, 321
	};
	t_triangle.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), top_triangle, 8));

	int heart_barrier_left[4] = {
		301, 118,
		300, 143
	};
	l_heart.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), heart_barrier_left, 4));

	int heart_barrier_right[4] = {
		346, 119,
		345, 145
	};
	r_heart.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), heart_barrier_right, 4));

	int flipper_base_left[16] = {
		125, 632,
		125, 722,
		132, 747,
		153, 768,
		216, 807,
		222, 790,
		147, 742,
		125, 701
	};
	l_flipper_base.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), flipper_base_left, 16));

	int flipper_base_right[16] = {
		516, 632,
		519, 685,
		518, 732,
		501, 759,
		433, 802,
		424, 787,
		495, 743,
		518, 703
	};
	r_flipper_base.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), flipper_base_right, 16));

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

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 15));
		circles.getLast()->data->listener = this;
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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	//App->audio->PlayFx(bonus_fx);

}



void ModuleSceneIntro::SpawnBall()
{
	balls.add(App->physics->CreateCircle(44, 791, 15));
	balls.getLast()->data->listener = this;
}

void ModuleSceneIntro::PlayerDeath() {
	

	lives = --lives;

	if (lives == 0) {
		defeat = true;
	}
	
	if (defeat == true) {

	}
}

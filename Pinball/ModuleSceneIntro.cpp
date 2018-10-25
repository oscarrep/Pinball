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
	scorebox = App->textures->Load("pinball/scoreboard.png");
	LflipperTexture = App->textures->Load("pinball/Lflipper.png");
	RflipperTexture = App->textures->Load("pinball/Rflipper.png");
	TopflipperTexture = App->textures->Load("pinball/Lflipper.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	flipper_fx = App->audio->LoadFx("pinball/fx/fx-flipper.ogg");
	heart_fx = App->audio->LoadFx("pinball/fx/fx-heart.ogg");

	bgm = App->audio->PlayMusic("pinball/fx/track.ogg");

	heartSensor1 = App->physics->CreateCircleSensor(279, 129, 15);
	heartSensor2 = App->physics->CreateCircleSensor(322, 129, 15);
	heartSensor3 = App->physics->CreateCircleSensor(364, 129, 15);

	//number sprites
	/*n0 = App->textures->Load("pinball/0.png");
	n1 = App->textures->Load("pinball/1.png");
	n1 = App->textures->Load("pinball/2.png");
	n1 = App->textures->Load("pinball/3.png");
	n1 = App->textures->Load("pinball/4.png");
	n1 = App->textures->Load("pinball/5.png");
	n1 = App->textures->Load("pinball/6.png");
	n1 = App->textures->Load("pinball/7.png");
	n1 = App->textures->Load("pinball/8.png");
	n1 = App->textures->Load("pinball/9.png");*/

	App->physics->CreateLFlipper();
	App->physics->CreateRFlipper();
	App->physics->CreateUpperFlipper();
	App->physics->CreatePiston();

	bumper1 = App->physics->CreateCircleStatic(295, 220, 25);
	bumper1->body->GetFixtureList()->SetDensity(10.0f);
	bumper1->body->GetFixtureList()->SetRestitution(1.5f);

	bumper2 = App->physics->CreateCircleStatic(280, 300, 25);
	bumper2->body->GetFixtureList()->SetDensity(10.0f);
	bumper2->body->GetFixtureList()->SetRestitution(1.5f);

	bumper3 = App->physics->CreateCircleStatic(372, 249, 25);
	bumper3->body->GetFixtureList()->SetDensity(10.0f);
	bumper3->body->GetFixtureList()->SetRestitution(1.5f);

	bumper4 = App->physics->CreateCircleStatic(380, 410, 25);
	bumper4->body->GetFixtureList()->SetDensity(10.0f);
	bumper4->body->GetFixtureList()->SetRestitution(1.5f);

	pond = App->physics->CreateCircleStatic(204, 449, 25);

	tree = App->physics->CreateCircleStatic(430, 127, 25);
	tree->body->GetFixtureList()->SetDensity(10.0f);
	tree->body->GetFixtureList()->SetRestitution(1.5f);

	Leftboost = App->physics->CreateRectangleStatic(105, 850, 30, 8);
	Leftboost->body->GetFixtureList()->SetDensity(50.0f);
	Leftboost->body->GetFixtureList()->SetRestitution(1.5f);

	Rightboost = App->physics->CreateRectangleStatic(537, 850, 30, 8);
	Rightboost->body->GetFixtureList()->SetDensity(50.0f);
	Rightboost->body->GetFixtureList()->SetRestitution(1.5f);

	backgroundrect.h = 907;
	backgroundrect.w = 609;

	scoreboxrect.h = 110;
	scoreboxrect.w = 270;
	
	lives = 6;
	
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
	balls->GetPosition(ballposx,ballposy);
	SDL_Rect ballposition;
	ballposition.w = 15;
	ballposition.h = 15;
	ballposition.x = ballposx;
	ballposition.y = ballposy;

	App->renderer->Blit(background, 0, 0, &backgroundrect);
	App->renderer->Blit(scorebox, 0, 0 , &scoreboxrect);
	App->renderer->Blit(ball, ballposx, ballposy, &ballposition);
	//App->renderer->Blit(LflipperTexture, , , &Lflipper);
	//App->renderer->Blit(background, , , &Rflipper);
	
	// Debug spawn ball
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 13));
		circles.getLast()->data->listener = this;
	}

	// Flipper inputs
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->physics->fbody->ApplyTorque(250.0, true);
		App->audio->PlayFx(flipper_fx);
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
		App->audio->PlayFx(flipper_fx);
	}
	else
	{
		if (App->physics->fbody2->IsAwake() && App->physics->fbody3->IsAwake())
		{
			App->physics->fbody2->ApplyTorque(250.0, false);
			App->physics->fbody3->ApplyTorque(250.0, false);
		}
	}

	// Piston input
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->physics->fbody4->ApplyForce(b2Vec2(0, 250), App->physics->fbody4->GetLocalCenter(), true);
	}
	else {
		if (App->physics->fbody4->IsAwake()) {
			App->physics->fbody4->ApplyForce(b2Vec2(0, -250), App->physics->fbody4->GetLocalCenter(), true);
		}
	}


	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// Player death / lost ball
	if (ballposy >= 907) {
		PlayerDeath();
	}
	/*
	while (defeat == false) {
		
		scoreaux = score;
		if (score > 0) {
			int count = 0;
			while (score > 0) {
				scorenumber = scoreaux % 10;
				scoreaux = scoreaux / 10;
				count++;
				if (scorenumber == 0) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n0, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n0, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n0, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n0, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n0, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n0, 0, 0, &sp6);
					}

				}
				if (scorenumber == 1) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n1, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n1, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n1, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n1, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n1, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n1, 0, 0, &sp6);
					}

				}
				
				if (scorenumber == 2) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n2, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n2, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n2, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n2, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n2, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n2, 0, 0, &sp6);
					}
				}
				
				if (scorenumber == 3) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n3, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n3, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n3, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n3, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n3, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n3, 0, 0, &sp6);
					}
				}
				if (scorenumber == 4) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n4, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n4, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n4, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n4, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n4, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n4, 0, 0, &sp6);
					}
				}
				
				if (scorenumber == 5) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n5, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n5, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n5, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n5, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n5, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n5, 0, 0, &sp6);
					}
				}
				if (scorenumber == 6) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n6, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n6, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n6, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n6, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n6, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n6, 0, 0, &sp6);
					}
				}
				if (scorenumber == 7) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n7, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n7, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n7, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n7, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n7, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n7, 0, 0, &sp6);
					}
				}
				if (scorenumber == 8) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n8, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n8, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n8, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n8, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n8, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n8, 0, 0, &sp6);
					}
				}
				if (scorenumber == 9) {
					while (scoreaux > 0) {
						scoreaux = scoreaux / 10;
						count++;
					}
					if (count == 1) {
						App->renderer->Blit(n9, 0, 0, &sp1);
					}
					if (count == 2) {
						App->renderer->Blit(n9, 0, 0, &sp2);
					}
					if (count == 3) {
						App->renderer->Blit(n9, 0, 0, &sp3);
					}
					if (count == 4) {
						App->renderer->Blit(n9, 0, 0, &sp4);
					}
					if (count == 5) {
						App->renderer->Blit(n9, 0, 0, &sp5);
					}
					if (count == 6) {
						App->renderer->Blit(n9, 0, 0, &sp6);
					}
				}
			}
		}

	}
*/
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA != NULL && bodyB != NULL)
	{
		if (bodyA == balls && bodyB == bumper1 || bodyA == bumper1 && bodyB == balls)
		{
			App->audio->PlayFx(bonus_fx);
			score += 10;
		}

		if (bodyA == balls && bodyB == bumper2 || bodyA == bumper2 && bodyB == balls)
		{
			App->audio->PlayFx(bonus_fx);
			score += 10;
		}

		if (bodyA == balls && bodyB == bumper3 || bodyA == bumper3 && bodyB == balls)
		{
			App->audio->PlayFx(bonus_fx);
			score += 10;
		}

		if (bodyA == balls && bodyB == bumper4 || bodyA == bumper4 && bodyB == balls)
		{
			App->audio->PlayFx(bonus_fx);
			score += 10;
		}

		if (bodyA == balls && bodyB == tree || bodyA == tree && bodyB == balls)
		{
			App->audio->PlayFx(bonus_fx);
			score += 30;
		}

		if (bodyB == heartSensor1)
		{
			App->audio->PlayFx(heart_fx);
			if (heart1 = false)
			{
				heart1 = true;
			}
		}

		if (bodyB == heartSensor2)
		{
			App->audio->PlayFx(heart_fx);
			if (heart2 = false)
			{
				heart2 = true;
			}
		}

		if (bodyB == heartSensor3)
		{
			App->audio->PlayFx(heart_fx);
			if (heart3 = false)
			{
				heart3 = true;
			}
		}
	}
}


PhysBody* ModuleSceneIntro::SpawnBall()
{
	balls = App->physics->CreateCircle(44, 791, 13);
	return balls;
}



void ModuleSceneIntro::PlayerDeath() {
	   
	App->physics->world->DestroyBody(balls->body);

	lives = lives - 1;
	
	if (lives > 0) {
		SpawnBall();
	}
	if (lives <= 0) {
	defeat = true;
	}
	   	 
}
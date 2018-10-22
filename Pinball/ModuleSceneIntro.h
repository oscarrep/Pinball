#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	////void CreateFlipperJoint();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> triangles;
	////p2List<PhysBody*>* Lflipper;
	////p2List<PhysBody*>* Rflipper;
	//PhysBody* sensor;
	bool sensed;
	////b2RevoluteJoint* flipperJoint;
	PhysBody* bouncer1;
	PhysBody* bouncer2;
	PhysBody* bouncer3;
	PhysBody* bouncer4;
	PhysBody* bouncer5;


	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* ball;
	SDL_Texture* flippers;
	SDL_Texture* piston;
	////SDL_Texture* LflipperTexture;
	////SDL_Texture* RflipperTexture;
	SDL_Rect backgroundrect;
	uint bonus_fx;
};

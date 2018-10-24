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
	void SpawnBall();
	void PlayerDeath();
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

	//elements
	PhysBody* balls;
	//PhysBody* Lflipper;
	//PhysBody* Rflipper;
	//PhysBody* UpperFlipper;
	PhysBody* bouncer1;
	PhysBody* bouncer2;
	PhysBody* bouncer3;
	PhysBody* bouncer4;
	PhysBody* bouncer5;
	PhysBody* piston;
	b2MouseJoint* mouse_joint;
	
	//contour
	p2List<PhysBody*> out;
	p2List<PhysBody*> r_inline;
	p2List<PhysBody*> l_inline;
	//p2List<PhysBody*> l_triangle;
	//p2List<PhysBody*> r_triangle;
	//p2List<PhysBody*> t_triangle;
	//p2List<PhysBody*> l_heart;
	//p2List<PhysBody*> r_heart;
	//p2List<PhysBody*> l_flipper_base;
	//p2List<PhysBody*> r_flipper_base;

	//Textures 
	SDL_Texture* pistontexture;
	SDL_Texture* background;
	SDL_Texture* ball;
	SDL_Texture* LflipperTexture;
	SDL_Texture* RflipperTexture;
	SDL_Texture* TopflipperTexture;
	SDL_Rect backgroundrect;

	//handout elements
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	//

	//values needed for the game
	int lives;
	int score;
	bool defeat = false;
	iPoint ballpos;

	uint bonus_fx;
};

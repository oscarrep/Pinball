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
	PhysBody* SpawnBall();
	void PlayerDeath();
	////void CreateFlipperJoint();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> triangles;
	////p2List<PhysBody*>* Lflipper;
	////p2List<PhysBody*>* Rflipper;
	//PhysBody* sensor;
	bool sensed;
	////b2RevoluteJoint* flipperJoint;


	//elements
	//PhysBody* Lflipper;
	//PhysBody* Rflipper;
	//PhysBody* UpperFlipper;
	PhysBody* balls;
	PhysBody* lefttribump;
	PhysBody* righttribump;
	PhysBody* bumper1;
	PhysBody* bumper2;
	PhysBody* bumper3;
	PhysBody* bumper4;
	PhysBody* tree;
	PhysBody* pond;
	PhysBody* Leftboost;
	PhysBody* Rightboost;

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
	SDL_Texture* piston;
	SDL_Texture* background;
	SDL_Texture* scorebox;
	SDL_Texture* ball;
	SDL_Texture* LflipperTexture;
	SDL_Texture* RflipperTexture;
	SDL_Texture* TopflipperTexture;
	SDL_Rect backgroundrect;
	SDL_Rect scoreboxrect;
	//handout elements
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	//

	//values needed for the game
	int ballposx,ballposy;
	int lflipposx, lflipposy;
	int lives=3;
	int score=0;
	int highscore = 0;
	bool defeat = false;


	uint bonus_fx;
};
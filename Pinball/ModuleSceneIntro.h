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
	//p2List<PhysBody*> balls;
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

	PhysBody* sensor;
	PhysBody* heartSensor1;
	PhysBody* heartSensor2;
	PhysBody* heartSensor3;
	PhysBody* bumperSensor1;
	PhysBody* bumperSensor2;
	PhysBody* bumperSensor3;
	PhysBody* bumperSensor4;
	PhysBody* bumperSensor5;

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

	//score textures
	/*SDL_Texture* n0;
	SDL_Texture* n1;
	SDL_Texture* n2;
	SDL_Texture* n3;
	SDL_Texture* n4;
	SDL_Texture* n5;
	SDL_Texture* n6;
	SDL_Texture* n7;
	SDL_Texture* n8;
	SDL_Texture* n9;
	SDL_Rect sp1;
	SDL_Rect sp2;
	SDL_Rect sp3;
	SDL_Rect sp4;
	SDL_Rect sp5;
	SDL_Rect sp6;*/


	//lives textures
	SDL_Texture* l1;
	SDL_Texture* l2;
	SDL_Texture* l3;
	SDL_Texture* l4;

	SDL_Rect life1;
	SDL_Rect life2;
	SDL_Rect life3;
	SDL_Rect life4;


	//values needed for the game
	int ballposx,ballposy;
	int lflipposx, lflipposy;
	int lives=3;
	int score=0;
	int scoreaux;
	int scorenumber;
	int highscore = 0;
	bool defeat = false;


	uint flipper_fx;
	uint heart_fx;
	uint bonus_fx;
	uint bgm;

	bool heart1 = false;
	bool heart2 = false;
	bool heart3 = false;
	bool bumper_fx1 = false;
	bool bumper_fx2 = false;
	bool bumper_fx3 = false;
	bool bumper_fx4 = false;
	bool bumper_fx5 = false;
};
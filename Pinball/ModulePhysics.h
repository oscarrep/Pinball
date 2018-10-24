#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateCircleStatic(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleStatic(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, Module* listener);
	PhysBody* CreateChain(int x, int y, int* points, int size);

	b2Body* CreateLFlipper();
	b2Body* CreateRFlipper();
	b2Body* CreateUpperFlipper();
	b2Body* CreatePiston();
	b2Body* fbody;
	b2Body* fbody2;
	b2Body* fbody3;
	b2Body* fbody4;

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	//void CreateLflipper(p2List<PhysBody*>* Lflipper);
	//void CreateRflipper(p2List<PhysBody*>* Rflipper);

	//pinball phisical contour
	p2List<PhysBody*> out;
	p2List<PhysBody*> r_inline;
	//p2List<PhysBody*> l_inline;
	p2List<PhysBody*> l_triangle;
	p2List<PhysBody*> r_triangle;
	p2List<PhysBody*> t_triangle;
	p2List<PhysBody*> l_base;
	p2List<PhysBody*> r_base;

	PhysBody* balls;
	PhysBody* Lflipper;
	PhysBody* Rflipper;
	PhysBody* Tflipper;
	/*PhysBody* bumper1;
	PhysBody* bumper2;
	PhysBody* bumper3;
	PhysBody* bumper4;
	PhysBody* pond;*/
	PhysBody* l_hearth;
	PhysBody* r_hearth;
	PhysBody* l_flipper_base1;
	PhysBody* r_flipper_base1;
	//PhysBody* l_triangle;
	//PhysBody* r_triangle;
	//PhysBody* t_triangle;
	//b2Vec2 l_flipper_base2;
	//b2Vec2 r_flipper_base2;


	b2World* world;

private:

	bool debug;
	b2Body* ground;

	// Mouse joint
	b2MouseJoint* mouse_joint;
	b2Body* body_clicked;

	b2RevoluteJoint* flipperJoint;
	//b2RevoluteJoint* RflipperJoint;

};
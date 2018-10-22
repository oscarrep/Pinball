#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// big static circle as "ground" in the middle of the screen
	/*int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* big_ball = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	big_ball->CreateFixture(&fixture);*/

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

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureB()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

//PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, Module *listener)
//{
//	b2BodyDef body;
//	body.type = b2_staticBody;
//	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//
//	b2Body* b = world->CreateBody(&body);
//
//	b2PolygonShape box;
//	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
//
//	b2FixtureDef fixture;
//	fixture.shape = &box;
//	fixture.density = 1.0f;
//	fixture.isSensor = true;
//
//	b->CreateFixture(&fixture);
//
//	PhysBody* pbody = new PhysBody();
//	pbody->body = b;
//	b->SetUserData(pbody);
//	pbody->width = width;
//	pbody->height = height;
//	pbody->listener = listener;
//
//	return pbody;
//}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Implement a simple mouse joint when a body is clicked for debug purposes
	bool mouse_down = (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN);
	bool mouse_repeat = (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT);
	bool mouse_up = (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP);

	b2Vec2 mouse_position(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
			if(mouse_down == true && body_clicked == NULL)
			{
				if(f->GetShape()->TestPoint(b->GetTransform(), mouse_position) == true)
					body_clicked = b;
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	if(body_clicked != NULL && mouse_joint == NULL)
	{
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body_clicked;
		def.target = mouse_position;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * body_clicked->GetMass();

		mouse_joint = (b2MouseJoint*) world->CreateJoint(&def);
	}

	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if(mouse_repeat == true && mouse_joint != NULL)
	{
		mouse_joint->SetTarget(mouse_position);
		App->renderer->DrawLine(
			METERS_TO_PIXELS(mouse_joint->GetAnchorA().x), METERS_TO_PIXELS(mouse_joint->GetAnchorA().y),
			METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y),
			255, 0, 0);

	}

	// TODO 4: If the player releases the mouse button, destroy the joint
	if(mouse_up == true && mouse_joint != NULL)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = NULL;
		body_clicked = NULL;
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}
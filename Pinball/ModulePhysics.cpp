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

	int outline[186] = {
		19, 904,
		19, 567,
		22, 401,
		28, 350,
		35, 311,
		45, 272,
		62, 232,
		85, 196,
		100, 168,
		122, 146,
		140, 125,
		171, 102,
		196, 82,
		229, 64,
		258, 54,
		286, 47,
		317, 42,
		335, 25,
		355, 10,
		399, 2,
		427, 1,
		444, 5,
		463, 12,
		479, 22,
		492, 32,
		508, 50,
		521, 66,
		535, 84,
		549, 104,
		559, 126,
		570, 150,
		582, 185,
		592, 227,
		597, 263,
		599, 304,
		597, 335,
		592, 368,
		584, 395,
		570, 426,
		559, 457,
		548, 477,
		538, 500,
		533, 527,
		534, 535,
		540, 541,
		548, 551,
		555, 565,
		557, 575,
		559, 597,
		557, 657,
		558, 718,
		557, 790,
		560, 858,
		517, 858,
		516, 802,
		378, 876,
		378, 903,
		379, 966,
		381, 1037,
		77, 1038,
		68, 892,
		65, 751,
		63, 642,
		63, 563,
		62, 477,
		65, 405,
		75, 338,
		96, 268,
		122, 221,
		150, 180,
		181, 154,
		217, 129,
		249, 112,
		254, 157,
		209, 174,
		160, 187,
		128, 222,
		101, 268,
		81, 341,
		82, 355,
		90, 358,
		134, 394,
		134, 527,
		116, 536,
		98, 547,
		89, 561,
		83, 578,
		82, 598,
		84, 861,
		128, 861,
		127, 803,
		261, 875,
		261, 903
	};

	int right_inline[134] = {
		400, 60,
		397, 74,
		386, 88,
		382, 101,
		382, 113,
		384, 135,
		386, 154,
		397, 164,
		417, 169,
		439, 177,
		457, 182,
		468, 186,
		477, 191,
		477, 239,
		464, 286,
		446, 324,
		419, 358,
		394, 384,
		405, 391,
		410, 403,
		461, 328,
		474, 297,
		483, 261,
		487, 227,
		488, 199,
		484, 180,
		445, 171,
		413, 159,
		408, 148,
		395, 138,
		392, 119,
		396, 102,
		401, 84,
		413, 82,
		418, 71,
		444, 77,
		466, 93,
		486, 110,
		504, 131,
		521, 156,
		535, 182,
		544, 210,
		553, 235,
		554, 262,
		555, 288,
		552, 318,
		544, 347,
		533, 374,
		523, 395,
		509, 418,
		500, 437,
		488, 455,
		496, 462,
		511, 439,
		524, 415,
		537, 394,
		548, 373,
		556, 346,
		565, 300,
		566, 264,
		562, 227,
		549, 191,
		532, 153,
		506, 117,
		462, 77,
		433, 60,
		412, 53
	};

	int left_base[18] = {
		135, 729,
		156, 746,
		196, 771,
		213, 781,
		203, 803,
		179, 790,
		153, 772,
		137, 758,
		129, 746
	};

	int left_triangle[8] = {
		175, 626,
		175, 708,
		221, 730,
		178, 625
	};

	int right_base[16] = {
		507, 728,
		486, 747,
		460, 764,
		432, 780,
		443, 803,
		475, 782,
		497, 767,
		514, 744
	};


	int right_triangle[8] = {
		469, 625,
		470, 711,
		423, 732,
		465, 626
	};

	int top_triangle[8] = {
		237, 320,
		280, 309,
		318, 346,
		237, 321
	};

	out.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), outline, 186));
	r_inline.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), right_inline, 134));
	r_base.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), right_base, 16));
	l_base.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), left_base, 18));
	r_triangle=App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), right_triangle, 8);
	l_triangle=App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), left_triangle, 8);
	t_triangle=App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), top_triangle, 8);
	r_triangle->body->GetFixtureList()->SetRestitution(2.0f);
	l_triangle->body->GetFixtureList()->SetRestitution(2.0f);



	/*bumper1 = App->physics->CreateCircleStatic(295, 220, 25);
	bumper2 = App->physics->CreateCircleStatic(280, 300, 25);
	bumper3 = App->physics->CreateCircleStatic(372, 249, 25);
	bumper4 = App->physics->CreateCircleStatic(380, 410, 25);
	pond = App->physics->CreateCircleStatic(204, 449, 25);*/

	l_hearth = App->physics->CreateRectangleStatic(300, 130, 13, 38);
	r_hearth = App->physics->CreateRectangleStatic(344, 130, 13, 38);

	l_flipper_base1 = App->physics->CreateRectangleStatic(126, 684, 13, 105);
	r_flipper_base1 = App->physics->CreateRectangleStatic(517, 684, 13, 105);




	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if (c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureB()->GetBody()->GetUserData();
			if (pb1 && pb2 && pb1->listener)
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

PhysBody* ModulePhysics::CreateCircleStatic(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
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

PhysBody* ModulePhysics::CreateRectangleStatic(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
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

PhysBody* ModulePhysics::CreateCircleSensor(int x, int y, int radius)

{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	b2Body* b = world->CreateBody(&body);
	b->SetBullet(true);

	b2CircleShape circle;
	circle.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &circle;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, Module *listener)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;
	pbody->listener = listener;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
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

PhysBody* ModulePhysics::CreateRFlipper() {
	/*b2BodyDef flipperBodyDef;
	flipperBodyDef.type = b2_dynamicBody;
	flipperBodyDef.position.Set(PIXEL_TO_METERS(400), PIXEL_TO_METERS(805));

	fbody = world->CreateBody(&flipperBodyDef);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(95) * 0.5f, PIXEL_TO_METERS(24) * 0.5f);

	b2FixtureDef flipperFixture;
	flipperFixture.shape = &box;
	flipperFixture.density = 2;
	fbody->CreateFixture(&flipperFixture);*/
	PhysBody* pbody = CreateRectangle(400,805, 95, 24);

	b2BodyDef flipperCircleDef;
	flipperCircleDef.position.Set(PIXEL_TO_METERS(410), PIXEL_TO_METERS(805));
	flipperCircleDef.type = b2_staticBody;

	b2Body* circleBody = world->CreateBody(&flipperCircleDef);

	b2CircleShape circle;
	circle.m_radius = PIXEL_TO_METERS(1);
	b2FixtureDef fixture;
	fixture.shape = &circle;
	circleBody->CreateFixture(&fixture);

	b2RevoluteJointDef flipperJointDef;
	flipperJointDef.bodyA = pbody->body;
	flipperJointDef.bodyB = circleBody;
	flipperJointDef.localAnchorA.Set(0.5, 0);
	flipperJointDef.localAnchorB.Set(0, 0);
	flipperJointDef.enableLimit = true;
	flipperJointDef.lowerAngle = -25 * DEGTORAD;
	flipperJointDef.upperAngle = 25 * DEGTORAD;
	flipperJointDef.maxMotorTorque = 10.0;
	flipperJointDef.motorSpeed = 5.0;
	flipperJointDef.enableMotor = true;
	world->CreateJoint(&flipperJointDef);

	return pbody;
}

PhysBody* ModulePhysics::CreateLFlipper() {
	/*b2BodyDef flipperBodyDef2;
	flipperBodyDef2.type = b2_dynamicBody;
	flipperBodyDef2.position.Set(PIXEL_TO_METERS(400), PIXEL_TO_METERS(805));

	fbody2 = world->CreateBody(&flipperBodyDef2);

	b2PolygonShape box2;
	box2.SetAsBox(PIXEL_TO_METERS(95) * 0.5f, PIXEL_TO_METERS(24) * 0.5f);

	b2FixtureDef flipperFixture2;
	flipperFixture2.shape = &box2;
	flipperFixture2.density = 2;
	fbody2->CreateFixture(&flipperFixture2);*/
	PhysBody* pbody = CreateRectangle(400, 805, 95,24);

	b2BodyDef flipperCircleDef2;
	flipperCircleDef2.position.Set(PIXEL_TO_METERS(235), PIXEL_TO_METERS(803));
	flipperCircleDef2.type = b2_staticBody;

	b2Body* circleBody2 = world->CreateBody(&flipperCircleDef2);

	b2CircleShape circle2;
	circle2.m_radius = PIXEL_TO_METERS(1);
	b2FixtureDef fixture2;
	fixture2.shape = &circle2;
	circleBody2->CreateFixture(&fixture2);

	b2RevoluteJointDef flipperJointDef2;
	flipperJointDef2.bodyA = pbody->body;
	flipperJointDef2.bodyB = circleBody2;
	flipperJointDef2.localAnchorA.Set(-0.5, 0);
	flipperJointDef2.localAnchorB.Set(0, 0);
	flipperJointDef2.enableLimit = true;
	flipperJointDef2.lowerAngle = -25 * DEGTORAD;
	flipperJointDef2.upperAngle = 25 * DEGTORAD;
	flipperJointDef2.maxMotorTorque = 10.0;
	flipperJointDef2.motorSpeed = 5.0;
	flipperJointDef2.enableMotor = true;
	world->CreateJoint(&flipperJointDef2);
	
	return pbody;
}

PhysBody* ModulePhysics::CreateUpperFlipper() {
	/*b2BodyDef flipperBodyDef3;
	flipperBodyDef3.type = b2_dynamicBody;
	flipperBodyDef3.position.Set(PIXEL_TO_METERS(105), PIXEL_TO_METERS(345));

	fbody3 = world->CreateBody(&flipperBodyDef3);

	b2PolygonShape box3;
	box3.SetAsBox(PIXEL_TO_METERS(85) * 0.5f, PIXEL_TO_METERS(24) * 0.5f);

	b2FixtureDef flipperFixture3;
	flipperFixture3.shape = &box3;
	flipperFixture3.density = 4;
	fbody3->CreateFixture(&flipperFixture3);*/
	PhysBody* pbody = CreateRectangle(105, 345, 85, 24);


	b2BodyDef flipperCircleDef3;
	flipperCircleDef3.position.Set(PIXEL_TO_METERS(105), PIXEL_TO_METERS(345));
	flipperCircleDef3.type = b2_staticBody;

	b2Body* circleBody3 = world->CreateBody(&flipperCircleDef3);

	b2CircleShape circle3;
	circle3.m_radius = PIXEL_TO_METERS(1);
	b2FixtureDef fixture3;
	fixture3.shape = &circle3;
	circleBody3->CreateFixture(&fixture3);

	b2RevoluteJointDef flipperJointDef3;
	flipperJointDef3.bodyA =pbody->body;
	flipperJointDef3.bodyB = circleBody3;
	flipperJointDef3.localAnchorA.Set(-0.5, 0);
	flipperJointDef3.localAnchorB.Set(0, 0);
	flipperJointDef3.enableLimit = true;
	flipperJointDef3.lowerAngle = -40 * DEGTORAD;
	flipperJointDef3.upperAngle = 25 * DEGTORAD;
	flipperJointDef3.maxMotorTorque = 10.0;
	flipperJointDef3.motorSpeed = 5.0;
	flipperJointDef3.enableMotor = true;
	world->CreateJoint(&flipperJointDef3);

	return pbody;

}

PhysBody* ModulePhysics::CreatePiston() {
	/*b2BodyDef flipperBodyDef4;
	flipperBodyDef4.type = b2_dynamicBody;
	flipperBodyDef4.position.Set(PIXEL_TO_METERS(40), PIXEL_TO_METERS(900));

	fbody4 = world->CreateBody(&flipperBodyDef4);

	b2PolygonShape box4;
	box4.SetAsBox(PIXEL_TO_METERS(25) * 0.5f, PIXEL_TO_METERS(20) * 0.5f);

	b2FixtureDef flipperFixture4;
	flipperFixture4.shape = &box4;
	flipperFixture4.density = 0;
	fbody4->CreateFixture(&flipperFixture4);*/
	PhysBody* pbody = CreateRectangle(40, 900, 25, 20);

	b2BodyDef quad;
	quad.position.Set(PIXEL_TO_METERS(40), PIXEL_TO_METERS(905));
	quad.type = b2_staticBody;

	b2Body* quadbody = world->CreateBody(&quad);

	b2PolygonShape quad2;
	quad2.SetAsBox(PIXEL_TO_METERS(25) * 0.5f, PIXEL_TO_METERS(20) * 0.5f);
	b2FixtureDef fixture4;
	fixture4.shape = &quad2;
	quadbody->CreateFixture(&fixture4);

	b2PrismaticJointDef prismaticJointDef4;
	prismaticJointDef4.bodyA = quadbody;
	prismaticJointDef4.bodyB = pbody->body;
	prismaticJointDef4.collideConnected = false;
	prismaticJointDef4.localAxisA.Set(0, 1);
	prismaticJointDef4.localAxisA.Normalize();
	prismaticJointDef4.localAnchorA.Set(0, 0);
	prismaticJointDef4.localAnchorB.Set(0, 0);
	prismaticJointDef4.enableLimit = true;
	prismaticJointDef4.lowerTranslation = -1.0;
	prismaticJointDef4.upperTranslation = 1.0;
	prismaticJointDef4.referenceAngle = 0 * DEGTORAD;
	prismaticJointDef4.enableMotor = true;
	prismaticJointDef4.maxMotorForce = 0;
	prismaticJointDef4.motorSpeed = -200;
	world->CreateJoint(&prismaticJointDef4);
	
	return pbody;

}

update_status ModulePhysics::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug)
		return UPDATE_CONTINUE;

	// Implement a simple mouse joint when a body is clicked for debug purposes
	bool mouse_down = (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN);
	bool mouse_repeat = (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT);
	bool mouse_up = (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP);

	b2Vec2 mouse_position(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
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

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
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

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
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
			if (mouse_down == true && body_clicked == NULL)
			{
				if (f->GetShape()->TestPoint(b->GetTransform(), mouse_position) == true)
					body_clicked = b;
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	if (body_clicked != NULL && mouse_joint == NULL)
	{
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body_clicked;
		def.target = mouse_position;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * body_clicked->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (mouse_repeat == true && mouse_joint != NULL)
	{
		mouse_joint->SetTarget(mouse_position);
		App->renderer->DrawLine(
			METERS_TO_PIXELS(mouse_joint->GetAnchorA().x), METERS_TO_PIXELS(mouse_joint->GetAnchorA().y),
			METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y),
			255, 0, 0);

	}

	// TODO 4: If the player releases the mouse button, destroy the joint
	if (mouse_up == true && mouse_joint != NULL)
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

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
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

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
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

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}
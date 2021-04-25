#include "Test.h"
#include <iostream>
#include "Quadtree.h"




//Quadtree* CreateQuadTree(vector<CGameObject*>* entity_list)
//{
//	// Init base game region for detecting collision
//	RECT base = { 0, 0, 800, 600 };
//	Quadtree* quadtree = new Quadtree(1, &base);
//
//	for (auto i = entity_list->begin(); i != entity_list->end(); i++)
//		quadtree->Insert(*i);
//
//	return quadtree;
//}

Test::Test()
{
	SetState("indie");
	this->vx = .05;
	//this->dx = 1;
	//this->dy = 1;
}

//void Test::GetBoundingBox(float& left, float& top, float& right, float& bottom)
//{
//	/*left = x;
//	top = y;
//	right = x + KOOPAS_BBOX_WIDTH;
//
//	if (state == KOOPAS_STATE_DIE)
//		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
//	else
//		bottom = y + KOOPAS_BBOX_HEIGHT;*/
//}

void Test::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	//DebugOut(L"[INFOR] HAPPEN!!!!! %s\n", IntToLPCWSTR(this->dx));

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	RECT base = { 0, 0, 800, 600 };
	Quadtree* quadtree = new Quadtree(1, &base);

	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
		quadtree->Insert(*i);


	//Quadtree* quadtree = CreateQuadTree(coObjects);

	vector<CGameObject*>* return_objects_list = new vector<CGameObject*>();

	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		//Get all objects that can collide with current entity
		quadtree->Retrieve(return_objects_list, *i);
		for (auto x = return_objects_list->begin(); x != return_objects_list->end(); x++)
		{

			//if (IsCollide(*i, *x))  // Your algorithm about Collision Detection
			//{
			//	// Do something here
			//}
			LPCOLLISIONEVENT e = SweptAABBEx(*x);

			if (e->t > 0 && e->t <= 1.0f) {

				//coEvents.push_back(e);
				DebugOut(L"[INFOR] HAPPEN!!!!!\n");
				coEvents.push_back(e);
				//x += dx + nx * 0.4f;
				//y += dy + ny * 0.4f;

				//if (nx != 0) vx = 0;
				//if (ny != 0) vy = 0;
			}
			else {
				//DebugOut(L"[INFOR] NO HAPPEN!!!!!\n");
				delete e;
			}
		}

		return_objects_list->clear();
	}

	////quadtree->Release();

	//return_objects_list->clear();
	delete return_objects_list;
	delete quadtree;

	if (coEvents.size() == 0) {

		x += dx;
		y += dy;
		if (vx < 0 && x < 0) {
			x = 0; vx = -vx;
		}

		if (vx > 0 && x > 290) {
			x = 290; vx = -vx;
		}
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		if (rdx != 0 && rdx != dx)
			x += nx * abs(rdx);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		DebugOut(L"[INFOR] Time to stop!!!!!\n");


		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	}
}

void Test::Render()
{

	//int ani = KOOPAS_ANI_WALKING_LEFT;
	/*if (state == "indie") {

	}*/
	if (vx == 0) this->state = "indie";
	if (vx > 0) this->state = "running-right";
	if (vx < 0) this->state = "running-left";

	int width = 0;
	int height = 0;
	animations_set.Get(type).at(state)->Render(x, y, 255, width, height);

	SetSize(width, height);


	RenderBoundingBox();
}

void Test::SetState(string state)
{
	CGameObject::SetState(state);

	if (state == "running-right") {
		vx = 0.15f;
		nx = 1;
	}
	else if (state == "running-left") {
		vx = -0.15f;
		nx = -1;
	}
	else if (state == "indie") {
		vx = 0;

	}

}

void Test::ParseFromJson(json data) {


	int id = stoi(string(data["id"])); //object id;


	string type = to_string(data["type"]); //object type;

	float x = data["x"]; //object x;
	float y = data["y"]; //object y;

	LPCWSTR sprite = ToLPCWSTR(string(data["sprite"]));
	LPCWSTR texture = ToLPCWSTR(string(data["texture"]));
	LPCWSTR animation = ToLPCWSTR(string(data["animation"]));

	// set inital position
	id = id;
	this->type = type;
	this->x = x;
	this->y = y;
	//
	D3DCOLOR transcolor;
	SetTexture(texture, D3DCOLOR_XRGB(255, 0, 255));
	ParseSpriteFromJson(sprite);

	ParseAnimationFromJson(animation);

	// set active animation set;
	SetActiveAnimationSet(type);
}

void Test::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (type == "1")
	{
		right = x + width;
		bottom = y + height;
	}
	else
	{
		right = x + width;
		bottom = y + height;

		//right = x + MARIO_SMALL_BBOX_WIDTH;
		//bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

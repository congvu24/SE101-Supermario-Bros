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
	width = 42;
	height = 54;
	SetState("indie");
	//this->v.x = .05;
	//v = Vector(0.05, 0.05);
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
	v = v + g * dt;
	if (v.y > 0.35f) v.y = 0.35f;

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	//DebugOut(L"[INFOR] HAPPEN!!!!! %s\n", IntToLPCWSTR(this->dx));

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	RECT base = { 0, 0, 200, 200 };
	Quadtree* quadtree = new Quadtree(1, &base);

	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
		quadtree->Insert(*i);


	//Quadtree* quadtree = CreateQuadTree(coObjects);

	vector<CGameObject*>* return_objects_list = new vector<CGameObject*>();

	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if (*i != this) {
			LPCOLLISIONEVENT e = SweptAABBEx(*i);

			if (e->t > 0 && e->t <= 1.0f) {
				DebugOut(L"[INFOR] HAPPEN!!!!!\n");
				coEvents.push_back(e);
			}
			else {
				delete e;
			}
		}



		//Get all objects that can collide with current entity
		//quadtree->Retrieve(return_objects_list, this);
		//if (return_objects_list->size() > 0) {
		//	DebugOut(L"[INFOR] Co kha nang!!!!! %s \n", IntToLPCWSTR(return_objects_list->size()));

		//}
		//for (auto x = return_objects_list->begin(); x != return_objects_list->end(); x++)
		//{

		//	//if (IsCollide(*i, *x))  // Your algorithm about Collision Detection
		//	//{
		//	//	// Do something here
		//	//}
		//	if (*x != this) {
		//		LPCOLLISIONEVENT e = SweptAABBEx(*x);

		//		if (e->t > 0 && e->t <= 1.0f) {

		//			//coEvents.push_back(e);
		//			DebugOut(L"[INFOR] HAPPEN!!!!!\n");
		//			coEvents.push_back(e);
		//			//x += dx + nx * 0.4f;
		//			//y += dy + ny * 0.4f;

		//			//if (nx != 0) vx = 0;
		//			//if (ny != 0) vy = 0;
		//		}
		//		else {
		//			//DebugOut(L"[INFOR] NO HAPPEN!!!!!\n");
		//			delete e;
		//		}
		//	}
		//}

		//return_objects_list->clear();
	}

	////quadtree->Release();

	//return_objects_list->clear();
	delete return_objects_list;
	delete quadtree;

	if (coEvents.size() == 0) {

		/*	p.x += d.x;
			p.y += d.y;*/
		p = p + d;

		/*if (v.x < 0 && p.x < 0) {
			p.x = 0; v.x = -v.x;
		}

		if (v.x > 0 && p.x > 290) {
			p.x = 290; v.x = -v.x;
		}*/
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		if (rdx != 0 && rdx != d.x)
			p.x += nx * abs(rdx);

		// block every object first!
		p.x += min_tx * d.x + nx * 0.4f;
		p.y += min_ty * d.y + ny * 0.4f;

		if (nx != 0) v.x = 0;
		if (ny != 0) v.y = 0;

		DebugOut(L"[INFOR] Time to stop!!!!!\n");


		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	}
}

void Test::Render()
{

	//int ani = KOOPAS_ANI_WALKING_LEFT;
	/*if (state == "indie") {

	}*/
	//if (v.x == 0) this->state = "indie";
	//if (v.x > 0) this->state = "running-right";
	//if (v.x < 0) this->state = "running-left";

	float width = 0;
	float height = 0;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);

	//SetSize(width, height);


	RenderBoundingBox();
}

void Test::SetState(string state)
{

	if (state == "running-right") {
		v.x = 0.15f;
		nx = 1;
	}
	else if (state == "running-left") {
		v.x = -0.15f;
		nx = -1;
	}
	else if (state == "running-up") {
		v.y = -0.15f;
		ny = -1;
	}
	else if (state == "running-down") {
		v.y = 0.15f;
		ny = -1;
	}
	else if (state == "jumping" && this->state != "jumping") {
		v.y = -0.4f;
		ny = -1;
	}
	else if (state == "indie") {
		v.x = 0;
		//v.y = 0;
	}

	CGameObject::SetState(state);

}

//void Test::ParseFromJson(json data) {
//
//
//}

void Test::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;


	right = p.x + width;
	bottom = p.y + height;

	//right = x + MARIO_SMALL_BBOX_WIDTH;
	//bottom = y + MARIO_SMALL_BBOX_HEIGHT;
}

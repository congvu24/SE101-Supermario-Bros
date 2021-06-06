#include "Test.h"
#include <iostream>
#include "Quadtree.h"
#include "MiniPortal.h"
#include "Game.h"


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

	width = 24;
	height = 30;
	SetState("indie");
	//this->v = Vector(0.01, 0.01);
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

	RECT base = { 0, 0, 800, 600 };
	Quadtree* quadtree = new Quadtree(1, &base);

	/*for (auto i = coObjects->begin(); i != coObjects->end(); i++)
		if (*i != this) {
			quadtree->Insert(*i);
		}

	*/
	int check = 0;


	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		/*float left = this->p.x - 200;
		float top = this->p.y - 200;
		float right = this->p.x + this->width + 200;
		float bottom = this->p.y + this->height + 200;

		LPGAMEOBJECT obj = *i;
		if (obj->p.x < left || obj->p.y < top || obj->p.x + obj->width > right || obj->p.y + obj->height > bottom) break;*/

		if ((*i)->isAllowCollision == true) {
			check++;
			LPCOLLISIONEVENT e = SweptAABBEx(*i);

			if (e->t > 0 && e->t <= 1.0f) {
				coEvents.push_back(e);
			}
			else {
				delete e;
			}
		}

	}

	//DebugOut(L"[INFOR] tat ca object!!!!! %s \n", IntToLPCWSTR(coObjects->size()));
	//DebugOut(L"[INFOR] checked object!!!!! %s \n", IntToLPCWSTR(check));


	//}




	//vector<CGameObject*>* return_objects_list = new vector<CGameObject*>();

	//for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	//	if (*i != this) {
	//		Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;

	//		if (camera->isInCamPosition(*i, 100) == true)
	//			return_objects_list->push_back(*i);
	//	}

	////quadtree->Retrieve(return_objects_list, this);

	//if (return_objects_list->size() > 0) {
	//	DebugOut(L"[INFOR] Co kha nang!!!!! %s \n", IntToLPCWSTR(return_objects_list->size()));

	//}

	//////Get all objects that can collide with current entity

	//for (auto x = return_objects_list->begin(); x != return_objects_list->end(); x++)
	//{
	//	if (*x != this) {
	//		LPCOLLISIONEVENT e = SweptAABBEx(*x);

	//		if (e->t > 0 && e->t <= 1.0f) {
	//			DebugOut(L"[INFOR] HAPPEN!!!!!\n");
	//			coEvents.push_back(e);
	//		}
	//		else {
	//			delete e;
	//		}
	//	}
	//}

	//return_objects_list->clear();

	////////quadtree->Release();

	//////return_objects_list->clear();
	//delete return_objects_list;
	//delete quadtree;

	if (coEvents.size() == 0) {

		p = p + d;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		if (rdx != 0 && rdx != d.x)
			p.x += nx * abs(rdx);

		// block every object first!
		p.x += min_tx * d.x + nx * 0.4f;
		p.y += min_ty * d.y + ny * 0.4f;

		Vector tempV = v;

		if (nx != 0) v.x = 0;
		if (ny != 0) v.y = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			if (coEvents[i]->obj->name != "") {

				DebugOut(L"[INFOR] Collision with %s !!!!!\n", ToLPCWSTR(coEvents[i]->obj->name));

				coEvents[i]->obj->HandleCollision(coEvents[i]);
			}
			if (coEvents[i]->obj->name == "RectPlatform") {
				v = tempV;
				p.x = p.x + d.x;
			}
			if (coEvents[i]->obj->name == "Leaf") {
				this->type = "2"; // move to next size

				this->p.x = p.x - (42 - this->width);
				this->p.y = p.y - (54 - this->height);

				width = 42;
				height = 54;
			}
			if (coEvents[i]->obj->name == "MiniPortal") {

				MiniPortal* start = dynamic_cast<MiniPortal*>(coEvents[i]->obj);
				MiniPortal* destination = NULL;
				         
				for (auto i = coObjects->begin(); i != coObjects->end(); i++)
				{
					if (MiniPortal* obj = dynamic_cast<MiniPortal*>(*i)) {
						if (obj->portalName == start->portalName && obj->type == "Out") {
							destination = obj;
							break;
						}
					}
				}

				if (destination != NULL) {
					CGame::GetInstance()->GetCurrentScene()->getCamera()->setCamPos(destination->camera_x, destination->camera_y);
					p = destination->p;
				}

			}
		}


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

	float w = width;
	float h = height;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h);


	/*int changex = p.x - (w - this->width);
	int changey = p.y - (h - this->height);
	if (changex >= 3) {
		p.x = p.x - (w - this->width);
		this->width = w;

	}
	if (changey >= 3) {
		p.y = p.y - (h - this->height);
		this->height = h;

	}*/

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
		//ny = -1;
	}
	else if (state == "running-down") {
		v.y = 0.15f;
		//ny = -1;
	}
	else if (state == "jumping" && this->state != "jumping") {
		v.y = -0.4f;
		//ny = -1;
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


void Test::HandleCollision(LPCOLLISIONEVENT e) {

}
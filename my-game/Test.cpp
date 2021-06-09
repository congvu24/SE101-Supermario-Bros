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
}


void Test::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	v = v + g * dt;
	if (v.y > 0.35f) v.y = 0.35f;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if ((*i)->isAllowCollision == true) {
			LPCOLLISIONEVENT e = SweptAABBEx(*i);

			if (e->t > 0 && e->t <= 1.0f) {
				coEvents.push_back(e);
			}
			else {
				delete e;
			}
		}
	}

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
				vector<CGameObject*>* allObjectOfSence = &(CGame::GetInstance()->GetCurrentScene()->objects);
				for (auto i = allObjectOfSence->begin(); i != allObjectOfSence->end(); i++)
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
	float w = width;
	float h = height;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h);
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
	}
	else if (state == "running-down") {
		v.y = 0.15f;
	}
	else if (state == "jumping" && this->state != "jumping") {
		v.y = -0.4f;
	}
	else if (state == "indie") {
		v.x = 0;
	}

	CGameObject::SetState(state);

}

void Test::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Test::HandleCollision(LPCOLLISIONEVENT e) {

}
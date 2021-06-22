#include "Test.h"
#include <iostream>
#include "Quadtree.h"
#include "MiniPortal.h"
#include "PlayScence.h"
#include "Game.h"


#define VX_SMALL 0.15f
#define VY_SMALL 0.4f
#define VY_BIG 0.5f


Test::Test()
{
	width = 24;
	height = 30;
	SetState("indie");
	max_move_x = VX_SMALL;
	max_move_y = VY_SMALL;
}


void Test::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	v = v + g * dt;
	if (v.y > max_move_y) v.y = max_move_y;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	DebugOut(L"[INFOR] Check collision with: %s !!!!!\n", IntToLPCWSTR(coObjects->size()));


	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if (state != "die") {
			if ((*i)->isAllowCollision == true && (*i)->isBlockPlayer == true) {
				LPCOLLISIONEVENT e = SweptAABBEx(*i);
				if (e->t > 0 && e->t <= 1.0f) {
					coEvents.push_back(e);
				}
				else {
					delete e;
				}
			}
		}
		else {
			if ((*i)->name == "Death") {
				LPCOLLISIONEVENT e = SweptAABBEx(*i);
				if (e->t > 0 && e->t <= 1.0f) {
					coEvents.push_back(e);
				}
				else {
					delete e;
				}
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

		if (rdx != 0 && rdx != d.x)
			p.x += nx * abs(rdx);


		Vector tempV = v;
		if (nx != 0) v.x = 0;
		if (ny != 0) v.y = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			if (ny < 0) canJump = true;

			if (coEventsResult[i]->obj->isBlockPlayer == true) {

			}

			DebugOut(L"[INFOR] Collision ny %s !!!!!\n", ToLPCWSTR(to_string(ny)));
			if (coEventsResult[i]->obj->name != "") {

				//DebugOut(L"[INFOR] Collision with %s !!!!!\n", ToLPCWSTR(coEventsResult[i]->obj->name));

				coEventsResult[i]->obj->HandleCollision(coEventsResult[i]);

			}

			if (coEventsResult[i]->obj->name == "Goomba") {
				if (coEventsResult[i]->nx != 0) {
					this->Die();
				}
			}

			if (coEventsResult[i]->obj->name == "RectPlatform") {
				v = tempV;
				p.x = p.x + d.x;
			}

			if (coEventsResult[i]->obj->name == "MiniPortal") {
				MiniPortal* start = dynamic_cast<MiniPortal*>(coEventsResult[i]->obj);
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

			if (coEventsResult[i]->obj->name == "Death") {
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GameOver();
			}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		p.x += min_tx * d.x + nx * 0.4f;
		p.y += min_ty * d.y + ny * 0.4f;

		coEventsResult.clear();
		coEvents.clear();
	}
}

void Test::Render()
{
	float w = width;
	float h = height;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h);
	p.y = p.y - (h - this->height);
	p.x = p.x - (w - this->width);
	width = w;
	height = h;
	RenderBoundingBox();
}

void Test::SetState(string state)
{

	if (isReadyChangeState == true) {

		if (state == "running-right") {
			v.x = max_move_x;
			nx = 1;
			if (canJump == true)
				CGameObject::SetState(state);
			else CGameObject::SetState("jumping-right");

		}
		else if (state == "running-left") {
			v.x = -max_move_x;
			nx = -1;
			if (canJump == true)
				CGameObject::SetState(state);
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "sitting" && type != "1") {
			if (this->state == "indie") {
				v.x = 0;
				v.y = 0;
				if (nx > 0)
					CGameObject::SetState("sitting-right");
				else
					CGameObject::SetState("sitting-left");
			}
		}
		else if (state == "jumping" && canJump == true) {
			v.y = -max_move_y;
			canJump = false;
			if (nx > 0)
				CGameObject::SetState("jumping-right");
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "indie" && canJump == true) {
			v.x = 0;
			if (nx < 0)
				CGameObject::SetState("indie-left");
			else
				CGameObject::SetState("indie-right");
		}
		else if (state == "die") {
			v.y = -0.3f;
			CGameObject::SetState(state);
			isReadyChangeState = false;
		}

	}

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

void Test::Die() {
	// handle event die here
	SetState("die");
}

void Test::Transform() {
	this->type = "2"; // move to next size
	this->p.x = p.x - (42 - this->width);
	this->p.y = p.y - (54 - this->height);
	width = 42;
	height = 54;
	max_move_y = VY_BIG;
}
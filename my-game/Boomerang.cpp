#include "Boomerang.h"
#include "Vector.h"
#include "Mario.h"
#include "BoomerangBrother.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Boomerang::texture = NULL;
unordered_map<string, LPSPRITE> Boomerang::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Boomerang::all_animations; //save all animations
CAnimationSets Boomerang::animations_set; //save all the animation sets
json Boomerang::data = NULL;

Boomerang::Boomerang()
{
	point = 0;
	SetState("running");
	isBlockPlayer = false;
	isAllowCollision = false;
	v = Vector(0.10f, 0.10f);
	g = Vector(0, 0);
	isUniversal = true;
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (count < 1.0f) {
		count += 1.0f * 0.01f;
		D3DXVECTOR3 start = D3DXVECTOR3(oldP.x, oldP.y, 0);
		D3DXVECTOR3 end = D3DXVECTOR3(destination.x, destination.y, 0);
		D3DXVECTOR3 control;
		if (moveDirection == BoomerangDirection::Forward) {
			if (nx >= 0) {
				control = D3DXVECTOR3(oldP.x + ANCHOR_DISTANCE_X, oldP.y - ANCHOR_DISTANCE_Y, 0);
			}
			else
				control = D3DXVECTOR3(oldP.x - ANCHOR_DISTANCE_X, oldP.y - ANCHOR_DISTANCE_Y, 0);
		}
		else
		{
			if (nx >= 0) {
				control = D3DXVECTOR3(oldP.x - ANCHOR_DISTANCE_X, oldP.y + ANCHOR_DISTANCE_Y, 0);
			}
			else
				control = D3DXVECTOR3(oldP.x + ANCHOR_DISTANCE_X, oldP.y + ANCHOR_DISTANCE_Y, 0);
		}

		D3DXVECTOR3 out;
		D3DXVec3Lerp(&out, &start, &control, count);
		D3DXVECTOR3 out2;
		D3DXVec3Lerp(&out2, &control, &end, count);
		D3DXVECTOR3 out3;
		D3DXVec3Lerp(&out3, &out, &out2, count);

		p.x = out3.x;
		p.y = out3.y;
	}
	else if (moveDirection != BoomerangDirection::Goback) {
		moveDirection = BoomerangDirection::Goback;
		Vector temp = oldP;
		oldP = destination;
		destination = temp;
		count = 0;
	}
	else {
		SetState("hidden");
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{

		if (Mario* obj = dynamic_cast<Mario*>((*i))) {
			LPCOLLISIONEVENT e = SweptAABBEx(*i);

			if (e->t > 0 && e->t <= 1.0f) {
				coEvents.push_back(e);
			}
			else {
				delete e;
			}
		}
		if (BoomerangBrother* obj = dynamic_cast<BoomerangBrother*>((*i))) {
			LPCOLLISIONEVENT e = SweptAABBEx(*i);

			if (e->t > 0 && e->t <= 1.0f) {
				coEvents.push_back(e);
			}
			else {
				delete e;
			}
		}
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

	float min_tx, min_ty, nx = 0, ny;
	float rdx = 0;
	float rdy = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	if (rdx != 0 && rdx != d.x)
		p.x += nx * abs(rdx);

	// block every object first!

	for (UINT i = 0; i < coEventsResult.size(); i++) {

		if (Mario* obj = dynamic_cast<Mario*>(coEventsResult[i]->obj)) {
			obj->Die();
		}
		if (BoomerangBrother* obj = dynamic_cast<BoomerangBrother*>(coEventsResult[i]->obj)) {
			obj->SetAction(BoomerangBrotherAction::MOVING);
		}
		if (coEventsResult[i]->obj->name == "Death") {
			SetState("hidden");
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	coEventsResult.clear();
	coEvents.clear();
}

void Boomerang::SetState(string state)
{
	if (state == "hidden") {
		parent->SetAction(BoomerangBrotherAction::MOVING);
	}

	CGameObject::SetState(state);

}

void Boomerang::HandleCollision(LPCOLLISIONEVENT e) {
}

void Boomerang::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (Mario* player = dynamic_cast<Mario*>(event->obj)) {
		KillPlayer(player);
		SetState("hidden");
	}
}

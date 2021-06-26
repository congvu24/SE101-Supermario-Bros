#include "Goomba.h"
#include "Vector.h"
#include "Test.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Goomba::texture = NULL;
unordered_map<string, LPSPRITE> Goomba::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Goomba::all_animations; //save all animations
CAnimationSets Goomba::animations_set; //save all the animation sets
json Goomba::data = NULL;

Goomba::Goomba()
{
	SetState("running");
	v = Vector(0.05f, 0);
	isAllowCollision = true;
	isBlockPlayer = false;
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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


		if (nx != 0) v.x = -v.x;
		if (ny != 0) v.y = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			HandleCollision(coEventsResult[i]);
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	coEvents.clear();
	coEventsResult.clear();
}


void Goomba::SetState(string state)
{
	if (state == "running") {
	}
	else if (state == "die") {
		v = Vector(0, -0.15f);
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void Goomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Goomba::HandleCollision(LPCOLLISIONEVENT e) {

	LPGAMEOBJECT obj = e->obj;
	if (Test* player = dynamic_cast<Test*>(obj)) {
		if (e->ny > 0) {
			SetState("die");
		}
		else if (e->nx != 0 && state != "die") {
			//player->Die();
		}
	}
}

void Goomba::Die() {
	this->SetState("die");
	isAllowCollision = false;
}
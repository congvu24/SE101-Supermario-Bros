#include "Goomba.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Goomba::texture = NULL;
unordered_map<string, LPSPRITE> Goomba::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Goomba::all_animations; //save all animations
CAnimationSets Goomba::animations_set; //save all the animation sets
json Goomba::data = NULL;

Goomba::Goomba()
{
	SetState("running");
	width = 14;
	height = 16;
	v = Vector(0.05f, 0);
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
		if ((*i)->isAllowCollision == true && (*i)->name == "RectCollision") {
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

		if (rdx != 0 && rdx != d.x)
			p.x += nx * abs(rdx);

		// block every object first!
		p.x += min_tx * d.x + nx * 0.4f;
		p.y += min_ty * d.y + ny * 0.4f;

		if (nx != 0) v.x = -v.x;
		if (ny != 0) v.y = 0;


		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	}
}

//void Goomba::Render() {
//	if (state == "hidden") return;
//	else {
//		float width = 0;
//		float height = 0;
//		Goomba::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
//		RenderBoundingBox();
//	}
//}

void Goomba::SetState(string state)
{
	if (state == "running") {
	}
	else if (state == "die") {
		v = Vector(0, 0);
		//g = Vector(0, 0);
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
	if (isAllowCollision == true && state != "hidden" && e->ny < 0) {
		isAllowCollision = false;
		SetState("die");
	}
}
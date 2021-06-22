#include "Mushroom.h"
#include "Vector.h"
#include "Test.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Mushroom::texture = NULL;
unordered_map<string, LPSPRITE> Mushroom::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Mushroom::all_animations; //save all animations
CAnimationSets Mushroom::animations_set; //save all the animation sets
json Mushroom::data = NULL;

Mushroom::Mushroom()
{
	SetState("fromMisteryBox");
	isBlockPlayer = false;
}

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	v = v + g * dt;
	if (v.y > 0.15f) v.y = 0.15f;

	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if (p.y + height <= oldP.y - 20) {
		SetState("running");
	}

	coEvents.clear();
	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(*i);

		if (e->t > 0 && e->t <= 1.0f) {
			coEvents.push_back(e);
		}
		else {
			delete e;
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


		for (UINT i = 0; i < coEventsResult.size(); i++) {

			if (Test* obj = dynamic_cast<Test*>(coEventsResult[i]->obj)) {
				obj->Transform();
				SetState("hidden");
			}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	}

	//}

}

//void Mushroom::Render() {
//	if (state == "hidden") return;
//	else {
//		float width = 0;
//		float height = 0;
//		Mushroom::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
//		RenderBoundingBox();
//	}
//}

void Mushroom::SetState(string state)
{
	if (state == "running") {
		v.x = -0.05f;
		v.y = 0;
		g.y = 0.001f;
	}
	else if (state == "fromMisteryBox") {
		oldP = p;
		v = Vector(0, -0.15f);
		g = Vector(0, 0);
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void Mushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Mushroom::HandleCollision(LPCOLLISIONEVENT e) {
	SetState("hidden");
}
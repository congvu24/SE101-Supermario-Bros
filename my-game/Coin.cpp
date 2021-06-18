#include "Coin.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Coin::texture = NULL;
unordered_map<string, LPSPRITE> Coin::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Coin::all_animations; //save all animations
CAnimationSets Coin::animations_set; //save all the animation sets
json Coin::data = NULL;

Coin::Coin()
{
	SetState("running");
	width = 14;
	height = 16;
	p = Vector(0, 0);

}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (state != "hidden") {
	v = v + g * dt;
	if (v.y > 1.0f) v.y = 1.0f;
	CGameObject::Update(dt, coObjects);
	if (state == "fromMisteryBox") {
		Vector newPos = p + d;
		if (newPos.y > oldP.y) {
			p = oldP;
			SetState("hidden");
		}
		else {
			p = newPos;
		}
	}
	//}
}

void Coin::Render() {
	if (state == "hidden") return;
	else {
		float width = 0;
		float height = 0;
		Coin::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
		RenderBoundingBox();
	}
}

void Coin::SetState(string state)
{
	if (state == "running") {
	}
	else if (state == "fromMisteryBox") {
		p.y = p.y - 20;
		oldP = p;
		//oldP.y = oldP.y - 50;
		v.y = -0.5f;
		g.y = 0.0015f;
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void Coin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Coin::HandleCollision(LPCOLLISIONEVENT e) {
	//SetState("fromMisteryBox");
	if (isAllowCollision == true && state != "hidden") {
		// handle score logic here
		SetState("hidden");
		isAllowCollision = false;
	}
}
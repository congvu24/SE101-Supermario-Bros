#include "Leaf.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Leaf::texture = NULL;
unordered_map<string, LPSPRITE> Leaf::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Leaf::all_animations; //save all animations
CAnimationSets Leaf::animations_set; //save all the animation sets
json Leaf::data = NULL;

Leaf::Leaf()
{
	SetState("fromMisteryBox");
	width = 14;
	height = 16;
	p = Vector(0, 0);

}

void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	v = v + g * dt;
	if (v.y > 0.15f) v.y = 0.15f;

	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	//if (state != "running") {

		coEvents.clear();
		for (auto i = coObjects->begin(); i != coObjects->end(); i++)
		{
			if ((*i)->name == "RectCollision") {
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

			//p = p + d;
			float time = GetTickCount64() - beginFalling;
			p.y = p.y + d.y;
			if (v.y > 0)
				p.x = oldP.x + LEAF_AMPLITUDE * cos(LEAF_SPEED * time * 1.0f - PI / 2);
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

			//stop move here

			if (nx == 0 && ny != 0) {
				SetState("running");
			}


			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		}
	//}

}

//void Leaf::Render() {
//	if (state == "hidden") return;
//	else {
//		float width = 0;
//		float height = 0;
//		Leaf::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
//		RenderBoundingBox();
//	}
//}

void Leaf::SetState(string state)
{
	if (state == "running") {
		v.x = 0;
		v.y = 0;
		g.y = 0;
	}
	else if (state == "fromMisteryBox") {
		p.y = p.y - 20;
		oldP = p;
		//oldP.y = oldP.y - 50;
		/*v.y = -0.5f;
		g.y = 0.0015f;*/
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void Leaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Leaf::HandleCollision(LPCOLLISIONEVENT e) {
	if (isAllowCollision == true && state != "hidden") {
		SetState("hidden");
	}
}
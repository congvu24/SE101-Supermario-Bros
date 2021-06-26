#include "VenusBullet.h"
#include "Vector.h"
#include "Test.h"
#include <iostream>


LPDIRECT3DTEXTURE9 VenusBullet::texture = NULL;
unordered_map<string, LPSPRITE> VenusBullet::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> VenusBullet::all_animations; //save all animations
CAnimationSets VenusBullet::animations_set; //save all the animation sets
json VenusBullet::data = NULL;

VenusBullet::VenusBullet()
{
	SetState("running");
	isBlockPlayer = false;
	isAllowCollision = false;
	v = Vector(0.10f, 0.10f);
	g = Vector(0, 0);
}

void VenusBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	p.x += direction.x * v.x * dt;
	p.y += direction.y * v.y * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;
	if (camera->isInCamPosition(this, 0) == false) {
		SetState("hidden");
	}

	coEvents.clear();
	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{

		if (Test* obj = dynamic_cast<Test*>((*i))) {
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

		if (Test* obj = dynamic_cast<Test*>(coEventsResult[i]->obj)) {
			obj->Die();
		}
		if (coEventsResult[i]->obj->name == "Death") {
			SetState("hidden");
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	coEventsResult.clear();
	coEvents.clear();
}

void VenusBullet::SetState(string state)
{
	

	CGameObject::SetState(state);

}

void VenusBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void VenusBullet::HandleCollision(LPCOLLISIONEVENT e) {
	
}
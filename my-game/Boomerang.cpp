#include "Boomerang.h"
#include "Vector.h"
#include "Test.h"
#include "BoomerangBrother.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Boomerang::texture = NULL;
unordered_map<string, LPSPRITE> Boomerang::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Boomerang::all_animations; //save all animations
CAnimationSets Boomerang::animations_set; //save all the animation sets
json Boomerang::data = NULL;

Boomerang::Boomerang()
{
	SetState("running");
	isBlockPlayer = false;
	isAllowCollision = false;
	point = 0;
	v = Vector(0.10f, 0.10f);
	g = Vector(0, 0);
	isUniversal = true;
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	/*if (flyTime >= 0 && flyTime - dt >= 0) {
		flyTime = flyTime - dt;

		p.x += direction.x * v.x * dt;
		p.y += direction.y * v.y * dt;
	}
	else {
		p.x -= direction.x * v.x * dt;
		p.y -= direction.y * v.y * dt;
	}*/

	if (count < 1.0f) {
		count += 1.0f * 0.01f;
		D3DXVECTOR3 start = D3DXVECTOR3(oldP.x, oldP.y, 0);
		D3DXVECTOR3 end = D3DXVECTOR3(destination.x, destination.y, 0);
		D3DXVECTOR3 control;
		if (moveDirection == BoomerangDirection::Forward) {
			if (nx >= 0) {
				control = D3DXVECTOR3(oldP.x + 200, oldP.y - 100, 0);
			}
			else
				control = D3DXVECTOR3(oldP.x - 200, oldP.y - 100, 0);

		}
		else
		{
			if (nx >= 0) {
				control = D3DXVECTOR3(oldP.x - 200, oldP.y + 100, 0);
			}
			else
				control = D3DXVECTOR3(oldP.x + 200, oldP.y + 100, 0);

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

	/*Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;
	if (camera->isInCamPosition(this, 0) == false) {
		SetState("hidden");
	}*/

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

		if (Test* obj = dynamic_cast<Test*>(coEventsResult[i]->obj)) {
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

void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Boomerang::HandleCollision(LPCOLLISIONEVENT e) {
	if (Test* player = dynamic_cast<Test*>(e->obj)) {
		//player->Die();
	}
}

void Boomerang::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (Test* player = dynamic_cast<Test*>(event->obj)) {
		KillPlayer(player);
		SetState("hidden");
	}
}

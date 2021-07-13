#include "RedGoomba.h"
#include "Vector.h"
#include "Test.h"
#include "PlayScence.h"
#include <iostream>


LPDIRECT3DTEXTURE9 RedGoomba::texture = NULL;
unordered_map<string, LPSPRITE> RedGoomba::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> RedGoomba::all_animations; //save all animations
CAnimationSets RedGoomba::animations_set; //save all the animation sets
json RedGoomba::data = NULL;

RedGoomba::RedGoomba()
{
	SetState("flying");
	v = Vector(-0.05f, -0.1f);
	isAllowCollision = true;
	isBlockPlayer = true;
	useLimit = false;
}

void RedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
	CGameObject::Update(dt, coObjects);
	Enemy::CheckToChangeDirection();

	v = v + g * dt;
	if (v.y > MAX_VY) v.y = MAX_VY;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vector<LPGAMEOBJECT>* checkObjects = new vector<LPGAMEOBJECT>();

	coEvents.clear();


	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if ((*i)->isAllowCollision == true && !CPlayScene::IsPlayer(*i)) {
			checkObjects->push_back((*i));
		}
	}

	CalcPotentialCollisions(checkObjects, coEvents);


	if (coEvents.size() == 0) {

		p = p + d;
	}
	else {
		p.x = p.x + d.x;

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		if (ny != 0) v.y = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			HandleCollision(coEventsResult[i]);
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	coEvents.clear();
	coEventsResult.clear();
	checkObjects->clear();
	delete checkObjects;
}


void RedGoomba::SetState(string state)
{
	if (state == "flying") {

	}
	if (state == "running") {
		v.y = 0;
	}
	else if (state == "die") {
		v = Vector(0, -0.15f);
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void RedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void RedGoomba::HandleCollision(LPCOLLISIONEVENT e) {
	Enemy::HandleCollision(e);

	if (state == "flying") {
		if (countJump == 3) Jump(-0.4f);
		else
			v.y = -0.2f;

		if (countJump < 4 && e->ny != 0) {
			countJump++;
		}
	}

}

void RedGoomba::Jump(float vy) {
	v.y = vy;
	countJump = 0;
}

void RedGoomba::BeingKill() {
	if (state == "running") {
		Enemy::BeingKill();
		isAllowCollision = false;
		isBlockPlayer = false;
	}
}

void RedGoomba::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	Enemy::OnHadCollided(obj, event);

	if (state == "running") {
		if (Test* player = dynamic_cast<Test*>(obj)) {
			if (isBlockPlayer == true) {
				player->SetAction(MarioAction::JUMP);
				isAllowCollision = true;
			}
		}
	}

	else if (state == "die") {
		if (Test* player = dynamic_cast<Test*>(obj)) {
			player->SetAction(MarioAction::JUMP);
			isBlockPlayer = false;
		}
	}
}

void RedGoomba::CollisionVertical(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (Test* player = dynamic_cast<Test*>(obj)) {
		if (state == "flying") {
			if (event->ny < 0) {
				SetState("running");
			}
		}
		else if (state == "running") {
			if (event->ny < 0) {
				BeingKill();
			}
		}
	}
}

void RedGoomba::HandleAfterCreated() {
	type = "2";
}
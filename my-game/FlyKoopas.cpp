#include "Koopas.h"
#include "Vector.h"
#include "Mario.h"
#include "PlayScence.h"
#include <iostream>
#include "FlyKoopas.h"


FlyKoopas::FlyKoopas()
{
	SetState("running");
	type = "fly";
	v = Vector(-0.05f, 0);
	isAllowCollision = true;
	isBlockPlayer = true;
	useLimit = false;
}

void FlyKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (isHolded == true && ((Mario*)holdedBy)->action == MarioAction::HOLD) {
		p.x = holdedBy->nx < 0 ? holdedBy->p.x - holdedBy->width : holdedBy->p.x + holdedBy->width;
		p.y = holdedBy->p.y;
		nx = holdedBy->nx;
		return;
	}
	else if (isHolded == true && ((Mario*)holdedBy)->action != MarioAction::HOLD) {
		isHolded = false;
		isUniversal = true;
		this->v.x = 0.5f * holdedBy->nx;
		isHitted = true;
		isBlockPlayer = true;
		useLimit = false;
	}

	Enemy::Update(dt, coObjects);
	v = v + g * (float)dt;
	if (v.y > MAX_VY) v.y = MAX_VY;

	CGameObject::Update(dt, coObjects);

	if (canJump == true && state != "die") {
		SetState("running");
	}

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
			if (ny < 0) canJump = true;
			HandleCollision(coEventsResult[i]);
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	coEvents.clear();
	coEventsResult.clear();
	checkObjects->clear();
	delete checkObjects;
}


void FlyKoopas::SetState(string state) {
	if (state == "running" && canJump == true) {
		v.y = -0.3f;
		canJump = false;
		CGameObject::SetState(state);
	}
	else {
		Koopas::SetState(state);
	}
}
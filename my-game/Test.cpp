#include "Test.h"
#include <iostream>
#include "Quadtree.h"
#include "MiniPortal.h"
#include "PlayScence.h"
#include "Game.h"


#define VX_SMALL 0.15f
#define VY_SMALL 0.6f
#define VY_BIG 0.6f


Test::Test()
{
	width = 24;
	height = 30;
	SetState("indie");
	max_move_x = VX_SMALL;
	max_move_y = VY_SMALL;
	renderOrder = 99999; //player should be rendered lastly
	name = "player";
}


void Test::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	v = v + g * dt;
	if (v.x != 0 && powerX != 0) {
		if (canJump == false) {
			v.x = (v.x / abs(v.x)) * 0.15f + (v.x / abs(v.x)) * 0.2f * (abs(powerX) / 1000);
		}
		else
			v.x = v.x + powerX * 0.0003;
	}
	else {
		if (canJump == true) {
			powerX = 0;
		}
		if (v.x != 0)
			v.x = (v.x / abs(v.x)) * max_move_x;
	}

	if (v.y > max_move_y) v.y = max_move_y;
	if (abs(v.x) > 0.5f) {
		v.x = (v.x / abs(v.x)) * 0.5f;
	}

	CGameObject::Update(dt, coObjects);

	vector<LPGAMEOBJECT>* checkObjects = new vector<LPGAMEOBJECT>();


	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if (state != "die") {
			if ((*i)->isBlockPlayer == true) {
				checkObjects->push_back((*i));
			}
		}
		else {
			if ((*i)->name == "Death") {
				checkObjects->push_back((*i));
			}
		}
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	int ny;
	int nx;

	//UpdateWithCollision(checkObjects, ny, nx);

	CalcPotentialCollisions(checkObjects, coEvents);

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


		if (nx != 0) v.x = 0;
		if (ny != 0) v.y = 0;



		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			if (ny < 0 && canJump == false) powerX = 0;
			if (ny < 0) canJump = true;
			//if (coEventsResult[i]->obj->name != "") {
			HandleCollision(coEventsResult[i]);
			coEventsResult[i]->obj->OnHadCollided(this, coEventsResult[i]);
			//}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		p.x += min_tx * d.x + nx * 0.4f;
		p.y += min_ty * d.y + ny * 0.4f;

		coEventsResult.clear();
		coEvents.clear();
	}

	checkObjects->clear();
	delete checkObjects;
	// collision done



	if (v.x != 0)
		DebugOut(L"V.x: %s\n", ToLPCWSTR(to_string(v.x)));
	if (powerX != 0)

		DebugOut(L"Power: %s\n", ToLPCWSTR(to_string(powerX)));
}

void Test::Render()
{
	float w = width;
	float h = height;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h);
	p.y = p.y - (h - this->height);
	p.x = p.x - (w - this->width);
	width = w;
	height = h;
	RenderBoundingBox();
}

void Test::SetState(string state)
{

	if (isReadyChangeState == true) {

		if (state == "running-up") {
			v.y = -0.15f;
			v.x = 0;
			CGameObject::SetState("jumping-right");

		}
		if (state == "running-down") {
			v.y = 0.15f;
			v.x = 0;
			CGameObject::SetState("jumping-right");
		}

		if (state == "running-right") {
			//v.y = 0;
			v.x = max_move_x;
			nx = 1;
			if (canJump == true)
				CGameObject::SetState(state);
			else CGameObject::SetState("jumping-right");

		}
		else if (state == "running-left") {
			//v.y = 0;
			v.x = -max_move_x;
			nx = -1;
			if (canJump == true)
				CGameObject::SetState(state);
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "sitting" && type != "1") {
			if (this->state == "indie") {
				v.x = 0;
				v.y = 0;
				if (nx > 0)
					CGameObject::SetState("sitting-right");
				else
					CGameObject::SetState("sitting-left");
			}
		}
		else if (state == "jumping" && canJump == true) {
			v.y = -max_move_y;
			canJump = false;
			if (nx > 0)
				CGameObject::SetState("jumping-right");
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "indie" && canJump == true) {
			v.x = 0;
			//v.y = 0;
			if (nx < 0)
				CGameObject::SetState("indie-left");
			else
				CGameObject::SetState("indie-right");
		}
		else if (state == "kick") {
			if (nx < 0)
				CGameObject::SetState("kick-left");
			else
				CGameObject::SetState("kick-right");
		}
		else if (state == "die") {
			v.y = -0.3f;
			v.x = 0;
			CGameObject::SetState(state);
			isReadyChangeState = false;
		}

	}

}

void Test::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Test::HandleCollision(LPCOLLISIONEVENT e) {
	LPGAMEOBJECT obj = e->obj;

	if (e->nx != 0) {
		HandleCollisionHorizontal(e);
	}
	if (e->ny != 0) {
		HandleCollisionVertical(e);
	}

	if (obj->name == "RectPlatform") {
		p.x = p.x + d.x;
	}

	if (obj->name == "MiniPortal") {
		MiniPortal* start = dynamic_cast<MiniPortal*>(obj);
		MiniPortal* destination = NULL;
		vector<CGameObject*>* allObjectOfSence = &(CGame::GetInstance()->GetCurrentScene()->objects);

		for (auto i = allObjectOfSence->begin(); i != allObjectOfSence->end(); i++)
		{
			if (MiniPortal* obj = dynamic_cast<MiniPortal*>(*i)) {
				if (obj->portalName == start->portalName && obj->type == "Out") {
					destination = obj;
					break;
				}
			}
		}

		if (destination != NULL) {
			CGame::GetInstance()->GetCurrentScene()->getCamera()->setCamPos(destination->camera_x, destination->camera_y);
			p = destination->p;
		}

	}

	if (obj->name == "Death") {
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GameOver();
	}

}

void Test::Die() {
	SetState("die");
	isAllowCollision = false;
}

void Test::Transform() {
	this->type = "2"; // move to next size
	this->p.x = p.x - (42 - this->width);
	this->p.y = p.y - (54 - this->height);
	width = 42;
	height = 54;
	max_move_y = VY_BIG;
}

void Test::ProcessKeyboard(KeyboardEvent kEvent)
{
	switch (kEvent.key) {
	case DIK_RIGHT:
		SetState("running-right");
		break;
	case DIK_LEFT:
		SetState("running-left");
		break;
	case DIK_UP:
		SetState("running-up");
		break;
	case DIK_DOWN:
		SetState("running-down");
		break;
	case DIK_SPACE:
		SetState("jumping");
		break;
	default:
		break;
	}

}


void Test::HandleCollisionVertical(LPCOLLISIONEVENT e) {
	if (this->nx != powerX / abs(powerX)) {
		powerX = 0;
	}

	if (v.x != 0) {
		if (abs(powerX + 10 * this->nx) <= 1000) {
			powerX = powerX + 10 * this->nx;
		}
	}
	else if (v.x == 0) {
		if (powerX >= 0)
			if (powerX - 30 >= 0)
				powerX = powerX - 30;
			else powerX = 0;
		else if (powerX <= 0) {
			if (powerX + 30 <= 0)
				powerX = powerX + 30;
			else powerX = 0;
		}
	}
}
void Test::HandleCollisionHorizontal(LPCOLLISIONEVENT e) {

}

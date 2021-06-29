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

	if (abs(powerX) == 1000 && timeMaxPower >= 0) {
		timeMaxPower = timeMaxPower - dt;
	}
	if (powerX != 0 && (timeMaxPower < 0)) {
		DecreasePowerX();
	}


	if (action == MarioAction::FLY) {
		DebugOut(L"Action: %s \n", ToLPCWSTR("Flying"));
	}
	else
		DebugOut(L"Action: %s \n", ToLPCWSTR("another"));

	if (action != MarioAction::FLY)
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
	DebugOut(L"Power: %s\n", ToLPCWSTR(to_string(powerX)));
	DebugOut(L"TimeMaxPower: %s\n", ToLPCWSTR(to_string(timeMaxPower)));
	DebugOut(L"P.y: %s\n", ToLPCWSTR(to_string(p.y)));

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
		else if (state == "running-down") {
			v.y = 0.15f;
			v.x = 0;
			CGameObject::SetState("jumping-right");
		}

		else if (state == "running-right") {
			//v.y = 0;
			SetAction(MarioAction::RUN);
			v.x = max_move_x;
			nx = 1;
			if (canJump == true)
				CGameObject::SetState(state);
			else CGameObject::SetState("jumping-right");
		}
		else if (state == "running-left") {
			//v.y = 0;
			SetAction(MarioAction::RUN);
			v.x = -max_move_x;
			nx = -1;
			if (canJump == true)
				CGameObject::SetState(state);
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "sitting" && type != "1") {
			if (this->state == "indie") {
				SetAction(MarioAction::CROUCH);
				v.x = 0;
				v.y = 0;
				if (nx > 0)
					CGameObject::SetState("sitting-right");
				else
					CGameObject::SetState("sitting-left");
			}
		}
		else if (state == "jumping" && canJump == true) {
			SetAction(MarioAction::JUMP);
			v.y = -max_move_y;
			canJump = false;
			if (nx > 0)
				CGameObject::SetState("jumping-right");
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "high-jump" && canJump == true && abs(powerX) == 1000) {
			SetAction(MarioAction::JUMP_HEIGHT);
			v.y = -max_move_y;
			canJump = false;
			if (nx > 0)
				CGameObject::SetState("jumping-right");
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "fly-up" && canJump == false) {
			if (abs(powerX) == 1000) {
			SetAction(MarioAction::FLY);
			v.y = -0.3f;
			canJump = false;
			if (nx > 0)
				CGameObject::SetState("jumping-right");
			else CGameObject::SetState("jumping-left");
			}
		}
		else if (state == "fall-down" && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetAction(MarioAction::FALL);
				if (nx > 0)
					CGameObject::SetState("jumping-right");
				else CGameObject::SetState("jumping-left");
			}
		}
		else if (state == "indie" && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetAction(MarioAction::JUMP_HEIGHT);
				if (nx < 0)
					CGameObject::SetState("flying-left");
				else
					CGameObject::SetState("flying-right");
			}
		}
		else if (state == "indie" && canJump == true) {
			SetAction(MarioAction::IDLE);
			v.x = 0;
			//v.y = 0;
			if (nx < 0)
				CGameObject::SetState("indie-left");
			else
				CGameObject::SetState("indie-right");
		}

		else if (state == "kick") {
			SetAction(MarioAction::KICK);
			if (nx < 0)
				CGameObject::SetState("kick-left");
			else
				CGameObject::SetState("kick-right");
		}
		else if (state == "flying" && canJump == false) {
			//SetAction(MarioAction::JUMP);
			v.y = -0.03f;
			v.x = 0;
			if (nx < 0)
				CGameObject::SetState("flying-left");
			else
				CGameObject::SetState("flying-right");
		}
		else if (state == "flying-left" && canJump == false) {
			SetAction(MarioAction::JUMP);
			v.x = -max_move_x;
			nx = -1;
			CGameObject::SetState("flying-left");
		}
		else if (state == "flying-right" && canJump == false) {
			SetAction(MarioAction::JUMP);
			v.x = max_move_x;
			nx = 1;
			CGameObject::SetState("flying-right");
		}
		else if (state == "fly-up-left" && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetAction(MarioAction::JUMP_HEIGHT);
				v.x = -max_move_x;
				nx = -1;
				CGameObject::SetState("flying-left");
			}
		}
		else if (state == "fly-up-right" && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetAction(MarioAction::JUMP_HEIGHT);
				v.x = max_move_x;
				nx = 1;
				CGameObject::SetState("flying-right");
			}
		}
		else if (state == "die") {
			SetAction(MarioAction::DIE);
			v.y = -0.3f;
			v.x = 0;
			CGameObject::SetState(state);
			isReadyChangeState = false;
		}

	}

}


void Test::SetAction(MarioAction newAction) {
	this->action = newAction;
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

void Test::Transform(int marioType) {

	switch (marioType)
	{
	case SmallMario:
		this->type = to_string(marioType);
		max_move_y = VY_SMALL;
		break;
	case BigMario:
		this->type = to_string(marioType);
		max_move_y = VY_BIG;
	case RacconMario:
		this->type = to_string(marioType);
		max_move_y = VY_BIG;
	default:
		break;
	}

}

void Test::ProcessKeyboard(KeyboardEvent kEvent)
{
	if (kEvent.isKeyUp == true) {
		holdingKeys[kEvent.key] = false;
		return;
	}

	if (kEvent.isHolding == true) holdingKeys[kEvent.key] = true;

	switch (kEvent.key) {
	case DIK_RIGHT:
		if (action == MarioAction::FLY && stoi(type) == RacconMario) SetState("fly-up-right");
		else if (action == MarioAction::JUMP_HEIGHT && stoi(type) == RacconMario) SetState("fly-up-right");
		else if (action == MarioAction::JUMP && stoi(type) == RacconMario) SetState("flying-right");
		else
			SetState("running-right");
		break;
	case DIK_LEFT:
		if (action == MarioAction::FLY && stoi(type) == RacconMario) SetState("fly-up-left");
		else if (action == MarioAction::JUMP_HEIGHT && stoi(type) == RacconMario) SetState("fly-up-left");
		else if (action == MarioAction::JUMP && stoi(type) == RacconMario) SetState("flying-left");
		else
			SetState("running-left");
		break;
	case DIK_S:
		if (action == MarioAction::JUMP_HEIGHT || action == MarioAction::FLY) {
			if (!kEvent.isHolding && !kEvent.isKeyUp) SetState("fly-up");
			if (!kEvent.isHolding && !kEvent.isKeyUp) SetState("fall-down");
			break;
		}
		else {
			if (holdingKeys[kEvent.key] == false && stoi(type) == RacconMario)
				SetState("flying");
			break;
		}

	case DIK_DOWN:
		SetState("running-down");
		break;
	case DIK_SPACE:
		if (abs(powerX) == 1000 && stoi(type) == RacconMario) SetState("high-jump"); else
			SetState("jumping");
		break;
	case DIK_1:
		Transform(SmallMario);
		break;
	case DIK_2:
		Transform(BigMario);
		break;
	case DIK_3:
		Transform(RacconMario);
		break;
	default:
		break;
	}

}


void Test::HandleCollisionVertical(LPCOLLISIONEVENT e) {
	if (this->nx != powerX / abs(powerX)) {
		powerX = 0;
	}

	if (timeMaxPower <= 0 && abs(powerX) == 1000) {
		DebugOut(L"Time to decrease power");
	}

	if (holdingKeys[DIK_A] == true) {
		if (v.x != 0) {
			IncreasePowerX();
		}
	}
}
void Test::HandleCollisionHorizontal(LPCOLLISIONEVENT e) {

}


void Test::IncreasePowerX() {
	if (abs(powerX + 10 * this->nx) <= 1000) {
		powerX = powerX + 10 * this->nx;
		timeMaxPower = 0;
		if (abs(powerX) == 1000) {
			timeMaxPower = 10000;
		}
	}
}
void Test::DecreasePowerX() {
	if (powerX >= 0)
		if (powerX - 40 >= 0)
			powerX = powerX - 40;
		else powerX = 0;
	else if (powerX <= 0) {
		if (powerX + 40 <= 0)
			powerX = powerX + 40;
		else powerX = 0;
	}
}

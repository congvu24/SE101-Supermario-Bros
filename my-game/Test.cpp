#include "Test.h"
#include <iostream>
#include "Quadtree.h"
#include "MiniPortal.h"
#include "PlayScence.h"
#include "Game.h"
#include "Effect.h"


#define VX_SMALL 0.15f
#define VY_SMALL 0.4f
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
	if (action == MarioAction::TRANSFORM && timeBeginAction >= 0) {
		timeBeginAction = timeBeginAction - dt;
		return;
	}
	if (action == MarioAction::GETTING_INTO_THE_HOLE && timeBeginAction >= 0) {
		UpdateAnimation(dt, coObjects);
		return;
	}

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
	if (abs(v.x) > 0.5f) {
		v.x = (v.x / abs(v.x)) * 0.5f;
	}



	CGameObject::Update(dt, coObjects);

	if (timeBeginAction >= 0) {
		timeBeginAction = timeBeginAction - dt;
	}

	if (abs(powerX) == 1000 && timeMaxPower >= 0) {
		timeMaxPower = timeMaxPower - dt;
	}
	if (powerX != 0 && (timeMaxPower < 0)) {
		DecreasePowerX();
	}

	DebugOut(L"State: %s \n", ToLPCWSTR(state));

	if (action != MarioAction::FLY)
		v = v + g * dt;
	if (v.y > max_move_y) v.y = max_move_y;



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
	if (action == MarioAction::TRANSFORM && timeBeginAction >= 100) {
		return;
	}
	else {
		float w = width;
		float h = height;
		animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h);
		if (action != MarioAction::ATTACK) {
			p.y = p.y - (h - this->height);
			p.x = p.x - (w - this->width);
			width = w;
			height = h;
		}
		RenderBoundingBox();
	}
}

void Test::SetState(string state)
{

	if (isReadyChangeState == true) {

		if (action == MarioAction::HOLD) {
			if (state == "running") {
				if (nx > 0)
					CGameObject::SetState("running-hold-right");
				else CGameObject::SetState("running-hold-left");
			}
			else if (state == "running-right") {
				CGameObject::SetState("running-hold-right");
			}
			else if (state == "running-left") {
				CGameObject::SetState("running-hold-left");
			}
			else if (state == "indie") {
				if (nx > 0)
					CGameObject::SetState("indie-hold-right");
				else CGameObject::SetState("indie-hold-left");
			}
			return;
		}

		if (state == "running-up") {
			CGameObject::SetState("running-up");
		}
		else if (state == "running-down") {
			CGameObject::SetState("running-down");
		}
		else if (state == "running-right") {
			CGameObject::SetState(state);
		}
		else if (state == "running-left") {
			CGameObject::SetState(state);
		}
		else if (state == "sitting") {
			if (nx > 0)
				CGameObject::SetState("sitting-right");
			else
				CGameObject::SetState("sitting-left");
		}
		else if (state == "jumping") {
			if (nx > 0)
				CGameObject::SetState("jumping-right");
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "jumping-left") {
			CGameObject::SetState(state);
		}
		else if (state == "jumping-right") {
			CGameObject::SetState(state);
		}
		else if (state == "jump-height") {
			if (nx > 0)
				CGameObject::SetState("jumping-right");
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "fly-up" && canJump == false) {
			if (nx > 0)
				CGameObject::SetState("jumping-right");
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "fall-down" && canJump == false) {
			if (nx > 0)
				CGameObject::SetState("jumping-right");
			else CGameObject::SetState("jumping-left");
		}
		else if (state == "indie") {
			v.x = 0;
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
		else if (state == "attack") {
			if (nx < 0)
				CGameObject::SetState("attack-left");
			else
				CGameObject::SetState("attack-right");
		}
		else if (state == "flying" && canJump == false) {
			if (nx < 0)
				CGameObject::SetState("flying-left");
			else
				CGameObject::SetState("flying-right");
		}
		else if (state == "flying-left" && canJump == false) {
			CGameObject::SetState("flying-left");
		}
		else if (state == "flying-right") {
			CGameObject::SetState("flying-right");
		}
		else if (state == "fly-up-left" && canJump == false) {
			CGameObject::SetState("flying-left");
		}
		else if (state == "fly-up-right" && canJump == false) {
			CGameObject::SetState("flying-right");
		}
		else if (state == "die") {
			CGameObject::SetState(state);
		}
		else if (state == "into-hole") {
			CGameObject::SetState(state);
		}
		/*else {
			CGameObject::SetState(state);
		}*/
	}

}

bool Test::IsReadyToChangeAction() {
	if (timeBeginAction <= 0) return true;
	return false;
}


void Test::SetAction(MarioAction newAction, DWORD time) {

	if (newAction == MarioAction::DIE) {
		action = newAction;
	}

	if (newAction == MarioAction::TRANSFORM) {
		action = newAction;
		timeBeginAction = time;
		LPGAMEOBJECT smoke = new Effect("smoke", time);
		smoke->p = Vector((p.x + width / 2), p.y + height / 2);
		CGame::GetInstance()->GetCurrentScene()->addObject(smoke);
	}

	if (newAction == MarioAction::RELEASE) {
		action = newAction;
		timeBeginAction = 200;
		SetState("kick");
	}

	if (action == MarioAction::HOLD && newAction == MarioAction::IDLE) {
		v.x = 0;
		SetState("indie");
	}

	if (action == MarioAction::HOLD) return;

	if (newAction == MarioAction::HOLD) {
		SetState("hold");
		action = newAction;
	}

	if (IsReadyToChangeAction() == false) return;

	switch (newAction)
	{
	case MarioAction::IDLE:
		if (canJump == true) {
			v.x = 0;
			SetState("indie");
			action = newAction;
			timeBeginAction = time;
		}
		else if (action == MarioAction::FLY && canJump == false) {
			SetAction(MarioAction::JUMP_HEIGHT);
		}
		break;
	case MarioAction::WALK:
		action = newAction;
		timeBeginAction = 0;
		break;
	case MarioAction::RUN:
		action = newAction;
		timeBeginAction = 0;
		break;
	case MarioAction::JUMP:
		if (abs(powerX) == 1000 && stoi(type) == RacconMario) {
			SetAction(MarioAction::JUMP_HEIGHT);
			break;
		}
		else {
			if (canJump == true) {
				v.y = -max_move_y;
				g.y = 0.001f;
				canJump = false;
				SetState("jumping");
				action = newAction;
				timeBeginAction = time;
			}
		}
		break;
	case MarioAction::CROUCH:
		break;
	case MarioAction::FLY:
		if (abs(powerX) == 1000 && (action == MarioAction::FLY || action == MarioAction::JUMP_HEIGHT)) {
			v.y = -0.3f;
			SetState("fly-up");
			action = MarioAction::FLY;
			timeBeginAction = time;
		}
		else if ((action == MarioAction::JUMP || action == MarioAction::JUMP_HEIGHT) && stoi(type) == RacconMario && canJump == false) {
			v.y = -0.03f;
			v.x = 0;
			SetState("flying");
			action = MarioAction::JUMP;
			timeBeginAction = time;
		}
		break;
	case MarioAction::JUMP_HEIGHT:
		if (action == MarioAction::FLY) {
			action = MarioAction::JUMP_HEIGHT;
			timeBeginAction = time;
		}
		else if (canJump == true && abs(powerX) == 1000) {
			v.y = -max_move_y;
			g.y = 0.001f;
			canJump = false;
			SetState("jump-height");
			action = newAction;
			timeBeginAction = time;
		}
		break;
	case MarioAction::FALL:
		if (holdingKeys[DIK_S] == false && action == MarioAction::FLY) {
			SetState("fall-down");
			action = newAction;
			timeBeginAction = time;
		}
		break;
	case MarioAction::ATTACK:
		if (stoi(type) == RacconMario && canJump == true) {
			SetState("attack");
			action = newAction;
			timeBeginAction = time;
		}
		break;
	case MarioAction::KICK:
		SetState("kick");
		action = newAction;
		timeBeginAction = time;
		break;
	case MarioAction::DIE:
		action = newAction;
		break;
	case MarioAction::GETTING_INTO_THE_HOLE:
		SetState("into-hole");
		v.x = 0;
		//renderOrder = 0;
		action = newAction;
		canJump = false;
		timeBeginAction = time;
		break;
	case MarioAction::PICK_UP:
		action = newAction;
		timeBeginAction = time;
		break;
	default:
		break;
	}


	//this->action = newAction;
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

	if (obj->name == "Death") {
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GameOver();
	}

}

void Test::Die() {

	if (IsReadyToChangeAction() == true) {
		v.y = -0.3f;
		v.x = 0;
		life = life - 1;

		if (life == 0) {
			SetState("die");
			SetAction(MarioAction::DIE);
			isReadyChangeState = false;
			isAllowCollision = false;
		}
		else if (stoi(type) == RacconMario) {
			Transform(BigMario);
		}
		else if (stoi(type) == BigMario) {
			Transform(SmallMario);
		}

	}
}

void Test::Transform(int marioType) {

	switch (marioType)
	{
	case SmallMario:
		this->type = to_string(marioType);
		max_move_y = VY_SMALL;
		v.y = 0.1f;
		SetAction(MarioAction::TRANSFORM, 500);
		life = 1;
		break;
	case BigMario:
		this->type = to_string(marioType);
		max_move_y = VY_BIG;
		v.y = 0.1f;
		SetAction(MarioAction::TRANSFORM, 500);
		life = 2;
		break;
	case RacconMario:
		this->type = to_string(marioType);
		max_move_y = VY_BIG;
		v.y = 0.1f;
		SetAction(MarioAction::TRANSFORM, 500);
		life = 3;
		break;
	default:
		break;
	}
	SetState("jumping");
}

void Test::ProcessKeyboard(KeyboardEvent kEvent)
{
	if (action == MarioAction::TRANSFORM || action == MarioAction::GETTING_INTO_THE_HOLE) return;

	if (kEvent.isKeyUp == true) {
		holdingKeys[kEvent.key] = false;
		return;
	}

	if (kEvent.isHolding == true) holdingKeys[kEvent.key] = true;

	switch (kEvent.key) {
	case DIK_RIGHT:
		v.x = max_move_x;
		nx = 1;

		if (action == MarioAction::FLY && stoi(type) == RacconMario && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetAction(MarioAction::JUMP_HEIGHT);
				SetState("flying-right");
			}
		}
		else if (action == MarioAction::JUMP_HEIGHT && stoi(type) == RacconMario && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetAction(MarioAction::JUMP_HEIGHT);
				SetState("flying-right");
			}
		}
		else if (action == MarioAction::JUMP && stoi(type) == RacconMario && canJump == false) {
			SetState("flying-right");
		}
		else if (canJump == false) {
			SetState("jumping-right");
		}
		else {
			SetState("running-right");
			SetAction(MarioAction::RUN);
		}
		break;
	case DIK_LEFT:
		v.x = -max_move_x;
		nx = -1;

		if (action == MarioAction::FLY && stoi(type) == RacconMario && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetAction(MarioAction::JUMP_HEIGHT);
				SetState("flying-left");
			}
		}
		else if (action == MarioAction::JUMP_HEIGHT && stoi(type) == RacconMario && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetAction(MarioAction::JUMP_HEIGHT);
				SetState("flying-left");
			}
		}
		else if (action == MarioAction::JUMP && stoi(type) == RacconMario && canJump == false) {
			SetState("flying-left");
		}
		else if (canJump == false) {
			SetState("jumping-left");
		}
		else {
			SetState("running-left");
			SetAction(MarioAction::RUN);
		}
		break;
	case DIK_S:
		if (!kEvent.isHolding && !kEvent.isKeyUp) {
			SetAction(MarioAction::FLY);
		}
		if (!kEvent.isHolding && !kEvent.isKeyUp) {
			SetAction(MarioAction::FALL);
		}
		break;
	case DIK_A:
		if (!kEvent.isHolding && !kEvent.isKeyUp) {
			SetAction(MarioAction::ATTACK, 300);
		}
		break;
	case DIK_B:
		if (action != MarioAction::HOLD) {
			SetAction(MarioAction::PICK_UP, 100);
		}
		else if (action == MarioAction::HOLD) {
			SetAction(MarioAction::RELEASE);
		}
		break;
	case DIK_UP:
		//develop propose
		v.y = -0.15f;
		v.x = 0;
		SetState("running-up");
		SetAction(MarioAction::RUN);
	case DIK_DOWN:
		if (action == MarioAction::IDLE && stoi(type) != SmallMario) {
			v.x = 0;
			v.y = 0;
			SetAction(MarioAction::CROUCH);
		}
		else {
			//develop propose
			v.y = 0.15f;
			v.x = 0;
			SetState("running-down");
			SetAction(MarioAction::RUN);
		}
		break;
	case DIK_SPACE:
		if (!kEvent.isHolding && !kEvent.isKeyUp) SetAction(MarioAction::JUMP);
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

	if (holdingKeys[DIK_A] == true) {
		if (v.x != 0) {
			IncreasePowerX();
		}
	}
}
void Test::HandleCollisionHorizontal(LPCOLLISIONEVENT e) {

}


void Test::IncreasePowerX() {
	if (abs(powerX + 20 * this->nx) <= 1000) {
		powerX = powerX + 20 * this->nx;
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


void Test::Teleport(MiniPortal* destination, int duration) {
	if (teleportDestination == NULL) {
		teleportDestination = destination;
		SetAction(MarioAction::GETTING_INTO_THE_HOLE, duration);
	}
}

void Test::UpdateAnimation(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (action == MarioAction::GETTING_INTO_THE_HOLE && timeBeginAction >= 100) {
		p.y = p.y + 0.5f;
		timeBeginAction = timeBeginAction - dt;
	}
	else if (action == MarioAction::GETTING_INTO_THE_HOLE && timeBeginAction < 100 && teleportDestination != NULL) {
		timeBeginAction = 0;
		Camera* camera = CGame::GetInstance()->GetCurrentScene()->getCamera();
		/*camera->cam_x = teleportDestination->camera_x;
		camera->cam_y = teleportDestination->camera_y;*/
		//isAllowCameraFollow = !isAllowCameraFollow;

		camera->isCameraMoving = !camera->isCameraMoving;
		camera->cam_top_limit = teleportDestination->camera_top_limit;
		camera->cam_right_limit = teleportDestination->camera_right_limit;
		camera->cam_left_limit = teleportDestination->camera_left_limit;
		camera->cam_bottom_limit = teleportDestination->camera_bottom_limit;

		

		p = teleportDestination->p;
		teleportDestination = NULL;
		nx = 1;
		v.x = 0;
		canJump = true;
		SetAction(MarioAction::IDLE);
		SetState("jumping");
		canJump = false;
		renderOrder = 1;
	}

	if (action == MarioAction::ATTACK && timeBeginAction >= 0) {
		timeBeginAction = timeBeginAction - dt;
	}
}
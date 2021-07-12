#include "Test.h"
#include <iostream>
#include "Quadtree.h"
#include "MiniPortal.h"
#include "PlayScence.h"
#include "Game.h"
#include "Effect.h"


#define VX_SMALL 0.15f
#define VY_SMALL 0.5f
#define VY_BIG 0.6f

Test::Test()
{
	width = 24;
	height = 30;
	SetState("indie");
	max_move_x = VX_SMALL;
	max_move_y = VY_SMALL;
	renderOrder = 99999;
	name = "player";
	isUniversal = true;
	g = Vector(0.0015f, 0.0015f);
}


void Test::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (untouchableTime > 0) {
		untouchableTime -= dt;
	}

	if (action == MarioAction::TRANSFORM && timeBeginAction >= 0) {
		timeBeginAction = timeBeginAction - dt;
		ax = 0;
		v.x = 0;
		return;
	}
	if (action == MarioAction::GETTING_INTO_THE_HOLE && timeBeginAction >= 0) {
		UpdateAnimation(dt, coObjects);
		return;
	}

	/*if (v.x != 0 && powerX != 0) {
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
	}*/

	if (abs(v.x) < 0.35f)
		v.x += ax * dt;

	if (v.x >= 0.15f && canJump == false) v.x = 0.15f;

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

	if (action != MarioAction::FLY)
		v.y = v.y + g.y * dt;

	/*if (v.y > 0.4f) {
		v.y = 0.4f;
	}*/



	if (v.x * nx < 0 && abs(v.x) > 0.3f) {
		SetAction(MarioAction::SKID, 500);
	}


	//DebugOut(L"V.x = %s \n", ToLPCWSTR(to_string(v.x)));


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


		//if (nx != 0) v.x = 0;
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
	ax = 0;
}

void Test::Render()
{
	if (untouchableTime > 0) {
		isRender = !isRender;
	}
	else {
		isRender = true;
	}

	if (action == MarioAction::TRANSFORM && timeBeginAction >= 100) {
		return;
	}
	if (isRender) {
		float w = width;
		float h = height;
		Vector scale = Vector((float)nx, 1.0f);

		animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h, scale);
		if (action != MarioAction::ATTACK) {
			if (abs(h - this->height) >= 10) {
				p.y = p.y - (h - this->height);
				height = h;
			}
			if (abs(w - this->width) >= 10) {
				p.x = p.x - (w - this->width);
				width = w;
			}
		}
		RenderBoundingBox();
	}
}

void Test::SetState(string state)
{

	if (isReadyChangeState == true) {

		if (action == MarioAction::HOLD) {
			if (state == "running") {
				CGameObject::SetState("running-hold");
			}
			else if (state == "indie") {
				CGameObject::SetState("indie-hold");
			}
			return;
		}

		if (state == "running-up") {
			CGameObject::SetState("running-up");
		}
		else if (state == "running-down") {
			CGameObject::SetState("running-down");
		}
		else if (state == "running") {
			CGameObject::SetState(state);
		}
		else if (state == "sitting") {
			CGameObject::SetState("sitting");
		}
		else if (state == "skid") {
			CGameObject::SetState("skid");
		}
		else if (state == "jumping") {
			CGameObject::SetState(state);
		}
		else if (state == "jump-height") {
			CGameObject::SetState("jumping");
		}
		else if (state == "fly-up" && canJump == false) {
			CGameObject::SetState("flying-speed");
		}
		else if (state == "fall-down" && canJump == false) {
			CGameObject::SetState("jumping");
		}
		else if (state == "indie") {
			v.x = 0;
			CGameObject::SetState("indie");
		}
		else if (state == "kick") {
			CGameObject::SetState("kick");
		}
		else if (state == "attack") {
			CGameObject::SetState("attack");
		}
		else if (state == "flying" && canJump == false) {
			if (abs(powerX) == 1000)
				CGameObject::SetState("flying-speed");
			else
				CGameObject::SetState("flying");
		}
		else if (state == "die") {
			CGameObject::SetState(state);
		}
		else if (state == "into-hole") {
			CGameObject::SetState(state);
		}
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
			//v.x = 0;
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
		SetState("running");
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
				v.y = -VY_SMALL;
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
			SetState("flying");
			action = MarioAction::JUMP;
			timeBeginAction = time;
		}
		break;
	case MarioAction::JUMP_HEIGHT:
		if (action == MarioAction::FLY) {
			action = MarioAction::JUMP_HEIGHT;
			timeBeginAction = time;
			ax = 0;
			v.x = 0;
		}
		else if (canJump == true && abs(powerX) == 1000) {
			v.y = -VY_BIG;

			canJump = false;
			SetState("jump-height");
			action = newAction;
			timeBeginAction = time;
		}
		/*else if (canJump == true) {
			v.y = -VY_BIG;
			SetState("jump");
			canJump = false;
			action = newAction;
			timeBeginAction = time;
		}*/
		else if (action == MarioAction::JUMP && v.y > -VY_SMALL * 0.6f && v.y < -VY_SMALL * 0.55 && powerX < 1000) {
			v.y = -VY_BIG;
			SetState("jump");
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
	case MarioAction::SKID:
		SetState("skid");
		action = newAction;
		v.x = -nx * 0.1f;
		timeBeginAction = time;
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

	if (untouchableTime > 0) return;
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

	if (marioType < stoi(type)) {
		untouchableTime = 1000;
	}
	else {
		SetAction(MarioAction::TRANSFORM, 500);
	}

	switch (marioType)
	{
	case SmallMario:
		this->type = to_string(marioType);
		max_move_y = VY_SMALL;
		v.y = 0.1f;
		life = 1;
		break;
	case BigMario:
		this->type = to_string(marioType);
		max_move_y = VY_BIG;
		v.y = 0.1f;
		life = 2;
		break;
	case RacconMario:
		this->type = to_string(marioType);
		max_move_y = VY_BIG;
		v.y = 0.1f;
		life = 3;
		break;
	default:
		break;
	}

	

	SetState("indie");
}

void Test::ProcessKeyboard(KeyboardEvent kEvent)
{
	if (action == MarioAction::TRANSFORM || action == MarioAction::GETTING_INTO_THE_HOLE) return;

	if (kEvent.isKeyUp == true) {
		holdingKeys[kEvent.key] = false;
		//return;
	}

	if (kEvent.isHolding == true) holdingKeys[kEvent.key] = true;

	switch (kEvent.key) {
	case DIK_RIGHT:
		//v.x = max_move_x;
		nx = 1;
		ax = 0.000575f;

		if (action == MarioAction::FLY && stoi(type) == RacconMario && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetState("flying");

			}
		}
		else if (action == MarioAction::JUMP_HEIGHT && stoi(type) == RacconMario && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetState("flying");
			}
		}
		else if (action == MarioAction::JUMP && stoi(type) == RacconMario && canJump == false) {
			SetState("flying");
		}
		else if (canJump == false) {
			SetState("jumping");
		}
		else {
			SetAction(MarioAction::RUN);
		}
		break;
	case DIK_LEFT:
		ax = -0.000575f;
		nx = -1;

		if (action == MarioAction::FLY && stoi(type) == RacconMario && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetState("flying");
			}
		}
		else if (action == MarioAction::JUMP_HEIGHT && stoi(type) == RacconMario && canJump == false) {
			if (holdingKeys[DIK_S] == false) {
				SetState("flying");
			}
		}
		else if (action == MarioAction::JUMP && stoi(type) == RacconMario && canJump == false) {
			SetState("flying");
		}
		else if (canJump == false) {
			SetState("jumping");
		}
		else {
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

		if (kEvent.isKeyUp == true) {
			DebugOut(L"out");
			if (action == MarioAction::HOLD) {
				SetAction(MarioAction::RELEASE);
			}
		}


		if (action != MarioAction::HOLD) {
			if (!kEvent.isHolding && !kEvent.isKeyUp)
				SetAction(MarioAction::ATTACK, 300);
		}


		break;
	case DIK_B:
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
		if (!kEvent.isHolding) {
			if (!kEvent.isKeyUp) SetAction(MarioAction::JUMP);
		}
		else SetAction(MarioAction::JUMP_HEIGHT);
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
			timeMaxPower = 5000;
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
		renderOrder = 0;
		if (destination->direction == "DOWN") {
			d.y = 0.5f;
		}
		else if (destination->direction == "UPP") {
			d.y = -10;
		}
		else {
			d.y = -0.5f;
		}
		SetAction(MarioAction::GETTING_INTO_THE_HOLE, duration);
	}
}

void Test::UpdateAnimation(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (action == MarioAction::GETTING_INTO_THE_HOLE && timeBeginAction >= 100) {
		p.y = p.y + d.y;
		timeBeginAction = timeBeginAction - dt;
	}
	else if (action == MarioAction::GETTING_INTO_THE_HOLE && timeBeginAction < 100 && teleportDestination != NULL) {
		timeBeginAction = 0;
		Camera* camera = CGame::GetInstance()->GetCurrentScene()->getCamera();
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

void Test::HandleAfterCreated() {
	switch (stoi(type))
	{
	case SmallMario:
		max_move_y = VY_SMALL;
		v.y = 0.1f;
		life = 1;
		break;
	case BigMario:
		max_move_y = VY_BIG;
		v.y = 0.1f;
		life = 2;
		break;
	case RacconMario:
		max_move_y = VY_BIG;
		v.y = 0.1f;
		life = 3;
		break;
	default:
		break;
	}
}

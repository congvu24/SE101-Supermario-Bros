#include "Venus.h"
#include "Vector.h"
#include "VenusBullet.h"
#include "PlayScence.h"
#include <iostream>



LPDIRECT3DTEXTURE9 Venus::texture = NULL;
unordered_map<string, LPSPRITE> Venus::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Venus::all_animations; //save all animations
CAnimationSets Venus::animations_set; //save all the animation sets
json Venus::data = NULL;


Venus::Venus()
{
	SetState("running");
	g = Vector(0, 0);
	action = VenusAction::HIDING;
	renderOrder = 0;
	isBlockPlayer = true;
}

void Venus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
	p = p + d;

	Vector playerPos = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->player->p;
	Vector bulletBeginPos = Vector(p.x, p.y);
	Vector distance = playerPos - bulletBeginPos;
	if (distance.x > 0) nx = 1;
	else nx = -1;

	if (action == VenusAction::HIDING && p.y >= oldP.y + height) {
		SetAction(VenusAction::SHOWING);
	}


	if (action == VenusAction::SHOWING && p.y <= oldP.y) {
		SetAction(VenusAction::ATTACK);
	}

	if (action == VenusAction::ATTACK) {

		if (abs(playerPos.x - p.x) <= 400) {
			LPGAMEOBJECT bullet = new VenusBullet();
			Vector direction = Vector::Normalize(distance);
			bullet->ParseFromOwnJson();
			bullet->p = bulletBeginPos;
			((VenusBullet*)bullet)->direction = direction;
			CGame::GetInstance()->GetCurrentScene()->addObject(bullet);
			SetAction(VenusAction::HIDING);
		}
		else {
			SetAction(VenusAction::HIDING);
		}
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
}

void Venus::Render() {
	if (state == "hidden") return;
	else {
		float width = 0;
		float height = 0;
		Vector scale = Vector(-nx, 1.0f);
		Venus::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height, scale);
		RenderBoundingBox();
	}
}

void Venus::SetState(string state)
{
	CGameObject::SetState(state);
}

void Venus::SetAction(VenusAction newAction) {
	if (newAction == VenusAction::HIDING) {
		v.y = 0.03f;
	}
	else if (newAction == VenusAction::SHOWING) {
		v.y = -0.03f;
	}
	else if (newAction == VenusAction::ATTACK) {
		v.y = 0;
		SetState("attack");
	}

	this->action = newAction;
}

void Venus::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Venus::HandleCollision(LPCOLLISIONEVENT e) {

}

void Venus::HandleAfterCreated() {
	DebugOut(L"here");
	oldP = p;
	SetAction(VenusAction::HIDING);
}

void Venus::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	//Enemy::OnHadCollided(obj, event);

	if (Test* player = dynamic_cast<Test*>(obj)) {
		KillPlayer(player);
	}
}
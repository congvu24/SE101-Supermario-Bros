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
		DebugOut(L"het han");
		SetAction(VenusAction::SHOWING);
	}


	if (action == VenusAction::SHOWING && p.y <= oldP.y) {
		DebugOut(L"het han");
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

	/*coEvents.clear();
	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if ((*i)->isAllowCollision == true && (*i)->name == "RectCollision") {
			LPCOLLISIONEVENT e = SweptAABBEx(*i);

			if (e->t > 0 && e->t <= 1.0f) {
				coEvents.push_back(e);
			}
			else {
				delete e;
			}
		}

	}*/
	//if (coEvents.size() == 0) {

	//	p = p + d;
	//}
	//else {
		//float min_tx, min_ty, nx = 0, ny;
		//float rdx = 0;
		//float rdy = 0;

		//FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//if (rdx != 0 && rdx != d.x)
		//	p.x += nx * abs(rdx);

		//// block every object first!

		//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//}
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
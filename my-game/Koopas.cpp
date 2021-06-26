#include "Koopas.h"
#include "Vector.h"
#include "Test.h"
#include "PlayScence.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Koopas::texture = NULL;
unordered_map<string, LPSPRITE> Koopas::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Koopas::all_animations; //save all animations
CAnimationSets Koopas::animations_set; //save all the animation sets
json Koopas::data = NULL;

Koopas::Koopas()
{
	SetState("running");
	v = Vector(0.05f, 0);
	isBlockPlayer = false;
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{


	CGameObject::Update(dt, coObjects);
	v = v + g * dt;
	if (v.y > 0.35f) v.y = 0.35f;

	if (v.x > 0) nx = 1;
	else nx = -1;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		//if (state != "die" || (state == "die" && isHitted == false)) {

		//	if ((*i)->isAllowCollision == true) {
		//		LPCOLLISIONEVENT e = SweptAABBEx(*i);

		//		if (e->t > 0 && e->t <= 1.0f) {
		//			coEvents.push_back(e);
		//		}
		//		else {
		//			delete e;
		//		}
		//	}
		//}
		//else if (state == "die" && isHitted == true && (*i)->isAllowCollision == true) {
		//	//if (CPlayScene::IsPlayer(*i) == true || (*i)->name == "RectCollision") {
		//	//if ((*i)->name != "RectPlatform") {

		//	LPCOLLISIONEVENT e = SweptAABBEx(*i);

		//	if (e->t > 0 && e->t <= 1.0f) {
		//		coEvents.push_back(e);
		//	}
		//	else {
		//		delete e;

		//	}
		//	//}
		//	//}
		//}
		if (state == "die") {
			if (((*i)->isAllowCollision == true && (*i)->name != "RectPlatform") || CPlayScene::IsPlayer(*i)) {
				LPCOLLISIONEVENT e = SweptAABBEx(*i);

				if (e->t > 0 && e->t <= 1.0f) {
					coEvents.push_back(e);
				}
				else {
					delete e;
				}
			}
		}
		else {
			if ((*i)->isAllowCollision == true) {
				LPCOLLISIONEVENT e = SweptAABBEx(*i);

				if (e->t > 0 && e->t <= 1.0f) {
					coEvents.push_back(e);
				}
				else {
					delete e;
				}
			}
		}

	}
	if (coEvents.size() == 0) {

		p = p + d;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		//if (state != "die") {
		if (nx != 0) v.x = -v.x;
		if (ny != 0) v.y = 0;
		//}


		for (UINT i = 0; i < coEventsResult.size(); i++) {

			HandleCollision(coEventsResult[i]);
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	coEvents.clear();
	coEventsResult.clear();
}


void Koopas::SetState(string state)
{
	if (state == "running") {
	}
	else if (state == "die") {
		v = Vector(0, 0);
	}
	else if (state == "hidden") {

	}

	CGameObject::SetState(state);

}

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void Koopas::HandleCollision(LPCOLLISIONEVENT e) {

	if (Test* player = dynamic_cast<Test*>(e->obj)) {

		if (state != "die") {

			if (e->nx != 0 && e->ny == 0) {
				player->Die();
			}
			else if (e->ny > 0) {
				SetState("die");
			}
		}
		else if (state == "die") {
			if (isHitted == false && e->nx != 0) {
				player->SetState("kick");
				isUniversal = true;
				this->v.x = 0.5f * e->nx;
				isHitted = true;
			}
			else if (isHitted == true && e->nx != 0) {
				player->Die();
				this->v.x = 0.5f * -e->nx;
			}
		}
	}

	if (state == "die" && isHitted == true && e->nx != 0) {
		if (Goomba* obj = dynamic_cast<Goomba*>(e->obj)) {
			obj->Die();
			this->v.x = 0.5f * -e->nx;

		}
	}
}
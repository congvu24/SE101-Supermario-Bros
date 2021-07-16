#include "MiniPortal.h"
#include "Vector.h"
#include "PlayScence.h"
#include <iostream>


LPDIRECT3DTEXTURE9 MiniPortal::texture = NULL;
unordered_map<string, LPSPRITE> MiniPortal::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> MiniPortal::all_animations; //save all animations
CAnimationSets MiniPortal::animations_set; //save all the animation sets
json MiniPortal::data = NULL;

MiniPortal::MiniPortal()
{
	isAllowCollision = false;
	isBlockPlayer = false;
}

void MiniPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* checkObjects = new vector<LPGAMEOBJECT>();

	coEvents.clear();
	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if (CPlayScene::IsPlayer((*i)) == true) {
			checkObjects->push_back((*i));
		}
	}

	CalcPotentialCollisions(checkObjects, coEvents);

	float min_tx, min_ty, nx = 0, ny;
	float rdx = 0;
	float rdy = 0;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	for (UINT i = 0; i < coEventsResult.size(); i++) {

		HandleCollision(coEventsResult[i]);
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	coEvents.clear();
	coEventsResult.clear();
	checkObjects->clear();
	delete checkObjects;

}


void MiniPortal::SetState(string state)
{
	CGameObject::SetState(state);

}


void  MiniPortal::Render()
{
}

void MiniPortal::HandleCollision(LPCOLLISIONEVENT e) {

	if (type == "In" && e->nx == 0 && e->ny != 0) {

		if (Mario* player = dynamic_cast<Mario*>(e->obj)) {

			MiniPortal* destination = NULL;
			vector<CGameObject*>* allObjectOfSence = &(CGame::GetInstance()->GetCurrentScene()->objects);

			for (auto i = allObjectOfSence->begin(); i != allObjectOfSence->end(); i++)
			{
				if (MiniPortal* obj = dynamic_cast<MiniPortal*>(*i)) {
					if (obj->portalName == this->portalName && obj->type == "Out") {
						destination = obj;
						break;
					}
				}
			}

			if (destination != NULL) {
				player->Teleport(destination, 2000);
			}
		}
	}

}

void MiniPortal::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
}
#include "MusicBox.h"
#include "Vector.h"
#include "PButton.h"
#include "PlayScence.h"
#include "Mushroom.h"
#include <iostream>


LPDIRECT3DTEXTURE9 MusicBox::texture = NULL;
unordered_map<string, LPSPRITE> MusicBox::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> MusicBox::all_animations; //save all animations
CAnimationSets MusicBox::animations_set; //save all the animation sets
json MusicBox::data = NULL;

MusicBox::MusicBox()
{
	SetState("running");
	oldP = Vector(0, 0);
	d = Vector(0, 0);
	v = Vector(0, 0);
	g = Vector(0, 0);
	nx = -1;
	isAllowCollision = true;
	isBlockPlayer = true;

	allowToHitBottom = false;
	allowToHitTop = true;
	allowToHitLeft = false;
	allowToHitRight = false;
}

void MusicBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Box::Update(dt, coObjects);
}


void MusicBox::Render() {
	if (type == "JumpUp" && countHit < 1) {
		return;
	}
	Box::Render();
}

void MusicBox::SetState(string state)
{
	CGameObject::SetState(state);

}


void MusicBox::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (type == "JumpUp") {

		if (CPlayScene::IsPlayer(obj)) {
			if (countHit == 0 && event->ny > 0 && event->nx == 0) {
				countHit = 1;
				return;
			}


			if (isHitted == false && event->ny < 0 && event->nx == 0 && countHit >= 1 && countHit < 4) {
				countHit++;
				Box::OnHadCollided(obj, event);
			}
			else if (isHitted == false && event->ny < 0 && event->nx == 0 && countHit == 4) {
				isHitted = true;
				if (Test* player = dynamic_cast<Test*>(obj)) {
					player->v.y = -1;
					MiniPortal* destination = NULL;
					vector<CGameObject*>* allObjectOfSence = &(CGame::GetInstance()->GetCurrentScene()->objects);

					for (auto i = allObjectOfSence->begin(); i != allObjectOfSence->end(); i++)
					{
						if (MiniPortal* obj = dynamic_cast<MiniPortal*>(*i)) {
							if (obj->portalName == this->portName && obj->type == "Out") {
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
	}
	else {
		Box::OnHadCollided(obj, event);
		if (isHitted == false && event->ny < 0 && event->nx == 0) {
			isHitted = true;
		}
	}
}

void MusicBox::OnHadCollidedHorizontal(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (Koopas* kopass = dynamic_cast<Koopas*>(obj)) {
		if (kopass->state == "die") {
			JumpUp(kopass);
		}
	}
}

void MusicBox::Explore() {

}

void MusicBox::HandleAfterCreated() {
	if (type == "JumpUp") {
		allowToHitBottom = true;
		isAllowCollision = false;
		isBlockPlayer = true;
	}
	else {
		allowToHitLeft = true;
		allowToHitRight = true;
	}
}

void MusicBox::GiveReward() {

}

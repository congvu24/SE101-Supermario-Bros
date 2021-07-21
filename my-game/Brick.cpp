#include "Brick.h"
#include "Mushroom.h"
#include "Vector.h"
#include "PlayScence.h"
#include <iostream>


LPDIRECT3DTEXTURE9 Brick::texture = NULL;
unordered_map<string, LPSPRITE> Brick::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> Brick::all_animations; //save all animations
CAnimationSets Brick::animations_set; //save all the animation sets
json Brick::data = NULL;
json Brick::spriteData = NULL;
Brick::Brick()
{
	SetState("running");
	oldP = Vector(0, 0);
	d = Vector(0, 0);
	v = Vector(0, 0);
	g = Vector(0, 0);
	nx = -1;
	isAllowCollision = true;
	allowToHitTop = true;
}

void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Box::Update(dt, coObjects);
}


void Brick::SetState(string state)
{

	if (state == "running") {
	}
	else if (state == "hitting") {

	}
	else if (state == "hitted") {

	}


	CGameObject::SetState(state);

}

void Brick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}

void Brick::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {

	Box::OnHadCollided(obj, event);

	if (CPlayScene::IsPlayer(obj)) {
		if (isHitted == false && event->ny > 0 && event->nx == 0) {
			GiveReward();
		}
	}
}

void Brick::GiveReward() {

	if (isHitted == false) {
		isHitted = true;
		LPGAMEOBJECT reward = NULL;
		if (name == "QuestionBox_Coin") {
			reward = new Coin();
			reward->ParseFromOwnJson();
			reward->height = height;
			reward->width = width;
			reward->p = p;
			reward->p.y = p.y - height;
			reward->isAllowCollision = false;
			CGame::GetInstance()->GetCurrentScene()->addObject(reward);
			reward->SetState("fromBrick");
		}
		else if (name == "QuestionBox_Mushroom") {
			reward = new Mushroom();
			reward->ParseFromOwnJson();
			reward->height = height;
			reward->width = width;
			reward->p = p;
			reward->p.y = p.y - height;
			reward->name = "Mushroom";
			reward->isAllowCollision = true;
			CGame::GetInstance()->GetCurrentScene()->addObject(reward);
			reward->SetState("fromBrick");
		}
		else {
			reward = new Leaf();
			reward->ParseFromOwnJson();
			reward->height = height;
			reward->width = width;
			reward->p = p;
			reward->p.y = p.y - height;
			reward->name = "Leaf";
			reward->isAllowCollision = true;
			CGame::GetInstance()->GetCurrentScene()->addObject(reward);
			reward->SetState("fromBrick");
		}
	}

}


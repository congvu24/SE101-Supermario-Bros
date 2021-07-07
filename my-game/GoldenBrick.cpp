#include "GoldenBrick.h"
#include "Vector.h"
#include "PButton.h"
#include "PlayScence.h"
#include "Mushroom.h"
#include <iostream>


LPDIRECT3DTEXTURE9 GoldenBrick::texture = NULL;
unordered_map<string, LPSPRITE> GoldenBrick::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> GoldenBrick::all_animations; //save all animations
CAnimationSets GoldenBrick::animations_set; //save all the animation sets
json GoldenBrick::data = NULL;

GoldenBrick::GoldenBrick()
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
	allowToHitTop = false;
	allowToHitLeft = false;
	allowToHitRight = false;
}

void GoldenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Box::Update(dt, coObjects);
}


void GoldenBrick::SetState(string state)
{

	if (state == "running") {
	}
	else if (state == "hitting") {
	}
	else if (state == "hitted") {
	}

	CGameObject::SetState(state);

}

void GoldenBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


//void GoldenBrick::CollisionWithMario(LPCOLLISIONEVENT e) {
//
//}

void GoldenBrick::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	Box::OnHadCollided(obj, event);

	if (CPlayScene::IsPlayer(obj)) {
		if (isHitted == false && event->ny > 0 && event->nx == 0) {
			GiveReward();
		}
	}
}

void GoldenBrick::OnHadCollidedHorizontal(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (type == "P" && isHitted == false) {
		vector<LPGAMEOBJECT>* effectList = new vector<LPGAMEOBJECT>();
		vector<LPGAMEOBJECT>* allObjects = &((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->objects;

		for (auto i = allObjects->begin(); i != allObjects->end(); i++)
			if (GoldenBrick* obj = dynamic_cast<GoldenBrick*>(*i)) {
				if (obj->type != "P" && obj->state == "running") effectList->push_back(*i);
			}

		this->SetState("hitted");
		LPGAMEOBJECT reward = NULL;
		reward = new PButton();
		reward->ParseFromOwnJson();
		reward->p.y = p.y;
		reward->p.x = p.x + width / 2;
		reward->isAllowCollision = true;
		((PButton*)reward)->listEffect = effectList;
		CGame::GetInstance()->GetCurrentScene()->addObject(reward);
		isHitted = true;
	}
}

void GoldenBrick::Explore() {
	if (type == "Break" && state == "running") {
		isAllowCollision = false;
		this->SetState("hidden");
	}
	else if (type != "P" && state == "running") {
		LPGAMEOBJECT reward = NULL;
		reward = new Coin();
		reward->ParseFromOwnJson();
		reward->p.y = p.y + height / 2;
		reward->p.x = p.x + width / 2;
		reward->SetState("stop");
		reward->isAllowCollision = false;
		CGame::GetInstance()->GetCurrentScene()->addObject(reward);
		this->SetState("hidden");
	}

}

void GoldenBrick::HandleAfterCreated() {
	if (type == "OneUpGreen") {
		allowToHitBottom = true;
		allowToHitTop = false;
	}

	if (type == "Break") {
		allowToHitBottom = false;
		allowToHitTop = false;
		allowToHitLeft = true;
		allowToHitRight = true;
	}

	if (type == "Coin") {
		allowToHitBottom = true;
		allowToHitTop = false;
		allowToHitLeft = false;
		allowToHitRight = false;
	}
}

void GoldenBrick::GiveReward() {
	if (isHitted == false) {
		LPGAMEOBJECT reward = NULL;
		if (type == "OneUpGreen") {
			isHitted = true;
			reward = new Mushroom("green");
			reward->ParseFromOwnJson();
			reward->height = height;
			reward->width = width;
			reward->p = p;
			reward->p.y = p.y - height;
			reward->v.x = -1 * v.x;
			reward->name = "Mushroom";
			reward->isAllowCollision = true;
			CGame::GetInstance()->GetCurrentScene()->addObject(reward);
			reward->SetState("fromMisteryBox");
			SetState("hitted");

		}
		if (type == "Coin" && countHit < 10) {
			isHitted = false;
			reward = new Coin();
			reward->ParseFromOwnJson();
			reward->height = height;
			reward->width = width;
			reward->p = p;
			reward->p.y = p.y - height;
			reward->isAllowCollision = false;
			CGame::GetInstance()->GetCurrentScene()->addObject(reward);
			reward->SetState("fromMisteryBox");
			countHit++;
			if (countHit == 10) {
				SetState("hitted");
				isHitted = true;
			}
		}
	}
}

// void GoldenBrick::JumpUp(LPGAMEOBJECT obj) {
//	if (isHitted == false && !isJumping) {
//		isJumping = true;
//		isHitted = true;
//		oldP = p;
//		v.y = -0.3f;
//		jumpDirection = -1;
//		g.y = 0.001f;
//	}
//}
#include "EndSceneItem.h"
#include "Vector.h"
#include "Test.h"
#include "PlayScence.h"
#include <iostream>


LPDIRECT3DTEXTURE9 EndSceneItem::texture = NULL;
unordered_map<string, LPSPRITE> EndSceneItem::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> EndSceneItem::all_animations; //save all animations
CAnimationSets EndSceneItem::animations_set; //save all the animation sets
json EndSceneItem::data = NULL;

EndSceneItem::EndSceneItem()
{
	SetState("Mushroom");
	g = Vector(0, 0);
	reward = EndSceneItemReward::Mushroom;
	currenRewardIndex = 0;
	isBlockPlayer = true;
	isAllowCollision = false;
}

void EndSceneItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);

	if (timeChangeItem >= 0) {
		timeChangeItem = timeChangeItem - dt;
	}
	else {
		ChangeReward();
	}

	p = p + d;
}

void  EndSceneItem::Render()
{
	float w = width;
	float h = height;
	Vector scale = Vector((float)-nx, 1.0f);
	EndSceneItem::animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h, scale);
	p.y = p.y - (h - this->height) / 2;
	p.x = p.x - (w - this->width) / 2;
	width = w;
	height = h;
	RenderBoundingBox();
}



void EndSceneItem::SetState(string state)
{
	CGameObject::SetState(state);
}

void EndSceneItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void EndSceneItem::HandleCollision(LPCOLLISIONEVENT e) {
}

void EndSceneItem::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	if (isTouched == false && state != "hidden") {
		//SetState("hidden");
		isTouched = true;
		isAllowCollision = false;
		v.y = -0.3f;

		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GameOver();
	}
}

void EndSceneItem::ChangeReward() {
	if (isTouched == false) {
		currenRewardIndex++;
		reward = EndSceneItemReward(currenRewardIndex % 3);
		switch (reward)
		{
		case EndSceneItemReward::Mushroom:
			SetState("Mushroom");
			break;
		case EndSceneItemReward::Star:
			SetState("Star");
			break;
		case EndSceneItemReward::Tree:
			SetState("Tree");
			break;
		default:
			break;
		}
		timeChangeItem = 500;
	}
} 
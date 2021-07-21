#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include <iostream>


enum class EndSceneItemReward {
	Mushroom = 0,
	Star,
	Tree
};


class EndSceneItem :public MapEntity<EndSceneItem>
{
	//virtual void  Render();
public:
	EndSceneItem();

	bool isTouched = false;
	int timeChangeItem = 500;
	int currenRewardIndex = 0;
	EndSceneItemReward reward = EndSceneItemReward::Mushroom;

	Vector oldP;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event);
	virtual void Render();
	virtual void ChangeReward();


	static json spriteData;
	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};
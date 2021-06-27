#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include "Coin.h"
#include "Leaf.h"
#include <iostream>

class MisteryBox :public MapEntity<MisteryBox>
{

public:
	MisteryBox();

	Vector oldP;
	bool isHitted = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(string state);
	virtual void CollisionWithMario(LPCOLLISIONEVENT e);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {}

	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};
#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include "Enemy.h"
#include <iostream>

class HammerBrother :public Enemy<HammerBrother>
{

public:
	HammerBrother();

	float sumMoveX = 0;
	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event);

	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};
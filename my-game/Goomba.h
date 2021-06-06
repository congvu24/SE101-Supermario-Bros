#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include <iostream>

class Goomba :public MapEntity<Goomba>
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	Vector oldP;
	//int maxMoveX = 200;
	//virtual void  Render();
public:
	Goomba();

	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);


	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};
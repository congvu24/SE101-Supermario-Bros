#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include <iostream>

class SelectPortal :public MapEntity<SelectPortal>
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	SelectPortal();
	string portalName;
	string scene_id;

	virtual void SetState(string state);
	virtual void HandleCollision(LPCOLLISIONEVENT e);

	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};
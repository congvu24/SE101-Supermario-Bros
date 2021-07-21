#pragma once
#include "GameObject.h"
#include "MapEntity.h"
#include "Game.h"
#include "Enemy.h"

enum class VenusAction {
	HIDING,
	ATTACK,
	SHOWING
};

class Venus :public Enemy<Venus>
{

public:
	Venus();
	VenusAction action;

	Vector oldP;
	virtual void SetState(string state);
	virtual void SetAction(VenusAction newAction);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void HandleAfterCreated();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event);
	virtual void BeingKill() {};


	static json spriteData;
	static json data;
	static LPDIRECT3DTEXTURE9 texture;
	static unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	static unordered_map<string, LPANIMATION> all_animations; //save all animations
	static CAnimationSets animations_set; //save all the animation 
};
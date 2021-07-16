#pragma once
#include "GameObject.h"
#include "Character.h"
#include "Game.h"
#include "MiniPortal.h"
#include <iostream>


#define SmallMario 1
#define BigMario 2
#define RacconMario 4

#define A_X 0.000575f
#define JUMP_VY 0.8f
#define HIGHT_JUMP_VY 1.2f
#define FLY_VY 0.8f
#define DEFAULT_GY 0.0015f
#define DEFAULT_RENDER_ORDER 9999

#define UNTOUCHABLE_TIME 1000
#define MAX_VX 0.35f
#define MAX_VY 0.6f
#define MAX_POWER 1000
#define ASC_POWER 20
#define TIME_MAX_POWER 5000
#define DESC_POWER 40
#define VX_TO_SKID 0.3f
#define VX_AFTER_SKID 0.1f

#define MIN_DIFF_UPDATE 10

#define SMALL_LIFE 1
#define BIG_LIFE 2
#define RACCON_LIFE 3

enum class MarioAction {
	IDLE,
	WALK,
	RUN,
	JUMP,
	CROUCH,
	FLY,
	JUMP_HEIGHT,
	FALL,
	HOLD,
	ATTACK,
	KICK,
	DIE,
	GETTING_INTO_THE_HOLE,
	TRANSFORM,
	PICK_UP,
	RELEASE,
	SKID
};



class Test : public Character
{
public:
	Test();
	bool canJump = true;
	bool isReadyChangeState = true;
	float powerX = 0;
	int timeMaxPower = 0;
	int timeBeginAction = 0;
	bool isAllowCameraFollow = true;
	int life = 1;
	int point = 0;
	float ax = 0;
	int untouchableTime = 0;
	bool isRender = true;

	MarioAction action;
	unordered_map<int, bool> holdingKeys;
	MiniPortal* teleportDestination;
	LPGAMEOBJECT* holdObject;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void UpdateAnimation(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(string state);
	virtual void SetAction(MarioAction newAction, DWORD time = 0);
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void HandleCollisionVertical(LPCOLLISIONEVENT e);
	virtual void HandleCollisionHorizontal(LPCOLLISIONEVENT e);
	virtual void Die();
	virtual void Transform(int marioType);
	virtual void ProcessKeyboard(KeyboardEvent kEvent);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {}
	virtual void IncreasePowerX();
	virtual void DecreasePowerX();
	virtual bool IsReadyToChangeAction();
	virtual void Teleport(MiniPortal* destination, int duration);
	virtual void HandleAfterCreated();
	virtual void UpdateTimeAction();
};
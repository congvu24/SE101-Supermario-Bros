#include "HammerBrother.h"
#include "Vector.h"
#include "Test.h"
#include "PlayScence.h"
#include <iostream>


LPDIRECT3DTEXTURE9 HammerBrother::texture = NULL;
unordered_map<string, LPSPRITE> HammerBrother::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> HammerBrother::all_animations; //save all animations
CAnimationSets HammerBrother::animations_set; //save all the animation sets
json HammerBrother::data = NULL;

HammerBrother::HammerBrother()
{
	SetState("running");
	v = Vector(0.05f, 0);
	isAllowCollision = true;
	isBlockPlayer = true;
	useLimit = false;
}

void HammerBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	p.x = p.x + d.x;
	sumMoveX = sumMoveX + abs(d.x);
	if (sumMoveX >= 100) {
		v.x = v.x * -1;
		sumMoveX = 0;
	}
	if (v.x >= 0) nx = 1;
	else nx = -1;
}


void HammerBrother::SetState(string state)
{
	CGameObject::SetState(state);

}

void HammerBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void HammerBrother::HandleCollision(LPCOLLISIONEVENT e) {
	Enemy::HandleCollision(e);
}




void HammerBrother::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
	Enemy::OnHadCollided(obj, event);
}
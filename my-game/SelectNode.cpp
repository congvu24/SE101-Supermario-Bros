#include "SelectNode.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 SelectNode::texture = NULL;
unordered_map<string, LPSPRITE> SelectNode::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> SelectNode::all_animations; //save all animations
CAnimationSets SelectNode::animations_set; //save all the animation sets
json SelectNode::data = NULL;

SelectNode::SelectNode()
{
	SetState("running");
	p = Vector(0, 0);

}

void SelectNode::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void SelectNode::SetState(string state)
{
	CGameObject::SetState(state);

}

void SelectNode::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}

void  SelectNode::Render()
{
	/*float width = 0;
	float height = 0;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, width, height);
	RenderBoundingBox();*/
}

void SelectNode::HandleCollision(LPCOLLISIONEVENT e) {

	/*DebugOut(L"nx %s \n", IntToLPCWSTR(e->nx));
	DebugOut(L"ny %s \n", IntToLPCWSTR(e->ny));*/



	/*if (e->nx == 0 && e->ny < 0) {
		isAllowCollision = true;
	}
	else {
		isAllowCollision = false;
	}*/
}
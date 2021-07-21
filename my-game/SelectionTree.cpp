#include "SelectionTree.h"
#include "Vector.h"
#include <iostream>


LPDIRECT3DTEXTURE9 SelectionTree::texture = NULL;
unordered_map<string, LPSPRITE> SelectionTree::sprites; //save all sprite of animation
unordered_map<string, LPANIMATION> SelectionTree::all_animations; //save all animations
CAnimationSets SelectionTree::animations_set; //save all the animation sets
json SelectionTree::data = NULL;
json SelectionTree::spriteData = NULL;
SelectionTree::SelectionTree()
{
	SetState("idle");

}

void SelectionTree::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}


void SelectionTree::SetState(string state)
{
	CGameObject::SetState(state);

}

void SelectionTree::HandleCollision(LPCOLLISIONEVENT e) {
}
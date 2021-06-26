#include "MarioSelection.h"
#include <iostream>
#include "Quadtree.h"
#include "MiniPortal.h"
#include "SelectNode.h"
#include "SelectPortal.h"
#include "WorldSelect.h"
#include "Game.h"


MarioSelection::MarioSelection()
{

	width = 24;
	height = 30;
	SetState("indie");
	g = Vector(0, 0);
	//v = Vector(0.15f, 0);
	right = "start";
	left = "";
	down = "";
	up = "";
}


void MarioSelection::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	v = v + g * dt;
	if (v.y > 0.35f) v.y = 0.35f;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	for (auto i = coObjects->begin(); i != coObjects->end(); i++)
	{
		if ((*i)->isAllowCollision == true) {
			LPCOLLISIONEVENT e = SweptAABBEx(*i);

			if (e->t > 0 && e->t <= 1.0f) {
				coEvents.push_back(e);
			}
			else {
				delete e;
			}
		}
	}

	if (coEvents.size() == 0) {

		p = p + d;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		if (rdx != 0 && rdx != d.x)
			p.x += nx * abs(rdx);

		p = p + d;

		for (UINT i = 0; i < coEvents.size(); i++)
		{
			if (coEvents[i]->obj->name == "SelectionNode") {

				if (WorldSelect* scene = dynamic_cast<WorldSelect*>(CGame::GetInstance()->GetCurrentScene())) {
					if (SelectNode* obj = dynamic_cast<SelectNode*>(coEvents[i]->obj)) {
						scene->currentNode = obj;
						scene->isMoving = true;

						this->right = obj->right;
						this->left = obj->left;
						this->down = obj->down;
						this->up = obj->up;
					}
				}

				isMoving = false;
				v = Vector(0, 0);
				DebugOut(L"collision with selection node %s \n", ToLPCWSTR(coEvents[i]->obj->name));
			}
			else
				if (coEvents[i]->obj->name == "SelectPortal") {
					isMoving = false;
					DebugOut(L"collision with selection portal %s \n", ToLPCWSTR(coEvents[i]->obj->name));
					if (SelectPortal* obj = dynamic_cast<SelectPortal*>(coEvents[i]->obj)) {
						DebugOut(L"collision with selection portal %s \n", ToLPCWSTR(obj->scene_id));
						if (WorldSelect* scene = dynamic_cast<WorldSelect*>(CGame::GetInstance()->GetCurrentScene())) {
							scene->currentPortal = obj;
						}
					}
					if (nx != 0) v.x = 0;
					if (ny != 0) v.y = 0;
				}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void MarioSelection::Render()
{
	float w = width;
	float h = height;
	animations_set.Get(type).at(state)->Render(p.x, p.y, 255, w, h);
	RenderBoundingBox();
}

void MarioSelection::SetState(string state)
{

	if (isMoving == false) {

		if (state == "running-right" && right != "") {
			v.x = 0.15f;
			v.y = 0;
			isMoving = true;
		}
		else if (state == "running-left" && left != "") {
			v.x = -0.15f;
			v.y = 0;
			isMoving = true;
		}
		else if (state == "running-up" && up != "") {
			v.y = -0.15f;
			v.x = 0;
			isMoving = true;
		}
		else if (state == "running-down" && down != "") {
			v.y = 0.15f;
			v.x = 0;
			isMoving = true;
		}
		else if (state == "indie") {
			v.x = 0;
			v.y = 0;
		}
	}
	CGameObject::SetState(state);

}

void MarioSelection::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}


void MarioSelection::HandleCollision(LPCOLLISIONEVENT e) {

}


void MarioSelection::ProcessKeyboard(KeyboardEvent kEvent) {

}
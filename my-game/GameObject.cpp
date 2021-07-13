#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "RectPlatform.h"


LPDIRECT3DTEXTURE9 CGameObject::bboxtex = NULL;

CGameObject::CGameObject()
{
	p = Vector(0, 0);
	v = Vector(0, 0);
	d = Vector(0, 0);
	isAllowCollision = true;
	name = "";
	nx = 1;
	type = "";
	renderOrder = 1; // initial render order should be 1;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	d = v * dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->d.x - sdx;
	float rdy = this->d.y - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);

	if (ml < mr && mt < mb && sl != sr && st != sb) {
		if (
			CGameObject::HasOverLap(Vector(ml, mt), Vector(mr, mb), Vector(sl, st), Vector(sr, sb))
			) {
			//coO->OnHadCollided(this);
			this->OnHadCollided(coO, e);
		}
	}


	return e;
}


//filter collision with moving object 
void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {

			if (dynamic_cast<RectPlatform*>(c->obj)) {
				//  allow Mario go in x
			}
			else {
				min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
			}

		}

		if (c->t < min_ty && c->ny != 0) {
			if (dynamic_cast<RectPlatform*>(c->obj) && c->ny == 1) {
				//min_ty = 1; ny = 2; min_iy = i; rdy = c->dy;8
			}
			else {
				min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
			}

		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}



void CGameObject::RenderBoundingBox()
{
	/*D3DXVECTOR3 p(p.x, p.y, 0);
	Rect rect;

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	if (CGameObject::bboxtex != NULL)
		CGame::GetInstance()->Draw(p.x, p.y, CGameObject::bboxtex, rect.left, rect.top, rect.right, rect.bottom, 100);*/
}


CGameObject::~CGameObject() {}

void CGameObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = p.x;
	top = p.y;
	right = p.x + width;
	bottom = p.y + height;
}

void CGameObject::clear()
{
	texture = NULL;
	active_animation_set.clear();
}

void CGameObject::LoadBoundedBox(LPCWSTR path) {
	if (CGameObject::bboxtex == NULL) {
		CGameObject::bboxtex = CGame::LoadTexture(path);
	}
}

void CGameObject::HandleCollision(LPCOLLISIONEVENT e) {

}

bool CGameObject::HasOverLap(Vector l1, Vector r1, Vector l2, Vector r2) {
	if (l1.x >= r2.x || l2.x >= r1.x)
		return false;

	// If one rectangle is above other 
	if (l1.y >= r2.y || l2.y >= r1.y)
		return false;

	return true;
}

void CGameObject::OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event) {
}

void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}


void CGameObject::UpdateWithCollision(vector<LPGAMEOBJECT>* coObjects, int& o_ny, int& o_nx) {
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		p = p + d;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (int i = 0; i < coEventsResult.size(); i++) {

			if (coEventsResult[i]->nx != 0) {
			/*	if (!VerifyCollidedLeftRight(coEventsResult[i]->obj) || coEventsResult[i]->obj->name == "RectPlatform") {
					min_tx = 1;
				}
				else {
					v.x = 0;
				}*/
				/* if (coEventsResult[i]->nx > 0) CollidedRight(coEventsResult[i]->obj);
				 else CollidedLeft(coEventsResult[i]->obj);*/
			}
			else if (coEventsResult[i]->ny != 0) {
			/*	if (coEventsResult[i]->obj->name == "RectPlatform") {
					min_ty = 1;
				}
				else {
					if (ny != 2) v.y = 0;
				}*/
				/* if (coEventsResult[i]->ny > 0) CollidedBottom(coEventsResult[i]->obj);
				 else CollidedTop(coEventsResult[i]->obj);*/
			}

			//if (coEventsResult[i]->obj->name != "") {
				HandleCollision(coEventsResult[i]);
			//}

		}


		// block every object first!
		p.x += min_tx * d.x + nx * 0.4f;
		//if(ny != 0)
		p.y += min_ty * d.y + ny * 0.4f;

		o_ny = ny;
		o_nx = nx;

	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

bool CGameObject::VerifyCollidedLeftRight(LPGAMEOBJECT obj) {

	float ml, mt, mr, mb, sl, st, sr, sb = 0;
	obj->GetBoundingBox(sl, st, sr, sb);
	this->GetBoundingBox(ml, mt, mr, mb);
	if ((mb > st && sb > mt) || abs(v.y) > 0.1f) return true;
	return false;
}
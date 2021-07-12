#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"
#include "library/json.hpp"
#include "Utils.h"
#include "Vector.h"


using json = nlohmann::json;

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box


typedef struct Rect {
	FLOAT left;
	FLOAT top;
	FLOAT right;
	FLOAT bottom;
} Rect;

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{

public:
	int id = 0;
	string name;
	bool isAllowCollision;
	Vector p;
	Vector d;
	Vector g = Vector(0, 0.001f);
	Vector v;

	int nx;
	int ny;

	float width;
	float height;
	int renderOrder;

	string state;
	string type;

	DWORD dt;

	bool isBlockPlayer = true;
	bool isUniversal = false;

	LPDIRECT3DTEXTURE9 texture;
	LPANIMATION_SET active_animation_set;


public:
	static void LoadBoundedBox(LPCWSTR path);
	static LPDIRECT3DTEXTURE9 bboxtex;

	void SetPosition(float x, float y) { this->p.x = x, this->p.y = y; }
	void SetSpeed(float vx, float vy) { this->v.x = vx, this->v.y = vy; }
	void SetSize(float w, float h) { this->width = w, this->height = h; }
	void GetSpeed(float& vx, float& vy) { vx = this->v.x; vy = this->v.y; }
	void GetPosition(float& x, float& y) { x = this->p.x; y = this->p.y; }

	virtual void ParseFromJson(json data) = 0;
	virtual void ParseFromOwnJson() = 0;
	void RenderBoundingBox();
	string GetState() { return this->state; }



	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	virtual void OnHadCollided(LPGAMEOBJECT obj, LPCOLLISIONEVENT event);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);
	static bool CGameObject::HasOverLap(Vector l1, Vector r1, Vector l2, Vector r2);

	CGameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(string state) { this->state = state; }
	virtual void HandleCollision(LPCOLLISIONEVENT e);
	virtual void clear();
	virtual void UpdateWithCollision(vector<LPGAMEOBJECT>* coObjects, int& o_ny, int& o_nx);
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual bool VerifyCollidedLeftRight(LPGAMEOBJECT obj);
	~CGameObject();
};


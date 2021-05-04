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

	int id;

	//float x;
	//float y;

	Vector p;

	// float dx;	// dx = vx*dt
	// float dy;	// dy = vy*dt

	Vector d;
	Vector g = Vector(0, 0.001f);

	/*float vx;
	float vy;*/

	Vector v;

	int nx;
	int ny;

	float width;
	float height;

	string state;
	string type;

	DWORD dt;

	LPDIRECT3DTEXTURE9 texture;
	LPDIRECT3DTEXTURE9 bboxtex;
	unordered_map<string, LPSPRITE> sprites; //save all sprite of animation
	unordered_map<string, LPANIMATION> all_animations; //save all animations
	CAnimationSets animations_set; //save all the animation sets

	//string active_animation_set; //active animation set to have different type of character
	LPANIMATION_SET active_animation_set;


public:
	void SetPosition(float x, float y) { this->p.x = x, this->p.y = y; }
	void SetSpeed(float vx, float vy) { this->v.x = vx, this->v.y = vy; }
	void SetSize(float w, float h) { this->width = w, this->height = h; }
	void GetPosition(float& x, float& y) { x = this->p.x; y = this->p.y; }
	void GetSpeed(float& vx, float& vy) { vx = this->v.x; vy = this->v.y; }

	string GetState() { return this->state; }

	void RenderBoundingBox();

	void SetActiveAnimationSet(string ani_set_id) { active_animation_set = animations_set.Get(ani_set_id); }

	//void SetAnimationSet(string ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	CGameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(string state) { this->state = state; }

	virtual void ParseFromJson(json data) = 0; // use this function to parse from data to object


	void AddSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE GetSprite(string id);
	void ClearSprite();

	void AddAnimation(string id, LPANIMATION ani);
	LPANIMATION GetAnimation(string id);
	void ClearAnimation();

	void SetTexture(LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 GetTexture();

	void AddAnimationSet(string id, LPANIMATION_SET ani_set);
	LPANIMATION_SET GetAnimationSet(string id);
	void ClearAnimationSet();

	void ParseSpriteFromJson(LPCWSTR filePath);
	void ParseAnimationFromJson(LPCWSTR filePath);

	~CGameObject();
};


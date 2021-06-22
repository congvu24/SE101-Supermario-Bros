#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "RectPlatform.h"
#include "Leaf.h"
#include "SelectNode.h"


class Intro : public CScene
{
protected:
	CGameObject* player;					// A play scene has to have player, right? 
	void _ParseSection_OBJECTS_FromJson(json data);
	void _ParseSection_MAP_FromJson(string mapPath);

public:
	Map* map;
	int currentSelect;
	vector<LPGAMEOBJECT> listOption;

	bool isMoving = true;
	Intro(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void addObject(LPGAMEOBJECT obj);
	virtual void ParseMapObject(json data, vector<LPGAMEOBJECT>* obCollisions);
	LPMAP GetMap() { return this->map; };

	CGameObject* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};


class CIntroScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CIntroScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
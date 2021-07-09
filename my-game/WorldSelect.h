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
#include "CUI.h"


class WorldSelect : public CScene
{
protected:
	CGameObject* player;					// A play scene has to have player, right? 
	void _ParseSection_OBJECTS_FromJson(json data);
	void _ParseSection_MAP_FromJson(string mapPath);


public:
	Map* map;
	SelectNode* currentNode;
	SelectPortal* currentPortal;
	CUI* UI;

	bool isMoving = true;
	WorldSelect(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void addObject(LPGAMEOBJECT obj);
	virtual void ParseMapObject(json data, vector<LPGAMEOBJECT>* obCollisions);
	virtual void DrawUI();
	LPMAP GetMap() { return this->map; };

	CGameObject* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};


class CSelectScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CSelectScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
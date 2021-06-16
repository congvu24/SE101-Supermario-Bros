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


class WorldSelect : public CScene
{
protected:
	CGameObject* player;					// A play scene has to have player, right? 


	void _ParseSection_TEXTURES_FromJson(LPCWSTR filePath, int id);
	void _ParseSection_SPRITES_FromJson(LPCWSTR filePath, int textID);
	void _ParseSection_ANIMATIONS_FromJson(LPCWSTR filePath);
	void _ParseSection_ANIMATION_SETS_FromJson(LPCWSTR filePath);
	void _ParseSection_OBJECTS_FromJson(json data);
	void _ParseSection_MAP_FromJson(string mapPath);


public:
	Map* map;
	SelectNode* currentNode;
	SelectPortal* currentPortal;

	bool isMoving = true;
	WorldSelect(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void addObject(LPGAMEOBJECT obj);
	virtual void switchScene(int sence_id);
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
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

enum CameraMoveDirection {
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class CPlayScene : public CScene
{
protected:
	CGameObject* player;					// A play scene has to have player, right? 

	void _ParseSection_OBJECTS_FromJson(json data);
	void _ParseSection_MAP_FromJson(string mapPath);


public:
	Map* map;
	CPlayScene(int id, LPCWSTR filePath);
	LPMAP GetMap() { return this->map; };

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void addObject(LPGAMEOBJECT obj);
	virtual void ParseMapObject(json data, vector<LPGAMEOBJECT>* obCollisions);
	virtual void restart();
	virtual void moveCamera(CameraMoveDirection);
	virtual void GameOver();

	CGameObject* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};


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
#include "CUI.h"

enum CameraMoveDirection {
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class CPlayScene : public CScene
{
protected:

	void _ParseSection_OBJECTS_FromJson(json data);
	void _ParseSection_MAP_FromJson(string mapPath);


public:
	Map* map;

	int timeLimit = 0;
	int playerPoint = 0;
	int playerMoney = 0;

	CPlayScene(int id, LPCWSTR filePath);
	LPMAP GetMap() { return this->map; };
	CGameObject* player;					// A play scene has to have player, right? 
	CUI* UI;

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void addObject(LPGAMEOBJECT obj);
	virtual void ParseMapObject(json data, vector<LPGAMEOBJECT>* obCollisions);
	virtual void restart();
	virtual void moveCamera(CameraMoveDirection);
	virtual void GameOver();
	virtual void DrawUI();
	virtual void AddPoint(int point) { playerPoint = playerPoint + point; }
	virtual void AddMoney(int money) { playerMoney = playerMoney + money; }
	virtual void UpdateTime(DWORD dt);

	static bool IsPlayer(LPGAMEOBJECT obj);

	CGameObject* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};


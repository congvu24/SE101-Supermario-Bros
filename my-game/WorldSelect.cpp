#include <iostream>
#include <fstream>

#include "WorldSelect.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Test.h"
#include "Enemy.h"
#include "MisteryBox.h"
#include "Coin.h"
#include "Quadtree.h"
#include "PlayScence.h"
#include "MarioSelection.h"
#include "SelectNode.h"


WorldSelect::WorldSelect(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

void WorldSelect::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	json scene = ReadJsonFIle(sceneFilePath);


	DebugOut(L"[INFO] Scene : %s \n", ToLPCWSTR(scene["name"].dump()));

	//parse texture, sprite, animation

	json object = scene["object"];

	_ParseSection_OBJECTS_FromJson(object);


	string map = string(scene["map"]);
	DebugOut(L"[INFO] LOAD MAP : %s \n", ToLPCWSTR(map));

	_ParseSection_MAP_FromJson(map);

	animationDirection = UNACTIVE;
	animationStartedTime = GetTickCount64();
	animationProgress = 0;
	lastTime = 0;
	animationDuration = 5000;
}

void WorldSelect::Update(DWORD dt)
{
	if (animationDirection != UNACTIVE) {
		//float currentTime = GetTickCount64() - animationStartedTime;
		lastTime = lastTime + dt;
		animationProgress = (lastTime / animationDuration);
		if (animationProgress > 1) {
			animationDirection = UNACTIVE;
		}
	}

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;
	camera->cam_x = 0;
	camera->cam_y = 0;

	RECT base = { camera->cam_x, camera->cam_y, camera->cam_x + 800 ,camera->cam_y + 600 };
	Quadtree* quadtree = new Quadtree(1, new RECT(base));

	for (auto i = objects.begin(); i != objects.end(); i++) {
		quadtree->Insert(*i);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		vector<CGameObject*>* return_objects_list = new vector<CGameObject*>();
		quadtree->Retrieve(return_objects_list, objects[i]);

		if (Test* v = dynamic_cast<Test*>(objects[i])) {
			objects[i]->Update(dt, return_objects_list);
		}

		else if (objects[i]->state != "hidden") {
			objects[i]->Update(dt, return_objects_list);
		}
		else {
			objects.erase(std::remove(objects.begin(), objects.end(), objects[i]), objects.end());
		}
		delete return_objects_list;
	}
	quadtree->Clear();
	delete quadtree;

}

void WorldSelect::Render()
{
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;

	map->render();
	player->Render();

	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->state != "hidden")
			objects[i]->Render();
	}


	if (animationDirection == CLOSING) {
		//left to center
		CGame::GetInstance()->Draw(camera->cam_x - (camera->cam_width * (1 - animationProgress)), camera->cam_y, blackTexture, 0, 0, camera->cam_width, camera->cam_height, 255);
		//right to center
		CGame::GetInstance()->Draw(camera->cam_x + camera->cam_width - (camera->cam_width * animationProgress), camera->cam_y, blackTexture, 0, 0, camera->cam_width, camera->cam_height, 255);
		//top to center
		CGame::GetInstance()->Draw(camera->cam_x, camera->cam_y - (camera->cam_height * (1 - animationProgress)), blackTexture, 0, 0, camera->cam_width, camera->cam_height, 255);
		////bottom to center
		CGame::GetInstance()->Draw(camera->cam_x, camera->cam_y + camera->cam_height - (camera->cam_height * animationProgress), blackTexture, 0, 0, camera->cam_width, camera->cam_height, 255);
	}
}

/*
	Unload current scene
*/
void WorldSelect::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	// delete map data here;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

//void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
//{
//}
//
//void CPlayScenceKeyHandler::KeyState(BYTE* states)
//{
//	CGame* game = CGame::GetInstance();
//	CGameObject* player = ((WorldSelect*)scence)->GetPlayer();
//
//	//// disable control key when Mario die 
//	if (player->GetState() == "die") return;
//	if (game->IsKeyDown(DIK_RIGHT))
//		player->SetState("running-right");
//	else if (game->IsKeyDown(DIK_LEFT))
//		player->SetState("running-left");
//	else if (game->IsKeyDown(DIK_UP))
//		player->SetState("running-up");
//	else if (game->IsKeyDown(DIK_DOWN))
//		player->SetState("running-down");
//	else
//		player->SetState("indie");
//}


void  WorldSelect::_ParseSection_TEXTURES_FromJson(LPCWSTR filePath, int id) {

	DebugOut(L"[INFO] Start loading texture resources from : %s \n", filePath);

	int R = 0;
	int G = 0;
	int B = 0;

	CTextures::GetInstance()->Add(id, filePath, D3DCOLOR_XRGB(R, G, B));


}
void  WorldSelect::_ParseSection_SPRITES_FromJson(LPCWSTR filePath, int textID) {

	json sprite = ReadJsonFIle(filePath);
	json frames = sprite["frames"];


	for (json::iterator it = frames.begin(); it != frames.end(); ++it) {

		json data = it.value();
		string id = it.key();
		json frame = data["frame"];


		int l = stoi(frame["x"].dump());
		int t = stoi(frame["y"].dump());
		int r = l + stoi(frame["w"].dump());
		int b = t + stoi(frame["h"].dump());

		LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(textID);
		if (tex == NULL)
		{
			DebugOut(L"[ERROR] Texture ID %d not found!\n", IntToLPCWSTR(textID));
			return;
		}

		CSprites::GetInstance()->Add(id, l, t, r, b, tex);
	}

}
void  WorldSelect::_ParseSection_ANIMATIONS_FromJson(LPCWSTR filePath) {

}
void  WorldSelect::_ParseSection_ANIMATION_SETS_FromJson(LPCWSTR filePath) {
}
void  WorldSelect::_ParseSection_OBJECTS_FromJson(json allObjects) {
	for (json::iterator it = allObjects.begin(); it != allObjects.end(); ++it) {

		json data = it.value();

		string name = string(data["name"]); //object name;
		bool visible = bool(data["visible"]); //object name;

		CGameObject* obj = NULL;

		switch (fromNameToCode(name))
		{
		case 1:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new MarioSelection();
			obj->ParseFromJson(data); //remember to set position, animation_set in this function
			player = obj;
			break;
		default:
			break;
		}
		if (obj != NULL)
			objects.push_back(obj);
		else delete obj;
	}
}

void  WorldSelect::_ParseSection_MAP_FromJson(string mapPath) {
	vector<LPGAMEOBJECT> obCollision;

	this->map = new Map();
	map->load(mapPath, &obCollision);

	for (size_t i = 0; i < obCollision.size(); i++)
	{
		objects.push_back(obCollision[i]);
	}
}

void WorldSelect::addObject(LPGAMEOBJECT obj) {
	objects.push_back(obj);
}
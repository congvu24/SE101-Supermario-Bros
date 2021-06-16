#include <iostream>
#include <fstream>

#include "Intro.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Test.h"
#include "Enemy.h"
#include "MisteryBox.h"
#include "Coin.h"
#include "Quadtree.h"
#include "MarioSelection.h"
#include "SelectNode.h"
#include "SelectionTree.h"
#include "IntroText.h"
#include "IntroCursor.h"


Intro::Intro(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CIntroScenceKeyHandler(this);
	currentSelect = 1;
	camera->cam_x = 0;
	camera->cam_y = 100;

	animationDirection = UNACTIVE;
	animationStartedTime = GetTickCount64();
	animationProgress = 0;
	lastTime = 0;
	animationDuration = 5000;
}

void Intro::Load()
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

}

void Intro::Update(DWORD dt)
{
	if (animationDirection != UNACTIVE && animationDirection != DONE) {
		//float currentTime = GetTickCount64() - animationStartedTime;
		lastTime = lastTime + dt;
		animationProgress = (lastTime / animationDuration);
		if (animationProgress > 0.5) {
			DebugOut(L"compelte");
			animationDirection = DONE;
			if (nextScene != 0) {
				CGame::GetInstance()->SwitchScene(nextScene);
			}
		}
	}


	vector<CGameObject*>* objects_list = new vector<CGameObject*>();



	for (auto i = objects.begin(); i != objects.end(); i++) {
		objects_list->push_back(*i);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, objects_list);
	}
	delete objects_list;


}

void Intro::Render()
{
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;

	map->render();
	player->Render();

	for (int i = 0; i < objects.size(); i++) {
		/*if (objects[i]->state != "hidden")*/
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
void Intro::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	// delete map data here;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CIntroScenceKeyHandler::OnKeyDown(int KeyCode)
{

	Intro* scene = ((Intro*)scence);
	switch (KeyCode)
	{
	case DIK_UP:
		if (scene->currentSelect == 0) scene->currentSelect = 2;
		else if (scene->currentSelect - 1 >= 0) scene->currentSelect = scene->currentSelect - 1;

		break;
	case DIK_DOWN:
		if (scene->currentSelect == 2) scene->currentSelect = 0;
		else if (scene->currentSelect + 1 <= 2) scene->currentSelect = scene->currentSelect + 1;

		break;
	case DIK_S:
		if (scene->currentSelect == 1)
			scene->switchScene(1);
		break;
	}
	scene->GetPlayer()->p.y = scene->listOption.at(scene->currentSelect)->p.y;


}

void CIntroScenceKeyHandler::KeyState(BYTE* states)
{
}


void  Intro::_ParseSection_TEXTURES_FromJson(LPCWSTR filePath, int id) {

	DebugOut(L"[INFO] Start loading texture resources from : %s \n", filePath);

	int R = 0;
	int G = 0;
	int B = 0;

	CTextures::GetInstance()->Add(id, filePath, D3DCOLOR_XRGB(R, G, B));


}
void  Intro::_ParseSection_SPRITES_FromJson(LPCWSTR filePath, int textID) {

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
void  Intro::_ParseSection_ANIMATIONS_FromJson(LPCWSTR filePath) {

}
void  Intro::_ParseSection_ANIMATION_SETS_FromJson(LPCWSTR filePath) {
}
void  Intro::_ParseSection_OBJECTS_FromJson(json allObjects) {
	for (json::iterator it = allObjects.begin(); it != allObjects.end(); ++it) {

		json data = it.value();

		string name = string(data["name"]); //object name;
		bool visible = bool(data["visible"]); //object name;

		CGameObject* obj = NULL;

		switch (fromNameToCode(name))
		{
		case 9:
			if (visible != true)
				IntroText::SaveStaticData(data);
			break;
		case 10:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] Curosr object was created before!\n");
				return;
			}
			obj = new IntroCursor();
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

void  Intro::_ParseSection_MAP_FromJson(string mapPath) {
	vector<LPGAMEOBJECT> obCollision;

	this->map = new Map();
	map->load(mapPath, &obCollision);
	listOption = vector<LPGAMEOBJECT>(3);

	for (size_t i = 0; i < obCollision.size(); i++)
	{
		objects.push_back(obCollision[i]);
		if (IntroText* obj = dynamic_cast<IntroText*>(obCollision[i])) {
			listOption.at(stoi(obj->type)) = (obj);
		}
	}
	player->p.y = listOption.at(currentSelect)->p.y;



}

void Intro::addObject(LPGAMEOBJECT obj) {
	objects.push_back(obj);
}


void Intro::switchScene(int sence_id) {
	this->animationDirection = CLOSING;
	this->nextScene = sence_id;
}
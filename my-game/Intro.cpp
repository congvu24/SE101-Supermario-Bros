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
#include "DefineCharacter.h"


Intro::Intro(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CIntroScenceKeyHandler(this);
	currentSelect = 1;
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

	animationDirection = UNACTIVE;
	animationStartedTime = GetTickCount64();
	animationProgress = 0;
	lastTime = 0;
}

void Intro::Update(DWORD dt)
{
	CScene::Update(dt);
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
		objects[i]->Render();
	}
	CScene::Render();

}

void Intro::Unload()
{
	player = NULL;
	//map->unload();
	CScene::Unload();
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


void  Intro::_ParseSection_OBJECTS_FromJson(json allObjects) {
	for (json::iterator it = allObjects.begin(); it != allObjects.end(); ++it) {

		json data = it.value();

		string name = string(data["name"]); //object name;
		bool visible = bool(data["visible"]); //object name;

		CGameObject* obj = NULL;

		switch (fromNameToCode(name))
		{
		case ObjectType::IntroText:
			if (visible != true)
				IntroText::SaveStaticData(data);
			break;
		case ObjectType::OptionCursor:
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
	map->load(mapPath, &obCollision, this);
	listOption = vector<LPGAMEOBJECT>(3);

	for (size_t i = 0; i < obCollision.size(); i++)
	{
		objects.push_back(obCollision[i]);

		if (IntroText* obj = dynamic_cast<IntroText*>(obCollision[i])) {
			if (stoi(obj->type) < 3) {
				listOption.at(stoi(obj->type)) = (obj);
			}
		}
	}
	player->p.y = listOption.at(currentSelect)->p.y;



}

void Intro::addObject(LPGAMEOBJECT obj) {
	objects.push_back(obj);
}


void Intro::ParseMapObject(json data, vector<LPGAMEOBJECT>* obCollisions) {
	string type = string(data["type"]);
	string name = string(data["name"]);

	if (type == "objectgroup" && name != "RectCollision") {
		DebugOut(L"[INFO] Load name: %s \n", ToLPCWSTR(name));

		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);
			string type = value["type"];

			CGameObject* obj = NULL;
			Camera* camera = CGame::GetInstance()->GetCurrentScene()->getCamera();

			switch (fromNameToCode(name))
			{
			case ObjectType::IntroText:
				obj = new IntroText();
				obj->type = type;
				break;
			case ObjectType::Camera:
				camera->setCamPos(x, y);
				camera->cam_left_limit = x;
				camera->cam_top_limit = y;
				break;
			default:
				break;
			}

			if (obj != NULL) {
				obj->ParseFromOwnJson();
				obj->width = width;
				obj->height = height;
				obj->name = name;
				obj->p.x = x;
				obj->p.y = y;
				DebugOut(L"[INFO] Size Of Object: %s \n", IntToLPCWSTR(sizeof(*obj)));

				obCollisions->push_back(obj);
			}
			else delete obj;

		}


	}

}
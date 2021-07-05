#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Test.h"
#include "Enemy.h"
#include "MisteryBox.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Quadtree.h"
#include "Death.h"
#include "Venus.h"
#include "VenusBullet.h"
#include "DefineCharacter.h"
#include "RedGoomba.h"
#include "GoldenBrick.h"
#include "PButton.h"
#include "Effect.h"
#include "Brick.h"
#include "CUI.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	UI = new CUI();
}


void CPlayScene::Load()
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

void CPlayScene::Update(DWORD dt)
{
	CScene::Update(dt);
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;

	RECT base = { camera->cam_x, camera->cam_y, camera->cam_x + 800 ,camera->cam_y + 600 };
	Quadtree* quadtree = new Quadtree(1, new RECT(base));


	//DebugOut(L"[INFO] NUMBER OF OBJECT: %s \n", IntToLPCWSTR(objects.size()));


	for (auto i = objects.begin(); i != objects.end(); i++) {
		quadtree->Insert(*i);
	}
	int count = 0;

	//player->Update(dt, coObjectsWithMario);

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (quadtree->IsContain(objects[i])) {
			count++;
			vector<CGameObject*>* return_objects_list = new vector<CGameObject*>();
			quadtree->Retrieve(return_objects_list, objects[i]);

			if (Test* v = dynamic_cast<Test*>(objects[i])) {
				// prevent re update player;
				v->Update(dt, return_objects_list);
			}

			else if (objects[i]->state != "hidden") {
				objects[i]->Update(dt, return_objects_list);
			}
			else {
				objects.erase(std::remove(objects.begin(), objects.end(), objects[i]), objects.end());
			}

			delete return_objects_list;
		}

		else if (objects[i]->isUniversal == true) {
			count++;
			objects[i]->Update(dt, &objects);
		}

	}
	quadtree->Clear();
	delete quadtree;

	//DebugOut(L"[INFO] UPDATE: %s \n", IntToLPCWSTR(count));

	if (player == NULL) return;

	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;
	//camera->setCamPos(cx, cy);


	if (camera->isCameraMoving == false) {

		if (((Test*)player)->isAllowCameraFollow == true) {
			if (player->p.y + player->height < camera->camera_default_top) {
				camera->setCamPos(cx, cy);
			}
			else {
				camera->setCamPos(cx, camera->camera_default_top);
			}
		}
	}
	else {
		camera->setCamPos(cx, cy - 170);
	}

	DebugOut(L"[INFO] Camera x: %s \n", IntToLPCWSTR(camera->cam_x));
	DebugOut(L"[INFO] Camera y: %s \n", IntToLPCWSTR(camera->cam_y));
}

bool comparePtrToNode(CGameObject* a, CGameObject* b) { return (a->renderOrder < b->renderOrder); }


void CPlayScene::Render()
{

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->camera;

	RECT* base = new RECT();
	base->left = camera->cam_x - 200;
	base->top = camera->cam_y - 200;
	base->right = camera->cam_x + 800 + 200;
	base->bottom = camera->cam_y + 600 + 200;

	Quadtree* quadtree = new Quadtree(5, base); // set the level to 5 to stop split function
	vector<CGameObject*>* return_objects_list = new vector<CGameObject*>();

	for (auto i = objects.begin(); i != objects.end(); i++) {
		quadtree->Insert(*i);
	}
	quadtree->Retrieve(return_objects_list, this->player);

	sort(return_objects_list->begin(), return_objects_list->end(), comparePtrToNode);

	for (int i = 0; i < return_objects_list->size(); i++) { // order < 1
		if (return_objects_list->at(i)->state != "hidden" && return_objects_list->at(i)->renderOrder < 1)
			return_objects_list->at(i)->Render();
	}
	if (player->renderOrder < 1)
		player->Render(); // render player 

	map->render();  // render map here


	for (int i = 0; i < return_objects_list->size(); i++) { // order >= 1
		if (return_objects_list->at(i)->state != "hidden" && return_objects_list->at(i)->renderOrder >= 1)
			return_objects_list->at(i)->Render();
	}

	if (player->renderOrder >= 1)
		player->Render();
	//player->Render(); // render player 
	DrawUI();
	CScene::Render();  //render scene animation


	// clear data
	quadtree->Clear();
	return_objects_list->clear();
	delete return_objects_list;
	delete quadtree;
	//delete base;
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	player = NULL;
	//map->unload();
	CScene::Unload();
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{

	CGameObject* player = ((CPlayScene*)scence)->GetPlayer();
	((Character*)player)->ProcessKeyboard(CGame::GenerateKeyboardEvent(KeyCode));

	/*switch (KeyCode)
	{
	case DIK_SPACE:
		player->SetState("jumping");
		DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
		break;
	}*/

}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{

	CGameObject* player = ((CPlayScene*)scence)->GetPlayer();

	((Character*)player)->ProcessKeyboard(CGame::GenerateKeyboardEvent(KeyCode, false, true));


	/*switch (KeyCode)
	{
	case DIK_SPACE:
		player->SetState("jumping");
		DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
		break;
	}*/

}


void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CGameObject* player = ((CPlayScene*)scence)->GetPlayer();



	std::vector<int> UnOrderProcessKey = { DIK_A, DIK_S };
	std::vector<int> OrderProcessKey = { DIK_LEFT,DIK_RIGHT, DIK_DOWN, DIK_UP };


	for (int i = 0; i < UnOrderProcessKey.size(); i++) {
		if (game->IsKeyDown(UnOrderProcessKey[i])) {
			((Character*)player)->ProcessKeyboard(CGame::GenerateKeyboardEvent(UnOrderProcessKey[i], true));
		}
	}

	for (int i = 0; i < OrderProcessKey.size(); i++) {
		if (game->IsKeyDown(OrderProcessKey[i])) {
			((Character*)player)->ProcessKeyboard(CGame::GenerateKeyboardEvent(OrderProcessKey[i], true));
			return;
		}
	}


	if (game->IsKeyDown(DIK_R))
		((CPlayScene*)scence)->restart();
	else
		((Test*)player)->SetAction(MarioAction::IDLE);


	//CGame::GenerateKeyboardEvent(UnOrderProcessKey[i], true);

	//// disable control key when Mario die 

	// move camera
	if (game->IsKeyDown(DIK_J))
		((CPlayScene*)scence)->moveCamera(LEFT);
	if (game->IsKeyDown(DIK_K))
		((CPlayScene*)scence)->moveCamera(DOWN);
	if (game->IsKeyDown(DIK_I))
		((CPlayScene*)scence)->moveCamera(UP);
	if (game->IsKeyDown(DIK_L))
		((CPlayScene*)scence)->moveCamera(RIGHT);
}

void CPlayScene::restart() {
	//this->player->SetPosition(100, 1000);
	//this->camera->move_x = 0;
	//this->camera->move_y = 0;
	//this->camera->isCameraMoving = false;
	CGame::GetInstance()->Restart();
}

void CPlayScene::moveCamera(CameraMoveDirection direction) {
	camera->isCameraMoving = true;
	if (direction == UP) {
		camera->move(0, -20);
	}
	else if (direction == DOWN) {
		camera->move(0, 20);
	}
	else if (direction == RIGHT) {
		camera->move(20, 0);
	}
	else if (direction == LEFT) {
		camera->move(-20, 0);
	}
}

void  CPlayScene::_ParseSection_OBJECTS_FromJson(json allObjects) {
	for (json::iterator it = allObjects.begin(); it != allObjects.end(); ++it) {

		json data = it.value();

		string name = string(data["name"]); //object name;
		bool visible = bool(data["visible"]); //object name;

		CGameObject* obj = NULL;

		switch (fromNameToCode(name))
		{
		case ObjectType::Test:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new Test();
			obj->ParseFromJson(data); //remember to set position, animation_set in this function
			player = obj;
			break;

		case  ObjectType::QuestionBox_Item:
			if (visible != true)
				MisteryBox::SaveStaticData(data);
		case  ObjectType::Coin:
			if (visible != true)
				Coin::SaveStaticData(data);
			break;
		case  ObjectType::Goomba:
			if (visible != true)
				Goomba::SaveStaticData(data);
			break;
		case  ObjectType::RectPlatform:
			if (visible != true)
				RectPlatform::SaveStaticData(data);
			break;
		case  ObjectType::Leaf:
			if (visible != true)
				Leaf::SaveStaticData(data);
			break;
		case  ObjectType::Mushroom:
			if (visible != true)
				Mushroom::SaveStaticData(data);
			break;
		case  ObjectType::Venus:
			if (visible != true)
				Venus::SaveStaticData(data);
			break;
		case  ObjectType::Venus_Bullet:
			if (visible != true)
				VenusBullet::SaveStaticData(data);
			break;
		case  ObjectType::Koopas:
			if (visible != true)
				Koopas::SaveStaticData(data);
			break;
		case  ObjectType::RedGoomba:
			if (visible != true)
				RedGoomba::SaveStaticData(data);
			break;
		case  ObjectType::GoldenBrick:
			if (visible != true)
				GoldenBrick::SaveStaticData(data);
			break;
		case  ObjectType::PButton:
			if (visible != true)
				PButton::SaveStaticData(data);
			break;
		case  ObjectType::Effect:
			if (visible != true)
				Effect::SaveStaticData(data);
			break;
		case ObjectType::UI:
			if (visible != true)
				CUI::SaveStaticData(data);
			break;
		default:
			break;
		}
		if (obj != NULL)
			objects.push_back(obj);
		else delete obj;
	}

}

void  CPlayScene::_ParseSection_MAP_FromJson(string mapPath) {
	vector<LPGAMEOBJECT> obCollision;

	this->map = new Map();
	map->load(mapPath, &obCollision, this);

	for (size_t i = 0; i < obCollision.size(); i++)
	{
		objects.push_back(obCollision[i]);
	}
}

void CPlayScene::addObject(LPGAMEOBJECT obj) {
	objects.push_back(obj);
}


void CPlayScene::ParseMapObject(json data, vector<LPGAMEOBJECT>* obCollisions) {
	string type = string(data["type"]);
	string name = string(data["name"]);

	if (type == "objectgroup" && name == "RectCollision") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			LPGAMEOBJECT obj = new Collision();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);


			obj->width = width;
			obj->height = height;
			obj->name = "RectCollision";
			obj->p = Vector(x, y);

			obCollisions->push_back(obj);
		}
	}
	else if (type == "objectgroup" && name == "MiniPortal") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			MiniPortal* obj = new MiniPortal();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);
			string portalName = value["name"];
			string type = value["type"];

			/*float can = float(value["y"]);
			float y = float(value["y"]);*/

			if (type == "Out") {
				json properties = value["properties"];
				for (json::iterator property = properties.begin(); property != properties.end(); ++property) {
					json data = property.value();
					string name = data["name"];
					if (name == "CameraLeftTopLimitX") {
						obj->camera_x = float(data["value"]);
						obj->camera_left_limit = float(data["value"]);
					}
					if (name == "CameraLeftTopLimitY") {
						obj->camera_y = float(data["value"]);
						obj->camera_top_limit = float(data["value"]);
					}
					if (name == "CameraRightBottomLimitX") {
						obj->camera_y = float(data["value"]);
						obj->camera_right_limit = float(data["value"]);
					}
					if (name == "CameraRightBottomLimitY") {
						obj->camera_bottom_limit = float(data["value"]);
					}
				}
			}


			obj->width = width;
			obj->height = height;
			obj->portalName = portalName;
			obj->name = "MiniPortal";
			obj->type = type;
			obj->p = Vector(x, y);

			obCollisions->push_back(obj);
		}
	}
	else if (type == "objectgroup" && name == "RectPlatform") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			LPGAMEOBJECT obj = new RectPlatform();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);



			obj->width = width;
			obj->height = height;
			obj->name = name;
			obj->p = Vector(x, y);

			obCollisions->push_back(obj);
		}
	}
	else if (type == "objectgroup" && name == "Death") {
		json objects = data["objects"];

		for (json::iterator objData = objects.begin(); objData != objects.end(); ++objData) {
			json value = objData.value();
			LPGAMEOBJECT obj = new Death();

			obj->ParseFromOwnJson();

			float width = float(value["width"]);
			float height = float(value["height"]);
			float x = float(value["x"]);
			float y = float(value["y"]);


			obj->width = width;
			obj->height = height;
			obj->name = "Death";
			obj->p = Vector(x, y);

			obCollisions->push_back(obj);
		}
	}
	else if (type == "objectgroup" && name != "RectCollision") {
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
			case ObjectType::QuestionBox_Item:
				obj = new MisteryBox();
				break;
			case ObjectType::Coin:
				obj = new Coin();
				break;
			case ObjectType::Goomba:
				obj = new Goomba();
				break;
			case ObjectType::Venus:
				obj = new Venus();
				break;
			case ObjectType::Koopas:
				obj = new Koopas();
				break;
			case ObjectType::RedGoomba:
				obj = new RedGoomba();
				break;
			case ObjectType::GoldenBrick:
				obj = new GoldenBrick();
				break;
			case ObjectType::Camera:
				camera->setCamPos(x, y);
				camera->camera_default_left = x;
				camera->camera_default_top = y;
				camera->cam_left_limit = x;
				camera->cam_top_limit = y;
				camera->cam_bottom_limit = y + 600;
				break;
			default:
				break;
			}

			if (obj != NULL) {
				obj->width = width;
				obj->height = height;
				obj->name = name;
				obj->p.x = x;
				obj->p.y = y;

				if (type != "") {
					obj->type = type;
				}
				obj->ParseFromOwnJson();
				DebugOut(L"[INFO] Size Of Object: %s \n", IntToLPCWSTR(sizeof(*obj)));

				obCollisions->push_back(obj);
			}
			else delete obj;

		}


	}
}

void CPlayScene::GameOver() {
	switchScene(3);
}

bool CPlayScene::IsPlayer(LPGAMEOBJECT obj) {
	if (Test* player = dynamic_cast<Test*>(obj))  return true;
	return false;
}

void CPlayScene::DrawUI() {
	UI->DrawUI("hub", Vector(0, 470));
	UI->DrawUI("reward-slot", Vector(500, 470));
	UI->DrawUI("1", Vector(124, 495), Vector(0.8, 0.8));
	UI->DrawUI("M-icon", Vector(12, 515));
	UI->DrawUI("1", Vector(90, 514));
	UI->DrawText("1234567", Vector(150, 515));
	UI->DrawText("123", Vector(393, 518), Vector(0.8, 0.8));
	UI->DrawText("456", Vector(413, 493), Vector(0.8, 0.8));


	float levelSpeed = (((Test*)player)->powerX / 1000) * 6;
	for (int i = 0; i < 6; i++) {
		UI->DrawUI(levelSpeed > i ? "arrow-white" : "arrow", Vector(152 + i * 24, 493));
	}
}
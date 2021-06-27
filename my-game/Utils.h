#pragma once

#include <Windows.h>

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include "library/json.hpp"
#include <fstream>



using json = nlohmann::json;


using namespace std;

void DebugOut(wchar_t* fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);
LPCWSTR IntToLPCWSTR(int st);

json ReadJsonFIle(LPCWSTR file);

enum class ObjectType
{
	NOT_FOUND,
	Camera,
	Test,
	Enemy,
	QuestionBox_Item,
	QuestionBox_Coin,
	QuestionBox_Mushroom,
	Coin,
	Goomba,
	RectPlatform,
	Leaf,
	SelectionTree,
	IntroText,
	OptionCursor,
	Mushroom,
	Venus,
	Venus_Bullet,
	Koopas
};


ObjectType fromNameToCode(string name);


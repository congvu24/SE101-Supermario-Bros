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

#include "DefineCharacter.h"


ObjectType fromNameToCode(string name) {
	unordered_map <string, ObjectType> character_code;
	character_code["Camera"] = ObjectType::Camera;
	character_code["test"] = ObjectType::Test;
	character_code["enemy"] = ObjectType::Enemy;
	character_code["QuestionBox_Item"] = ObjectType::QuestionBox_Item;
	character_code["QuestionBox_Coin"] = ObjectType::QuestionBox_Item;
	character_code["QuestionBox_Mushroom"] = ObjectType::QuestionBox_Item;
	character_code["Coin"] = ObjectType::Coin;
	character_code["Goomba"] = ObjectType::Goomba;
	character_code["RectPlatform"] = ObjectType::RectPlatform;
	character_code["Leaf"] = ObjectType::Leaf;
	character_code["SelectionTree"] = ObjectType::SelectionTree;
	character_code["IntroText"] = ObjectType::IntroText;
	character_code["OptionCursor"] = ObjectType::OptionCursor;
	character_code["Mushroom"] = ObjectType::Mushroom;
	character_code["Venus"] = ObjectType::Venus;
	character_code["Venus_Bullet"] = ObjectType::Venus_Bullet;
	character_code["Koopas"] = ObjectType::Koopas;
	character_code["RedGoomba"] = ObjectType::RedGoomba;
	character_code["GoldenBrick"] = ObjectType::GoldenBrick;
	character_code["PButton"] = ObjectType::PButton;
	character_code["Effect"] = ObjectType::Effect;
	character_code["UI"] = ObjectType::UI;

	if (character_code.find(name) != character_code.end()) {
		return character_code.at(name);
	}
	else return ObjectType::NOT_FOUND;
}

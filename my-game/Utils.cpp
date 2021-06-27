#include <Windows.h>
#include <iostream>

#include "Utils.h"

void DebugOut(wchar_t* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring* w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}

LPCWSTR IntToLPCWSTR(int st)
{
	char s[256];

	sprintf_s(s, "%d", st);
	return ToLPCWSTR(s);
}

LPCWSTR FloatToLPCWSTR(float st)
{
	char s[256];

	sprintf_s(s, "%.2f", st);
	return ToLPCWSTR(s);
}

json ReadJsonFIle(LPCWSTR file) {

	ifstream readData(file);
	DebugOut(L"[INFO] Start loading game file : %s\n", file);

	json jsonData;
	readData >> jsonData;
	return jsonData;
}


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

	if (character_code.find(name) != character_code.end()) {
		return character_code.at(name);
	}
	else return ObjectType::NOT_FOUND;
}

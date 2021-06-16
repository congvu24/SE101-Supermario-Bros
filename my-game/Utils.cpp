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

int fromNameToCode(string name) {
	unordered_map <string, int> character_code;
	character_code["test"] = 1;
	character_code["enemy"] = 2;
	character_code["QuestionBox_Item"] = 3;
	character_code["QuestionBox_Coin"] = 3;
	character_code["Coin"] = 4;
	character_code["Goomba"] = 5;
	character_code["RectPlatform"] = 6;
	character_code["Leaf"] = 7;
	character_code["SelectionTree"] = 8;
	
	if (character_code.find(name) != character_code.end()) {
		return character_code.at(name);
	}
	else return 0;
}
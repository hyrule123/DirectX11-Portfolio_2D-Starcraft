#pragma once

namespace PresetPath
{
	constexpr std::string_view ScriptProj = "./Project/Script";

#ifdef _DEBUG
	constexpr std::string_view Content = "./OutputFile/Bin_Debug/Content";
#else
	constexpr std::string_view Content = "./OutputFile/Bin_Release/Content";
#endif

	constexpr std::string_view strKeyScript = "strKeyScript.h";
	constexpr std::string_view strKeyShader = "strKeyShader.h";
	constexpr std::string_view strKeyTexture = "strKeyTexture.h";
}

namespace PresetStr
{
	constexpr std::string_view NameSpace = "namespace ";
	constexpr std::string_view ConstexprStringView = "constexpr std::string_view ";
	constexpr std::string_view EqualDoubleQuotation = " = \"";
	constexpr std::string_view EnumClass = "enum class ";
	constexpr std::string_view strKey = "strKey_";

	constexpr std::string_view Head = R"(
#pragma once

#include <string_view>

//=========================================================
//This Code is Automatically generated by CodeGenerator.exe
//=========================================================

)";
}



constexpr std::string_view IncludeBeginA = "#include \"";
constexpr std::wstring_view IncludeBeginW = L"#include \"";

extern int g_iIndentation;


//아무 값도 입력하지 않을 경우 한 줄 내림
//반환값 : 현재 indent 단계
void WriteCodeA(ofstream& _pFile, const string& _strCode = "");
void WriteCodeW(wofstream& _pFile, const wstring& _wstrCode = L"");

inline void WriteBracketOpenA(ofstream& _pFile)
{
	//먼저 중괄호를 추가한뒤
	WriteCodeA(_pFile, "{");

	//들여쓰기 값을 늘려준다.
	++g_iIndentation;
}

inline void WriteBracketCloseA(ofstream& _pFile, bool _bAddSemiColon = false)
{
	//먼저 들여쓰기를 뺴준뒤
	--g_iIndentation;
	if (g_iIndentation < 0)
		g_iIndentation = 0;

	//WriteCode 호출
	if (true == _bAddSemiColon)
		WriteCodeA(_pFile, "};");
	else
		WriteCodeA(_pFile, "}");
}

inline void WriteBracketCloseAllA(ofstream& _pFile)
{
	while (0 < g_iIndentation)
	{
		WriteBracketCloseA(_pFile);
	}
}

inline void WriteBracketOpenW(wofstream& _pFile)
{
	//먼저 중괄호를 추가한뒤
	WriteCodeW(_pFile, L"{");

	//들여쓰기 값을 늘려준다.
	++g_iIndentation;
}

inline void WriteBracketCloseW(wofstream& _pFile)
{
	//먼저 들여쓰기를 뺴준뒤
	--g_iIndentation;
	if (g_iIndentation < 0)
		g_iIndentation = 0;

	//WriteCode 호출
	WriteCodeW(_pFile, L"}");
}

inline void WriteBracketCloseAllW(wofstream& _pFile)
{
	while (0 < g_iIndentation)
	{
		WriteBracketCloseW(_pFile);
	}
}

inline void IncludeFileA(std::ofstream& _pFile, const string& _strIncludeFileName)
{
	string include = string(IncludeBeginA);
	include += _strIncludeFileName + '\"';

	WriteCodeA(_pFile, include);
}

inline void IncludeFileW(std::wofstream& _pFile, const wstring& _strIncludeFileName)
{
	wstring include = wstring(IncludeBeginW);
	include += _strIncludeFileName + L'\"';

	WriteCodeW(_pFile, include);
}


inline void UpperCaseA(string& _str)
{
	transform(_str.begin(), _str.end(), _str.begin(), ::toupper);
}

inline void UpperCaseW(wstring& _str)
{
	transform(_str.begin(), _str.end(), _str.begin(), ::towupper);
}

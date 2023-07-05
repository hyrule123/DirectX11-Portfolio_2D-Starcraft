#pragma once

#ifndef DEBUG_BREAK
#ifdef _DEBUG
#define DEBUG_BREAK DebugBreak()
#else
#define DEBUG_BREAK
#endif
#endif


//#define PRESET constexpr inline const char*

#define PRESET(_VarNameBase, _String) \
namespace _VarNameBase {\
constexpr const char* A = _String;\
constexpr const wchar_t* W = L##_String;\
constexpr const char8_t* U8 = u8##_String;\
}


namespace define_Preset
{
	namespace Path
	{
		PRESET(ScriptProj, "./Project/Script");
		PRESET(EngineProj, "./Project/Engine");


#ifdef _DEBUG
		PRESET(Content, "./OutputFile/Bin_Debug/Content");
#else
		PRESET(Content, "./OutputFile/Bin_Release/Content");
#endif

		PRESET(strKey_Script, "strKey_Script.h");
		PRESET(strKey_CShader, "strKey_CShader.h");
		PRESET(strKey_GShader, "strKey_GShader.h");
		PRESET(strKey_Texture, "strKey_Texture.h");
		PRESET(strKey_Prefab, "strKey_Prefab.h");
	}

	namespace Keyword
	{
		PRESET(IncludeBegin, "#include \"");

		PRESET(NameSpace, "namespace ");
		PRESET(ConstexprInlineConstChar, "PRESET(");
		PRESET(EqualDoubleQuotation, ", \"");
		PRESET(EnumClass, "enum class ");
		PRESET(strKey, "strKey_");

		PRESET(DefineSTRKEY, R"(
#ifndef STRKEY
#define STRKEY constexpr inline const char*
#endif
)");


		PRESET(Head, R"(
#pragma once

#include <Engine/define.h>

//=========================================================
//This Code is Automatically generated by CodeGenerator.exe
//=========================================================

)");


		PRESET(define_T_Constructor, R"(#define CONSTRUCTOR_T(_Type) pMgr->AddBaseCS(#_Type, Constructor_T<_Type>))"
);
		PRESET(T_Constructor, "CONSTRUCTOR_T(");


	}

	namespace Regex_Keyword
	{
		//[   numthreads   ( %d, %d, %d )   ]
		//[ ] 안에 둘러싸여 있고, 공백 갯수에 상관없이 숫자 3개를 추출
		PRESET(Numthread, R"(\[\s*numthreads\s*\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\).*\])");
	}

	namespace VarNameFilter
	{
		constexpr inline const char FiltersA[] =
		{
			'.',
			',',
			'-',
			'(',
			')'
		};

		constexpr inline const wchar_t FiltersW[] =
		{
			L'.',
			L',',
			L'-',
			L'(',
			L')'
		};

		constexpr inline const wchar_t FiltersU8[] =
		{
			u8'.',
			u8',',
			u8'-',
			u8'(',
			u8')'
		};
	}
}


#include <Engine/CComputeShader.h>
namespace define_ShaderGen
{
	struct tShaderSetting
	{
		stdfs::path FileName[(int)define_Shader::eGS_TYPE::END];
	};
}

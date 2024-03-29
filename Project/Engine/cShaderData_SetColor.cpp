#include "pch.h"
#include "cShaderData_SetColor.h"

#include "cTexture.h"

#include "cStructBuffer.h"

#include "strKey_Default.h"

#ifdef _DEBUG
#include "DefaultShader\S_C_SetColor_Debug.h"
#else
#include "DefaultShader\S_C_SetColor_Release.h"
#endif
#include "S_H_SetColor.hlsli"

cShaderData_SetColor::cShaderData_SetColor()
{
	tSBufferDesc Desc = {};
	Desc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
	Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_WRITE;
	Desc.REGISLOT_t_SRV = REGISLOT_t_SBUFFER_SETCOLOR;
	Desc.REGISLOT_u_UAV = REGISLOT_u_SBUFFERRW_SETCOLOR;

	m_StructBufferTest = std::make_unique<cStructBuffer>();
	m_StructBufferTest->SetDesc(Desc);

	for (int i = 0; i < 1280u; ++i)
	{
		float c = (float)i / 1280.f;
		m_vecSBuffer[i] = Vec4(c, c, c, 1.f);
	}

	//UAV 바인딩
	m_StructBufferTest->Create((UINT)sizeof(Vec4), 1280u, m_vecSBuffer, 1280u);
}

cShaderData_SetColor::~cShaderData_SetColor()
{
}

tNumDataCS cShaderData_SetColor::BindDataCS()
{
	m_OutTex->BindData_UAV(REGISLOT_u_TEXTURERW_SETCOLOR);

	m_StructBufferTest->UploadData((void*)m_vecSBuffer, 1280u);
	m_StructBufferTest->BindBufferUAV();
	
	// 그룹 개수 계산
	return tNumDataCS((UINT)m_OutTex->GetWidth(), (UINT)m_OutTex->GetHeight(), 1u);
}

bool cShaderData_SetColor::BindDataGS()
{
	return false;
}


void cShaderData_SetColor::UnBind()
{
	m_OutTex->UnBind();
	m_StructBufferTest->UnBind();

	//데이터 다시 받기(테스트)
	m_StructBufferTest->DownloadData(m_vecSBuffer, (UINT)sizeof(m_vecSBuffer));

	//SRV에 바인딩5
	m_StructBufferTest->BindBufferSRV();
}

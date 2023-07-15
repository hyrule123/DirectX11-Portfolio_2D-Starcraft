#include "pch.h"
#include "cCom_Renderer_Particle.h"

#include "cResMgr.h"

#include "cStructBuffer.h"

#include "strKey_Default.h"

#include "cCom_Transform.h"

#include "cCSModule_ParticleBasic.h"

#include "cTimeMgr.h"
#include "RandGen.h"

//상수버퍼 생성용
#include "cDevice.h"
#include "cConstBuffer.h"

#include "cShaderDataModule.h"


cCom_Renderer_Particle::cCom_Renderer_Particle()
	: IRenderer(eCOMPONENT_TYPE::PARTICLE_SYSTEM)
	, m_pShaderDataModule()
	, m_pCS()
{
	//점 메쉬
	SetMesh(cResMgr::GetInst()->FindRes<cMesh>(string(strKey_RES_DEFAULT::MESH::POINT)));

	//기본 파티클 렌더러
	SetMaterial(cResMgr::GetInst()->FindRes<cMaterial>(string(strKey_RES_DEFAULT::MATERIAL::PARTICLE_RENDERER_BASIC)));
}

cCom_Renderer_Particle::~cCom_Renderer_Particle()
{
}

cCom_Renderer_Particle::cCom_Renderer_Particle(cCom_Renderer_Particle const& _other)
	: IRenderer(_other)
	, m_pShaderDataModule()
	, m_pCS(_other.m_pCS)
{
	//새 객체를 만들어서 unique로 변경
	m_pShaderDataModule = std::make_unique<cShaderDataModule>(_other.m_pShaderDataModule->Clone());
}


void cCom_Renderer_Particle::init()
{
}

void cCom_Renderer_Particle::finaltick()
{
	//버퍼 바인딩은 쉐이더 클래스에서 일괄적으로 진행함.
	//여기는 데이터 업로드만 담당.

	//모듈데이터 전송
	static cConstBuffer* const s_CBuffer_ModuleData = cDevice::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_PARTICLE_MODULEDATA);
	s_CBuffer_ModuleData->UploadData(&m_tModuleData);
	//s_CBuffer_ModuleData->BindBuffer(define_Shader::ePIPELINE_STAGE_FLAG::__ALL);


	//몇개 스폰할지 정보를 SharedRW 버퍼에 담아서 전송
	float fSpawnCountPerTime = 1.f / (float)m_tModuleData.iSpawnRate;
	m_AccTime += DELTA_TIME;

	if (fSpawnCountPerTime < m_AccTime)
	{
		float fData = m_AccTime / fSpawnCountPerTime;

		m_AccTime = fSpawnCountPerTime * (fData - floor(fData));


		//GPU에서 사용할 시드값을 전달
		tParticleShareData rwbuffer = { (int)fData, UINT32_2(RandGen::GetRand<UINT>(0u, UINT_MAX), RandGen::GetRand<UINT>(0u, UINT_MAX)),};

		m_pSBufferRW_Shared->UploadData(&rwbuffer, 1u);
	}

	//m_pCSParticle->SetBuffers(this, m_pSBufferRW_ParticleTransform, m_pSBufferRW_Shared, s_CBuffer_ModuleData);

	//파티클 위치정보를 계산시킴.
	m_pCSParticle->Execute();

	//렌더링하고 나면 PrevPos를 업데이트
	m_tModuleData.vOwnerPrevWorldPos = m_tModuleData.vOwnerCurWorldPos;
	m_tModuleData.vOwnerCurWorldPos = Transform().GetWorldPos();
}

bool cCom_Renderer_Particle::render()
{
	//true 반환해서 인스턴싱 필요없다고 전달
	if (nullptr == m_pCSParticle || false == m_bIsCreated)
		return true;

	//Transform().UpdateData();

	cMaterial* pMtrl = GetCurMaterial().Get();
	pMtrl->BindData();

	BindMtrlScalarDataToCBuffer();

	m_pSBufferRW_ParticleTransform->BindBufferSRV();

	GetMesh()->renderInstanced(m_tModuleData.iMaxParticleCount);

	//드로우콜이 발생했으므로 -> 인스턴싱 필요 없다고 반환
	return true;
}



void cCom_Renderer_Particle::CreateParticle()
{
	assert(nullptr != m_pCSParticle);

	m_bIsCreated = true;

	m_tModuleData.iMaxParticleCount = 200;

	//파티클을 처리할 버퍼 생성
	m_pSBufferRW_ParticleTransform->Create(sizeof(tParticleTransform), m_tModuleData.iMaxParticleCount, nullptr, 0u);

	m_tModuleData.bModule_Spawn = TRUE;

	m_tModuleData.iSpawnRate = 30;

	m_tModuleData.vSpawnColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.vSpawnScaleMin = Vec3(50.f, 50.f, 1.f);
	m_tModuleData.vSpawnScaleMax = Vec3(100.f, 100.f, 1.f);

	m_tModuleData.eSpawnShapeType = 0;
	m_tModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);
	m_tModuleData.bFollowing = 0; // 시뮬레이션 좌표계

	m_tModuleData.fMinLifeTime = 3.f;
	m_tModuleData.fMaxLifeTime = 5.f;

	m_tModuleData.bModule_ScaleChange = TRUE;
	m_tModuleData.fStartScale = 2.f;
	m_tModuleData.fEndScale = 0.1f;

	m_tModuleData.bModule_ColorChange = TRUE;
	m_tModuleData.vStartColor = Vec3(0.2f, 0.3f, 1.0f);
	m_tModuleData.vEndColor = Vec3(0.4f, 1.f, 0.4f);

	m_tModuleData.bModule_AddVelocity = TRUE;
	m_tModuleData.eAddVelocityType = 0; // From Center
	m_tModuleData.fSpeed = 500.f;
	m_tModuleData.vVelocityDir;
	m_tModuleData.fOffsetAngle;

	m_tModuleData.bModule_Drag = TRUE;
	m_tModuleData.fStartDrag = 200.f;
	m_tModuleData.fEndDrag = 0.f;

	m_tModuleData.bModule_Rotation = TRUE;
	m_tModuleData.vRotRadPerSec = Vec3(0.f, 0.f, 10.f);
	m_tModuleData.vRotRandomRange = Vec3(0.f, 0.f, 0.3f);

	m_tModuleData.bModule_ApplyGravity = TRUE;
	m_tModuleData.bModule_ApplyGravity = TRUE;
	m_tModuleData.fGravity = 9.8f;


	//노이즈 텍스처 지정


	//공유 데이터 구조화 버퍼 생성
	tParticleShareData rwbuffer = { (int)0, UINT32_2(RandGen::GetRand<UINT>(0u, UINT_MAX), RandGen::GetRand<UINT>(0u, UINT_MAX)), };


	m_pSBufferRW_Shared->Create((UINT)sizeof(tParticleShareData), 1, &rwbuffer, 1u);
}

void cCom_Renderer_Particle::SetParticleCS(const string_view _ResKeyCS)
{
	m_pCSParticle = cResMgr::GetInst()->FindRes<cComputeShader>(_ResKeyCS);
}
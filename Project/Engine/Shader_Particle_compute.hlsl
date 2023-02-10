#include "Shader_header_register.hlsli"
#include "Shader_Func.hlsli"

//=================
//Using Buffers
//=================



//cbuffer CBuffer_SBuffer_SharedInfo : register(b3)
//{
//    tSBufferInfo g_SBufferInfo[eSBUFFER_SHARED_CBUFFER_END];
//}


//cbuffer g_ParticleModuleData : register(b4)
//{
//    float4 vSpawnColor;

//    float4 vSpawnScale;

//    float3 vBoxShapeScale;
//    float fSphereShapeRadius;

//    int SpawnShapeType; // Sphere , Box
//    int SpawnRate; // 초당 생성 개수
//    float2 Padding;

//	// Color Change 모듈
//    float4 vStartColor; // 초기 색상
//    float4 vEndColor; // 최종 색상

//	// Scale Change 모듈
//    float4 vStartScale; // 초기 크기
//    float4 vEndScale; // 최종 크기	

//	// 버퍼 최대크기
//    int iMaxParticleCount;

//	// Module Check(현재 int 3개짜리 배열)
//    int bModule_ParticleSpawn;
//    int bModule_ColorChange;
//    int bModule_ScaleChange;
//}

////RWStructuredBuffer<tParticle> g_SBufferRW_Particle : register(u2);
////RWStructuredBuffer<tRWParticleBuffer> g_SBufferRW_Particle_Shared : register(u3);

#define ePARTICLE_MODULE_PARTICLE_SPAWN 0
#define	ePARTICLE_MODULE_COLOR_CHANGE 1
#define	ePARTICLE_MODULE_SCALE_CHANGE 2


//컴퓨트쉐이더가 들고있는 개별 Mtrl 구조체에 들어있는값 참조
#define ObjectPos           g_vec4_0

//컴퓨트쉐이더가 들고있는 노이즈텍스처의 해상도를 저장
#define NoiseTexResolution  g_vec2_0


#define SpawnCount          g_SBufferRW_Particle_Shared[0].iSpawnCount
#define ParticleMaxCount    g_iMaxParticleCount

#define SpawnModule         g_bSpawn
#define ColorChangeModule   g_bModule_ColorChange
#define ScaleChangeModule   g_bModule_ScaleChange

[numthreads(128, 1, 1)]
void CS_Particle(uint3 _ID : SV_DispatchThreadID )
{
    // 스레드 ID 가 파티클버퍼 최대 수를 넘긴경우 or 스레드 담당 파티클이 비활성화 상태인 경우
    if ((uint) ParticleMaxCount <= _ID.x)
        return;
        
    tParticle particle = g_SBufferRW_Particle[_ID.x];

           
    if (SpawnModule)
    {
        // 파티클이 비활성화 상태인 경우
        if (particle.bActive == 0)
        {
            // SpawnCount 를 확인
            // 만약 SpawnCount 가 0 이상이라면, 파티클을 활성화시킴      
            while (0 < SpawnCount)
            {
                int orgvalue = SpawnCount;
                int outvalue = 0;
                InterlockedCompareExchange(SpawnCount, orgvalue, SpawnCount - 1, outvalue);
            
                if (orgvalue == outvalue)
                {
                    particle.bActive = 1;
                    
                    // 랜덤 결과를 받을 변수
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                    
                    // 전체 유효 스레드의 아이디를 0 ~ 1 로 정규화
                    float fNormalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                    GaussianSample(g_Tex_Noise, NoiseTexResolution, fNormalizeThreadID, vOut1);
                    GaussianSample(g_Tex_Noise, NoiseTexResolution, fNormalizeThreadID + 0.1f, vOut2);
                    GaussianSample(g_Tex_Noise, NoiseTexResolution, fNormalizeThreadID + 0.2f, vOut3);
                    
                    // Box 스폰
                    if (g_eSpawnShapeType == 0)
                    {
                        particle.vLocalPos.xyz = float3(g_vBoxShapeScale.x * vOut1.r - g_vBoxShapeScale.x * 0.5f
                                                      , g_vBoxShapeScale.y * vOut2.r - g_vBoxShapeScale.y * 0.5f
                                                      , g_vBoxShapeScale.z * vOut3.r - g_vBoxShapeScale.z * 0.5f);
                        particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
                        
                        particle.vWorldScale.xyz = float3(10.f, 10.f, 1.f);
                    }
                    
                    // Sphere 스폰
                    else if (g_eSpawnShapeType == 1)
                    {
                        
                    }
                    
                    
                    float fRadius = 500.f; //vOut1.r * 200.f;
                    float fAngle = vOut2.r * 2 * 3.1415926535f;
                    //particle.vWorldPos.xyz = float3(fRadius * cos(fAngle), fRadius * sin(fAngle), 100.f);                    
                   
                   
                    particle.fAge = 0.f;
                    particle.fLifeTime = 10.f;
                    break;
                }
            }
        }
    }
           
    
    // 파티클이 활성화인 경우
    if (particle.bActive)
    {
        // 속도에 따른 파티클위치 이동
        if (g_bFollowing == 0)
        {
            particle.vWorldPos += particle.vVelocity * g_DeltaTime;
        }
        else if (g_bFollowing == 1)
        {
            particle.vLocalPos += particle.vVelocity * g_DeltaTime;
            particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
        }
        
        
        
        // 파티클의 Age 에 시간을 누적시킴
        particle.fAge += g_DeltaTime;
        
        // 파티클의 수명이 끝나면, 다시 비활성화 상태로 되돌림
        if (particle.fLifeTime <= particle.fAge)
        {
            particle.bActive = 0.f;
        }
    }
    
    // 변경점 적용
    g_SBufferRW_Particle[_ID.x] = particle;
    
        
}
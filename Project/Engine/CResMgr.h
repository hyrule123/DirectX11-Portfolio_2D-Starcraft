#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CPathMgr.h"

//템플릿에 사용되는 리소스들
#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"
#include "CPrefab.h"

//새 Res 추가
//1. define.h enum에 Res 타입 추가했는지 확인
//2. m_umapResClassTypeIndex에 타입 인덱스와 eRES_TYPE을 바인딩


class CResMgr :
    public CSingleton<CResMgr>
{
private:
    unordered_map<wstring, Ptr<CRes>> m_arrRes[(UINT)eRES_TYPE::END];
    unordered_map<std::type_index, eRES_TYPE> m_umapResClassTypeIndex;

private:
    void CreateResClassTypeIndex();
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultMaterial();
    void LoadDefaultTexture();

public:
    template <typename T>
    eRES_TYPE GetResType();

    template<typename T>
    Ptr<T> FindRes(const wstring& _strKey);

    template<typename T>
    void AddRes(const wstring& _strKey, Ptr<T>& _Res);

    template<typename T>
    Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);

    const unordered_map<wstring, Ptr<CRes>>& GetResMap(eRES_TYPE _ResType);


public:
    void init();

    SINGLETON(CResMgr)
};

template<typename T>
inline eRES_TYPE CResMgr::GetResType()
{
    return m_umapResClassTypeIndex[std::type_index(typeid(T))];
}


template<typename T>
inline Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
    eRES_TYPE type = CResMgr::GetInst()->GetResType<T>();
      
    auto iter = m_arrRes[(UINT)type].find(_strKey);
    if (iter == m_arrRes[(UINT)type].end())
        return nullptr;

    if (type == eRES_TYPE::MATERIAL)
        return (T*)iter->second.Get()->Clone();

    return (T*)iter->second.Get();    
}


template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, Ptr<T>& _Res)
{
    // 중복키로 리소스 추가하려는 경우
    assert( ! FindRes<T>(_strKey).Get() );

    eRES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, _Res.Get()));
    _Res->SetKey(_strKey);
}


template<typename T>
inline Ptr<T> CResMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    Ptr<CRes> pRes = FindRes<T>(_strKey).Get();
    
    // 이미 해당 키로 리소스가 있다면, 반환
    if (nullptr != pRes)
        return (T*)pRes.Get();
        
    pRes = new T;
    pRes->SetKey(_strKey);
    pRes->SetRelativePath(_strRelativePath);

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    if (FAILED(pRes->Load(strFilePath)))
    {
        assert(nullptr);
    }

    eRES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, pRes));

    return (T*)pRes.Get();
}

inline const unordered_map<wstring, Ptr<CRes>>& CResMgr::GetResMap(eRES_TYPE _ResType)
{
    return m_arrRes[(UINT)_ResType];
}
#pragma once
#include "CRes.h"

class CMesh
	: public CRes
{
private:
	ComPtr<ID3D11Buffer>	m_VB;
	D3D11_BUFFER_DESC		m_tVBDesc;
	UINT					m_VtxCount;

	ComPtr<ID3D11Buffer>	m_IB;
	D3D11_BUFFER_DESC		m_tIBDesc;
	UINT					m_IdxCount;

	void*					m_pVtxSys;
	void*					m_pIdxSys;

public:
	//이거 가지고 W / VP 행렬을 나눠서 보낼지, WVP 행렬을 만들어서 보낼지를 결정
	UINT GetIdxBufferCount() const { return m_IdxCount; }

public:
	void Create(void* _VtxSysMem, UINT _iVtxCount, void* _IdxSysMem, UINT _IdxCount);

private:
	virtual int Load(const wstring& _strFilePath) { return S_OK; }
public:
	virtual int Save(const wstring& _strRelativePath) { return S_OK; }


	void render();
	void renderInstanced(UINT _uInstancingCount);


private:
	virtual void BindData() override;
	

public:
	CMesh();
	~CMesh();
};
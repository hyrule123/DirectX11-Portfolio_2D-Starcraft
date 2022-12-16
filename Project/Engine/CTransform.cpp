#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"

CTransform::CTransform()
	: CComponent(eCOMPONENT_TYPE::TRANSFORM),
	m_vRelativeScale(1.f, 1.f, 1.f)
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	//크기행렬(CreateScale을 해주면 자동으로 동차좌표를 추가해서 행렬에 삽입해 준다.
	const Matrix& matScale = Matrix::CreateScale(m_vRelativeScale);


	//Matrix matRot = Matrix::CreateRotationX(m_vRelativeRot.x);
	//matRot *= Matrix::CreateRotationY(m_vRelativeRot.y);
	//matRot *= Matrix::CreateRotationZ(m_vRelativeRot.z);

	const DirectX::XMVECTOR& XmVec = XMQuaternionRotationRollPitchYaw(m_vRelativeRot.x, m_vRelativeRot.y, m_vRelativeRot.z);

	const Matrix& matRot = Matrix::CreateFromQuaternion(XmVec);
	//const Matrix& matRot = XMMatrixIdentity();
	const Matrix& matTranslation = Matrix::CreateTranslation(m_vRelativePos);

	m_matWorld = matScale * matRot * matTranslation;
	//m_matWorld = m_matWorld.Transpose();
}

void CTransform::UpdateData()
{
	//월드뷰투영행렬을 곱한 후 전치한다.
	Matrix matWVP = (m_matWorld * g_transform.MatView * g_transform.MatProj).Transpose();

	// 위의 행렬을 상수버퍼에 전달 및 바인딩
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(eCB_TYPE::TRANSFORM);
	pTransformBuffer->SetData(&matWVP, sizeof(Matrix));
	//pTransformBuffer->SetData(&m_vRelativePos, sizeof(Vec3));
	pTransformBuffer->UpdateData();
}

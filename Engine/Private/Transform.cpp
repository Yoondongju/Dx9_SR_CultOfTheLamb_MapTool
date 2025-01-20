#include "../Public/Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform& Prototype)
	: CComponent(Prototype)
	, m_WorldMatrix(Prototype.m_WorldMatrix)
{

}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

	return S_OK;
}

HRESULT CTransform::Bind_Transform()
{
	return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
}

void CTransform::Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	_float3 vRight = Get_State(STATE::STATE_RIGHT);
	Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * fScaleX);

	_float3 vUp = Get_State(STATE::STATE_UP);
	Set_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * fScaleY);

	_float3 vLook = Get_State(STATE::STATE_LOOK);
	Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fScaleZ);
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vLook = Get_State(STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vLook = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vRight = Get_State(STATE_RIGHT);


	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vRight = Get_State(STATE_RIGHT);


	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Turn(const _float3& vAxis, _float fTimeDelta)
{
	// �̰� ���� ������ǥ������ ����  ���� �� ������ǥ�� ��ġ���� ��
	_float3 vRight = Get_State(STATE_RIGHT);		// ���� ���� �մ� ������� ������ ����
	_float3	vUp = Get_State(STATE_UP);
	_float3	vLook = Get_State(STATE_LOOK);

	_float4x4 RotationMatrix {};		// ȸ���ϱ����� ȸ������� ������ش�.
										// vAxis ������ ���� �������� �󸶸�ŭ ȸ�������� ���� ������� ����
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_fRotationPerSec * fTimeDelta);


	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);


	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation(const _float3& vAxis, _float fRadian)	// 1ȸ���̳� �ڱⰡ ȸ���Ǳ� ���� ó�� ��������
{
	// �׵�� �����Ѱ�����ŭ ����
	_float3 vScaled = Get_Scaled();							
	
	_float3 vRight = _float3{ 1.f,0.f,0.f };
	_float3 vUp = _float3{ 0.0f,1.f,0.f };   
	_float3 vLook = _float3{ 0.0f,0.f,1.f };





	//_float3   vRight = Get_State(STATE_RIGHT);
	//_float3	vUp = Get_State(STATE_UP);
	//_float3	vLook = Get_State(STATE_LOOK);


	_float4x4 RotationMatrix {};

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	// ȸ���� ���� �������� �ޱ��� ���� 1.f,1.f,1.f���� ȸ����Ű�� ������÷�

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, vRight * vScaled.x);
	Set_State(STATE_UP, vUp * vScaled.y);
	Set_State(STATE_LOOK, vLook * vScaled.z);
}

void CTransform::LookAt(const _float3& vAt) 
{
	_float3 vScaled = Get_Scaled();

	_float3 vPosition = Get_State(STATE_POSITION);
	_float3 vLook = vAt - vPosition;
	_float3 vRight{}, vUp{};
	

	

	//_float3 vMyup = Get_State(STATE_UP);
	//D3DXVec3Cross(&vRight, &vMyup, &vLook);// << ������ �����Ͱ� 0,1,0�� �ƴ϶� ������ �����ͷ� �ϸ�?
	// Ȯ���غ����� �����ذ� X
	
	_float3 vUpSample = { 0.f, 1.f, 0.f };
	D3DXVec3Cross(&vRight, &vUpSample, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x);
	Set_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScaled.y);
	Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScaled.z);
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();


}

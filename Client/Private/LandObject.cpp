#include "stdafx.h"
#include "..\Public\LandObject.h"

#include "GameInstance.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject{ pGraphic_Device }
{
}

CLandObject::CLandObject(const CLandObject& Prototype)
	: CBlendObject{ Prototype }
{
}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void* pArg)
{
	LANDOBJECT_DESC* pDesc = static_cast<LANDOBJECT_DESC*>(pArg);

	m_pTerrainVIBuffer = pDesc->pTerrainVIBuffer;
	Safe_AddRef(m_pTerrainVIBuffer);

	m_pTerrainTransform = pDesc->pTerrainTransform;
	Safe_AddRef(m_pTerrainTransform);






	m_strLayerName = pDesc->strLayerName;

	return S_OK;
}

void CLandObject::Priority_Update(_float fTimeDelta)
{
}

void CLandObject::Update(_float fTimeDelta)
{
}

void CLandObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CLandObject::Render()
{
	return S_OK;
}

HRESULT CLandObject::SetUp_OnTerrain(CTransform* pTransform, _float fOffsetY)
{
	/* ������ Ÿ���ϴ� ��ü�� ���� ��ġ�� ���´�. */
	_float3		vWorldPos = pTransform->Get_State(CTransform::STATE_POSITION);

	/* ���� ������ �� ��, ���ý����̽����� ���� �䷮�̱�Ť̤��� ��ü�� ��ġ�� ���������� ���÷� ��ȯ��Ű��. */
	/* ��ü�� ������ġ * �������� �׸��� ��ü�� ���� ����� */
	_float4x4		WorldMatrixInverse = m_pTerrainTransform->Get_WorldMatrix_Inverse();
	_float4x4		WorldMatrix = m_pTerrainTransform->Get_WorldMatrix();

	_float3		vLocalPos{};
	D3DXVec3TransformCoord(&vLocalPos, &vWorldPos, &WorldMatrixInverse);

	_float		fHeight = m_pTerrainVIBuffer->Compute_Height(vLocalPos);

	vLocalPos.y = fHeight + (fOffsetY * pTransform->Get_Scaled().y);  // << �� �����Ͽ� �°� y ��ġ����

	/* ������ ���� �����̽� �� ���ŵ� ������ġ�� ���Ѱ��̱⶧���� */
	/* �̸� �ٽ� ����� ������(���������� �����͸� -> ���� )���ؼ��� ��������� ���ؾ��Ѵ�. */
	D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, &WorldMatrix);

	pTransform->Set_State(CTransform::STATE_POSITION, vWorldPos);

	return S_OK;
}



void CLandObject::Free()
{
	__super::Free();

	Safe_Release(m_pTerrainVIBuffer);
	Safe_Release(m_pTerrainTransform);
}

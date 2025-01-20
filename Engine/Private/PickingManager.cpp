#include "../Public/PickingManager.h"

#include "../Public/Layer.h"
#include "../Public/VIBuffer.h"
#include "../Public/Transform.h"

#include "../Public/GameObject.h"

CPickingManager::CPickingManager(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device{ pGraphic_Device}
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CPickingManager::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	return S_OK;
}


_float3 CPickingManager::Get_PickingObject_CenterPos(class CLayer* pLayer)
{
	// 1. ���콺�� ��ũ�� ��ǥ�� ���´�
	// 2. �� ���콺�� ���� ��ǥ�� ���ؾ��Ѵ�.
	// 3. �׷����� �Ųٷ� ������    ���� -> �� - > ����

	D3DVIEWPORT9 ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	_float4x4 MatProjection;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &MatProjection);


	POINT		ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);
	



	//(2.f *ptMouse.x) / ViewPort.Width  == 0 ~ 2 ���� ����ȭ��Ű�� -1 ���ָ� ���������̽��� -1 ~ 1 ��������

	m_vOriginal = _float3{ 0.f,0.f,0.f }; // ī�޶��� eye ��ġ

	m_vDirection.x = (((2.f * ptMouse.x) / ViewPort.Width) - 1.f) / MatProjection._11;
	m_vDirection.y = (((-2.f * ptMouse.y) / ViewPort.Height) + 1.f) / MatProjection._22;
	m_vDirection.z = 1.f;

	_float4x4 matView, matInvView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);


	D3DXMatrixInverse(&matInvView, nullptr, &matView);

	//////// �������ϸ� �� ���콺 ��ǥ�� View �����̽� ��ȯ�����޴�
	// ���� ���庯ȯ����


	D3DXVec3TransformCoord(&m_vOriginal, &m_vOriginal, &matInvView);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matInvView);
	// �� ����� ���ؼ� ������ǥ�� ��ȯ���ִ�



	
	_uint iCount = pLayer->Get_GamObjectSize();

	for (_uint i = 0; i < iCount; i++)
	{	
		CComponent* pComponent = pLayer->Find_Component(TEXT("Com_VIBuffer"), i);
		CComponent* pComponent_Transfrom = pLayer->Find_Component(TEXT("Com_Transform"), i);


		CVIBuffer* pModelVIBuffer = dynamic_cast<CVIBuffer*>(pComponent);
		CTransform* pTransfrom = dynamic_cast<CTransform*>(pComponent_Transfrom);


		
		_float4x4 InvWolrdMat = pTransfrom->Get_WorldMatrix_Inverse();

		D3DXVec3TransformCoord(&m_vOriginal, &m_vOriginal, &InvWolrdMat);
		D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &InvWolrdMat);
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);


		LPDIRECT3DVERTEXBUFFER9 pVB = pModelVIBuffer->Get_VB();

		VTXTEX* pVertices = { nullptr };
		/* ���������� ���� ������ ��ġ�� �޸� �ּҸ� ���� �� �ִ�. */
		_uint iX = pModelVIBuffer->m_iNumVerticesX;
		_uint iZ = pModelVIBuffer->m_iNumVerticesZ;

		pVB->Lock(0, 0, (void**)&pVertices, 0);


		float		fU, fV, fDist;
		for (size_t i = 0; i < iZ-1; i++)
		{
			for (size_t j = 0; j < iX-1; j++)
			{
				_uint		iIndex = i * iX + j;
				_uint		iIndices[4] = {
					iIndex + iX,
					iIndex + iX + 1,
					iIndex + 1 ,
					iIndex,
				};
				//pVertices[iIndices[0]]  , pVertices[iIndices[1]] , pVertices[iIndices[2]]
				if (D3DXIntersectTri(&pVertices[iIndices[0]].vPosition,
					&pVertices[iIndices[1]].vPosition,
					&pVertices[iIndices[2]].vPosition,
					&m_vOriginal,
					&m_vDirection,
					&fU,
					&fV,
					&fDist))		// �޻�� �ﰢ��
				{	
					_float fX = (pVertices[iIndices[1]].vPosition.x - pVertices[iIndices[0]].vPosition.x) * 0.5f + j;
					_float fY = (pVertices[iIndices[1]].vPosition.y - pVertices[iIndices[2]].vPosition.y) * 0.5f;
					_float fZ = (pVertices[iIndices[0]].vPosition.z - pVertices[iIndices[2]].vPosition.z) * 0.5f + i;

					
 					return _float3(fX, fY, fZ);
				}

				else if (D3DXIntersectTri(&pVertices[iIndices[0]].vPosition,
					&pVertices[iIndices[2]].vPosition,
					&pVertices[iIndices[3]].vPosition,
					&m_vOriginal,
					&m_vDirection,
					&fU,
					&fV,
					&fDist))	   // ���ϴ� �ﰢ��
				{
					_float fX = (pVertices[iIndices[2]].vPosition.x - pVertices[iIndices[3]].vPosition.x) * 0.5f + j;
					_float fY = (pVertices[iIndices[0]].vPosition.y - pVertices[iIndices[3]].vPosition.y) * 0.5f;
					_float fZ = (pVertices[iIndices[0]].vPosition.z - pVertices[iIndices[2]].vPosition.z) * 0.5f + i;

					return _float3(fX, fY, fZ);
				}
			}
		}
		pVB->Unlock();
	}

	return _float3(0.f, 0.f, 0.f);
}


_float3 CPickingManager::Get_PickingObject_JustPos(CLayer* pLayer)
{
	D3DVIEWPORT9 ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	_float4x4 MatProjection;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &MatProjection);


	POINT		ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);




	//(2.f *ptMouse.x) / ViewPort.Width  == 0 ~ 2 ���� ����ȭ��Ű�� -1 ���ָ� ���������̽��� -1 ~ 1 ��������

	m_vOriginal = _float3{ 0.f,0.f,0.f }; // ī�޶��� eye ��ġ

	m_vDirection.x = (((2.f * ptMouse.x) / ViewPort.Width) - 1.f) / MatProjection._11;
	m_vDirection.y = (((-2.f * ptMouse.y) / ViewPort.Height) + 1.f) / MatProjection._22;
	m_vDirection.z = 1.f;

	_float4x4 matView, matInvView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);


	D3DXMatrixInverse(&matInvView, nullptr, &matView);

	//////// �������ϸ� �� ���콺 ��ǥ�� View �����̽� ��ȯ�����޴�
	// ���� ���庯ȯ����


	D3DXVec3TransformCoord(&m_vOriginal, &m_vOriginal, &matInvView);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matInvView);
	// �� ����� ���ؼ� ������ǥ�� ��ȯ���ִ�




	_uint iCount = pLayer->Get_GamObjectSize();

	for (_uint i = 0; i < iCount; i++)
	{
		CComponent* pComponent = pLayer->Find_Component(TEXT("Com_VIBuffer"), i);
		CComponent* pComponent_Transfrom = pLayer->Find_Component(TEXT("Com_Transform"), i);


		CVIBuffer* pModelVIBuffer = dynamic_cast<CVIBuffer*>(pComponent);
		CTransform* pTransfrom = dynamic_cast<CTransform*>(pComponent_Transfrom);



		_float4x4 InvWolrdMat = pTransfrom->Get_WorldMatrix_Inverse();

		D3DXVec3TransformCoord(&m_vOriginal, &m_vOriginal, &InvWolrdMat);
		D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &InvWolrdMat);
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);


		LPDIRECT3DVERTEXBUFFER9 pVB = pModelVIBuffer->Get_VB();

		VTXTEX* pVertices = { nullptr };
		/* ���������� ���� ������ ��ġ�� �޸� �ּҸ� ���� �� �ִ�. */
		_uint iX = pModelVIBuffer->m_iNumVerticesX;
		_uint iZ = pModelVIBuffer->m_iNumVerticesZ;

		pVB->Lock(0, 0, (void**)&pVertices, 0);


		float		fU, fV, fDist;
		for (size_t i = 0; i < iZ - 1; i++)
		{
			for (size_t j = 0; j < iX - 1; j++)
			{
				_uint		iIndex = i * iX + j;
				_uint		iIndices[4] = {
					iIndex + iX,
					iIndex + iX + 1,
					iIndex + 1 ,
					iIndex,
				};
				//pVertices[iIndices[0]]  , pVertices[iIndices[1]] , pVertices[iIndices[2]]
				if (D3DXIntersectTri(&pVertices[iIndices[0]].vPosition,
					&pVertices[iIndices[1]].vPosition,
					&pVertices[iIndices[2]].vPosition,
					&m_vOriginal,
					&m_vDirection,
					&fU,
					&fV,
					&fDist))		// �޻�� �ﰢ��
				{
					int a = 1;

					return _float3(m_vOriginal + (m_vDirection * fDist));
				}

				else if (D3DXIntersectTri(&pVertices[iIndices[0]].vPosition,
					&pVertices[iIndices[2]].vPosition,
					&pVertices[iIndices[3]].vPosition,
					&m_vOriginal,
					&m_vDirection,
					&fU,
					&fV,
					&fDist))	   // ���ϴ� �ﰢ��
				{
					int a = 1;


					return _float3(m_vOriginal + (m_vDirection * fDist));
				}
			}
		}
		pVB->Unlock();
	}

	return _float3(0.f, 0.f, 0.f);
}

_float3 CPickingManager::Get_PickingBackGround_JustPos()
{
	D3DVIEWPORT9 ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	_float4x4 MatProjection;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &MatProjection);


	POINT		ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);




	//(2.f *ptMouse.x) / ViewPort.Width  == 0 ~ 2 ���� ����ȭ��Ű�� -1 ���ָ� ���������̽��� -1 ~ 1 ��������

	m_vOriginal = _float3{ 0.f,0.f,0.f }; // ī�޶��� eye ��ġ

	m_vDirection.x = (((2.f * ptMouse.x) / ViewPort.Width) - 1.f) / MatProjection._11;
	m_vDirection.y = (((-2.f * ptMouse.y) / ViewPort.Height) + 1.f) / MatProjection._22;
	m_vDirection.z = 1.f;

	_float4x4 matView, matInvView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);


	D3DXMatrixInverse(&matInvView, nullptr, &matView);

	//////// �������ϸ� �� ���콺 ��ǥ�� View �����̽� ��ȯ�����޴�
	// ���� ���庯ȯ����


	D3DXVec3TransformCoord(&m_vOriginal, &m_vOriginal, &matInvView);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matInvView);
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	float t = -(m_vOriginal.y / m_vDirection.y);
	D3DXVECTOR3 vIntersection = m_vOriginal + t * m_vDirection;

	// �� ����� ���ؼ� ������ǥ�� ��ȯ���ִ�

	return _float3(vIntersection);
}

void CPickingManager::Update()
{
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);

	/* ����Ʈ == ���� * ������� * ����� * ������� /w -> ����Ʈ ��ȯ */
	ScreenToClient(m_hWnd, &ptMouse);

	/* ���������̽� == ���� * ������� * ����� * ������� /w */
	D3DVIEWPORT9 ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	_float3		vMousePos{};
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / (ViewPort.Height * -0.5f) + 1.f;
	vMousePos.z = 0.f; /* Near����� Ŭ���Ѱž�!! */

	/* �佺���̽� == ���� * ������� * ����� */
	_float4x4			ProjMatrixInv{};
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInv);
	D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &ProjMatrixInv);

	/* �佺���̽� �󿡼��� ���콺 ���̿� ������ �������� ���س��´�. */
	/* �佺���̽� ���̱� ������ ī�޶� ��ǥ�� �����̴�. */
	m_vOriginal = _float3(0.f, 0.f, 0.f);
	m_vDirection = vMousePos - m_vOriginal;

	/* ���彺���̽� == ���� * ������� */
	_float4x4			ViewMatrixInv{};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInv);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);

	D3DXVec3TransformCoord(&m_vOriginal, &m_vOriginal, &ViewMatrixInv);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &ViewMatrixInv);

	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
}

void CPickingManager::Transform_ToLocalSpace(const _float4x4& WorldMatrix)
{
	_float4x4		WorldMatrixInverse = *D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	D3DXVec3TransformCoord(&m_vRayPos_InLocalSpace, &m_vOriginal, &WorldMatrixInverse);
	D3DXVec3TransformNormal(&m_vRayDir_InLocalSpace, &m_vDirection, &WorldMatrixInverse);

	D3DXVec3Normalize(&m_vRayDir_InLocalSpace, &m_vRayDir_InLocalSpace);
}

_bool CPickingManager::isPicked_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	_float		fU{}, fV{}, fDist{};

	if (TRUE == D3DXIntersectTri(&vPointA, &vPointB, &vPointC, &m_vOriginal, &m_vDirection, &fU, &fV, &fDist))
	{
		*pOut = m_vOriginal + m_vDirection * fDist;
		return true;
	}

	return false;
}

_bool CPickingManager::isPicked_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	_float		fU{}, fV{}, fDist{};

	if (TRUE == D3DXIntersectTri(&vPointA, &vPointB, &vPointC, &m_vRayPos_InLocalSpace, &m_vRayDir_InLocalSpace, &fU, &fV, &fDist))
	{
		*pOut = m_vRayPos_InLocalSpace + m_vRayDir_InLocalSpace * fDist;
		return true;
	}

	return false;
}


CPickingManager* CPickingManager::Create(LPDIRECT3DDEVICE9 pGraphic_Device ,HWND hWnd)
{
	CPickingManager* pInstance = new CPickingManager(pGraphic_Device);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX(TEXT("Failed to Created : CPickingManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPickingManager::Free()
{
	__super::Free();

	Safe_Release(m_pGraphic_Device);
}

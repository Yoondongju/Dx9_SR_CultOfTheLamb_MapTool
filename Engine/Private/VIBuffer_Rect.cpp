#include "../Public/VIBuffer_Rect.h"

#include "../Public/GameInstance.h"

CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	m_iVertexStride = sizeof(VTXTEX);
	m_iNumVertices = 4;			// ���ؽ� ���۸� ����Ұ�� ��� ���������� ��ġ�� �ߺ��Ǵ��� ���� �ʿ�������?
								// �ε��� ���۸� ����ҰŴϱ� (�簢�� �ϳ� �׸��ٰ� ����)4���� �����ϰ� �ߺ��Ⱦֵ��� �ε����� �־��ٰ��� 6�� 
								// ���� ���ؽ� ����ü ũ�Ⱑ Ŭ���� �޸������� �̵��� ��
								// 
	// FVF �����Ҷ� D3DFVF_XYZ ��� �ϸ� ����������� 0,0�̰�
	// D3DFVF_XYZ�� 3D�������� X,Y,Z��ǥ�� ������ȯ������ ��ǥ��  (��ũ����ǥ)
	// ��ũ����ǥ: 0,0�� ��� ������ +y �Ʒ��� -y ������ ��ǥ�� �Ȱ��ƿ�
	// D3DFVF_XYZRHW�� ������ȯ�� �Ϸ�� 2D���� ��ǥ��			   (��������ǥ)
	// FVF ������ D3DFVF_XYZRHW��� �ϸ� ������ winapió�� 2d��ǥ �״�� ��� �����ϸ��
	// �ֳĸ� D3DFVF_XYZRHW�� ���������������� ��ġ���ʰ� x,y�� �״�ΰ� ������ ��ǥ�� �Ǿ����
	// ��ĺ�ȯ�̳� ��ǥ�輳���� ���൵ �����ʾƿ�
	// �����̵���ü�� �ȴ� �� ������â�� 2D��ǥ������Ҷ� (UI ������?) ������ ����?

	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_iNumFaces = 2;
	m_iIndexStride = sizeof(WORD);

	m_pVerticesPos = new _float3[m_iNumVertices];	

	m_iNumIndices = 6; // �ε����� 6�� �ʿ����� �簢�� 1�� �׸��ǵ� �ﰢ�� 2���� �ʿ��ϰ� �ﰢ�� ���� ���� 3���� �ʿ��ϴϰ� 
	m_eIndexFormat = D3DFMT_INDEX16;

	/* ���� �ǵ��� �����ŭ�� ���� �迭������ �Ҵ��ߴ�. (���ؽ� ����) */
	if (FAILED(__super::Ready_VertexBuffer()))
		return E_FAIL;



	/* ���������� ���� ������ ��ġ�� �޸� �ּҸ� ���� �� �ִ�. */
	//  /*m_iNumVertices * m_iVertexStride*/ �������� ��ü�� �����Ÿ� �̷�������ִ°ǵ�
	// �ι�° ���ڿ� 0�־ �� ��ü�� �д´ٴ°Ŷ� ���� 
	// ���۸� �����  pVertices�� ������ �����Ҽ��ն�
	// �̷��� ���� �޸𸮿����� �������۷� ����ϸ� ����Ҽ� �ִ�.
	// �츮�� ���� VRAM�� �����Ҽ������� �̷� ������ġ�� �����ϴ°�
	// m_pVB�� Lock�� �θ��� ����ϱ� Lock ~ UnLock ���̿��� �ٲ� ���������� (���ؽ�)�� ����Ǿ�
	// m_pVB�� ���������� 

#pragma region ���ؽ� ���۸� ����ؼ� �׸����  ���ؽ� ����ֱ� 
	// VTXTEX*		pVertices = { nullptr };

	// m_pVB->Lock(0, 0, (void**)&pVertices, 0); 
	// 
	// pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	// pVertices[0].vTexcoord = _float2(0.f, 0.f);
	// 
	// pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	// pVertices[1].vTexcoord = _float2(1.f, 0.f);
	// 
	// pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	// pVertices[2].vTexcoord = _float2(1.f, 1.f);
	// 
	// pVertices[3].vPosition = _float3(-0.5f, 0.5f, 0.f);
	// pVertices[3].vTexcoord = _float2(0.f, 0.f);
	// 
	// pVertices[4].vPosition = _float3(0.5f, -0.5f, 0.f);
	// pVertices[4].vTexcoord = _float2(1.f, 1.f);
	// 
	// pVertices[5].vPosition = _float3(-0.5f, -0.5f, 0.f);
	// pVertices[5].vTexcoord = _float2(0.f, 1.f);
	// 
	// m_pVB->Unlock();

#pragma endregion ���ؽ� ���۸� ����ؼ� �׸����  ���ؽ� ����ֱ� 

#pragma region �ε��� ���۸� ����ؼ� ���ؽ� ����ֱ� 
	VTXTEX* pVertices = { nullptr };
	/* ���������� ���� ������ ��ġ�� �޸� �ּҸ� ���� �� �ִ�. */
	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);
	//pVertices[0].vColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);
	//pVertices[1].vColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);
	//pVertices[2].vColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);
	//pVertices[3].vColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	m_pVB->Unlock();



	if (FAILED(Ready_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	m_pIB->Unlock();

#pragma endregion �ε��� ���۸� ����ؼ� ���ؽ� ����ֱ� 

	

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{	
	return S_OK;
}

_bool CVIBuffer_Rect::isPicking(const _float4x4& WorldMatrix, _float3* pOut)
{
	m_pGameInstance->Transform_MouseRay_ToLocalSpace(WorldMatrix);


			/* ������ �� �ﰢ�� */
			if (true == m_pGameInstance->isPicked_InLocalSpace(m_pVerticesPos[0],
				m_pVerticesPos[1],
				m_pVerticesPos[2],
				pOut))
				goto Compute_WorldPos;


			/* ���� �Ʒ� �ﰢ�� */
			if (true == m_pGameInstance->isPicked_InLocalSpace(m_pVerticesPos[0],
				m_pVerticesPos[2],
				m_pVerticesPos[3],
				pOut))
				goto Compute_WorldPos;
	
	return false;

Compute_WorldPos:
	D3DXVec3TransformCoord(pOut, pOut, &WorldMatrix);
	return true;

}

CVIBuffer_Rect* CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVerticesPos);	
}

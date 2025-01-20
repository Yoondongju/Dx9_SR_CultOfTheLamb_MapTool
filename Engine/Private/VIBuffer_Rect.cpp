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
	m_iNumVertices = 4;			// 버텍스 버퍼를 사용할경우 모든 정점에대한 위치가 중복되더라도 각각 필요하지만?
								// 인덱스 버퍼를 사용할거니까 (사각형 하나 그린다고 가정)4개만 설정하고 중복된애들을 인덱스에 넣어줄거임 6개 
								// 따라서 버텍스 구조체 크기가 클수록 메모리적으로 이득을 봄
								// 
	// FVF 설정할때 D3DFVF_XYZ 라고 하면 모니터중점이 0,0이고
	// D3DFVF_XYZ는 3D공간에서 X,Y,Z좌표로 정점변환이전의 좌표임  (스크린좌표)
	// 스크린좌표: 0,0이 가운데 위쪽이 +y 아래가 -y 수학적 좌표랑 똑같아욧
	// D3DFVF_XYZRHW는 정점변환이 완료된 2D상의 좌표임			   (윈도우좌표)
	// FVF 설정을 D3DFVF_XYZRHW라고 하면 기존에 winapi처럼 2d좌표 그대로 라고 생각하면됨
	// 왜냐면 D3DFVF_XYZRHW은 고정파이프라인을 거치지않고 x,y값 그대로가 윈도우 좌표가 되어버림
	// 행렬변환이나 좌표계설정을 해줘도 먹질않아용
	// 월드이동자체가 안댐 걍 윈도우창에 2D좌표값사용할때 (UI 같은거?) 쓸때만 쓸듯?

	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_iNumFaces = 2;
	m_iIndexStride = sizeof(WORD);

	m_pVerticesPos = new _float3[m_iNumVertices];	

	m_iNumIndices = 6; // 인덱스는 6개 필요하지 사각형 1개 그릴건데 삼각형 2개가 필요하고 삼각형 개당 정점 3개가 필요하니가 
	m_eIndexFormat = D3DFMT_INDEX16;

	/* 내가 의도한 사이즈만큼의 정점 배열공간을 할당했다. (버텍스 버퍼) */
	if (FAILED(__super::Ready_VertexBuffer()))
		return E_FAIL;



	/* 정점버퍼의 내가 지정한 위치의 메모리 주소를 얻어올 수 있다. */
	//  /*m_iNumVertices * m_iVertexStride*/ 정점버퍼 전체를 읽을거면 이렇게잡아주는건데
	// 두번째 인자에 0넣어도 걍 전체를 읽는다는거랑 같음 
	// 버퍼를 잠근후  pVertices의 정보를 수정할수잇땅
	// 이렇게 락된 메모리영역을 정점버퍼로 통신하며 사용할수 있다.
	// 우리가 직접 VRAM에 접근할수없으니 이런 안전장치로 접근하는겨
	// m_pVB가 Lock을 부르는 멤버니까 Lock ~ UnLock 사이에서 바뀐 정보데이터 (버텍스)가 저장되어
	// m_pVB에 쓰여질것임 

#pragma region 버텍스 버퍼만 사용해서 그릴경우  버텍스 잡아주기 
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

#pragma endregion 버텍스 버퍼만 사용해서 그릴경우  버텍스 잡아주기 

#pragma region 인덱스 버퍼를 사용해서 버텍스 잡아주기 
	VTXTEX* pVertices = { nullptr };
	/* 정점버퍼의 내가 지정한 위치의 메모리 주소를 얻어올 수 있다. */
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

#pragma endregion 인덱스 버퍼를 사용해서 버텍스 잡아주기 

	

	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{	
	return S_OK;
}

_bool CVIBuffer_Rect::isPicking(const _float4x4& WorldMatrix, _float3* pOut)
{
	m_pGameInstance->Transform_MouseRay_ToLocalSpace(WorldMatrix);


			/* 오른쪽 위 삼각형 */
			if (true == m_pGameInstance->isPicked_InLocalSpace(m_pVerticesPos[0],
				m_pVerticesPos[1],
				m_pVerticesPos[2],
				pOut))
				goto Compute_WorldPos;


			/* 왼쪽 아래 삼각형 */
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

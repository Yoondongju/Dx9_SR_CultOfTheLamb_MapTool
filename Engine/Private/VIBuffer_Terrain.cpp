#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
	: CVIBuffer{ Prototype }
	, m_pFilterVerticesCoord { Prototype.m_pFilterVerticesCoord }
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iVertexStride = sizeof(VTXTEX);
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;		// 갯수를 의미


	m_pVerticesPos = new _float3[m_iNumVertices];


	m_pFilterVerticesCoord = new _float2[m_iNumVertices];

	m_iNumVerticesX = iNumVerticesX;   // 가로 세로를 기억해야해 
	m_iNumVerticesZ = iNumVerticesZ;


	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_iNumFaces = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
	m_iIndexStride = 4;  // sizeof(DWORD)
	m_iNumIndices = m_iNumFaces * 3;
	m_eIndexFormat = D3DFMT_INDEX32;

	/* 내가 의도한 사이즈만큼의 정점 배열공간을 할당했다. */
	if (FAILED(__super::Ready_VertexBuffer()))
		return E_FAIL;

	VTXTEX* pVertices = { nullptr };
	/* 정점버퍼의 내가 지정한 위치의 메모리 주소를 얻어올 수 있다. */
	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, (void**)&pVertices, 0);

	for (size_t i = 0; i < iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < iNumVerticesX; j++)
		{
			_uint		iIndex = i * iNumVerticesX + j;



			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(j, 0.f, i);
			pVertices[iIndex].vTexcoord = _float2(j / (iNumVerticesX - 1.f) * iNumVerticesX, i / (iNumVerticesZ - 1.f) * iNumVerticesZ); // 어짜피 똑같은걸 걍 계속 그려주는거임  0~1사이 범위니까 이를 초과하면 다시 0으로 ex) 1.1이면 0.1 
			m_pFilterVerticesCoord[iIndex] = _float2(j / (iNumVerticesX - 1.f) , i / (iNumVerticesZ - 1.f));
		}
	}
	
	m_pVB->Unlock();

	if (FAILED(Ready_IndexBuffer()))
		return E_FAIL;

	_uint* pIndices = { nullptr };

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + iNumVerticesX, /* 왼쪽 위 정점의 인덱스 */
				iIndex + iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];
		}
	}

	m_pIB->Unlock();


	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_Terrain::Bind_FilterCoord()
{

	IDirect3DVertexBuffer9* pTexCoordBuffer = nullptr;

	_ulong d3dTexCoordFormat = D3DFVF_TEXCOORDSIZE2(0);
	DWORD dwTexCoordSize = _msize(m_pFilterVerticesCoord) / sizeof(_float2);
	m_pGraphic_Device->CreateVertexBuffer(dwTexCoordSize, D3DUSAGE_WRITEONLY, d3dTexCoordFormat, D3DPOOL_MANAGED, &pTexCoordBuffer, NULL);

	// 텍스처 좌조 버퍼 설정

	// 앵간하면 0임 , 사용할 정점버퍼 , 오프셋 0 , 정점구조체의 사이즈
	m_pGraphic_Device->SetStreamSource(0, pTexCoordBuffer, 0, dwTexCoordSize);
	m_pGraphic_Device->SetFVF(dwTexCoordSize);
	m_pGraphic_Device->SetIndices(m_pIB);


}

_float CVIBuffer_Terrain::Compute_Height(const _float3& vLocalPos)
{
	/* 객체가 존재하는 네모 영역의 왼쪽 하단 인덱스를 구했다. */
	_uint		iIndex = _uint(vLocalPos.z) * m_iNumVerticesX + _uint(vLocalPos.x);

	if (iIndex > 100000000)
		return 0.f;

	/* 왼쪽하단이넫ㄱ스 기준으로 네방향의 ㄱ꼭지점 인덷ㄱ스를 구하자.  */
	_uint		iIndices[4] = {
		iIndex + m_iNumVerticesX, /* 왼위*/
		iIndex + m_iNumVerticesX + 1, /* 오위 */
		iIndex + 1, /* 오아 */
		iIndex /* 왼아 */
	};

	_float		fWidth = vLocalPos.x - m_pVerticesPos[iIndices[0]].x;
	_float		fDepth = m_pVerticesPos[iIndices[0]].z - vLocalPos.z;

	D3DXPLANE		Plane{};

	/* 오른쪽 위 삼각형안에있어. */
	if (fWidth > fDepth)
	{
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]]);
		
	}
	/* 왼쪽 아래 삼각형안에있어. */
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[2]], &m_pVerticesPos[iIndices[3]]);
		
	}

	/* ax + by + cz + d */
	/* y = (-ax - cz - d) / b */
	return (-Plane.a * vLocalPos.x - Plane.c * vLocalPos.z - Plane.d) / Plane.b;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

/* 원형객체를 찾아서 원형객체의 Clone함수를 호출한다.  */
CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pFilterVerticesCoord);
	}
		

	__super::Free();

}


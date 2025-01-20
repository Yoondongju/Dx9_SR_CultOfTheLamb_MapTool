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
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;		// ������ �ǹ�


	m_pVerticesPos = new _float3[m_iNumVertices];


	m_pFilterVerticesCoord = new _float2[m_iNumVertices];

	m_iNumVerticesX = iNumVerticesX;   // ���� ���θ� ����ؾ��� 
	m_iNumVerticesZ = iNumVerticesZ;


	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_iNumFaces = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
	m_iIndexStride = 4;  // sizeof(DWORD)
	m_iNumIndices = m_iNumFaces * 3;
	m_eIndexFormat = D3DFMT_INDEX32;

	/* ���� �ǵ��� �����ŭ�� ���� �迭������ �Ҵ��ߴ�. */
	if (FAILED(__super::Ready_VertexBuffer()))
		return E_FAIL;

	VTXTEX* pVertices = { nullptr };
	/* ���������� ���� ������ ��ġ�� �޸� �ּҸ� ���� �� �ִ�. */
	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, (void**)&pVertices, 0);

	for (size_t i = 0; i < iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < iNumVerticesX; j++)
		{
			_uint		iIndex = i * iNumVerticesX + j;



			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(j, 0.f, i);
			pVertices[iIndex].vTexcoord = _float2(j / (iNumVerticesX - 1.f) * iNumVerticesX, i / (iNumVerticesZ - 1.f) * iNumVerticesZ); // ��¥�� �Ȱ����� �� ��� �׷��ִ°���  0~1���� �����ϱ� �̸� �ʰ��ϸ� �ٽ� 0���� ex) 1.1�̸� 0.1 
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
				iIndex + iNumVerticesX, /* ���� �� ������ �ε��� */
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

	// �ؽ�ó ���� ���� ����

	// �ް��ϸ� 0�� , ����� �������� , ������ 0 , ��������ü�� ������
	m_pGraphic_Device->SetStreamSource(0, pTexCoordBuffer, 0, dwTexCoordSize);
	m_pGraphic_Device->SetFVF(dwTexCoordSize);
	m_pGraphic_Device->SetIndices(m_pIB);


}

_float CVIBuffer_Terrain::Compute_Height(const _float3& vLocalPos)
{
	/* ��ü�� �����ϴ� �׸� ������ ���� �ϴ� �ε����� ���ߴ�. */
	_uint		iIndex = _uint(vLocalPos.z) * m_iNumVerticesX + _uint(vLocalPos.x);

	if (iIndex > 100000000)
		return 0.f;

	/* �����ϴ��̆����� �������� �׹����� �������� �ΉK������ ������.  */
	_uint		iIndices[4] = {
		iIndex + m_iNumVerticesX, /* ����*/
		iIndex + m_iNumVerticesX + 1, /* ���� */
		iIndex + 1, /* ���� */
		iIndex /* �޾� */
	};

	_float		fWidth = vLocalPos.x - m_pVerticesPos[iIndices[0]].x;
	_float		fDepth = m_pVerticesPos[iIndices[0]].z - vLocalPos.z;

	D3DXPLANE		Plane{};

	/* ������ �� �ﰢ���ȿ��־�. */
	if (fWidth > fDepth)
	{
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]]);
		
	}
	/* ���� �Ʒ� �ﰢ���ȿ��־�. */
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

/* ������ü�� ã�Ƽ� ������ü�� Clone�Լ��� ȣ���Ѵ�.  */
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


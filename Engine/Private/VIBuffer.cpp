#include "../Public/VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
	: CComponent(Prototype)
	, m_pVB {Prototype.m_pVB}
	, m_pIB{ Prototype.m_pIB }

	, m_iVertexStride{ Prototype.m_iVertexStride }
	, m_iNumVertices{ Prototype.m_iNumVertices }
	, m_dwFVF{ Prototype.m_dwFVF }
	, m_iNumFaces{ Prototype.m_iNumFaces }

	, m_iIndexStride{ Prototype.m_iIndexStride }
	, m_iNumIndices{ Prototype.m_iNumIndices }
	, m_eIndexFormat{ Prototype.m_eIndexFormat }
	, m_iNumVerticesX(Prototype.m_iNumVerticesX)
	, m_iNumVerticesZ{Prototype.m_iNumVerticesZ}
	, m_pVerticesPos{ Prototype.m_pVerticesPos }

{
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	// 하드웨어적인 그래픽을 그리기위해 사용하는 인터페이스는 m_pGraphic_Device
	// 정점버퍼를 사용하기위한 인터페이스는 CreateVertexBuffer로 정점버퍼를 만들고 
	// CreateVertexBuffer만 했을때 정점버퍼를 다룰수있는 포인터를 m_pVB에 받는거고
	// 그 포인터로 메모리를 얻어오려면 m_pVB->Lock을 써야함 ㅇㅇ
	// 만들어진 정점버퍼에서 메모리를 읽어오려면 Lock함수를 통해서 메모리를 가져와야한다.

	// 버텍스버퍼로 부터 그릴게 
	// 어떤도형으로 , 시작버텍스의 인덱스임 , 몇개의 면을 그릴래

	//m_pGraphic_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_iNumFaces);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumFaces);

	return S_OK;
}

HRESULT CVIBuffer::Bind_Buffers()
{
	// 정점버퍼에 정점들 정보 잇는데 , 이 정점버퍼를 출력 스트림으로 결합시켜줄게
	// 앵간하면 0임 , 사용할 정점버퍼 , 오프셋 0 , 정점구조체의 사이즈
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->SetIndices(m_pIB);

	return S_OK;
}

HRESULT CVIBuffer::Ready_VertexBuffer()
{
	/* 정점배열의 공간을 생성할 수 있도록 함수를 호출한다. */
	/* 1. Length : 정점 배열공간을 몇 바이트나 할당할까요? (정점하나의 바이트 크기 * 정점의 갯수) */
	/* 2. Usage : 이 버퍼의 속성(STATIC : 0, DYNAMIC: D3DUSAGE_DYNAMIC)을 설정해달라. */
	/* 3. fvf : D3DFVF 내 정점이 이러이러한 구성으로 되어있어.구성만 알려주느 ㄴ것이기 때문에 변수 선언의 순서를 유의해한다. */
	/* 4. MemoryPool : D3DPOOL_DEFAULT(VRam), D3DPOOL_MANAGED(VRam or Systemmem), D3DPOOL_SYSTEMMEM(Ram) */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices,
		0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CVIBuffer::Ready_IndexBuffer()
{
	m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices,
		0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr);

	return S_OK;
}


void CVIBuffer::Free()
{
	__super::Free();
	

	if (false == m_isCloned)				// 사본이 아니라면 == 프로토타입만
		Safe_Delete_Array(m_pVerticesPos);


	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}

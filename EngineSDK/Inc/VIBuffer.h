#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;


public:
	LPDIRECT3DVERTEXBUFFER9 Get_VB() { return m_pVB; }


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();


public:
	virtual _bool isPicking(const _float4x4& WorldMatrix, _float3* pOut) { return false; }


public:
	HRESULT Bind_Buffers();




protected:
	/* dx�� �˾Ƽ� �Ҵ��س� ���� �迭�� ������ ��ǥ�ϴ� �İ�ü.  */
	LPDIRECT3DVERTEXBUFFER9			m_pVB = { nullptr };		// ���ؽ� ����
	LPDIRECT3DINDEXBUFFER9			m_pIB = { nullptr };		// �ε��� ����

	_uint							m_iVertexStride = { 0 };
	_uint							m_iNumVertices = { 0 };
	_ulong							m_dwFVF = {};
	_uint							m_iNumFaces = {};  // ��� ���� �׸���


	_uint							m_iIndexStride = { 0 };		// �ε����� ũ��
	_uint							m_iNumIndices = { 0 };		// �ε��� ����
	D3DFORMAT						m_eIndexFormat = {  };		// �ε��� ����

	_float3*						m_pVerticesPos = { nullptr };


public:
	_uint		m_iNumVerticesX;
	_uint		m_iNumVerticesZ;


protected:
	HRESULT Ready_VertexBuffer();
	HRESULT	Ready_IndexBuffer();


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
#include "../Public/Renderer.h"

#include "../Public/GameObject.h"

#include "../Public/BlendObject.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device}
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CRenderer::Initialize()
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERGROUP eRenderGroupID, CGameObject* pRenderObject)
{
	if (nullptr == pRenderObject ||
		eRenderGroupID >= RG_END)
		return E_FAIL;


	m_RenderObjects[eRenderGroupID].emplace_back(pRenderObject);
	Safe_AddRef(pRenderObject);			// �߰��ϴºκе� ���� �����Ҵ��ؼ� �߰��Ѱ��� �ƴϰ� 

	return S_OK;
}

HRESULT CRenderer::Draw()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[RG_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& pGameObject : m_RenderObjects[RG_NONBLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	m_RenderObjects[RG_BLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			return dynamic_cast<CBlendObject*>(pSour)->Get_ViewZ() > dynamic_cast<CBlendObject*>(pDest)->Get_ViewZ();
		});

	for (auto& pGameObject : m_RenderObjects[RG_BLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjects[RG_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);			// �������ִ°Ծƴ϶� �׳� ���۷��� ī��Ʈ�� ���ҽ�Ű������

	}
	m_RenderObjects[RG_UI].clear();

	return S_OK;
}

CRenderer* CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer* pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CRenderer::Free()
{
	__super::Free();

	
	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pGameObject : RenderObjects)
			Safe_Release(pGameObject);

		RenderObjects.clear();
	}

	Safe_Release(m_pGraphic_Device);
}

#include "../Public/GameObject.h"
#include "../Public/GameInstance.h"

#include "../Public/Texture.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance {CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject& Prototype)
	: m_pGraphic_Device { Prototype.m_pGraphic_Device}
	, m_pGameInstance   { Prototype.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);


}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{


	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CComponent* CGameObject::Find_Component(const _wstring& strComponentTag)
{
	auto iter = m_Components.find(strComponentTag);
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _wstring strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	CComponent* pPreComponent = Find_Component(strComponentTag);
	if (nullptr != pPreComponent)
	{
		Safe_Release(pPreComponent);
		m_Components.erase(strComponentTag);		
	}

	CComponent* pComoponent =  m_pGameInstance->Clone_Component(iLevelIndex, strPrototypeTag, pArg);

	m_Components.emplace(strComponentTag, pComoponent);
	
	*ppOut = pComoponent;
	Safe_AddRef(pComoponent);

	return S_OK;
}

CGameObject* CGameObject::Clone(void* pArg)
{
	return nullptr;
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}

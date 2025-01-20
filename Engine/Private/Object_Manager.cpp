#include "../Public/Object_Manager.h"

#include "../Public/GameObject.h"
#include "../Public/Layer.h"


CObject_Manager::CObject_Manager()
{
}


HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
    if (nullptr != m_pLayers)
        return E_FAIL;

    m_pLayers = new LAYERS[iNumLevels];
    m_iNumLevels = iNumLevels;

    return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const _wstring& strPrototypeTag, CGameObject* pPrototype)
{
    CGameObject* pGameObj = Find_Prototype(strPrototypeTag);

    if (nullptr != pGameObj)      // �̹� �ش� PrototypeTag���� ���� ��ü�� ������// Prototype���� ���� �ϳ��� PrototypeTag���� �����ָ� �߰��ϴ°ű⿡ ���п���
    {
        Safe_Release(pGameObj);
        m_Prototypes.erase(strPrototypeTag);
    }

    m_Prototypes.emplace(strPrototypeTag, pPrototype);
    return S_OK;
}

HRESULT CObject_Manager::Add_CloneObject_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strPrototypeTag, void* pArg)
{
    if (iLevelIndex >= m_iNumLevels)
        return E_FAIL;


    CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
    if (nullptr == pPrototype)    // ���� �߰��Ұ�ü�� ������Ÿ���� �������
        return E_FAIL;           // �̰͵� �׳� ���Ͻ��Ѿ����� assert�� �ɾ������ �߰��� ��������� �𸣰ڴ�


    CGameObject* pGameObject = pPrototype->Clone(pArg);
    if (nullptr == pGameObject)     // Ŭ���� nullptr �������°�찡 �ֳ�?   
        return E_FAIL;



    CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

    if (nullptr == pLayer)         // ���̾ �մٴ¼Ҹ�
    {
        pLayer = CLayer::Create(); // ���̾ �����Ҷ� strLayerTag�� �޴°� �� ���� �ʾ�����? 
        pLayer->Add_GameObj(pGameObject);
        m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
    }
    else
    {
        if (L"Layer_BackGround_Terrain" == strLayerTag)   // ������ �纻�� �ϳ�������
        {
            pLayer->Clear_Layer();
            pLayer->Add_GameObj(pGameObject);
            m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
        }
        else
        {
            pLayer->Add_GameObj(pGameObject);
            m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
        }
    }



    return S_OK;
}



HRESULT CObject_Manager::Priority_Update(_float fTimeDelta)
{
    for (size_t i = 0; i < m_iNumLevels; ++i)
    {
        for (auto& Pair : m_pLayers[i])
        {
            Pair.second->Priority_Update(fTimeDelta);
        }
    }
    return S_OK;
}

HRESULT CObject_Manager::Update(_float fTimeDelta)
{
    for (size_t i = 0; i < m_iNumLevels; ++i)
    {
        // �ڱⰡ ����ִ� ������ �����°� �ƴ�? �̶�� �����ߴµ�
        // ��¥�� �ڱⰡ ����ִ� ������ ��ü�� ������ �ι�° for���� �ȵ�����
        // ���� ������ ������ŭ�� ������ ����. 
        for (auto& Pair : m_pLayers[i])
        {
            // ex)  ���ⷹ�̾� Object�� ����������, Ȱ�������� �̷��� �����ϱ� 
            Pair.second->Update(fTimeDelta);
        }
    }
    return S_OK;
}

HRESULT CObject_Manager::Late_Update(_float fTimeDelta)
{
    for (size_t i = 0; i < m_iNumLevels; ++i)   // ���� ���� ��������
    {
        for (auto& Pair : m_pLayers[i])
        {
            Pair.second->Late_Update(fTimeDelta);
        }
    }
    return S_OK;
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
    for (auto& Pair : m_pLayers[iLevelIndex])
        Safe_Release(Pair.second);

    m_pLayers[iLevelIndex].clear();
}

CComponent* CObject_Manager::Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
    CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
    if (nullptr == pLayer)
        return nullptr;

    return pLayer->Find_Component(strComponentTag, iIndex);
}

CGameObject* CObject_Manager::Find_Prototype(const _wstring& strPrototypeTag)
{
    auto iter = m_Prototypes.find(strPrototypeTag);

    if (iter == m_Prototypes.end())
        return nullptr;

    return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndx, const _wstring& strLayerTag)
{
    auto iter = m_pLayers[iLevelIndx].find(strLayerTag);

    if (iter == m_pLayers[iLevelIndx].end())
        return nullptr;

    return iter->second;
}

list<class CGameObject*>& CObject_Manager::Get_GameObjects(_uint iLevelIndx, const _wstring& strLayerTag)
{
    CLayer* pLayer = Find_Layer(iLevelIndx, strLayerTag);
    if (nullptr == pLayer)
        assert(pLayer);



    return pLayer->Get_GamObjects();
}

CGameObject* CObject_Manager::IsObjectAtPosition(_uint iLevelIndex, const _wstring& strLayerTag , _float3 vPosition)
{
    auto iter = m_pLayers[iLevelIndex].find(strLayerTag);
   
    if (iter == m_pLayers[iLevelIndex].end())
        return false;

    return (iter->second)->IsObjectAtPosition(vPosition);
}

CGameObject* CObject_Manager::IsObjectAtPosition_GoodVerdict(_uint iLevelIndex, const _wstring& strLayerTag, _float3 vPosition)
{
    auto iter = m_pLayers[iLevelIndex].find(strLayerTag);

    if (iter == m_pLayers[iLevelIndex].end())
        return false;

    return (iter->second)->IsObjectAtPosition_GoodVerdict(vPosition);
}


CGameObject* CObject_Manager::ObjectAtPosition(_uint iLevelIndex, const _wstring& strLayerTag ,_float3 vPosition)
{
    auto iter = m_pLayers[iLevelIndex].find(strLayerTag);

    if (iter == m_pLayers[iLevelIndex].end())
        return false;

    return (iter->second)->ObjectAtPosition(vPosition);
}


CGameObject* CObject_Manager::Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
    CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
    if (nullptr == pLayer)
        return nullptr;

    return pLayer->Find_Object(iIndex);
}

CGameObject* CObject_Manager::Find_Player()
{
    CGameObject* pPlayer = Find_Object(0, TEXT("Layer_Player"), 0);
    if (nullptr == pPlayer)
        return nullptr;

    return pPlayer;
}


CObject_Manager* CObject_Manager::Create(_uint iNumLevels)
{
    CObject_Manager* pInstance = new CObject_Manager;

    if (FAILED(pInstance->Initialize(iNumLevels)))
    {
        MSG_BOX(TEXT("Failed to Created : CObject_Manager"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CObject_Manager::Free()
{
    __super::Free();

    for (size_t i = 0; i < m_iNumLevels; ++i)
    {
        for (auto& Pair : m_pLayers[i])
            Safe_Release(Pair.second);
        m_pLayers[i].clear();
    }
    Safe_Delete_Array(m_pLayers);


    for (auto& Pair : m_Prototypes)
        Safe_Release(Pair.second);
    m_Prototypes.clear();
}

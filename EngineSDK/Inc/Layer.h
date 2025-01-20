#pragma once
#include "Base.h"

BEGIN(Engine)
class CLayer final : public CBase
{
private:
    CLayer();
    virtual ~CLayer() = default;


public:
    _uint Get_GamObjectSize();
    list<class CGameObject*>& Get_GamObjects() { return m_GameObjects; }

public:

    class CGameObject* ObjectAtPosition(_float3 vPosition);
    class CGameObject* IsObjectAtPosition(_float3 vPosition);

    class CGameObject* IsObjectAtPosition_GoodVerdict(_float3 vPosition);

public:
    HRESULT   Add_GameObj(class CGameObject* pGameObject);
    void Clear_Layer()
    {
        for (auto& pGameObj : m_GameObjects)
        {
            Safe_Release(pGameObj);
        }
        m_GameObjects.clear();
    }

    HRESULT Priority_Update(_float fTimeDelta);
    HRESULT Update(_float fTimeDelta);
    HRESULT Late_Update(_float fTimeDelta);



public:
    class CComponent* Find_Component(const _wstring& strComponentTag, _uint iIndex);    // Ư�� �ε����� �����ϴ� �ѳ� ã�°�
    class CGameObject* Find_Object(_uint iIndex);

private:
    list<class CGameObject*>      m_GameObjects;

public:
    static CLayer* Create();
    virtual void Free() override;
};
END


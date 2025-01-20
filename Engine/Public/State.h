#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CState :
    public CBase
{
protected:
    CState();
    virtual ~CState() = default;

public:
    _uint				    Get_State() { return m_iStateNum; }

    void					SetFSM(class CFsm* pFSM) { 
        m_pFsm = pFSM; 
        Safe_AddRef(m_pFsm);
    }

public:
    virtual HRESULT Initialize(_uint iStateNum) = 0;
    virtual HRESULT Start_State() = 0;
    virtual void    Update(_float fTimeDelta) = 0;
    virtual void    End_State() = 0;

protected:
    class CFsm*         m_pFsm = { nullptr };
    _uint 			    m_iStateNum;
    _float              m_fStateTimer = { 0.f };
    _float2 m_vMoveVec = { 0.f,0.f };

    class CGameInstance*    m_pGameInstance = { nullptr };

public:
    virtual void Free() override;

};

END
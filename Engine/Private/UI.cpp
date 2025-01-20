#include "../Public/UI.h"

#include "../Public/GameInstance.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CUI::CUI(const CGameObject& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	return S_OK;
}

void CUI::Priority_Update(_float fTimeDelta)
{
}

void CUI::Update(_float fTimeDelta)
{
}

void CUI::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI::Render()
{
	return S_OK;
}

void CUI::MouseOnCheck()
{
	POINT vMousePos = m_pGameInstance->Get_MosePos();
	D3DXVECTOR3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	RECT tUIRect;
	SetRect(&tUIRect, m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f,
		m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);

	if (vMousePos.x > tUIRect.left
		&& vMousePos.x < tUIRect.right
		&& vMousePos.y > tUIRect.top
		&& vMousePos.y < tUIRect.bottom)
	{
		m_bMouseOn = true;
	}
	else
		m_bMouseOn = false;

}

void CUI::MouseLBtnDown()
{
}

void CUI::MouseLBtnUp()
{
}

void CUI::MouseClick()
{
}

void CUI::MouseOn()
{
}

void CUI::MouseOut()
{
}


void CUI::Free()
{
	__super::Free();
}

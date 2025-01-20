#include "stdafx.h"
#include "../Public/Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CCamera::CCamera(const CCamera& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

	if(FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vEye);
	m_pTransformCom->LookAt(pDesc->vAt);


	m_fFovy = pDesc->fFovy;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
	if (g_hWnd != GetFocus())
		return;

	POINT		ptMouse = {};

	GetCursorPos(&ptMouse);

	/* 기본적인 제어*/
	if (m_pGameInstance->Get_KeyState(KEY::W) == KEY_STATE::HOLD)
		m_pTransformCom->Go_Straight(fTimeDelta * 4.f);
	if (m_pGameInstance->Get_KeyState(KEY::S) == KEY_STATE::HOLD)
		m_pTransformCom->Go_Backward(fTimeDelta * 4.f);
	if (m_pGameInstance->Get_KeyState(KEY::A) == KEY_STATE::HOLD)
		m_pTransformCom->Go_Left(fTimeDelta * 4.f);
	if (m_pGameInstance->Get_KeyState(KEY::D) == KEY_STATE::HOLD)
		m_pTransformCom->Go_Right(fTimeDelta * 4.f);

	_long	MouseMove = {};

	if (!g_bFocusImGui)
	{
		if (m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::HOLD)
		{
			if (MouseMove = ptMouse.x - m_ptOldMousePos.x)
			{
				m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.1f);
			}

			if (MouseMove = ptMouse.y - m_ptOldMousePos.y)
			{
				m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
			}
		}
	}
	

	//if (m_pGameInstance->Get_KeyState(KEY::ENTER) == KEY_STATE::HOLD)
	//{
	//	Cam_ZoomIn();
	//}
	//
	//if (m_pGameInstance->Get_KeyState(KEY::SPACE) == KEY_STATE::HOLD)
	//{
	//	Cam_ZoomOut();
	//}

	

	/* 장치에 뷰랑 투영행렬을 한번 셋팅 해주면 다시 갱신해 주기 전까지 모든 객체들을 렌더링시에 셋팅해준 행렬을 곱하면서 그리게 된다. */
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransformCom->Get_WorldMatrix_Inverse()); // 카메라의 월드역행렬

	_float4x4		ProjMatrix{};
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_fFovy, (_float)g_iWinSizeX / g_iWinSizeY, m_fNear, m_fFar));

	m_ptOldMousePos = ptMouse;
	
}

void CCamera::Update(_float fTimeDelta)
{
	
}

void CCamera::Late_Update(_float fTimeDelta)
{
}


HRESULT CCamera::Render()
{
	return E_NOTIMPL;
}


HRESULT CCamera::Ready_Components(void* pArg)
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera::Cam_ZoomIn()
{
	static float fDeltaTime(0);
	if (D3DXToDegree(m_fFovy) > 0.f)
	{
		fDeltaTime += m_pGameInstance->Compute_TimeDelta(TEXT("Timer_60"));

		if (fDeltaTime < 0.6f)
		{
			static float fTime(0.f);
			fTime = fDeltaTime * 2;
			m_fFovy -= D3DXToRadian(fTime);    // 내 시야각이 180도 넘어가면  좌우가 반전됨 (거울같음?)
		}
		else
		{
			fDeltaTime = 0.f;
		}

		if (D3DXToDegree(m_fFovy) < 0.f)
			m_fFovy = D3DXToRadian(1.f);
	}
}

void CCamera::Cam_ZoomOut()
{
	if (D3DXToDegree(m_fFovy) < 180.f)
	{
		m_fFovy += D3DXToRadian(1.f);    // 내 시야각이 180도 넘어가면  좌우가 반전됨 (거울같음?)

		if (D3DXToDegree(m_fFovy) > 180.f)
			m_fFovy = D3DXToRadian(179.f);
	}
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera* pInstance = new CCamera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera"));
		
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera::Clone(void* pArg)
{
	CCamera* pInstance = new CCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}

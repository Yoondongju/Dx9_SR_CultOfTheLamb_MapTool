#include "stdafx.h"
#include "../Public/State_PlayerIdle.h"
#include "../Public/Player.h"

#include "GameInstance.h"
#include "Transform.h"
#include "Animator.h"

CState_PlayerIdle::CState_PlayerIdle()
{
}

HRESULT CState_PlayerIdle::Initialize(_uint iStateNum)
{
	m_iStateNum = iStateNum;
	return S_OK;
}

HRESULT CState_PlayerIdle::Start_State()
{
	m_pPlayer = static_cast<CPlayer*>(m_pFsm->Get_Owner());
	//Safe_AddRef(m_pPlayer);

	if(m_pGameInstance->Get_KeyState(KEY::W)==KEY_STATE::AWAY)
		m_pPlayer->Change_Animation(TEXT("IDLE_UP"));
	else
		m_pPlayer->Change_Animation(TEXT("IDLE"));

	return S_OK;
}

void CState_PlayerIdle::Update(_float fTimeDelta)
{
	/*Check_Hit();
	Check_Dead();
	Check_Attack();
	Check_Move();*/
}

void CState_PlayerIdle::End_State()
{
	//Safe_Release(m_pPlayer);
}

void CState_PlayerIdle::Check_Attack()
{
	if (m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::TAP)
	{
		m_pPlayer->Change_State(CPlayer::ATTACK);
	}

	if (m_pGameInstance->Get_KeyState(KEY::RBUTTON) == KEY_STATE::HOLD)
	{
		m_pPlayer->Change_State(CPlayer::SPELL);
	}
}

void CState_PlayerIdle::Check_Move()
{
	if(m_pGameInstance->Get_KeyState(KEY::W) == KEY_STATE::HOLD
		|| m_pGameInstance->Get_KeyState(KEY::S) == KEY_STATE::HOLD
		|| m_pGameInstance->Get_KeyState(KEY::A) == KEY_STATE::HOLD
		|| m_pGameInstance->Get_KeyState(KEY::D) == KEY_STATE::HOLD)
		m_pPlayer->Change_State(CPlayer::RUN);
	 
	if (m_pGameInstance->Get_KeyState(KEY::LSHIFT) == KEY_STATE::TAP)
		m_pPlayer->Change_State(CPlayer::ROLL);

	//if (m_pGameInstance->Get_KeyState(KEY::SPACE) == KEY_STATE::TAP)
	//	m_pPlayer->Change_State(CPlayer::WARP);
	
}

void CState_PlayerIdle::Check_Dead()
{
	if (m_pPlayer->IsDead())
	{
		m_pPlayer->Change_State(CPlayer::DIE);
	}
}

void CState_PlayerIdle::Check_Hit()
{
	if (m_pPlayer->IsHit())
		m_pPlayer->Change_State(CPlayer::KNOCKBACK);
}

CState_PlayerIdle* CState_PlayerIdle::Create(_uint iStateNum)
{
	CState_PlayerIdle* pInstance = new CState_PlayerIdle();

	if (FAILED(pInstance->Initialize(iStateNum)))
	{
		MSG_BOX(TEXT("Failed to Created : CState_PlayerIdle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_PlayerIdle::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
#include "VIBuffer_Rect.h"
#include "Texture.h"
#include "Transform.h"


#include "../Public/State_PlayerIdle.h"
//#include "State_PlayerAttack.h"
//#include "State_PlayerRun.h"
//#include "State_PlayerRoll.h"
//#include "State_PlayerResurrect.h"
//#include "State_PlayerDie.h"
//#include "State_PlayerSpell.h"
//#include "State_PlayerKnockBack.h"
//#include "State_PlayerWarp.h"
//#include "State_PlayerAxe.h"
//#include "State_PlayerPickAxe.h"
//#include "State_PlayerHammer.h"
//#include "State_PlayerSermon.h"
//#include "State_PlayerGF.h"
//#include "State_PlayerGS.h"
//#include "State_PlayerCard.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer & Prototype)
	: CLandObject{ Prototype }
	, m_tPlayerInfo{Prototype.m_tPlayerInfo}
{

}


HRESULT CPlayer::Initialize_Prototype()
{
	m_tPlayerInfo.iHp = 3;
	m_tPlayerInfo.iAttackPower = 10;

	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (FAILED(Ready_Fsm()))
		return E_FAIL;



	m_pTransformCom->Scaling(5.f, 5.f, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(1.f, 1.f , 1.f));



	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	
}

void CPlayer::Update(_float fTimeDelta)
{
	if (m_eCurState != BUILD)
	{
		__super::SetUp_OnTerrain(m_pTransformCom, 0.5f);

		Key_Input(fTimeDelta);

		m_pFsmCom->Update(fTimeDelta);
	}
	
	//if (m_pGameInstance->Get_KeyState(KEY::P) == KEY_STATE::TAP)
	//{
	//	m_eCurState = BUILD;
	//
	//	// 집 짓는 상태 모드변경 
	//	// 카메라는 이 빌딩 오브젝트를 따라다녀야함
	//	
	//	CLandObject::LANDOBJECT_DESC	Desc = {};
	//
	//	Desc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround_Terrain"), TEXT("Com_Transform")));
	//	Desc.pTerrainVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround_Terrain"), TEXT("Com_VIBuffer")));
	//	Desc.iInitBindTexNum = 2;		// 일단 성역
	//	Desc.vInitPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//	Desc.vInitScale = m_pTransformCom->Get_Scaled();
	//
	//
	//	if (FAILED(m_pGameInstance->Event_CreateObject(LEVEL_GAMEPLAY, TEXT("Layer_BuildingObject"), TEXT("Prototype_GameObject_BuildingObject"), sizeof(LANDOBJECT_DESC), &Desc)))
	//		MSG_BOX(L"건축 오브젝트를 생성하지 못했습니다");
	//
	//
	//	// 건축상태일때 패널 UI변경, 카메라 타겟 -> 빌딩오브젝트로 변경 
	//	// 내가 PlaceTile 보여주기
	//
	//}






}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pAnimatorCom->Update(fTimeDelta);

	//m_pColliderCom->Update_Collider(m_pTransformCom->Get_WorldMatrix());

	m_bPlayerHit = false;
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer::Render()
{	

	if (FAILED(m_pTransformCom->Bind_Transform()))
		return E_FAIL;
	
	m_pAnimatorCom->Render(NULL);

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;		

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//디버그 모드 추가하기
	//m_pColliderCom->Render_Collider();

	return S_OK;
}

void CPlayer::Set_InitState(PLAYERSTATE eState, void* pArg)
{
	m_pFsmCom->Set_State(eState, pArg);
}

void CPlayer::Change_State(PLAYERSTATE eState, void* pArg)
{
	m_pFsmCom->Change_State(eState, pArg);
}

void CPlayer::Change_Animation(const _wstring& strAnimationTag, _uint iStartFrame)
{
	m_pAnimatorCom->Change_Animation(strAnimationTag, iStartFrame);
}

bool CPlayer::IsEnd_CurrentAnimation()
{
	return m_pAnimatorCom->IsEnd_CurrentAnimation();
}

HRESULT CPlayer::Ready_Components()
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};

	TransformDesc.fSpeedPerSec = 10.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider_Cube */
	//CCollider_Cube::COLLIDER_CUBE_INFO ColliderInfo;
	//
	//ZeroMemory(&ColliderInfo, sizeof(CCollider_Cube::COLLIDER_CUBE_INFO));
	//ColliderInfo.fSizeX = 0.15f;
	//ColliderInfo.fSizeY = 0.2f;
	//ColliderInfo.fSizeZ = 0.2f;
	//ColliderInfo.fOffSetX = 0.f;
	//ColliderInfo.fOffSetY = 0.f;
	//ColliderInfo.fOffSetZ = 0.f;
	//
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
	//	TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderInfo)))
	//	return E_FAIL;
	//
	return S_OK;
}

HRESULT CPlayer::Ready_Animation()
{
	CTexture* pTextureCom;

	if (FAILED(__super::Add_Component(Client::LEVEL_STATIC, TEXT("Prototype_Component_Animation"),
		TEXT("Com_Animation"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), nullptr)))
		return E_FAIL;
	// IDLE
	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Idle"),
		TEXT("Com_Texture_Player_Idle"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
		return E_FAIL;
	m_pAnimatorCom->Add_Animation(TEXT("IDLE"), pTextureCom, 0, 50, 0.03f);

	// IDLE_UP
	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Idle_Up"),
		TEXT("Com_Texture_Player_Idle_Up"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
		return E_FAIL;
	m_pAnimatorCom->Add_Animation(TEXT("IDLE_UP"), pTextureCom, 0, 100, 0.03f);

	//// Get_Sword
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Get_Sword"),
	//	TEXT("Com_Texture_Player_Get_Sword"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Get_Sword"), pTextureCom, 0, 82, 0.05f, false);

	//// Get_Follower
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Get_Follower"),
	//	TEXT("Com_Texture_Player_Get_Follower"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Get_Follower"), pTextureCom, 0, 124, 0.03f, false);

//#pragma region ATTACK
//	// Attack_Combo1
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Attack_Sword_Combo1_red"),
//		TEXT("Com_Texture_Attack_Sword_Combo1_red"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	// 애니메이션 태그 , 텍스처 컴포넌트 , 시작 프레임 , 끝 프레임 , 출력 속도 , 루프
//	m_pAnimatorCom->Add_Animation(TEXT("Attack1"), pTextureCom, 0, 20, 0.03f , false);
//
//	// Attack_Combo2
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Attack_Sword_Combo2_red"),
//		TEXT("Com_Texture_Attack_Sword_Combo2_red"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Attack2"), pTextureCom, 0, 25, 0.03f, false);
//
//	// Attack_Combo3
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Attack_Sword_Combo3_red"),
//		TEXT("Com_Texture_Attack_Sword_Combo3_red"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Attack3"), pTextureCom, 0, 25, 0.03f, false);
//#pragma endregion
//
//#pragma region RUN
//	// Run_Diagonal_Down
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Run_Diagonal_Down"),
//		TEXT("Com_Texture_Player_Run_Diagonal_Down"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Run_Diagonal_Down"), pTextureCom, 0, 10, 0.05f, true);
//
//	// Run_Diagonal_Up
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Run_Diagonal_Up"),
//		TEXT("Com_Texture_Player_Run_Diagonal_Up"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Run_Diagonal_Up"), pTextureCom, 0, 10, 0.05f, true);
//
//	// Run_Down
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Run_Down"),
//		TEXT("Com_Texture_Player_Run_Down"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Run_Down"), pTextureCom, 0, 10, 0.05f, true);
//
//	// Run_Up
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Run_Up"),
//		TEXT("Com_Texture_Player_Run_Up"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Run_Up"), pTextureCom, 0, 10, 0.05f, true);
//
//	// Run_Horizontal
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Run_Horizontal"),
//		TEXT("Com_Texture_Player_Run_Horizontal"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Run_Horizontal"), pTextureCom, 0, 10, 0.05f, true);
//#pragma endregion
//
//#pragma region ROLL
//	// Roll_Up
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Roll_Up"),
//		TEXT("Com_Texture_Player_Roll_Up"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Roll_Up"), pTextureCom, 0, 10, 0.02f,false);
//
//	// Roll_Down
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Roll_Down"),
//		TEXT("Com_Texture_Player_Roll_Down"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Roll_Down"), pTextureCom, 0, 10, 0.02f,false);
//
//	// Roll_Horizontal
//	if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Roll_Horizontal"),
//		TEXT("Com_Texture_Player_Roll_Horizontal"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
//		return E_FAIL;
//	m_pAnimatorCom->Add_Animation(TEXT("Roll_Horizontal"), pTextureCom, 0, 12, 0.02f,false);
//#pragma endregion

	//// Die
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Die"),
	//	TEXT("Com_Texture_Player_Die"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Die"), pTextureCom, 0, 100, 0.03f, false);

	//// Spell_Charge
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Spell_Charge"),
	//	TEXT("Com_Texture_Player_Spell_Charge"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Spell_Charge"), pTextureCom, 0, 10, 0.05f, true);

	//// Spell_Shoot
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Spell_Shot"),
	//	TEXT("Com_Texture_Player_Spell_Shot"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Spell_Shot"), pTextureCom, 0, 10, 0.05f, false);

	//// KnockBack
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_KnockBack"),
	//	TEXT("Com_Texture_Player_KnockBack"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("KnockBack"), pTextureCom, 0, 26, 0.05f, false);

	//// WarpIn
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_WarpIn"),
	//	TEXT("Com_Texture_Player_WarpIn"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("WarpIn"), pTextureCom, 0, 98, 0.03f, false);

	//// WarpOut
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_WarpOut"),
	//	TEXT("Com_Texture_Player_WarpOut"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("WarpOut"), pTextureCom, 0, 72, 0.03f, false);

	//// Resurrect
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Resurrect"),
	//	TEXT("Com_Texture_Player_Resurrect"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Resurrect"), pTextureCom, 0, 100, 0.03f, false);

	//// Axe
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Axe"),
	//	TEXT("Com_Texture_Player_Axe"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Axe"), pTextureCom, 0, 25, 0.03f, true);

	//// PickAxe
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_PickAxe"),
	//	TEXT("Com_Texture_Player_PickAxe"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("PickAxe"), pTextureCom, 0, 25, 0.03f, true);

	//// Hammer
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Hammer"),
	//	TEXT("Com_Texture_Player_Hammer"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Hammer"), pTextureCom, 0, 18, 0.03f, true);

	//// Sermon_Start
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Sermon_Start"),
	//	TEXT("Com_Texture_Player_Sermon_Start"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Sermon_Start"), pTextureCom, 0, 48, 0.03f, false);

	//// Sermon_Loop
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Sermon_Loop"),
	//	TEXT("Com_Texture_Player_Sermon_Loop"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Sermon_Loop"), pTextureCom, 0, 133, 0.03f, false);

	//// Sermon_End
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Sermon_End"),
	//	TEXT("Com_Texture_Player_Sermon_End"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Sermon_End"), pTextureCom, 0, 22, 0.03f, false);

	//// Card_Start
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Cards_Start"),
	//	TEXT("Com_Texture_Player_Cards_Start"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Card_Start"), pTextureCom, 0, 37, 0.03f, false);

	//// Card_Loop
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Cards_Loop"),
	//	TEXT("Com_Texture_Player_Cards_Loop"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Card_Loop"), pTextureCom, 0, 33, 0.03f, false);

	//// Card_End
	//if (FAILED(__super::Add_Component(Client::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Cards_End"),
	//	TEXT("Com_Texture_Player_Cards_End"), reinterpret_cast<CComponent**>(&pTextureCom), nullptr)))
	//	return E_FAIL;
	//m_pAnimatorCom->Add_Animation(TEXT("Card_End"), pTextureCom, 0, 107, 0.03f, false);

	return S_OK;
}

HRESULT CPlayer::Ready_Fsm()
{	
	/* For.Com_Fsm */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Fsm"),
		TEXT("Com_Fsm"), reinterpret_cast<CComponent**>(&m_pFsmCom), this)))
		return E_FAIL;

	m_pFsmCom->Add_State(CState_PlayerIdle::Create(IDLE));
	/*m_pFsmCom->Add_State(CState_PlayerAttack::Create(ATTACK));
	m_pFsmCom->Add_State(CState_PlayerRun::Create(RUN));
	m_pFsmCom->Add_State(CState_PlayerRoll::Create(ROLL));
	m_pFsmCom->Add_State(CState_PlayerSpell::Create(SPELL));
	m_pFsmCom->Add_State(CState_PlayerKnockBack::Create(KNOCKBACK));
	m_pFsmCom->Add_State(CState_PlayerWarp::Create(WARP));
	m_pFsmCom->Add_State(CState_PlayerAxe::Create(AXE));
	m_pFsmCom->Add_State(CState_PlayerPickAxe::Create(PICKAXE));
	m_pFsmCom->Add_State(CState_PlayerHammer::Create(HAMMER));
	m_pFsmCom->Add_State(CState_PlayerResurrect::Create(RESURRECT));
	m_pFsmCom->Add_State(CState_PlayerDie::Create(DIE));
	m_pFsmCom->Add_State(CState_PlayerSermon::Create(SERMON));
	m_pFsmCom->Add_State(CState_PlayerGF::Create(GET_FOLLOWER));
	m_pFsmCom->Add_State(CState_PlayerCard::Create(CARD));
	m_pFsmCom->Add_State(CState_PlayerGS::Create(GET_SWORD));*/
	return S_OK;
}

HRESULT CPlayer::SetUp_RenderState()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	/* 픽셀이 알파 테스트를 통과하여 그려질 조건을 설정한다. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 127);

	return S_OK;
}

HRESULT CPlayer::Reset_RenderState()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CPlayer::OnCollisionEnter(CGameObject* pOther)
{
}

void CPlayer::OnCollisionStay(CGameObject* pOther)
{
	//if (pOther->Get_LayerTag() == TEXT("Layer_Boss") && m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::TAP)
	//{
	//	//Change_State(CPlayer::CARD);
	//	//m_eCurState = AXE;
	//}

}

void CPlayer::OnCollisionExit(CGameObject* pOther)
{
	//if (pOther->Get_LayerTag() == TEXT("Layer_Boss"))
	//{
	//	Change_State(CPlayer::IDLE);
	//	m_eCurState = AXE;
	//}
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	if (m_pGameInstance->Get_KeyState(KEY::UP) == KEY_STATE::HOLD)
		m_pTransformCom->Go_Straight(fTimeDelta*0.2  * 5.f);
	if (m_pGameInstance->Get_KeyState(KEY::DOWN) == KEY_STATE::HOLD)
		m_pTransformCom->Go_Backward(fTimeDelta*0.2 * 5.f);
	if (m_pGameInstance->Get_KeyState(KEY::LEFT) == KEY_STATE::HOLD)
	{
		//m_isFilp = false;
		//m_pVIBufferCom->Filp_Texture(m_isFilp);
		m_pTransformCom->Go_Left(fTimeDelta * 0.2 * 5.f);
	}
	if (m_pGameInstance->Get_KeyState(KEY::RIGHT) == KEY_STATE::HOLD)
	{
		//m_isFilp = true;
		//m_pVIBufferCom->Filp_Texture(m_isFilp);
		m_pTransformCom->Go_Right(fTimeDelta * 0.2 * 5.f);
	}
	if (m_pGameInstance->Get_KeyState(KEY::SPACE) == KEY_STATE::TAP)
	{
		/*m_tPlayerInfo.iHp--;
		m_bPlayerHit = true;*/
	}
}


CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*		pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

/* 원형객체를 찾아서 원형객체의 Clone함수를 호출한다.  */
CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	//일단 임시로 이렇게 해둠 아이디어 있으면 반영좀
	if (nullptr != m_pFsmCom)
		m_pFsmCom->Release_States();
	Safe_Release(m_pFsmCom);

	Safe_Release(m_pAnimatorCom);
	Safe_Release(m_pVIBufferCom);
}

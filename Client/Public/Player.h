#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CAnimator;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Rect;
class CFsm;
END

BEGIN(Client)

class CPlayer final : public CLandObject
{
public:
	enum PLAYERSTATE
	{
		IDLE,
		ATTACK,
		RUN,
		ROLL,
		SPELL,
		KNOCKBACK,
		WARP,
		AXE,
		PICKAXE,
		HAMMER,
		SERMON,
		GET_FOLLOWER,
		CARD,
		GET_SWORD,
		RESURRECT,
		DIE,

		BUILD,
		STATE_END

	};

public:
	typedef struct {
		_uint iHp;
		_uint iAttackPower;
	}PLAYER_INFO;

private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_InitState(PLAYERSTATE eState, void* pArg = nullptr);
	void Change_State(PLAYERSTATE eState, void* pArg = nullptr);
	void Change_Animation(const _wstring &strAnimationTag, _uint iStartFrame = 0);
	bool IsEnd_CurrentAnimation();

public:
	void Set_Player_State(PLAYERSTATE _eState) { m_eCurState = _eState;}

	PLAYERSTATE Get_Player_State()
	{
		return m_eCurState;
	}
	bool IsDead()
	{
		if (m_tPlayerInfo.iHp<=0)
			return true;

		return false;
	}
	bool IsHit()
	{
		return m_bPlayerHit;
	}
	

private:
	CAnimator* m_pAnimatorCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CFsm* m_pFsmCom = { nullptr };

	

	PLAYER_INFO		m_tPlayerInfo = {};
	PLAYERSTATE		m_eCurState = IDLE;
	_bool			m_bPlayerHit = false;
	_bool			m_bBuildDead = false;


private:
	HRESULT Ready_Components();
	HRESULT Ready_Animation();
	HRESULT Ready_Fsm();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();

private:
	virtual void OnCollisionEnter(CGameObject* pOther);
	virtual void OnCollisionStay(CGameObject* pOther);
	virtual void OnCollisionExit(CGameObject* pOther);

private:
	void Key_Input(_float fTimeDelta);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
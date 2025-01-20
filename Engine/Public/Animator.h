#pragma once
#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CAnimator final
	: public CComponent
{
public:
	typedef struct
	{
		_uint		iStartFrame = {  };
		_uint		iEndFrame = {  };
		_float		fDuration = {  };
		_bool		isLoop = { false };
	}ANIMATION_DESC;

private:
	CAnimator(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimator(const CAnimator& Prototype);
	virtual ~CAnimator() = default;

public:
	_bool IsEnd_CurrentAnimation() { return m_isEnd; }
	_uint Get_CurrentFrame() { return m_iCurrentFrame; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	void Update(_float fTimeDelta);
	void Render(HDC hDC);

public:
	HRESULT Change_Animation(const _wstring& strAnimationTag, _uint iStartFrame);
	HRESULT Add_Animation(const _wstring& strAnimationTag, CTexture* pTexture, _uint iStartFrame, _uint iEndFrame, _float fDuration, _bool isLoop = true);
	CTexture* Find_Animation(const _wstring& strAnimationTag);
	void Reset_Frame();

private:
	map<const _wstring, CTexture*>				m_Animation;
	map<const _wstring, ANIMATION_DESC*>		m_AnimationDesc;

	CTexture*			m_pCurrentAnimation = { nullptr };
	ANIMATION_DESC*		m_pCurrentAnimationDesc = {};

	int					m_iCurrentFrame = {};
	float				m_fAccTime = {}; // 시간 누적값
	bool				m_isEnd = { false };

public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
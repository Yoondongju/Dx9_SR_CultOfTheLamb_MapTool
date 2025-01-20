#pragma once
#include "Base.h"

BEGIN(Engine)

class CKey_Manager final : public CBase
{
public:
	typedef struct
	{
		KEY_STATE	eState;  // Ű�� ���°�
		bool		bPrev;   // ���� �����ӿ��� ���ȴ��� �ȴ��ȴ��� 
	}KEYINFO;

private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	KEY_STATE	Get_KeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	
public:
	HRESULT Initialize(HWND hWnd);
	void Update();

private:
	HWND		m_hWnd = {};

	vector<KEYINFO>		m_vecKey;

public:
	static CKey_Manager* Create(HWND hWnd);
	virtual void Free() override;

};

END
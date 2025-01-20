#pragma once
/*
class C클래스이름
{
private: or protected:
	생성자
	소멸자.

public:
	Getter
	Setter
public:
	변수
public:
	함수
protected:
	변수
protected:
	함수
private:
	변수
private:
	함수

public:
	생성관련된 함수. (Create)
	삭제관련된 함수. (Free)
};
*/

#include "Client_Defines.h"
#include "Base.h"



BEGIN(Engine)
class CGameInstance;
END


BEGIN(Client) 

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;


public:
	HRESULT Initialize();
	void	Update(_float fTimeDelta);
	void    Final_Update();
	HRESULT Render();



	
private:
	CGameInstance*      m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9   m_pGraphic_Device = { nullptr };


private:
	HRESULT Ready_Default_Setting();
	HRESULT Ready_Prototype_Component_Static();
	HRESULT  Open_Level(LEVELID eStartLevelID);





public:
	static CMainApp* Create();
	virtual void Free() override;

};

END

#pragma once

#include "Base.h"




// CGraphic_Device ��� Ŭ�󿡼� �� �ʿ䰡 ���� 
// Engine�� GameObject���� �׷��ȵ���̽��� ��� �Ұű� ������ 
// ���� DLL�� ������(export) �ʿ䵵 Ŭ�󿡼� ����(import) �ʿ䵵 ����
// ���� ENGINE_DLL�� ������.


BEGIN(Engine) 

class CGraphic_Device final : public CBase
{
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;


public:
	HWND		Get_Handle() {
		D3DDEVICE_CREATION_PARAMETERS creationParams;
		m_pDevice->GetCreationParameters(&creationParams);

		return creationParams.hFocusWindow;
	}

public:
	// ��ġ �ʱ�ȭ ����

	// 1. ��ġ�� ������ ��ü�� ���� ����
	// 2. ��ġ�� �����ϵ��� ����(���� ���� �ľ�)
	// 3. ��ġ�� ������ ��ü�� ����

	HRESULT		Init_Device(const ENGINE_DESC& EngineDesc, _Inout_ LPDIRECT3DDEVICE9* ppDevice);
	void		Render_Begin();
	void		Render_End(HWND hWnd = nullptr);

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp, const ENGINE_DESC& EngineDesc);

private:
	LPDIRECT3D9			m_pSDK;		// 1���� �ش��ϴ� ��ü
	LPDIRECT3DDEVICE9	m_pDevice;	// 3���� �ش��ϴ� ��ü
	LPD3DXSPRITE		m_pSprite;	// dx �󿡼� 2d �̹��� ����� ����ϴ� ��ü
	LPD3DXFONT			m_pFont;

public:
	static CGraphic_Device* Create(const ENGINE_DESC& EngineDesc, _Inout_ LPDIRECT3DDEVICE9* ppDevice);
	virtual void Free() override;
};

END
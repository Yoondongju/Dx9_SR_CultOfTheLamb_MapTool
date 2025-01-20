#pragma once
#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_TEXTURE2D/* : LPDIRECT3DTEXTURE9*/, TYPE_TEXTURECUBE/* : LPDIRECT3DCUBETEXTURE9*/, TYPE_END };

private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	IDirect3DBaseTexture9* Get_Texture(_uint iNum) 
	{
		if (iNum >= m_iNumTextures)
			assert(nullptr);

		return m_Textures[iNum]; 
	}

public:
	virtual HRESULT Initialize_Prototype(TYPE eTextureType, const _wstring& strTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_Texture(_uint iIndex = 0);
	HRESULT Bind_SecondTexture(_uint iIndex = 0);		// 필터 전용 
	

private:
	_uint								m_iNumTextures = { 0 };
	vector<IDirect3DBaseTexture9*>		m_Textures;

public:
	static  CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eTextureType, const _wstring& strTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void *pArg) override;
	virtual void Free() override;
};

END

#include "../Public/Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CTexture::CTexture(const CTexture& Prototype)  
	: CComponent(Prototype)		// (다형성이라 보기 어려움) 자식을 부모에 대입하는 개념인데 이때 Prototype의 부모쪽의멤버만 복사됨
	, m_iNumTextures(Prototype.m_iNumTextures)
	, m_Textures(Prototype.m_Textures)
{
	// 복사생성은 객체가 생기기 이전에 데이터복사
	// 대입연산은 객체가 생기고 데이터 복사	
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);

}

HRESULT CTexture::Initialize_Prototype(TYPE eTextureType, const _wstring& strTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;
	m_Textures.reserve(iNumTextures);

	for (size_t i = 0; i < iNumTextures; ++i)
	{
		IDirect3DBaseTexture9*		pTexture = nullptr;

		_tchar szTextureFilePath[_MAX_PATH] = TEXT("");

		wsprintf(szTextureFilePath, strTextureFilePath.c_str(), i);

		HRESULT hr = eTextureType == TYPE_TEXTURE2D
			? D3DXCreateTextureFromFile(m_pGraphic_Device, szTextureFilePath, reinterpret_cast<LPDIRECT3DTEXTURE9*>(&pTexture))
			: D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szTextureFilePath, reinterpret_cast<LPDIRECT3DCUBETEXTURE9*>(&pTexture));

		if (FAILED(hr))
			return E_FAIL;

		m_Textures.emplace_back(pTexture);
	}


	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_Texture(_uint iIndex)
{
	if (iIndex >= m_iNumTextures || iIndex < 0)
		return E_FAIL;

	
	m_pGraphic_Device->SetTexture(0, m_Textures[iIndex]);

	return S_OK;
}

HRESULT CTexture::Bind_SecondTexture(_uint iIndex)
{
	if (iIndex >= m_iNumTextures || iIndex < 0)
		return E_FAIL;


	m_pGraphic_Device->SetTexture(1, m_Textures[iIndex]);

	return S_OK;
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eTextureType, const _wstring& strTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eTextureType, strTextureFilePath, iNumTextures)))
	{
		MSG_BOX(TEXT("Failed to Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();
}

#pragma once
#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	// strLayerTag : ������
	// strPrototypeTag: ���������, �������� �̷���
	HRESULT	Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(const _wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_CloneObject_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strPrototypeTag, void* pArg = nullptr);
	
	HRESULT Priority_Update(_float fTimeDelta);
	HRESULT Update(_float fTimeDelta);
	HRESULT Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);
	
	
public:
	class CComponent* Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex);
	class CLayer*     Find_Layer(_uint iLevelIndx, const _wstring& strLayerTag);
	list<class CGameObject*>& Get_GameObjects(_uint iLevelIndx, const _wstring& strLayerTag);



	class CGameObject* ObjectAtPosition(_uint iLevelIndex, const _wstring& strLayerTag , _float3 vPosition);

	class CGameObject* IsObjectAtPosition(_uint iLevelIndex, const _wstring& strLayerTag, _float3 vPosition);
	class CGameObject* IsObjectAtPosition_GoodVerdict(_uint iLevelIndex, const _wstring& strLayerTag, _float3 vPosition);


public:
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex);
	class CGameObject* Find_Player();




private:
	// ���� ��ü ����
	map<const _wstring, class CGameObject*>		m_Prototypes;	

	// �������� �纻 ��ü�� ������ �뵵
	_uint										m_iNumLevels = {};
	map<const _wstring, class CLayer*>*			m_pLayers = { nullptr };
	typedef map<const _wstring, class CLayer*>  LAYERS;			


private:
	class CGameObject* Find_Prototype(const _wstring& strPrototypeTag);
	


public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END


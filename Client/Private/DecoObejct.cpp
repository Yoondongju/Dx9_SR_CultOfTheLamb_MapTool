#include "stdafx.h"
#include "../Public/DecoObejct.h"

#include "GameInstance.h"
#include "../Public/Level_MapTool.h"

CDecoObject::CDecoObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLandObject(pGraphic_Device)
{
}

CDecoObject::CDecoObject(const CDecoObject& Prototype)
	: CLandObject(Prototype)
{
}

HRESULT CDecoObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDecoObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	LANDOBJECT_DESC* pDesc = static_cast<LANDOBJECT_DESC*>(pArg);
	m_iPreModelType = pDesc->iPreModelType;
	m_iPreAdornType = pDesc->iPreAdornType;
	m_iPreLandType = pDesc->iPreLandMode;


	m_iBindTexNum = pDesc->iInitBindTexNum;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_strName = TEXT("Prototype_GameObject_DecoObject");
	if (m_strLayerName == TEXT("Layer_Preview_Object"))
	{
		m_iAlpha = 200.f;
	}




	_float3 vInitPos = pDesc->vInitPos;
	_float3 vSacling = pDesc->vInitScale;


	_float fRotationX = D3DXToRadian(pDesc->fRotationX);


	m_pTransformCom->Scaling(vSacling.x, vSacling.y, vSacling.z);



	m_pTransformCom->Rotation(_float3(1.f,0.f,0.f),  fRotationX);
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vInitPos);

	return S_OK;
}

void CDecoObject::Priority_Update(_float fTimeDelta)
{
	if (m_strLayerName == TEXT("Layer_Preview_Object"))
	{

		CLevel* pLevel = m_pGameInstance->GetCurLevel();
		CLevel_MapTool* pLevel_MapTool = dynamic_cast<CLevel_MapTool*>(pLevel);

		_uint iModelType = pLevel_MapTool->GetModelType();
		_uint iLandMode = pLevel_MapTool->GetLandMode();
		_uint iAdornType = pLevel_MapTool->Get_AdornType();


		wstring ModelPrototypeTag = L"";
		switch (iModelType)
		{
		case CLevel_MapTool::RECT:
			ModelPrototypeTag = TEXT("Prototype_Component_VIBuffer_Rect");
			break;

		case CLevel_MapTool::GRIDRECT:
			ModelPrototypeTag = TEXT("Prototype_Component_VIBuffer_GridRect");
			break;

		default:
			break;
		}	


		if (m_iPreAdornType != iAdornType)
		{
			m_iPreAdornType = iAdornType;
			switch (iAdornType)
			{
			case CLevel_MapTool::VILLAGE:
			{
				if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Village_Deco"),
					TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
					MSG_BOX(TEXT("Tex_Change nono"));
			}				
				break;
			case CLevel_MapTool::STAGE1:
			{
				if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon_Deco"),
					TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
					MSG_BOX(TEXT("Tex_Change nono"));
			}
				
				break;
			case CLevel_MapTool::STAGE2:
			{
				if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon2_Deco"),
					TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
					MSG_BOX(TEXT("Tex_Change nono"));
			}
				break;
			case CLevel_MapTool::STAGE3:
			{
				if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon_ETC_Deco"),
					TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
					MSG_BOX(TEXT("Tex_Change nono"));
			}
				break;
			default:
				break;
			}

		}
		
		

		if (m_iPreModelType != iModelType)
		{
			m_iPreModelType = iModelType;
			Change_ComponentModel(ModelPrototypeTag);			
		}
		
		if (m_iPreLandType != iLandMode)
		{
			m_iPreLandType = iLandMode;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
		}

	}
		
}

void CDecoObject::Update(_float fTimeDelta)
{
	_float3		vPickPos{};

	if (m_pGameInstance->Get_KeyState(KEY::RBUTTON) == KEY_STATE::TAP)
	{
		if (m_strLayerName != TEXT("Layer_Preview_Object"))
		{
			if (m_pVIBufferCom->isPicking(m_pTransformCom->Get_WorldMatrix(), &vPickPos))
				m_pGameInstance->Event_DestoryObject(this);
		}
	}

	CLevel* pLevel = m_pGameInstance->GetCurLevel();
	CLevel_MapTool* pLevel_MapTool = dynamic_cast<CLevel_MapTool*>(pLevel);

	if(m_iPreLandType != CLevel_MapTool::BACKGROUNDLAND)
		__super::SetUp_OnTerrain(m_pTransformCom, 0.5f);


	if (FAILED(Compute_ViewZ(TEXT("Com_Transform"))))
		return;

}

void CDecoObject::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CDecoObject::Render()
{
	if (m_strLayerName == TEXT("Layer_Preview_Object"))
	{
		CLevel* pLevel = m_pGameInstance->GetCurLevel();
		CLevel_MapTool* pLevel_MapTool = dynamic_cast<CLevel_MapTool*>(pLevel);

		if (pLevel_MapTool->GetSelectObjectType() != CLevel_MapTool::DECO)
			return S_OK;
		else
		{
			_float3 vSacle = pLevel_MapTool->Get_Scale();
			_float3 fTerrainPos = {};
			_float3 vRotation = D3DXToRadian(pLevel_MapTool->Get_Rotation_Degree());


			m_pTransformCom->Scaling(vSacle.x, vSacle.y, vSacle.z);
			m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), vRotation.x);


			switch (pLevel_MapTool->GetLandMode())
			{
			case CLevel_MapTool::CENTERLAND:
				fTerrainPos = m_pGameInstance->Get_PickingObject_CenterPos(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"));
				break;
			case CLevel_MapTool::JUSTLAND:
				fTerrainPos = m_pGameInstance->Get_PickingObject_JustPos(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"));
				break;
				
			case CLevel_MapTool::BACKGROUNDLAND:
				fTerrainPos = m_pGameInstance->Get_PickingBackGround_JustPos();
				fTerrainPos.y = (0.5f * m_pTransformCom->Get_Scaled().y);
				break;

			default:
				break;
			}
				
			switch (pLevel_MapTool->Get_AdornType())
			{
			case CLevel_MapTool::VILLAGE:
				m_iBindTexNum = pLevel_MapTool->GetDecoNum();
				break;
			case CLevel_MapTool::STAGE1:
				m_iBindTexNum = pLevel_MapTool->GetDungeonDecoNum();
				break;
			case CLevel_MapTool::STAGE2:
				m_iBindTexNum = pLevel_MapTool->GetDungeon2DecoNum();
				break;
			case CLevel_MapTool::STAGE3:
				m_iBindTexNum = pLevel_MapTool->GetDungeonETCDecoNum();
				break;
			default:
				break;
			}


			



			
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, fTerrainPos);


			CLevel* pLevel = m_pGameInstance->GetCurLevel();
			CLevel_MapTool* pLevel_MapTool = dynamic_cast<CLevel_MapTool*>(pLevel);

			if (pLevel_MapTool->GetLandMode() != CLevel_MapTool::BACKGROUNDLAND)
				__super::SetUp_OnTerrain(m_pTransformCom, 0.5f);

		}
	}


	if (FAILED(m_pTransformCom->Bind_Transform()))
		return E_FAIL;

	
	if (FAILED(m_pTextureCom->Bind_Texture(m_iBindTexNum)))			// << 여기서 
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;


	SetUp_RenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	Reset_RenderState();

	return S_OK;
}

HRESULT CDecoObject::Ready_Components()
{
	/* For.Com_Texture */

	CLevel* pLevel = m_pGameInstance->GetCurLevel();
	CLevel_MapTool* pLevel_MapTool = dynamic_cast<CLevel_MapTool*>(pLevel);

	if (pLevel_MapTool != nullptr)
	{
		switch (m_iPreAdornType)
		{
		case CLevel_MapTool::VILLAGE:
		{
			if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Village_Deco"),
				TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
				return E_FAIL;
		}		
			break;
		case CLevel_MapTool::STAGE1:
		{
			if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon_Deco"),
				TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
				return E_FAIL;
		}	
			break;
		case CLevel_MapTool::STAGE2:
		{
			if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon2_Deco"),
				TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
				return E_FAIL;
		}
			break;
		case CLevel_MapTool::STAGE3:
		{
			if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon_ETC_Deco"),
				TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
				return E_FAIL;
		}
		break;

		default:
			break;
		}
	}


	_uint iModelType = pLevel_MapTool->GetModelType();

	wstring ModelPrototypeTag = L"";
	switch (m_iPreModelType)
	{
	case CLevel_MapTool::RECT:
		ModelPrototypeTag = TEXT("Prototype_Component_VIBuffer_Rect");
		break;

	case CLevel_MapTool::GRIDRECT:
		ModelPrototypeTag = TEXT("Prototype_Component_VIBuffer_GridRect");
		break;

	default:
		break;
	}

	if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, ModelPrototypeTag,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;



	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};

	TransformDesc.fSpeedPerSec = 20.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDecoObject::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	
	// 알파채널에서 알파를 얻는다.



	D3DCOLOR color = D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, color);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);



	return S_OK;
}

HRESULT CDecoObject::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}

HRESULT CDecoObject::Change_ComponentModel(const wstring& strComponentTag)
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, strComponentTag,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;


	return S_OK;
}

CDecoObject* CDecoObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDecoObject* pInstance = new CDecoObject(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CDecoObject"));

		(pInstance);
	}

	return pInstance;
}

CGameObject* CDecoObject::Clone(void* pArg)
{
	CDecoObject* pInstance = new CDecoObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDecoObject::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}

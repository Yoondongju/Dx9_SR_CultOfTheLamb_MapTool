#pragma once

#include "Client_Defines.h"

#include "InteractObject.h"
#include "Level.h"


BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

class CLevel_MapTool final : public CLevel
{
public:
	enum { MAPBTN, UIBTN, BTNEND };

	enum { VILLAGE, STAGE1, STAGE2 ,STAGE3 , ADORN_END};

	enum { DECO, INTERACT};

	enum { RECT, GRIDRECT};								// 세로랙트말고 눕혀서 가로 그리고싶으면 GRid 선택하세요
	
	enum { CENTERLAND, JUSTLAND,BACKGROUNDLAND};

	typedef struct tTerrainDesc
	{
		


	}TERRAIN_DESC;


private:
	CLevel_MapTool(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_MapTool() = default;

public:
	int  GetTileNum() { return m_iTileNum; }

	int  GetDecoNum() { return m_iVillageDecoNum; }
	int  GetInteractNum() { return m_iVillageInteractNum;  }

	int  GetDungeonDecoNum() { return m_iDungeonDecoNum; }
	int  GetDungeon2DecoNum() { return m_iDungeon2DecoNum; }
	int  GetDungeonETCDecoNum() { return m_iDungeonETC_DecoNum; }

	



	_char GetSelectObjectType() { return m_iSelectObjectType; }
	
	_float3 Get_Scale() { return m_vObject_Scaling; }
	_float3 Get_Rotation_Degree() {
		return _float3(m_vObject_RotationX_Degree, m_vObject_RotationY_Degree, m_vObject_RotationZ_Degree);}


	_int GetModelType() { return m_iModelType; }
	_int GetLandMode() { return m_iLandMode; }


	_uint Get_AdornType() 
	{
		for (_uint i = 0; i < ADORN_END; i++)
		{
			if (m_bAdornBtn[i])
				return i;
		}
	}



public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_ImGui();
	void    ImGui_Render();





private:
	void Open_MapDialog();

	void Open_About_Village();
	void Open_About_Stage1();
	void Open_About_Stage2();
	void Open_About_Stage3();



	void Open_TileList();

	void Open_VillageDecoList();
	void Open_VillageInteractList();
	void Open_VillageSanctuary();




	void Open_DungeonDecoList();
	void Open_Dungeon2DecoList();


	void Open_Dungeon_ETC_DecoList();




	void Create_PreViewObject();
	void Create_DecoObject();
	void Create_InteractObject();

	void Open_UIDialog();




	void Set_LandMode_Center();
	void Set_LandMode_Just();


private:
	void Save_Map();
	
	void Load_Map();
	void Load_Terrain();
	void Load_Object(const string& szName,const string& szLayerName , _uint eInteractType, _uint& iPreModelType, _uint& iPreAdornType, _uint& iPreLandType , const int& iBindNum, const _float4x4& fWorldMat);



	// 랜드오브젝트에서 내가 지형에 대한 정보(컴포넌트가) 필요해 그럼 어떻게 가져와?
	// 랜드오브젝트가 생성될때 값을 채워줘  그럼 이 채워질 값이 pArg로 들어오는거고
	// 던져주는곳이 있어야겠네?
	// 랜드오브젝트가 생성되는 타이밍에서 던져줘  이 렌더오브젝트는 Mon, Player의 부모야 그니까 이 자식들이 초기화 되기전에 세팅해줘야하고
	// 던져줄 정보를 채워야해  그치 그럼 던져줄 정보를 채워야하는데? 지형에 대한 정보는 Terrain이 이미 가지고잇어 걍 이걸 참조하는거지
	// 그럼 지형의 컴포넌트를 찾자 어떻게 찾아 ObjMgr를 통해 내 레이어태그를 통해 레이어를 찾아 
	// 찾았으면 그 레이어의 컴포넌트를 찾아
	// 그 레이어의 컴포넌트는 레이어가 GameObj를 가지고 잇고 GameObj가 list니까 순회하면서 
	// GameObj가 컴포넌트를 알고잇네 그럼 그새키를 가져와


private:
	_bool				m_LandCheckOk = false;

	TERRAIN_DESC		m_tTerrainDesc = {};


	_char		m_iVerticesX[50] = {};				// 가로 버텍스갯수
	_char		m_iVerticesZ[50] = {};				// 세로 버텍스갯수



	_int			m_iModelType = RECT;
	_int			m_iLandMode = CENTERLAND;			// 배치할 모드 골라라  false면 중앙이 아니라 걍 누른곳 선택된다


	_bool			m_bBtnCheck[BTNEND] = { false };	// << Map,UI, etc...

	_bool			m_bAdornBtn[ADORN_END] = { false };	// 마을인지 스테이지1,2 인지..

	_int			m_iTileNum = 0;						// Tile Image 개숫 


	_int			m_iVillageDecoNum = 0;				// 마을전용 데코Obj 이미지
	_int			m_iVillageInteractNum = 0;			// 마을전용 상호작용Obj 이미지


	_int			m_iDungeonDecoNum = 0;				// 던전 데코Obj 이미지
	_int			m_iDungeon2DecoNum = 0;				// 던전 데코Obj 이미지
	_int			m_iDungeonETC_DecoNum = 0;


	_char			m_iSelectObjectType = -1;			// 내가 선택한 오브젝트 타입
	

	
	_float3		m_vObject_InitPos = {0.f,0.f,0.f};
	_float3		m_vObject_Scaling = {1.f,1.f,1.f};

	_float		m_vObject_RotationX_Degree = 0.f;
	_float		m_vObject_RotationY_Degree = 0.f;
	_float		m_vObject_RotationZ_Degree = 0.f;



public:
	static CLevel_MapTool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
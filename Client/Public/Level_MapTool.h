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

	enum { RECT, GRIDRECT};								// ���η�Ʈ���� ������ ���� �׸�������� GRid �����ϼ���
	
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



	// ���������Ʈ���� ���� ������ ���� ����(������Ʈ��) �ʿ��� �׷� ��� ������?
	// ���������Ʈ�� �����ɶ� ���� ä����  �׷� �� ä���� ���� pArg�� �����°Ű�
	// �����ִ°��� �־�߰ڳ�?
	// ���������Ʈ�� �����Ǵ� Ÿ�ֿ̹��� ������  �� ����������Ʈ�� Mon, Player�� �θ�� �״ϱ� �� �ڽĵ��� �ʱ�ȭ �Ǳ����� ����������ϰ�
	// ������ ������ ä������  ��ġ �׷� ������ ������ ä�����ϴµ�? ������ ���� ������ Terrain�� �̹� �������վ� �� �̰� �����ϴ°���
	// �׷� ������ ������Ʈ�� ã�� ��� ã�� ObjMgr�� ���� �� ���̾��±׸� ���� ���̾ ã�� 
	// ã������ �� ���̾��� ������Ʈ�� ã��
	// �� ���̾��� ������Ʈ�� ���̾ GameObj�� ������ �հ� GameObj�� list�ϱ� ��ȸ�ϸ鼭 
	// GameObj�� ������Ʈ�� �˰��ճ� �׷� �׻�Ű�� ������


private:
	_bool				m_LandCheckOk = false;

	TERRAIN_DESC		m_tTerrainDesc = {};


	_char		m_iVerticesX[50] = {};				// ���� ���ؽ�����
	_char		m_iVerticesZ[50] = {};				// ���� ���ؽ�����



	_int			m_iModelType = RECT;
	_int			m_iLandMode = CENTERLAND;			// ��ġ�� ��� ����  false�� �߾��� �ƴ϶� �� ������ ���õȴ�


	_bool			m_bBtnCheck[BTNEND] = { false };	// << Map,UI, etc...

	_bool			m_bAdornBtn[ADORN_END] = { false };	// �������� ��������1,2 ����..

	_int			m_iTileNum = 0;						// Tile Image ���� 


	_int			m_iVillageDecoNum = 0;				// �������� ����Obj �̹���
	_int			m_iVillageInteractNum = 0;			// �������� ��ȣ�ۿ�Obj �̹���


	_int			m_iDungeonDecoNum = 0;				// ���� ����Obj �̹���
	_int			m_iDungeon2DecoNum = 0;				// ���� ����Obj �̹���
	_int			m_iDungeonETC_DecoNum = 0;


	_char			m_iSelectObjectType = -1;			// ���� ������ ������Ʈ Ÿ��
	

	
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
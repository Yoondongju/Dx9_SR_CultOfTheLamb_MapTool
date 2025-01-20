#include "stdafx.h"
#include "../Public/Level_MapTool.h"
#include "../Public/Terrain.h"
#include "../Public/Camera.h"

#include "GameInstance.h"
#include "GameObject.h"

#include <fstream>
#include <iostream>
#include <iomanip>

#include <codecvt>

#include "../Public/Player.h"




CLevel_MapTool::CLevel_MapTool(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_MapTool::Initialize()
{
    if (FAILED(Ready_ImGui()))         // IMGUI
        return E_FAIL;


    static CCamera::CAMERA_DESC   Desc{};

    Desc.vEye = _float3(0.f, 9.f, -10.f);
    Desc.vAt = _float3(0.f, 0.f, 0.f);
    Desc.fFovy = D3DXToRadian(55.0f);   // 라디안이네
    Desc.fNear = 0.1f;
    Desc.fFar = 1000.f;
    Desc.fSpeedPerSec = 10.f;
    Desc.fRotationPerSec = D3DXToRadian(90.0f);


    if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_MAPTOOL, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_Camera"), &Desc)))
        return E_FAIL;



    return S_OK;
}

void CLevel_MapTool::Update(_float fTimeDelta)
{
    if (!m_LandCheckOk)
        return;


    POINT point = {};
    if (m_pGameInstance->Get_KeyState(KEY::LBUTTON) == KEY_STATE::TAP)
    {
        _float3 fTerrainPos = {};
        if (m_iLandMode == CENTERLAND)
        {
            fTerrainPos = m_pGameInstance->Get_PickingObject_CenterPos(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"));
        }
        else if (m_iLandMode == JUSTLAND)
        {
            fTerrainPos = m_pGameInstance->Get_PickingObject_JustPos(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"));
        }
        else if (m_iLandMode == BACKGROUNDLAND)
        {
            fTerrainPos = m_pGameInstance->Get_PickingBackGround_JustPos();
        }

        
        if (fTerrainPos.x != 0.f &&
            fTerrainPos.z != 0.f)
        {
            m_vObject_InitPos = fTerrainPos;         
            

            // 분기 들어가야해 데코인지 인터랙트인지
            if (!g_bFocusImGui)
            {
                if (m_iSelectObjectType == DECO)
                    Create_DecoObject();
                else if (m_iSelectObjectType == INTERACT)
                    Create_InteractObject();
                else
                    return;
            }

        }
    }   
    //else if (m_pGameInstance->Get_KeyState(KEY::RBUTTON) == KEY_STATE::TAP)
    //{
    //    _float3 fTerrainPos = {};
    //
    //    if (m_iLandMode == CENTERLAND)
    //    {
    //        fTerrainPos = m_pGameInstance->Get_PickingObject_CenterPos(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"));
    //
    //        if (fTerrainPos.x != 0.f &&
    //            fTerrainPos.z != 0.f)
    //        {
    //            m_vObject_InitPos = fTerrainPos;
    //
    //            CGameObject* pGameObj = m_pGameInstance->IsObjectAtPosition(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"), m_vObject_InitPos);
    //            if (nullptr != pGameObj)
    //            {
    //                m_pGameInstance->Event_DestoryObject(pGameObj);
    //                return;
    //            }
    //        }
    //    }
    //    else if (m_iLandMode == JUSTLAND)
    //    {
    //        fTerrainPos = m_pGameInstance->Get_PickingObject_JustPos(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"));
    //
    //        if (fTerrainPos.x != 0.f &&
    //            fTerrainPos.z != 0.f)
    //        {
    //            m_vObject_InitPos = fTerrainPos;
    //
    //            CGameObject* pGameObj = m_pGameInstance->IsObjectAtPosition_GoodVerdict(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"), m_vObject_InitPos);
    //            if (nullptr != pGameObj)
    //            {
    //                m_pGameInstance->Event_DestoryObject(pGameObj);
    //                return;
    //            }
    //        }
    //    }
    //    else if (m_iLandMode == BACKGROUNDLAND)
    //    {
    //        fTerrainPos = m_pGameInstance->Get_PickingBackGround_JustPos();
    //
    //        if (fTerrainPos.x != 0.f &&
    //            fTerrainPos.z != 0.f)
    //        {
    //            m_vObject_InitPos = fTerrainPos;
    //
    //            CGameObject* pGameObj = m_pGameInstance->IsObjectAtPosition_GoodVerdict(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"), m_vObject_InitPos);
    //            if (nullptr != pGameObj)
    //            {
    //                m_pGameInstance->Event_DestoryObject(pGameObj);
    //                return;
    //            }
    //        }
    //    }
    //
    //}
}


void CLevel_MapTool::Create_PreViewObject()
{    
    CLandObject::LANDOBJECT_DESC	Desc = {};

    Desc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_Transform")));
    Desc.pTerrainVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_VIBuffer")));
    Desc.vInitPos = m_vObject_InitPos;
    Desc.vInitScale = m_vObject_Scaling;

    Desc.fRotationX = m_vObject_RotationX_Degree;
    Desc.fRotationY = m_vObject_RotationY_Degree;
    Desc.fRotationZ = m_vObject_RotationZ_Degree;

    Desc.strLayerName = TEXT("Layer_Preview_Object");
    
  
    if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_MAPTOOL, TEXT("Layer_Preview_Object"), TEXT("Prototype_GameObject_DecoObject"),&Desc)))
        assert(nullptr);

    if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_MAPTOOL, TEXT("Layer_Preview_Object"), TEXT("Prototype_GameObject_InteractObject"),&Desc)))
        assert(nullptr);

}

void CLevel_MapTool::Create_DecoObject()
{
    if (m_iLandMode == CENTERLAND && nullptr != m_pGameInstance->IsObjectAtPosition(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"), m_vObject_InitPos))
    {
        MSG_BOX(TEXT("해당 위치에 이미 OBJECT가 존재합니다\n다른곳에 배치하세요."));
        return;
    }
    else if (m_iLandMode == CENTERLAND)
    {
        m_vObject_InitPos.z += 0.01f;
    }

    CLandObject::LANDOBJECT_DESC	Desc = {};

    Desc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_Transform")));
    Desc.pTerrainVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_VIBuffer")));
    Desc.vInitPos = m_vObject_InitPos;
    Desc.vInitScale = m_vObject_Scaling;

    Desc.fRotationX = m_vObject_RotationX_Degree;
    Desc.fRotationY = m_vObject_RotationY_Degree;
    Desc.fRotationZ = m_vObject_RotationZ_Degree;

    Desc.iPreModelType = m_iModelType;
    Desc.iPreAdornType = Get_AdornType();
    Desc.iPreLandMode = m_iLandMode;

    Desc.strLayerName = TEXT("Layer_BackGround_Object");

    switch (Desc.iPreAdornType)
    {
    case CLevel_MapTool::VILLAGE:
        Desc.iInitBindTexNum = GetDecoNum();
        break;
    case CLevel_MapTool::STAGE1:
        Desc.iInitBindTexNum = GetDungeonDecoNum();
        break;
    case CLevel_MapTool::STAGE2:
        Desc.iInitBindTexNum = GetDungeon2DecoNum();
        break;
    case CLevel_MapTool::STAGE3:
        Desc.iInitBindTexNum = GetDungeonETCDecoNum();
        break;
    default:
        break;
    }

    
   
    if (m_iLandMode == BACKGROUNDLAND)
        Desc.vInitPos.y = 0.5f * Desc.vInitScale.y;


   
    if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"), TEXT("Prototype_GameObject_DecoObject"),&Desc)))
        assert(nullptr);
}

void CLevel_MapTool::Create_InteractObject()
{
    if (m_iLandMode == CENTERLAND && nullptr != m_pGameInstance->IsObjectAtPosition(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"), m_vObject_InitPos))
    {
        MSG_BOX(TEXT("해당 위치에 이미 OBJECT가 존재합니다\n다른곳에 배치하세요."));
        return;
    }
    else if (m_iLandMode == CENTERLAND)
    {
        m_vObject_InitPos.z += 0.01f;
    }

    CLandObject::LANDOBJECT_DESC	Desc = {};

    Desc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_Transform")));
    Desc.pTerrainVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_VIBuffer")));
    Desc.vInitPos = m_vObject_InitPos;
    Desc.vInitScale = m_vObject_Scaling;

    Desc.fRotationX = m_vObject_RotationX_Degree;
    Desc.fRotationY = m_vObject_RotationY_Degree;
    Desc.fRotationZ = m_vObject_RotationZ_Degree;

    Desc.iPreModelType = m_iModelType;
    Desc.iPreAdornType = Get_AdornType();
    Desc.iInitBindTexNum = GetInteractNum();
    Desc.iPreLandMode = m_iLandMode;

    Desc.strLayerName = TEXT("Layer_BackGround_Object");

    if (m_iLandMode == BACKGROUNDLAND)
        Desc.vInitPos.y = 0.5f * Desc.vInitScale.y;


    if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"), TEXT("Prototype_GameObject_InteractObject"), &Desc)))
        assert(nullptr);
}


HRESULT CLevel_MapTool::Render()
{
    SetWindowText(g_hWnd, TEXT("맵툴_레벨입니다."));

    m_iSelectObjectType = -1; // 기본 세팅 -1
    ImGui_Render();
    
    return S_OK;
}


void CLevel_MapTool::ImGui_Render()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(450, 600));
    ImGui::Begin("First_ImGui");

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))      // ImGui 창을 누르고 있지 않을때만
    {
        g_bFocusImGui = true;
    }
    else
        g_bFocusImGui = false;



    if (ImGui::Button(("Map")))
    {
        if (!m_bBtnCheck[MAPBTN])
            m_bBtnCheck[MAPBTN] = true;
        for (_uint i = 0; i < BTNEND; i++)
        {
            if (i == MAPBTN)
                continue;
            m_bBtnCheck[i] = false;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button(("UI")))
    {
        if (!m_bBtnCheck[UIBTN])
            m_bBtnCheck[UIBTN] = true;
        for (_uint i = 0; i < BTNEND; i++)
        {
            if (i == UIBTN)
                continue;
            m_bBtnCheck[i] = false;
        }
    }


    if (m_bBtnCheck[MAPBTN])
        Open_MapDialog();

    if (m_bBtnCheck[UIBTN])
        Open_UIDialog();



    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}


void CLevel_MapTool::Open_MapDialog()
{
    if (ImGui::Button(u8"지형 만들기"))
    {      
        _uint iX = atoi(m_iVerticesX) ;
        _uint iZ = atoi(m_iVerticesZ) ;
        if (iX <= 1 || iZ <= 1)
        {
            MSG_BOX(L"가로,세로가 1개이상이여야함");
            return;
        }         
        else
        {
            /* For. Prototype_Component_VIBuffer_Terrain */
            if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_Terrain"),
                CVIBuffer_Terrain::Create(m_pGraphic_Device, iX, iZ))))
                assert(nullptr);

            if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
                CTerrain::Create(m_pGraphic_Device))))
                assert(nullptr);
        
            if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Prototype_GameObject_Terrain"),&m_tTerrainDesc)))
                assert(nullptr);
        }

        
        static bool bCheck = false;
        if (!bCheck)
        {
            CLandObject::LANDOBJECT_DESC	LandDesc = {};

            LandDesc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_Transform")));
            LandDesc.pTerrainVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_VIBuffer")));

            if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player"), &LandDesc)))
                MSG_BOX(TEXT("Player 생성 안대써요"));

            dynamic_cast<CPlayer*>(m_pGameInstance->Find_Player())->Set_InitState(CPlayer::IDLE);

            Create_PreViewObject();     // 프리뷰 오브젝 사진            터레인 만들어지면 생성해 ㅂ

            bCheck = true;
        }
   
    }


    ImGui::InputText(u8"가로버텍스 개수입력", m_iVerticesX , IM_ARRAYSIZE(m_iVerticesX));
    ImGui::InputText(u8"세로버텍스 개수입력", m_iVerticesZ , IM_ARRAYSIZE(m_iVerticesZ));



    if (ImGui::Button(u8"Village 꾸미기"))
    {
        if (!m_bAdornBtn[VILLAGE])
            m_bAdornBtn[VILLAGE] = true;
        for (_uint i = 0; i < ADORN_END; i++)
        {
            if (i == VILLAGE)
                continue;
            m_bAdornBtn[i] = false;
        }
       
    }
    ImGui::SameLine();
    if (ImGui::Button(u8"던전1 꾸미기"))
    {
        if (!m_bAdornBtn[STAGE1])
            m_bAdornBtn[STAGE1] = true;
        for (_uint i = 0; i < ADORN_END; i++)
        {
            if (i == STAGE1)
                continue;
            m_bAdornBtn[i] = false;
        }   
    }
    ImGui::SameLine();
    if (ImGui::Button(u8"던전2 꾸미기"))
    {
        if (!m_bAdornBtn[STAGE2])
            m_bAdornBtn[STAGE2] = true;
        for (_uint i = 0; i < ADORN_END; i++)
        {
            if (i == STAGE2)
                continue;
            m_bAdornBtn[i] = false;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button(u8"문,타로,기슭 꾸미기"))
    {
        if (!m_bAdornBtn[STAGE3])
            m_bAdornBtn[STAGE3] = true;
        for (_uint i = 0; i < ADORN_END; i++)
        {
            if (i == STAGE3)
                continue;
            m_bAdornBtn[i] = false;
        }
    }


    
    ImGui::Dummy(ImVec2(0.0f, 20.0f)); // 높이가 10인 빈 공간 추가
    ImGui::Checkbox("Object Landing State", &m_LandCheckOk);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0, 1, 0, 1), u8"<< 카메라 이동할때 이거 해제하셈 ㅎ");
    ImGui::TextColored(ImVec4(0, 1, 0, 1), u8"<< Object 배치,삭제하고싶으면 위에체크해");
    ImGui::TextColored(ImVec4(0, 1, 0, 1), u8"<< 배치: LBUTTON / 삭제: RBUTTON");

    ImGui::TextColored(ImVec4(0, 1, 0, 1), u8"<< RECT종류 꼭 확인하고, Land모드확인해");
    ImGui::TextColored(ImVec4(0, 1, 0, 1), u8"<< 배치및삭제는 Land모드마다 달라져");
    ImGui::TextColored(ImVec4(1, 0, 0, 1), u8"<< 맵만들고 꼭 아래 Save로 저장해!!");
    ImGui::TextColored(ImVec4(1, 0, 0, 1), u8"<< 주의사항: Map_Data에 덮어쓰기여서 저장신중하게하삼");
    ImGui::TextColored(ImVec4(1, 0, 0, 1), u8"<< 경로: MapTool/Client/Bin/Map_Data");

    if (m_bAdornBtn[VILLAGE])
        Open_About_Village();
   
    if(m_bAdornBtn[STAGE1])
        Open_About_Stage1();

    if (m_bAdornBtn[STAGE2])
        Open_About_Stage2();

    if (m_bAdornBtn[STAGE3])
        Open_About_Stage3();

  


    ImGui::Dummy(ImVec2(0.0f, 200.0f)); // 높이가 10인 빈 공간 추가

    if (ImGui::Button(u8"WIREFRAME 으로보기"))
    {
        m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    }
    ImGui::SameLine();
    if (ImGui::Button(u8"TEXTURE 로보기"))
    {
        m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    }


    if (ImGui::Button(u8"Save"))
        Save_Map();
    ImGui::SameLine();

    if (ImGui::Button(u8"Load"))
        Load_Map();
}

void CLevel_MapTool::Open_About_Village()
{
    if (ImGui::TreeNode("All_Terrain_Tile"))
    {
        Open_TileList();

        ImGui::TreePop();
    }
    else if (ImGui::TreeNode("Village_Deco"))
    {
        m_iSelectObjectType = DECO;
        Open_VillageDecoList();

        ImGui::TreePop();
    }
    else if (ImGui::TreeNode("Village_Interact"))
    {     
        m_iSelectObjectType = INTERACT;
        Open_VillageInteractList();

        ImGui::TreePop();
    }
    else if (ImGui::TreeNode("Village_Sanctuary"))
    {
        m_iSelectObjectType = DECO;
        Open_VillageSanctuary();

        ImGui::TreePop();
    }

}

void CLevel_MapTool::Open_About_Stage1()
{
    if (ImGui::TreeNode("Dungeon_Deco"))
    {
        m_iSelectObjectType = DECO;
        Open_DungeonDecoList();

        ImGui::TreePop();
    }

}

void CLevel_MapTool::Open_About_Stage2()
{
    if (ImGui::TreeNode("Dungeon2_Deco"))
    {
        m_iSelectObjectType = DECO;
        Open_Dungeon2DecoList();

        ImGui::TreePop();
    }   
}

void CLevel_MapTool::Open_About_Stage3()
{
    if (ImGui::TreeNode("ETC_Deco"))
    {
        m_iSelectObjectType = DECO;
        Open_Dungeon_ETC_DecoList();

        ImGui::TreePop();
    }
}

void CLevel_MapTool::Open_TileList()
{
    for (size_t i = 0; i < 16; i++)
    {
        string strDecoName = "";
        strDecoName = "Tile_" + to_string(i);
        ImGui::RadioButton(strDecoName.c_str(), &m_iTileNum, i);
    }
}



void CLevel_MapTool::Open_VillageDecoList()
{
    ImGui::SliderFloat("Scaling X", &m_vObject_Scaling.x, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Y", &m_vObject_Scaling.y, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Z", &m_vObject_Scaling.z, 1.f, 10.f);

    ImGui::SliderFloat("Rotation X", &m_vObject_RotationX_Degree, 0.f, 90.f);
   
    // 2개만 
    ImGui::RadioButton("Vertical Rect", &m_iModelType, RECT);
    ImGui::SameLine();
    ImGui::RadioButton("Horizontal Rect", &m_iModelType, GRIDRECT);


    ImGui::RadioButton("CenterLand On", &m_iLandMode, CENTERLAND);
    ImGui::SameLine();
    ImGui::RadioButton("JustLand On", &m_iLandMode, JUSTLAND);
    ImGui::SameLine();
    ImGui::RadioButton("BackGroundLand On", &m_iLandMode, BACKGROUNDLAND);

    if (ImGui::TreeNode(u8"잡다구리"))
    {
        for (size_t i = 0; i < 42; i++)
        {
            string strDecoName = "";
            strDecoName = "Village_Deco_" + to_string(i);

            ImGui::RadioButton(strDecoName.c_str(), &m_iVillageDecoNum, i);
        }
        ImGui::TreePop();
    }




    // 내가 고른 이미지 사진을 보기위해 새로운 창 생성하기
    ImGui::SetNextWindowSize(ImVec2(200, 200));
    ImGui::Begin("Deco Image");
    ImGui::Text("Picked Image");

    CTexture* pTex = dynamic_cast<CTexture*>(m_pGameInstance->Get_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Village_Deco")));
    ImGui::Image(reinterpret_cast<ImTextureID>(pTex->Get_Texture(m_iVillageDecoNum)), ImVec2(100, 100)); // 두번째인자 텍스쳐의 크기
    ImGui::End();
}

void CLevel_MapTool::Open_VillageInteractList()
{
    ImGui::SliderFloat("Scaling X", &m_vObject_Scaling.x, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Y", &m_vObject_Scaling.y, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Z", &m_vObject_Scaling.z, 1.f, 10.f);


    ImGui::SliderFloat("Rotation X", &m_vObject_RotationX_Degree, 0.f, 90.f);

    // 2개만 
    ImGui::RadioButton("Vertical Rect", &m_iModelType, RECT);
    ImGui::SameLine();
    ImGui::RadioButton("Horizontal Rect", &m_iModelType, GRIDRECT);

    ImGui::RadioButton("CenterLand On", &m_iLandMode, CENTERLAND);
    ImGui::SameLine();
    ImGui::RadioButton("JustLadn On", &m_iLandMode, JUSTLAND);
    ImGui::SameLine();
    ImGui::RadioButton("BackGroundLand On", &m_iLandMode, BACKGROUNDLAND);


    if (ImGui::TreeNode(u8"돌"))
    {
     
        for (size_t i = 0; i < 3; i++)
        {
            string strDecoName = "";
            strDecoName = "RockRubble_" + to_string(i);
            ImGui::RadioButton(strDecoName.c_str(), &m_iVillageInteractNum, i);
        }
        ImGui::TreePop();
    }
    else if (ImGui::TreeNode(u8"신사"))
    {     
        for (size_t i = 0; i < 2; i++)
        {
            string strDecoName = "";
            strDecoName = "Shrine_" + to_string(i);
            ImGui::RadioButton(strDecoName.c_str(), &m_iVillageInteractNum, 3 + i);
        }
        ImGui::TreePop();
    }
    else if (ImGui::TreeNode(u8"나무"))
    {   
        for (size_t i = 0; i < 4; i++)
        {
            string strDecoName = "";
            strDecoName = "WoodRubble_" + to_string(i);
            ImGui::RadioButton(strDecoName.c_str(), &m_iVillageInteractNum, 3 + 2 + i);
        }
        ImGui::TreePop();
    }
    else if (ImGui::TreeNode(u8"잡초"))
    {
        for (size_t i = 0; i < 46; i++)
        {
            string strDecoName = "";
            strDecoName = "WeedShrub_" + to_string(i);
            ImGui::RadioButton(strDecoName.c_str(), &m_iVillageInteractNum, 3+ 2+ 4+i);
        }
        ImGui::TreePop();
    }

    // 내가 고른 이미지 사진을 보기위해 새로운 창 생성하기
    ImGui::SetNextWindowSize(ImVec2(200, 200));
    ImGui::Begin("Interact_Image");
    ImGui::Text("Picked Image");

    CTexture* pTex = dynamic_cast<CTexture*>(m_pGameInstance->Get_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Village_Interact")));
    ImGui::Image(reinterpret_cast<ImTextureID>(pTex->Get_Texture(m_iVillageInteractNum)), ImVec2(100, 100)); // 두번째인자 텍스쳐의 크기
    ImGui::End();

}

void CLevel_MapTool::Open_VillageSanctuary()
{
    ImGui::SliderFloat("Scaling X", &m_vObject_Scaling.x, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Y", &m_vObject_Scaling.y, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Z", &m_vObject_Scaling.z, 1.f, 10.f);

    ImGui::SliderFloat("Rotation X", &m_vObject_RotationX_Degree, 0.f, 90.f);

    // 2개만 
    ImGui::RadioButton("Vertical Rect", &m_iModelType, RECT);
    ImGui::SameLine();
    ImGui::RadioButton("Horizontal Rect", &m_iModelType, GRIDRECT);


    ImGui::RadioButton("CenterLand On", &m_iLandMode, CENTERLAND);
    ImGui::SameLine();
    ImGui::RadioButton("JustLand On", &m_iLandMode, JUSTLAND);
    ImGui::SameLine();
    ImGui::RadioButton("BackGroundLand On", &m_iLandMode, BACKGROUNDLAND);


    if (ImGui::TreeNode(u8"기도하는 맵"))
    {
        for (size_t i = 41; i < 78; i++)
        {
            string strDecoName = "";
            strDecoName = "Village_Sanctuary_" + to_string(i);

            ImGui::RadioButton(strDecoName.c_str(), &m_iVillageDecoNum, i);
        }
        ImGui::TreePop();
    }


    // 내가 고른 이미지 사진을 보기위해 새로운 창 생성하기
    ImGui::SetNextWindowSize(ImVec2(200, 200));
    ImGui::Begin("Deco Image");
    ImGui::Text("Picked Image");

    CTexture* pTex = dynamic_cast<CTexture*>(m_pGameInstance->Get_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Village_Deco")));
    ImGui::Image(reinterpret_cast<ImTextureID>(pTex->Get_Texture(m_iVillageDecoNum)), ImVec2(100, 100)); // 두번째인자 텍스쳐의 크기
    ImGui::End();

}

void CLevel_MapTool::Open_DungeonDecoList()
{
    ImGui::SliderFloat("Scaling X", &m_vObject_Scaling.x, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Y", &m_vObject_Scaling.y, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Z", &m_vObject_Scaling.z, 1.f, 10.f);

    ImGui::SliderFloat("Rotation X", &m_vObject_RotationX_Degree, 0.f, 90.f);

    // 2개만 
    ImGui::RadioButton("Vertical Rect", &m_iModelType, RECT);
    ImGui::SameLine();
    ImGui::RadioButton("Horizontal Rect", &m_iModelType, GRIDRECT);


    ImGui::RadioButton("CenterLand On", &m_iLandMode, CENTERLAND);
    ImGui::SameLine();
    ImGui::RadioButton("JustLand On", &m_iLandMode, JUSTLAND);
    ImGui::SameLine();
    ImGui::RadioButton("BackGroundLand On", &m_iLandMode, BACKGROUNDLAND);


    if (ImGui::TreeNode(u8"Dungeon_Deco1"))
    {
        for (size_t i = 0; i < 12; i++)
        {
            string strDecoName = "";
            strDecoName = "Dungeon_Deco1_" + to_string(i);

            ImGui::RadioButton(strDecoName.c_str(), &m_iDungeonDecoNum, i);
        }
        ImGui::TreePop();
    }



    // 내가 고른 이미지 사진을 보기위해 새로운 창 생성하기
    ImGui::SetNextWindowSize(ImVec2(200, 200));
    ImGui::Begin("Deco Image");
    ImGui::Text("Picked Image");

    CTexture* pTex = dynamic_cast<CTexture*>(m_pGameInstance->Get_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon_Deco")));
    ImGui::Image(reinterpret_cast<ImTextureID>(pTex->Get_Texture(m_iDungeonDecoNum)), ImVec2(100, 100)); // 두번째인자 텍스쳐의 크기
    ImGui::End();
}

void CLevel_MapTool::Open_Dungeon2DecoList()
{
    ImGui::SliderFloat("Scaling X", &m_vObject_Scaling.x, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Y", &m_vObject_Scaling.y, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Z", &m_vObject_Scaling.z, 1.f, 10.f);

    ImGui::SliderFloat("Rotation X", &m_vObject_RotationX_Degree, 0.f, 90.f);

    // 2개만 
    ImGui::RadioButton("Vertical Rect", &m_iModelType, RECT);
    ImGui::SameLine();
    ImGui::RadioButton("Horizontal Rect", &m_iModelType, GRIDRECT);


    ImGui::RadioButton("CenterLand On", &m_iLandMode, CENTERLAND);
    ImGui::SameLine();
    ImGui::RadioButton("JustLand On", &m_iLandMode, JUSTLAND);
    ImGui::SameLine();
    ImGui::RadioButton("BackGroundLand On", &m_iLandMode, BACKGROUNDLAND);


    if (ImGui::TreeNode(u8"Dungeon_Deco2"))
    {
        for (size_t i = 0; i < 134; i++)
        {
            string strDecoName = "";
            strDecoName = "Dungeon_Deco2_" + to_string(i);

            ImGui::RadioButton(strDecoName.c_str(), &m_iDungeon2DecoNum, i);
        }
        ImGui::TreePop();
    }


    // 내가 고른 이미지 사진을 보기위해 새로운 창 생성하기
    ImGui::SetNextWindowSize(ImVec2(200, 200));
    ImGui::Begin("Deco Image");
    ImGui::Text("Picked Image");

    CTexture* pTex = dynamic_cast<CTexture*>(m_pGameInstance->Get_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon2_Deco")));
    ImGui::Image(reinterpret_cast<ImTextureID>(pTex->Get_Texture(m_iDungeon2DecoNum)), ImVec2(100, 100)); // 두번째인자 텍스쳐의 크기
    ImGui::End();
}

void CLevel_MapTool::Open_Dungeon_ETC_DecoList()
{
    ImGui::SliderFloat("Scaling X", &m_vObject_Scaling.x, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Y", &m_vObject_Scaling.y, 1.f, 10.f);
    ImGui::SliderFloat("Scaling Z", &m_vObject_Scaling.z, 1.f, 10.f);

    ImGui::SliderFloat("Rotation X", &m_vObject_RotationX_Degree, 0.f, 90.f);

    // 2개만 
    ImGui::RadioButton("Vertical Rect", &m_iModelType, RECT);
    ImGui::SameLine();
    ImGui::RadioButton("Horizontal Rect", &m_iModelType, GRIDRECT);


    ImGui::RadioButton("CenterLand On", &m_iLandMode, CENTERLAND);
    ImGui::SameLine();
    ImGui::RadioButton("JustLand On", &m_iLandMode, JUSTLAND);
    ImGui::SameLine();
    ImGui::RadioButton("BackGroundLand On", &m_iLandMode, BACKGROUNDLAND);


    if (ImGui::TreeNode(u8"문"))
    {
        for (size_t i = 0; i < 12; i++)
        {
            string strDecoName = "";
            strDecoName = "Door_" + to_string(i);

            ImGui::RadioButton(strDecoName.c_str(), &m_iDungeonETC_DecoNum, i);
        }
        ImGui::TreePop();
    }

    else if (ImGui::TreeNode(u8"타로"))
    {
        for (size_t i = 0; i < 23; i++)
        {
            string strDecoName = "";
            strDecoName = "Taro_" + to_string(i);
            ImGui::RadioButton(strDecoName.c_str(), &m_iDungeonETC_DecoNum, 12 + i);
        }
        ImGui::TreePop();
    }
    else if (ImGui::TreeNode(u8"기슭"))
    {
        for (size_t i = 0; i < 6; i++)
        {
            string strDecoName = "";
            strDecoName = "BigRoot_" + to_string(i);
            ImGui::RadioButton(strDecoName.c_str(), &m_iDungeonETC_DecoNum, 12 + 23 + i);
        }
        ImGui::TreePop();
    }
  


    // 내가 고른 이미지 사진을 보기위해 새로운 창 생성하기
    ImGui::SetNextWindowSize(ImVec2(200, 200));
    ImGui::Begin("Deco Image");
    ImGui::Text("Picked Image");

    CTexture* pTex = dynamic_cast<CTexture*>(m_pGameInstance->Get_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon_ETC_Deco")));
    ImGui::Image(reinterpret_cast<ImTextureID>(pTex->Get_Texture(m_iDungeonETC_DecoNum)), ImVec2(100, 100)); // 두번째인자 텍스쳐의 크기
    ImGui::End();
}


void CLevel_MapTool::Open_UIDialog()
{
    if (ImGui::Button(u8"UI TEST"))
    {

    }
}

void CLevel_MapTool::Set_LandMode_Center()
{
}

void CLevel_MapTool::Set_LandMode_Just()
{
}



void CLevel_MapTool::Save_Map()
{
    ofstream fout;

    fout.open("../Bin/Map_Data.txt");
    if (!fout.is_open())    // 파일 열었다면
    {
        MSG_BOX(TEXT("파일 쓰기를 실패했어요.."));
        return;
    }


    fout << m_iVerticesX << "\n";
    fout << m_iVerticesZ << "\n";
    fout << m_iTileNum << "\n";




   
    list<CGameObject*>& GameObjects = m_pGameInstance->Get_GameObjects(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"));

    _uint iObjectCount  = GameObjects.size();      

    fout << iObjectCount << "\n";                    // 오브젝트의 갯수
  
    for (auto& pGameObj : GameObjects)
    {
       _wstring strMyPrototypeName = pGameObj->Get_PrototypeName();
       _wstring wstrMyLayerName = pGameObj->Get_LayerName();
   
        int iBindNum = pGameObj->Get_BindTexNum();  

        CTransform* pTransform = dynamic_cast<CTransform*>(pGameObj->Find_Component(TEXT("Com_Transform")));

        _float4x4 fWorldMat = pTransform->Get_WorldMatrix();



        string strName(strMyPrototypeName.begin(), strMyPrototypeName.end());
        string strMyLayerName(wstrMyLayerName.begin(), wstrMyLayerName.end());


        fout.write(strName.c_str(), strName.size()) << "\n";     // 오브젝트 프로토타입 이름
        fout.write(strMyLayerName.c_str(), strMyLayerName.size()) << "\n";


        if (strName == "Prototype_GameObject_InteractObject")
        {
            fout << dynamic_cast<CInteractObject*>(pGameObj)->Get_MyType() << "\n";
        }
        else
            fout << CInteractObject::INTERACT_TYPE::INTERACTTYPE_END << "\n";



        fout << ((CLandObject*)pGameObj)->m_iPreModelType << "\n";
        fout << ((CLandObject*)pGameObj)->m_iPreAdornType << "\n";
        fout << ((CLandObject*)pGameObj)->m_iPreLandType << "\n";


        fout << iBindNum << "\n";                                // 오브젝트 바인드넘버

        fout << fixed << setprecision(8);
        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {            
                fout << fWorldMat.m[i][j] << "\n";                   // 오브젝트 월행
            }
        } 
    }
    



    fout.close();
    MSG_BOX(TEXT("파일 쓰기를 성공했습니다.."));
}




std::istream& operator>>(std::istream& is, _float4x4& mat) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            is >> mat.m[i][j];
        }
    }
    return is;
}

void CLevel_MapTool::Load_Map()
{
    ifstream fin;

    fin.open("../Bin/Map_Data.txt");
    if (!fin.is_open())    // 파일 열었다면
    {
        MSG_BOX(TEXT("파일 읽기를 실패했어요.."));
        return;
    }

    fin.getline(m_iVerticesX, 50);
    fin.getline(m_iVerticesZ, 50);
    fin >> m_iTileNum;
    
    Load_Terrain();

    //////////////////////////////// ^ 터레인 ^ //////////////////////////////////////////////


    _uint iObjectCount = 0;
    fin >> iObjectCount;
    
    fin >> fixed >> setprecision(8);

    for (_uint i = 0; i < iObjectCount; i++)
    {
        string szName; 
        string szLayerName;
    
        _uint eInteractType = { CInteractObject::INTERACT_TYPE::INTERACTTYPE_END };

        int iBindNum;
    
        _float4x4 fWorldMat;
        D3DXMatrixIdentity(&fWorldMat);
           
        fin >> szName;
        fin >> szLayerName;

        if (szName == "Prototype_Component_Texture_Village_Interact")
        {
            fin >> eInteractType;
        }
        else
            fin >> eInteractType;


        _uint iPreModelType = {};
        _uint iPreAdornType = {};
        _uint iPreLandType = {};

        fin >> iPreModelType;
        fin >> iPreAdornType;
        fin >> iPreLandType;

        fin >> iBindNum;  
        fin >> fWorldMat;

        if (iPreModelType == 1)
            int a = 1;
      

        Load_Object(szName, szLayerName ,eInteractType, iPreModelType, iPreAdornType , iPreLandType , iBindNum , fWorldMat);
    }




    CLandObject::LANDOBJECT_DESC	LandDesc = {};

    LandDesc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_Transform")));
    LandDesc.pTerrainVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_VIBuffer")));

    if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player"), &LandDesc)))
        MSG_BOX(TEXT("Player 생성 안대써요"));

    dynamic_cast<CPlayer*>(m_pGameInstance->Find_Player())->Set_InitState(CPlayer::IDLE);

    Create_PreViewObject();     // 프리뷰 오브젝 사진            터레인 만들어지면 생성해 ㅂ



    fin.close();
    MSG_BOX(TEXT("파일 읽기를 성공했습니다.."));
}

void CLevel_MapTool::Load_Terrain()
{
    _uint iX = atoi(m_iVerticesX);
    _uint iZ = atoi(m_iVerticesZ);
    if (iX <= 1 || iZ <= 1)
        MSG_BOX(L"가로,세로가 1개이상이여야함");
    else
    {
        /* For. Prototype_Component_VIBuffer_Terrain */
        if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_Terrain"),
            CVIBuffer_Terrain::Create(m_pGraphic_Device, iX, iZ))))
            assert(nullptr);

        if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
            CTerrain::Create(m_pGraphic_Device))))
            assert(nullptr);



        if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Prototype_GameObject_Terrain"), &m_tTerrainDesc)))
            assert(nullptr);

    }
}

void CLevel_MapTool::Load_Object(const string& szName, const string& szLayerName , _uint eInteractType , _uint& iPreModelType, _uint& iPreAdornType , _uint& iPreLandType ,const int& iBindNum, const _float4x4& fWorldMat)
{
    CLandObject::LANDOBJECT_DESC	Desc = {};

    Desc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_Transform")));
    Desc.pTerrainVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Find_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Terrain"), TEXT("Com_VIBuffer")));
    Desc.vInitPos = *((_float3*)&fWorldMat.m[3][0]);

    Desc.vInitScale = _float3(D3DXVec3Length((_float3*)&fWorldMat.m[0][0]),
                            D3DXVec3Length((_float3*)&fWorldMat.m[1][0]),
                            D3DXVec3Length((_float3*)&fWorldMat.m[2][0]));

    Desc.iPreModelType = iPreModelType;
    Desc.iPreAdornType = iPreAdornType;
    Desc.iPreLandMode = iPreLandType;

    Desc.iInitBindTexNum = iBindNum;


    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    Desc.strLayerName = converter.from_bytes(szLayerName);

  

    if (szName == "Prototype_GameObject_DecoObject")
    {
        if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"), TEXT("Prototype_GameObject_DecoObject"), &Desc)))
            assert(nullptr);
    }
    else if (szName == "Prototype_GameObject_InteractObject")
    {          
        if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_MAPTOOL, TEXT("Layer_BackGround_Object"), TEXT("Prototype_GameObject_InteractObject"), &Desc)))
            assert(nullptr);
    }





}


HRESULT CLevel_MapTool::Ready_ImGui()
{
    // 생성부분
    ImGui::CreateContext();          // Create 무조건 먼저 해줘야함 , 얘가 순서가 먼저여야함
    ImGui::StyleColorsDark();        // Style -> 색상

    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig font_cfg;
    font_cfg.OversampleH = 2;
    font_cfg.OversampleV = 2;
    font_cfg.PixelSnapH = true;

    // 폰트 파일 경로를 적절하게 변경하세요
    io.Fonts->AddFontFromFileTTF("C://Windows//Fonts//gulim.ttc", 14.0f, nullptr, io.Fonts->GetGlyphRangesKorean());


    if (!ImGui_ImplWin32_Init(g_hWnd))  // win32 관련된 초기화
    {
        MSG_BOX(L"IMGUI WIN32 초기화 실패");
        return E_FAIL;
    }

    if (!ImGui_ImplDX9_Init(m_pGraphic_Device))  // Dx 관련된 초기화
    {
        MSG_BOX(L"IMGUI DX9 초기화 실패");
        return E_FAIL;
    }

    return S_OK;
}



CLevel_MapTool* CLevel_MapTool::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CLevel_MapTool* pInstance = new CLevel_MapTool(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed to Created : CLevel_MapTool"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_MapTool::Free()
{
    // 해제
    ImGui_ImplDX9_Shutdown();  // Dx 해제 
    ImGui_ImplWin32_Shutdown(); // Win32 해제 순서상관 X

    ImGui::DestroyContext(); // 무조건 마지막에 넣기
    // ImGui 먼저 삭제해야해요~~~


    __super::Free();
}

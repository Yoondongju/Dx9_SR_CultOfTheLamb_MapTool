#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "../Public/Level_MapTool.h"
#include "../Public/Level_Logo.h"
#include "../Public/Level_GamePlay.h"
#include "../Public/BackGround.h"
#include "../Public/Terrain.h"
#include "../Public/Camera.h"
#include "../Public/DecoObejct.h"
#include "../Public/InteractObject.h"

#include "GameInstance.h"

#include "../Public/Player.h"




CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel{ pGraphic_Device }
{
}

/* unsigned (__stdcall* _beginthreadex_proc_type)(void*); */
unsigned int APIENTRY LoadingMain(void* pArg)
{
    CLevel_Loading* pLoading = static_cast<CLevel_Loading*>(pArg);

    if (FAILED(pLoading->Loading()))
        return 1;

    return 0;
}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID)
{
    m_eNextLevelID = eNextLevelID;
    InitializeCriticalSection(&m_CriticalSection);


    m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
    if (0 == m_hThread)
        return E_FAIL;


    return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
    if (true == m_isFinished)                  // ex) Loading -> GamePlay 가기이전에 GamePlay에서 필요한 리소스 할당안햇으면 못가용 
    {
        if (GetKeyState(VK_SPACE) & 0x8000)
        {
            HRESULT         hr = {};

            switch (m_eNextLevelID)
            {
            case LEVEL_MAPTOOL:
                hr = m_pGameInstance->Change_Level(LEVEL_MAPTOOL, CLevel_MapTool::Create(m_pGraphic_Device));
                break;

            case LEVEL_LOGO:
                hr = m_pGameInstance->Change_Level(LEVEL_LOGO, CLevel_Logo::Create(m_pGraphic_Device));
                break;
            case LEVEL_GAMEPLAY:
                hr = m_pGameInstance->Change_Level(LEVEL_GAMEPLAY, CLevel_GamePlay::Create(m_pGraphic_Device));
                break;
            }

            if (FAILED(hr))
                return;
        }
    }
}

HRESULT CLevel_Loading::Render()
{
    SetWindowText(g_hWnd, m_szLoadingText);



    return S_OK;
}

HRESULT CLevel_Loading::Loading()
{
    EnterCriticalSection(&m_CriticalSection);

    HRESULT      hr = {};

    switch (m_eNextLevelID)
    {
    case LEVEL_MAPTOOL:
        hr = Loading_Resource_For_MapTool_Level();
        break;

    case LEVEL_LOGO:
        hr = Loading_Resource_For_LogoLevel();
        break;
    case LEVEL_GAMEPLAY:
        hr = Loading_Resource_For_GamePlayLevel();
        break;
    }

    if (FAILED(hr))
        return E_FAIL;

    LeaveCriticalSection(&m_CriticalSection);

    return S_OK;
}

HRESULT CLevel_Loading::Loading_Resource_For_LogoLevel()
{
    lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
    /* For. Prototype_Component_Texture_BackGround */
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
        return E_FAIL;

    lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
    /* For. Prototype_Component_VIBuffer_Rect */
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
        CVIBuffer_Rect::Create(m_pGraphic_Device))))
        return E_FAIL;

    lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

    /* For. Prototype_GameObject_BackGround */
    lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩중입니다."));
    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
        CBackGround::Create(m_pGraphic_Device))))
        return E_FAIL;

    lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
    m_isFinished = true;

    return S_OK;
}

HRESULT CLevel_Loading::Loading_Resource_For_GamePlayLevel()
{

    lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
    /* For. Prototype_Component_Texture_Terrrain */
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Resources/Textures/Terrain/yeoung.jpg"), 1))))
        return E_FAIL;


    lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
    /* For. Prototype_Component_VIBuffer_Terrain */
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
        CVIBuffer_Terrain::Create(m_pGraphic_Device, 50, 50))))
        return E_FAIL;




    lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


    lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩중입니다."));
    /* For. Prototype_GameObject_Terrain */
    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
        CTerrain::Create(m_pGraphic_Device))))
        return E_FAIL;

    /* For. Prototype_GameObject_Terrain */
    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
        CCamera::Create(m_pGraphic_Device))))
        return E_FAIL;


    lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
    m_isFinished = true;

    return S_OK;
}

HRESULT CLevel_Loading::Loading_Resource_For_MapTool_Level()
{

    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Idle"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Cult_Resources/Textures/Player/Player_Idle/Player_IDLE%d.png"), 50))))
        return E_FAIL;


    //// IDLE_UP
    //if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player_Idle_Up"),
    //    CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Cult_Resources/Textures/Player/Player_Idle_Up/Player_Idle_Up%d.png"), 100))))
    //    return E_FAIL;


    /* For. Prototype_GameObject_Player */
    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
        CPlayer::Create(m_pGraphic_Device))))
        return E_FAIL;





    lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
    /* For. Prototype_Component_Texture_Terrrain_Tile */
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Terrain"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Cult_Resources/Textures/Terrain/Tile%d.png"), 14))))
        return E_FAIL;


    // 맵 필터
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Terrain_Filter"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Cult_Resources/Textures/Terrain/Filter1.png"), 1))))
        return E_FAIL;


    /* For. Prototype_Component_Texture_Terrrain_Deco */
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Village_Deco"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Cult_Resources/Textures/Village/Deco/Village_Deco%d.png"), 78))))
        return E_FAIL;


    /* For. Prototype_Component_Texture_Terrrain_Interact */
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Village_Interact"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Cult_Resources/Textures/Village/Interact/Crop/Interact%d.png"), 55))))
        return E_FAIL;


    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon_Deco"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Cult_Resources/Textures/Dungeon_Deco/Dungeon_Deco%d.png"), 12))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon2_Deco"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Cult_Resources/Textures/Dungeon_Deco2/Dungeon_Deco2_%d.png"), 134))))
        return E_FAIL;


    // ETC
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Dungeon_ETC_Deco"),
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D, TEXT("../Bin/Cult_Resources/Textures/Dungeon/ETC/ETC%d.png"), 41))))
        return E_FAIL;


 



    lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
    /* For. Prototype_Component_VIBuffer_Terrain */
    //if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
    //   CVIBuffer_Terrain::Create(m_pGraphic_Device, 50, 50))))
    //   return E_FAIL;

    lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
    /* For. Prototype_Component_VIBuffer_Rect */
    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_Rect"),
        CVIBuffer_Rect::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_GridRect"),
        CVIBuffer_GridRect::Create(m_pGraphic_Device))))
        return E_FAIL;


    lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


    lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩중입니다."));
    /* For. Prototype_GameObject_Terrain */
    //if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
    //   CTerrain::Create(m_pGraphic_Device))))
    //   return E_FAIL;

    /* For. Prototype_GameObject_Terrain */
    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"),
        CCamera::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DecoObject"),
        CDecoObject::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_InteractObject"),
        CInteractObject::Create(m_pGraphic_Device))))
        return E_FAIL;



    lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
    m_isFinished = true;

    return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVELID eNextLevelID)
{
    CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device);

    if (FAILED(pInstance->Initialize(eNextLevelID)))
    {
        MSG_BOX(TEXT("Failed to Created : CLevel_Loading"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Loading::Free()
{
    WaitForSingleObject(m_hThread, INFINITE);

    DeleteObject(m_hThread);

    CloseHandle(m_hThread);

    DeleteCriticalSection(&m_CriticalSection);

    __super::Free();

}

#include "../Public/ImGuiMgr.h"

CImGuiMgr::CImGuiMgr(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device{pGraphic_Device}
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CImGuiMgr::Initialize(const ENGINE_DESC& EngineDesc)
{
	
	// �����κ�
	m_ImGuiContext = ImGui::CreateContext();          // Create ������ ���� ������� , �갡 ������ ����������
	ImGui::StyleColorsDark();        // Style -> ����

	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig font_cfg;
	font_cfg.OversampleH = 2;
	font_cfg.OversampleV = 2;
	font_cfg.PixelSnapH = true;

	// ��Ʈ ���� ��θ� �����ϰ� �����ϼ���
	io.Fonts->AddFontFromFileTTF("C://Windows//Fonts//gulim.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesKorean());


	if (!ImGui_ImplWin32_Init(EngineDesc.hWnd))  // win32 ���õ� �ʱ�ȭ
	{
		MSG_BOX(L"IMGUI WIN32 �ʱ�ȭ ����");
		return E_FAIL;
	}

	if (!ImGui_ImplDX9_Init(m_pGraphic_Device))  // Dx ���õ� �ʱ�ȭ
	{
		MSG_BOX(L"IMGUI DX9 �ʱ�ȭ ����");
		return E_FAIL;
	}
	

	ImGui::SetCurrentContext(m_ImGuiContext);

    return S_OK;
}

void CImGuiMgr::Render()
{	
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();


	ImGui::Begin("Example Window");
	if (ImGui::Button("Click Me"))
	{
		// ��ư�� Ŭ���Ǿ��� �� ó��
		printf("Button was clicked!\n");
	}
	ImGui::Text("Move the slider:");
	float shininess = 10.f;
	ImGui::SliderFloat("Shininess", &shininess, 1.0f, 50.0f);
	ImGui::End();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

CImGuiMgr* CImGuiMgr::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const ENGINE_DESC& EngineDesc)
{
	CImGuiMgr* pInstance = new CImGuiMgr(pGraphic_Device);

	if (FAILED(pInstance->Initialize(EngineDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : ImGuiMgr"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiMgr::Free()
{
	// ����
	ImGui_ImplDX9_Shutdown();  // Dx ���� 
	ImGui_ImplWin32_Shutdown(); // Win32 ���� ������� X

	ImGui::DestroyContext(); // ������ �������� �ֱ�
	// ImGui ���� �����ؾ��ؿ�~~~


	__super::Free();

	Safe_Release(m_pGraphic_Device);

}

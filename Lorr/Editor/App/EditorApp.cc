#include "EditorApp.hh"

#include "Engine/ECS/Entity.hh"
#include "Engine/ECS/Components/CameraComponent.hh"

#define rgb(r, g, b) ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f)
#define SetColor(enum, col) style.Colors[enum] = col

void EditorApp::Init()
{
    lr::BaseWindow *pWindow = lr::GetEngine()->GetWindow();

    u32 width = pWindow->GetWidth();
    u32 height = pWindow->GetHeight();

    /// ECS INITIALIZATION ///
    m_pCurrentScene = new lr::Scene;
    m_pCurrentScene->Init("scene://editor");

    lr::Entity camera3D = m_pCurrentScene->CreateEntity("entity://camera3d");
    camera3D.AttachCamera3D(glm::vec3(0, 0, -5), glm::vec2(width, height), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), 60.f, 0.1f, 10000.f);

    lr::Entity camera2D = m_pCurrentScene->CreateEntity("entity://camera2d");
    camera2D.AttachCamera2D(glm::vec2(0, 0), glm::vec2(width, height));

    pWindow->OnResolutionChanged.connect<&EditorApp::OnResolutionChanged>(this);

    /// STYLE INITIALIZATION ///
    ImGuiStyle &style = ImGui::GetStyle();

    /// Shape
    style.FramePadding = ImVec2(6, 4);

    style.WindowRounding = 0.f;
    style.ScrollbarRounding = 0.f;
    style.FrameRounding = 0.f;
    style.GrabRounding = 0.f;
    style.PopupRounding = 0.f;
    style.TabRounding = 5.f;
    style.WindowTitleAlign = ImVec2(0, 0);

    // const ImVec4 kDefaultGreen = rgb(39, 174, 96);
    // const ImVec4 kDefaultGreenLower = rgb(21, 156, 78);
    const ImVec4 kDefaultGray = rgb(32, 32, 32);
    const ImVec4 kDefaultGrayLower = rgb(22, 22, 22);
    const ImVec4 kDefaultBlack = rgb(15, 15, 15);

    /// Text
    SetColor(ImGuiCol_Text, rgb(218, 218, 218));

    /// Window
    SetColor(ImGuiCol_WindowBg, kDefaultGray);

    SetColor(ImGuiCol_MenuBarBg, kDefaultBlack);

    SetColor(ImGuiCol_DockingEmptyBg, rgb(0, 0, 0));
    SetColor(ImGuiCol_DockingPreview, kDefaultGrayLower);

    SetColor(ImGuiCol_Border, kDefaultBlack);

    SetColor(ImGuiCol_ResizeGrip, ImVec4(0, 0, 0, 0));
    SetColor(ImGuiCol_ResizeGripActive, kDefaultGray);
    SetColor(ImGuiCol_ResizeGripHovered, kDefaultGrayLower);

    SetColor(ImGuiCol_Separator, kDefaultBlack);
    SetColor(ImGuiCol_SeparatorHovered, kDefaultBlack);
    SetColor(ImGuiCol_SeparatorActive, kDefaultBlack);

    SetColor(ImGuiCol_Header, kDefaultBlack);
    SetColor(ImGuiCol_HeaderHovered, kDefaultBlack);
    SetColor(ImGuiCol_HeaderActive, kDefaultBlack);

    SetColor(ImGuiCol_FrameBg, kDefaultBlack);
    SetColor(ImGuiCol_FrameBgHovered, kDefaultBlack);
    SetColor(ImGuiCol_FrameBgActive, kDefaultBlack);

    SetColor(ImGuiCol_TitleBg, kDefaultBlack);
    SetColor(ImGuiCol_TitleBgActive, kDefaultBlack);
    SetColor(ImGuiCol_TitleBgCollapsed, kDefaultBlack);

    SetColor(ImGuiCol_Tab, kDefaultGray);
    SetColor(ImGuiCol_TabHovered, kDefaultGray);
    SetColor(ImGuiCol_TabActive, kDefaultGray);
    SetColor(ImGuiCol_TabUnfocused, kDefaultGray);
    SetColor(ImGuiCol_TabUnfocusedActive, kDefaultGray);

    // ImGuiIO &io = ImGui::GetIO();
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("segoeui.ttf", 17);
    // io.Fonts->AddFontFromFileTTF("segoeuib.ttf", 17);
    // ImGui::PushFont(pRegularFont);
    // ImGui::PopFont();

    /// LAYER INITIALIZATION ///
    m_MainLayer->Init();
}

void EditorApp::Tick(float fDelta)
{
}

void EditorApp::Draw()
{
    m_MainLayer->Update();
}

void EditorApp::OnResolutionChanged(u32 width, u32 height)
{
    ZoneScoped;

    lr::Entity camera2D = m_pCurrentScene->GetEntity("entity://camera2d");
    lr::Entity camera3D = m_pCurrentScene->GetEntity("entity://camera3d");
    auto &camera2DComp = camera2D.GetComponent<lr::Component::Camera2DController>();
    auto &camera3DComp = camera3D.GetComponent<lr::Component::Camera3DController>();

    camera2DComp.m_Handle.SetSize(glm::vec2(width, height));
    camera3DComp.m_Handle.SetSize(glm::vec2(width, height));
}
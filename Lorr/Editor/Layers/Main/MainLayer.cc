#include "MainLayer.hh"

#include "Engine/App/Engine.hh"

using namespace Lorr;

void KeyDown(Lorr::Key eKey, Lorr::ButtonState eState, Lorr::KeyMod eMod)
{
    Camera3D *pCam = GetEngine()->GetCamera3D();

    if (eState == ButtonState::Pressed)
    {
        switch (eKey)
        {
            case Key::Key_W: pCam->StartMoving(Direction::FORWARD); break;
            case Key::Key_S: pCam->StartMoving(Direction::BACKWARD); break;
            case Key::Key_A: pCam->StartMoving(Direction::LEFT); break;
            case Key::Key_D: pCam->StartMoving(Direction::RIGHT); break;
            case Key::Key_Q: pCam->StartMoving(Direction::UP); break;
            case Key::Key_E: pCam->StartMoving(Direction::DOWN); break;
            default: break;
        }
    }
    else if (eState == ButtonState::Released)
    {
        switch (eKey)
        {
            case Key::Key_W: pCam->StopMoving(Direction::FORWARD); break;
            case Key::Key_S: pCam->StopMoving(Direction::BACKWARD); break;
            case Key::Key_A: pCam->StopMoving(Direction::LEFT); break;
            case Key::Key_D: pCam->StopMoving(Direction::RIGHT); break;
            case Key::Key_Q: pCam->StopMoving(Direction::UP); break;
            case Key::Key_E: pCam->StopMoving(Direction::DOWN); break;
            default: break;
        }
    }
}

bool down = false;
void MouseMove(glm::ivec2, glm::ivec2 offset)
{
    if (!down) return;

    Camera3D *pCam = GetEngine()->GetCamera3D();
    pCam->SetDirection(offset.x, offset.y);
}

void MouseDown(KeyMod, MouseButton, ButtonState state, const glm::ivec2 &)
{
    switch (state)
    {
        case ButtonState::Pressed: down = true; break;
        case ButtonState::Released: down = false; break;
        default: break;
    }
}

void MainLayer::Init()
{
    ZoneScoped;

    GetEngine()->GetWindow()->OnSetKeyState.connect<KeyDown>();
    GetEngine()->GetWindow()->OnSetMousePosition.connect<MouseMove>();
}

void MainLayer::Delete()
{
    ZoneScoped;
}

void MainLayer::BuildDock(ImGuiID DockID)
{
    ZoneScoped;

    PlatformWindow *pWindow = GetEngine()->GetWindow();

    ImGui::DockBuilderRemoveNode(DockID);
    ImGui::DockBuilderAddNode(DockID, ImGuiDockNodeFlags_DockSpace);

    ImGui::DockBuilderSetNodeSize(DockID, ImVec2(pWindow->GetWidth(), pWindow->GetHeight()));

    ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(DockID, ImGuiDir_Left, 0.15f, NULL, &DockID);
    ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(DockID, ImGuiDir_Right, 0.20, NULL, &DockID);
    ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(DockID, ImGuiDir_Down, 0.33f, NULL, &DockID);

    ImGui::DockBuilderDockWindow(m_EntityWindow.m_Title, dock_id_left);
    ImGui::DockBuilderDockWindow(m_PropertiesWindow.m_Title, dock_id_right);
    ImGui::DockBuilderDockWindow(m_ResourceWindow.m_Title, dock_id_down);
    ImGui::DockBuilderDockWindow("Game View", DockID);
}

float rotate = 0;

void MainLayer::Update()
{
    ZoneScoped;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("MainDock", nullptr, g_DockWindowStyle);
    {
        ImGui::PopStyleVar();

        const auto dockspace_id = ImGui::GetID("EngineDockSpace");
        if (!ImGui::DockBuilderGetNode(dockspace_id)) BuildDock(dockspace_id);
        ImGui::DockSpace(dockspace_id);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::Text("yepp");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::Text("yepp");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::Text("yepp");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::Text("yepp");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                ImGui::Text("yepp");
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
        ImGui::SetNextWindowClass(&window_class);

        ImGui::Begin(m_ResourceWindow.m_Title, nullptr, g_WindowStyle);
        {
            m_ResourceWindow.Update();
        }
        ImGui::End();

        ImGui::SetNextWindowClass(&window_class);

        ImGui::Begin(m_EntityWindow.m_Title, nullptr, g_WindowStyle);
        {
            m_EntityWindow.Update();
        }
        ImGui::End();

        ImGui::SetNextWindowClass(&window_class);

        ImGui::Begin(m_PropertiesWindow.m_Title, nullptr, g_WindowStyle);
        {
            m_PropertiesWindow.Update();
        }
        ImGui::End();
    }

    ImGui::End();
}
#include "MainLayer.hh"

#include "Engine/App/Engine.hh"

using namespace Lorr;

void MainLayer::Init()
{
    ZoneScoped;
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
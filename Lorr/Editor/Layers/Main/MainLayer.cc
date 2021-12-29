#include "MainLayer.hh"

#include "Engine/App/Engine.hh"

using namespace lr;

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

void MainLayer::Update()
{
    ZoneScoped;

    ImGuiViewport *pViewport = ImGui::GetMainViewport();

    float frameHeight = ImGui::GetFrameHeight();
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;

    if (ImGui::BeginViewportSideBar("##MenuBar1", pViewport, ImGuiDir_Up, frameHeight, windowFlags))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Open Project...");
                ImGui::MenuItem("Save");
                ImGui::Separator();
                ImGui::MenuItem("Save");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::Text("yepp");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Statistics");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::MenuItem("Audio Editor");
                ImGui::MenuItem("Shader Sandbox");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                ImGui::Text("yepp");
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    if (ImGui::BeginViewportSideBar("##MenuBar2", pViewport, ImGuiDir_Up, frameHeight, windowFlags))
    {
        ImGui::End();
    }

    if (ImGui::BeginViewportSideBar("##MenuBar3", pViewport, ImGuiDir_Down, frameHeight, windowFlags))
    {
        if (ImGui::BeginMenuBar())
        {
            ImGui::Text("Test");
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    ImGui::SetNextWindowPos(pViewport->WorkPos);
    ImGui::SetNextWindowSize(pViewport->WorkSize);
    ImGui::SetNextWindowViewport(pViewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("MainDock", nullptr, g_DockWindowStyle);
    {
        ImGui::PopStyleVar();

        const auto dockspace_id = ImGui::GetID("EngineDockSpace");
        if (!ImGui::DockBuilderGetNode(dockspace_id)) BuildDock(dockspace_id);
        ImGui::DockSpace(dockspace_id);

        ImGuiWindowClass windowClass;
        windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoWindowMenuButton;

        ImGui::SetNextWindowClass(&windowClass);
        ImGui::Begin(m_EntityWindow.m_Title, nullptr);
        {
            m_EntityWindow.Update();
        }
        ImGui::End();

        ImGui::SetNextWindowClass(&windowClass);
        ImGui::Begin(m_ResourceWindow.m_Title, nullptr);
        {
            m_ResourceWindow.Update();
        }
        ImGui::End();

        ImGui::SetNextWindowClass(&windowClass);
        ImGui::Begin(m_PropertiesWindow.m_Title, nullptr);
        {
            m_PropertiesWindow.Update();
        }
        ImGui::End();
    }

    ImGui::End();
}
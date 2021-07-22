#include "MainLayer.hh"

#include <Engine.hh>

void MainLayer::Init()
{
    ZoneScoped;
}

void MainLayer::Delete()
{
    ZoneScoped;
}

void MainLayer::BuildDock( ImGuiID DockID )
{
    ZoneScoped;

    Lorr::Window *pWindow = Lorr::GetEngine()->GetWindow();

    ImGui::DockBuilderRemoveNode( DockID );                             // Yeet away our old layout
    ImGui::DockBuilderAddNode( DockID, ImGuiDockNodeFlags_DockSpace );  // Add empty node

    ImGui::DockBuilderSetNodeSize( DockID, ImVec2( pWindow->GetWidth(), pWindow->GetHeight() ) );

    ImGuiID dock_id_left = ImGui::DockBuilderSplitNode( DockID, ImGuiDir_Left, 0.15f, NULL, &DockID );
    ImGuiID dock_id_down = ImGui::DockBuilderSplitNode( DockID, ImGuiDir_Down, 0.23f, NULL, &DockID );

    ImGui::DockBuilderDockWindow( m_WorldLayerWindow.m_szTitle, dock_id_left );
    ImGui::DockBuilderDockWindow( m_ResourceWindow.m_szTitle, dock_id_down );
    ImGui::DockBuilderDockWindow( "Game View", DockID );
}

void MainLayer::Update()
{
    ZoneScoped;

    // Basic logic of managing windows, instead of calling more functions to update their outside metrices(like pos and size)
    // we simply do that here to be more clean and reliable
    //

    bool open = true;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos( viewport->WorkPos );
    ImGui::SetNextWindowSize( viewport->WorkSize );
    ImGui::SetNextWindowViewport( viewport->ID );

    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { 0, 0 } );
    ImGui::Begin( "MainDock", &open, g_DockWindowStyle );
    {
        ImGui::PopStyleVar();

        const auto dockspace_id = ImGui::GetID( "EngineDockSpace" );
        if ( !ImGui::DockBuilderGetNode( dockspace_id ) ) BuildDock( dockspace_id );
        ImGui::DockSpace( dockspace_id );

        if ( ImGui::BeginMenuBar() )
        {
            if ( ImGui::BeginMenu( "File" ) )
            {
                ImGui::Text( "yepp" );
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
        ImGui::SetNextWindowClass( &window_class );

        ImGui::Begin( m_ResourceWindow.m_szTitle, &m_ResourceWindow.m_bIsOpen, g_WindowStyle );
        {
            m_ResourceWindow.Update();
        }
        ImGui::End();

        ImGui::SetNextWindowClass( &window_class );

        ImGui::Begin( m_WorldLayerWindow.m_szTitle, &m_WorldLayerWindow.m_bIsOpen, g_WindowStyle );
        {
            m_WorldLayerWindow.Update();
        }
        ImGui::End();
    }
    ImGui::End();
}
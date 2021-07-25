#include "MainLayer.hh"

#include <Engine.hh>

struct Vertex
{
    glm::vec4 Pos;
    glm::vec4 Color;
};

Vertex Vertices[3] = {
    { { -0.5, -0.5, 0.0, 1.0 }, { 1, 1, 1, 1 } },
    { { 0.5, -0.5, 0.0, 1.0 }, { 1, 1, 1, 1 } },
    { { 0.0, 0.5, 0.0, 1.0 }, { 1, 1, 1, 1 } },
};

uint32_t Indices[3] = { 0, 1, 2 };

void MainLayer::Init()
{
    ZoneScoped;

    Lorr::VertexLayout layout = {
        { Lorr::VertexAttribType::Vec4, "POSITION" },
        // { Lorr::VertexAttribType::Vec3, "NORMAL" },
        // { Lorr::VertexAttribType::Vec2, "TEXCOORD" },
        { Lorr::VertexAttribType::Vec4, "COLOR" },
    };

    m_pShader->Init( Lorr::GetEngine()->GetAPI(), L"mainv.hlsl", L"mainp.hlsl", layout );

    m_pVertexBuffer = Lorr::GetEngine()->GetAPI()->CreateBuffer( Vertices, sizeof( Vertices ), D3D11_BIND_VERTEX_BUFFER );
    m_pIndexBuffer = Lorr::GetEngine()->GetAPI()->CreateBuffer( Indices, sizeof( Indices ), D3D11_BIND_INDEX_BUFFER );
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

    // bool open = true;

    // const ImGuiViewport *viewport = ImGui::GetMainViewport();
    // ImGui::SetNextWindowPos( viewport->WorkPos );
    // ImGui::SetNextWindowSize( viewport->WorkSize );
    // ImGui::SetNextWindowViewport( viewport->ID );

    // ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { 0, 0 } );
    // ImGui::Begin( "MainDock", &open, g_DockWindowStyle );
    // {
    //     ImGui::PopStyleVar();

    //     const auto dockspace_id = ImGui::GetID( "EngineDockSpace" );
    //     if ( !ImGui::DockBuilderGetNode( dockspace_id ) ) BuildDock( dockspace_id );
    //     ImGui::DockSpace( dockspace_id );

    //     if ( ImGui::BeginMenuBar() )
    //     {
    //         if ( ImGui::BeginMenu( "File" ) )
    //         {
    //             ImGui::Text( "yepp" );
    //             ImGui::EndMenu();
    //         }

    //         ImGui::EndMenuBar();
    //     }

    //     ImGuiWindowClass window_class;
    //     window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
    //     ImGui::SetNextWindowClass( &window_class );

    //     ImGui::Begin( m_ResourceWindow.m_szTitle, &m_ResourceWindow.m_bIsOpen, g_WindowStyle );
    //     {
    //         m_ResourceWindow.Update();
    //     }
    //     ImGui::End();

    //     ImGui::SetNextWindowClass( &window_class );

    //     ImGui::Begin( m_WorldLayerWindow.m_szTitle, &m_WorldLayerWindow.m_bIsOpen, g_WindowStyle );
    //     {
    //         m_WorldLayerWindow.Update();
    //     }
    //     ImGui::End();
    // }
    // ImGui::End();

    uint32_t vertexStride = sizeof( Vertex );
    uint32_t offset = 0;

    ID3D11DeviceContext *pContext = Lorr::GetEngine()->GetAPI()->GetDeviceContext();
    ID3D11DepthStencilState *pDepthStencilState = Lorr::GetEngine()->GetAPI()->GetDepthStencilState();
    ID3D11RenderTargetView *pRenderTargetView = Lorr::GetEngine()->GetAPI()->GetRenderTargetView();
    ID3D11DepthStencilView *pDepthStencilView = Lorr::GetEngine()->GetAPI()->GetDepthStencilView();
    ID3D11RasterizerState *pRasterizerState = Lorr::GetEngine()->GetAPI()->GetRasterizerState();

    pContext->IASetInputLayout( m_pShader->GetInputLayout() );

    pContext->VSSetShader( m_pShader->GetVertexShader(), 0, 0 );

    pContext->PSSetShader( m_pShader->GetPixelShader(), 0, 0 );
    pContext->RSSetState( pRasterizerState );

    pContext->OMSetDepthStencilState( pDepthStencilState, 0 );
    pContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

    pContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &vertexStride, &offset );
    pContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
    pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    pContext->DrawIndexed( 3, 0, 0 );
}
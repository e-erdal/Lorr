#include "MainLayer.hh"

#include <glm/gtc/matrix_transform.hpp>

#include <Engine.hh>
#include <obj_loader.h>

struct Vertex
{
    glm::vec3 Pos;
    glm::vec3 Norm{0};
    glm::vec4 Color = {1, 1, 1, 1};
};

#pragma pack(push, 1)
struct InputData
{
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 normalM;
};
#pragma pack(pop)

void KeyDown(Lorr::Key eKey, Lorr::ButtonState eState, Lorr::KeyMod eMod)
{
    using namespace Lorr;
    Camera *pCam = GetEngine()->GetCamera();

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

void MouseMove(glm::ivec2, glm::ivec2 offset)
{
    using namespace Lorr;
    Camera *pCam = GetEngine()->GetCamera();
    pCam->SetDirection(offset.x, offset.y);
}

void MainLayer::Init()
{
    ZoneScoped;

    Lorr::VertexLayout layout = {
        {Lorr::VertexAttribType::Vec3, "POSITION"},
        {Lorr::VertexAttribType::Vec3, "NORMAL"},
        {Lorr::VertexAttribType::Vec2, "TEXCOORD"},
        {Lorr::VertexAttribType::Vec4, "COLOR"},
    };

    Lorr::GetEngine()->GetWindow()->OnSetKeyState.connect<KeyDown>();
    Lorr::GetEngine()->GetWindow()->OnSetMousePosition.connect<MouseMove>();

    m_pShader->Init(Lorr::GetEngine()->GetAPI(), L"mainv.hlsl", L"mainp.hlsl", layout);

    m_pVConstantBuffer = Lorr::GetEngine()->GetAPI()->CreateBuffer(0, sizeof(InputData), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, false);

    m_Model.Init("monkey.obj");
    m_Model.InitAsRect(1.0f, 50);
    // m_Model.InitAsSphere( 10.0f, 1024 );
}

void MainLayer::Delete()
{
    ZoneScoped;
}

void MainLayer::BuildDock(ImGuiID DockID)
{
    ZoneScoped;

    Lorr::Window *pWindow = Lorr::GetEngine()->GetWindow();

    ImGui::DockBuilderRemoveNode(DockID);                             // Yeet away our old layout
    ImGui::DockBuilderAddNode(DockID, ImGuiDockNodeFlags_DockSpace);  // Add empty node

    ImGui::DockBuilderSetNodeSize(DockID, ImVec2(pWindow->GetWidth(), pWindow->GetHeight()));

    ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(DockID, ImGuiDir_Left, 0.15f, NULL, &DockID);
    ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(DockID, ImGuiDir_Down, 0.23f, NULL, &DockID);

    ImGui::DockBuilderDockWindow(m_WorldLayerWindow.m_szTitle, dock_id_left);
    ImGui::DockBuilderDockWindow(m_ResourceWindow.m_szTitle, dock_id_down);
    ImGui::DockBuilderDockWindow("Game View", DockID);
}

float rotate = 0;

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
    rotate += 0.01f;
    uint32_t vertexStride = sizeof(Lorr::Vertex);
    uint32_t offset = 0;

    Lorr::Camera *pCamera = Lorr::GetEngine()->GetCamera();

    ID3D11DeviceContext *pContext = Lorr::GetEngine()->GetAPI()->GetDeviceContext();

    pContext->IASetInputLayout(m_pShader->GetInputLayout());

    pContext->PSSetShader(m_pShader->GetPixelShader(), 0, 0);
    pContext->VSSetShader(m_pShader->GetVertexShader(), 0, 0);

    D3D11_MAPPED_SUBRESOURCE ms;
    ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
    if (SUCCEEDED(pContext->Map(m_pVConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)))
    {
        InputData *id = (InputData *)ms.pData;

        id->projectionMatrix = glm::transpose(pCamera->GetProjection());
        id->viewMatrix = glm::transpose(pCamera->GetView());

        id->modelMatrix = glm::translate(glm::mat4(1.f), {0, 0, 0});
        id->modelMatrix = glm::rotate(id->modelMatrix, glm::radians(90.f), {1, 0, 0});
        id->modelMatrix = glm::scale(id->modelMatrix, {1, 1, 1});
        id->modelMatrix = glm::transpose(id->modelMatrix);

        id->normalM = id->modelMatrix;

        pContext->Unmap(m_pVConstantBuffer, 0);
    }

    pContext->VSSetConstantBuffers(0, 1, &m_pVConstantBuffer);

    m_Model.Render();
}
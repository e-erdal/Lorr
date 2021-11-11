#include "GameApp.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/PhysicsComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"
#include "Engine/Graphics/Font.hh"

#include "Engine/Model/Model.hh"

#include "Engine/Job/Worker.hh"

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

TextureHandle texture;
ShaderHandle compute;
TextureHandle RWTexture;
TextureHandle computeOutput;

void GameApp::Init()
{
    //* Init signals
    GetEngine()->GetWindow()->OnSetMouseState.connect<MouseDown>();
    GetEngine()->GetWindow()->OnSetKeyState.connect<KeyDown>();
    GetEngine()->GetWindow()->OnSetMousePosition.connect<MouseMove>();

    //* Required vars
    IRenderer *pRenderer = GetEngine()->GetRenderer();
    u32 width = GetEngine()->GetWindow()->GetWidth();
    u32 height = GetEngine()->GetWindow()->GetHeight();
    glm::mat4 mat = {};

    //* Scene initialization
    m_pCurrentScene = new Scene;
    m_pCurrentScene->Init("scene://default-scene");

    //* Preload resources
    FontDesc desc;
    desc.SizePX = 32;
    Font *pFont = GetEngine()->GetResourceMan()->LoadResource<Font>("font://font", "font.lr", &desc);

    // Post-process target
    pRenderer->CreateTarget("renderer://postprocess", width, height, 0, 6);

    // Load shaders
    GetEngine()->GetShaderMan()->CreateProgram("shader://model", kMeshLayout, "shaders/modelv.lr", "shaders/modelp.lr");
    GetEngine()->GetShaderMan()->CreateProgram("shader://batcher", VertexBatcher::m_Layout, "shaders/batchv.lr", "shaders/batchp.lr");

    RenderBufferDesc modelCBuf;
    modelCBuf.DataLen = sizeof(mat);
    modelCBuf.Type = RenderBufferType::Constant;
    modelCBuf.Usage = RenderBufferUsage::Dynamic;
    modelCBuf.MemFlags = RenderBufferMemoryFlags::Access_CPUW;
    GetEngine()->GetShaderMan()->CreateRenderBuffer("cbuffer://model", modelCBuf);

    modelCBuf.DataLen = sizeof(mat);
    GetEngine()->GetShaderMan()->CreateRenderBuffer("cbuffer://batcher", modelCBuf);

    //* Init entities
    // Create entities
    Entity postProcess = m_pCurrentScene->CreateEntity("test");
    postProcess.AddComponent<Component::Transform>(glm::vec3(0, 0, 1), glm::vec3(width, height, 1));
    auto &rend = postProcess.AddComponent<Component::Renderable>(true);
    texture = rend.texture = pRenderer->GetTargetTexture("renderer://postprocess");

    Entity modelEntity = m_pCurrentScene->CreateEntity("test");
    modelEntity.AddComponent<Component::Transform>(glm::vec3(), glm::vec3());
    Model &model = modelEntity.AddComponent<Model>();

    model.Init("teapot.obj");
    // model.Init("sponza.obj");
    // model.AddSphere(100, 128, pRenderer->GetPlaceholder());

    //** Compute Shader Test **//
    compute = Shader::Create("test://compute", "shaders/testc.lr");

    TextureDesc rwDesc;
    rwDesc.Type = TEXTURE_TYPE_RW;

    TextureData rwData;
    rwData.Format = TEXTURE_FORMAT_RGBAF32;
    rwData.Width = width;
    rwData.Height = height;
    RWTexture = Texture::Create("test", &rwDesc, &rwData);

    TextureDesc outputDesc;
    outputDesc.Type = TEXTURE_TYPE_REGULAR;
    
    TextureData outputData;
    outputData.Format = TEXTURE_FORMAT_RGBAF32;
    outputData.Width = width;
    outputData.Height = height;
    computeOutput = Texture::Create("test", &outputDesc, &outputData);

    // RenderBufferDesc computeResult;
    // // computeResult.DataLen = sizeof(glm::vec4) * texture->GetWidth() * texture->GetHeight();
    // // computeResult.ByteStride = sizeof(glm::vec4);
    // computeResult.Usage = RenderBufferUsage::Staging;
    // computeResult.MemFlags = RenderBufferMemoryFlags::Access_CPUR;
    // computeOutputResultBuffer = RenderBuffer::Create(computeResult);
}

void GameApp::Tick(float fDelta)
{
    m_pCurrentScene->Tick(fDelta);
    GetEngine()->GetCamera3D()->Update(fDelta);
}

void GameApp::Draw()
{
    m_pCurrentScene->Draw();

    IRenderer *pRenderer = GetEngine()->GetRenderer();

    pRenderer->UseShader(compute);

    pRenderer->UseShaderBuffer((TextureHandle)0, RenderBufferTarget::Compute, 0);
    pRenderer->UseUAV(RWTexture, RenderBufferTarget::Compute, 0);
    pRenderer->UseShaderBuffer(texture, RenderBufferTarget::Compute, 0);

    pRenderer->Dispatch(64, 64, 1);

    pRenderer->TransferResourceData(RWTexture, computeOutput);

    // // TextureHandle outTexture = (TextureHandle)computeOutputResultBuffer->GetData();
    // // computeOutputResultBuffer->UnmapData();

    pRenderer->UseShaderBuffer((TextureHandle)0, RenderBufferTarget::Compute, 0);
    pRenderer->UseUAV((TextureHandle)0, RenderBufferTarget::Compute, 0);

    ImGui::Begin("GameApp", nullptr);
    ImGui::Text("FPS: %2.f", ImGui::GetIO().Framerate);

    ImGui::SliderInt("Mip", (int *)&texture->m_UsingMip, 0, 6);
    ImGui::Image((ImTextureID)computeOutput, { 1280 / 2.f, 720 / 2.f });

    ImGui::End();
}

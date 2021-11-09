#include "ImGuiHandler.hh"

#include "Engine/App/Engine.hh"
#include "Engine/Core/Window/IWindow.hh"

namespace Lorr
{
    static InputLayout kImGuiInputLayout = {
        { VertexAttribType::Vec2, "POSITION" },
        { VertexAttribType::Vec2, "TEXCOORD" },
        { VertexAttribType::UInt, "COLOR" },
    };

    static uint32_t g_VertexBufferSize = 5000;
    static uint32_t g_IndexBufferSize = 10000;

    void ImGui_ImplSurface_KeyPress(Key eKey, ButtonState eState, KeyMod eMod)
    {
        ZoneScoped;

        if (ImGui::GetCurrentContext() == NULL) return;

        ImGuiIO &io = ImGui::GetIO();

        io.KeyCtrl = eMod & KeyMod::CONTROL;
        io.KeyShift = eMod & KeyMod::SHIFT;
        io.KeyAlt = eMod & KeyMod::ALT;
        io.KeySuper = eMod & KeyMod::SUPER;

        if ((int)eKey < IM_ARRAYSIZE(io.KeysDown))
        {
            if (eState == ButtonState::Pressed)
            {
                io.KeysDown[(int)eKey] = true;
            }

            if (eState == ButtonState::Released)
            {
                io.KeysDown[(int)eKey] = false;
            }
        }
    }

    void ImGui_ImplSurface_OnChar(uint32_t Char, KeyMod eMod)
    {
        ZoneScoped;

        if (ImGui::GetCurrentContext() == NULL) return;

        ImGuiIO &io = ImGui::GetIO();

        io.AddInputCharacter(Char);
    }

    void ImGui_ImplSurface_MouseStateChange(KeyMod eMod, MouseButton eButton, ButtonState eState, const glm::ivec2 &ivPos)
    {
        ZoneScoped;

        if (ImGui::GetCurrentContext() == NULL) return;

        ImGuiIO &io = ImGui::GetIO();

        int mouse = 0;

        switch (eButton)
        {
            case MouseButton::BTN_1: mouse = 0; break;
            case MouseButton::BTN_2: mouse = 1; break;
            case MouseButton::BTN_3: mouse = 2; break;
            case MouseButton::BTN_4: mouse = 3; break;
            case MouseButton::BTN_5: mouse = 4; break;
            default: break;
        }

        if (eState == ButtonState::Pressed)
        {
            io.MouseDown[mouse] = true;
        }

        else if (eState == ButtonState::Released)
        {
            io.MouseDown[mouse] = false;
        }

        if (eButton == MouseButton::BTN_4)
        {
            io.MouseWheel += 1;
        }

        else if (eButton == MouseButton::BTN_5)
        {
            io.MouseWheel -= 1;
        }
    }

    void ImGui_ImplSurface_UpdateMouseCursor()
    {
        ZoneScoped;

        if (ImGui::GetCurrentContext() == NULL) return;

        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) return;

        ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        PlatformWindow *pSurface = (PlatformWindow *)io.BackendPlatformUserData;

        ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
        if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
        {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
            pSurface->SetCursor(Cursor::Hidden);
        }
        else
        {
            // Show OS mouse cursor
            auto cursor = Cursor::Arrow;
            switch (imgui_cursor)
            {
                case ImGuiMouseCursor_Arrow: cursor = Cursor::Arrow; break;
                case ImGuiMouseCursor_TextInput: cursor = Cursor::TextInput; break;
                case ImGuiMouseCursor_ResizeAll: cursor = Cursor::ResizeAll; break;
                case ImGuiMouseCursor_ResizeEW: cursor = Cursor::ResizeEW; break;
                case ImGuiMouseCursor_ResizeNS: cursor = Cursor::ResizeNS; break;
                case ImGuiMouseCursor_ResizeNESW: cursor = Cursor::ResizeNESW; break;
                case ImGuiMouseCursor_ResizeNWSE: cursor = Cursor::ResizeNWSE; break;
                case ImGuiMouseCursor_Hand: cursor = Cursor::Hand; break;
                case ImGuiMouseCursor_NotAllowed: cursor = Cursor::NotAllowed; break;
            }

            pSurface->SetCursor(cursor);
        }
    }

    void ImGui_ImplSurface_UpdateMousePos()
    {
        ZoneScoped;

        if (ImGui::GetCurrentContext() == NULL) return;

        ImGuiIO &io = ImGui::GetIO();

        ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        PlatformWindow *pSurface = (PlatformWindow *)io.BackendPlatformUserData;

        IM_ASSERT(pSurface != 0);

        auto pos = pSurface->GetCursorPos();

        if (io.WantSetMousePos)
        {
            pSurface->OnSetMousePosition(glm::vec2{ io.MousePos.x, io.MousePos.y }, glm::vec2{ io.MouseDelta.x, io.MouseDelta.y });
        }

        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
        io.MouseHoveredViewport = 0;

        io.MousePos = ImVec2((float)pos.x, (float)pos.y);
    }

    ImGuiHandler::~ImGuiHandler()
    {
    }

    void ImGuiHandler::Init(Engine *pEngine)
    {
        ZoneScoped;

        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
        io.IniFilename = 0;
        io.Fonts->AddFontFromFileTTF("font.ttf", 14);

        ImGui::StyleColorsDark();

        InitImGui(pEngine->GetWindow());
    }

    void ImGuiHandler::BeginFrame()
    {
        ZoneScoped;

        ImGui_ImplSurface_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiHandler::EndFrame()
    {
        ZoneScopedN("ImGuiHandler::EndFrame");

        ImGui::EndFrame();

        ImGui::Render();
        ImGui_ImplIRenderer_Draw();
    }

    void ImGuiHandler::InitImGui(PlatformWindow *pWindow)
    {
        ZoneScoped;

        ImGuiIO &io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;          // We can honor GetMouseCursor() values (optional)
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;           // We can honor io.WantSetMousePos requests (optional, rarely used)
        io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;  // We can set io.MouseHoveredViewport correctly (optional, not easy)
        // io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;  // We can create multi-viewports on the Platform side (optional)
        io.BackendPlatformName = "imgui_impl_surface";

        io.KeyMap[ImGuiKey_Tab] = (int)Key::Key_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = (int)Key::Key_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = (int)Key::Key_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = (int)Key::Key_UP;
        io.KeyMap[ImGuiKey_DownArrow] = (int)Key::Key_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = (int)Key::Key_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = (int)Key::Key_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = (int)Key::Key_HOME;
        io.KeyMap[ImGuiKey_End] = (int)Key::Key_END;
        io.KeyMap[ImGuiKey_Insert] = (int)Key::Key_INSERT;
        io.KeyMap[ImGuiKey_Delete] = (int)Key::Key_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = (int)Key::Key_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = (int)Key::Key_SPACE;
        io.KeyMap[ImGuiKey_Enter] = (int)Key::Key_ENTER;
        io.KeyMap[ImGuiKey_Escape] = (int)Key::Key_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = (int)Key::Key_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = (int)Key::Key_A;
        io.KeyMap[ImGuiKey_C] = (int)Key::Key_C;
        io.KeyMap[ImGuiKey_V] = (int)Key::Key_V;
        io.KeyMap[ImGuiKey_X] = (int)Key::Key_X;
        io.KeyMap[ImGuiKey_Y] = (int)Key::Key_Y;
        io.KeyMap[ImGuiKey_Z] = (int)Key::Key_Z;

        io.BackendPlatformUserData = (void *)pWindow;

        ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        main_viewport->PlatformHandle = (void *)pWindow->GetHandle();

        pWindow->OnSetKeyState.connect<&ImGui_ImplSurface_KeyPress>();
        pWindow->OnSetMouseState.connect<&ImGui_ImplSurface_MouseStateChange>();
        pWindow->OnChar.connect<&ImGui_ImplSurface_OnChar>();

        //* IRenderer backend initialization *//
        ShaderDesc vsDesc{ .Layout = kImGuiInputLayout };
        m_VertexShader = Shader::Create("imgui://vertex-shader", "shaders/imguiv.lr", &vsDesc);
        m_PixelShader = Shader::Create("imgui://pixel-shader", "shaders/imguip.lr");

        uint8_t *pFontData;
        int32_t fontW, fontH;
        io.Fonts->GetTexDataAsRGBA32(&pFontData, &fontW, &fontH);

        TextureDesc desc;
        TextureData texData{ .Width = (uint32_t)fontW, .Height = (uint32_t)fontH, .Data = pFontData };
        m_FontTexture = Texture::Create("imgui://font", &desc, &texData);

        io.Fonts->TexID = m_FontTexture;  // We dont use IDs, so just scrap that in

        RenderBufferDesc cbDesc;
        cbDesc.DataLen = sizeof(float[4][4]);
        cbDesc.Type = RenderBufferType::Constant;
        cbDesc.Usage = RenderBufferUsage::Dynamic;
        cbDesc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

        m_VertexConstantBuffer = RenderBuffer::Create(cbDesc);

        cbDesc.DataLen = sizeof(glm::vec4);
        m_PixelConstantBuffer = RenderBuffer::Create(cbDesc);
    }

    void ImGuiHandler::ImGui_ImplSurface_Shutdown()
    {
        ZoneScoped;
    }

    void ImGuiHandler::ImGui_ImplSurface_NewFrame()
    {
        ZoneScoped;

        static Cursor lastCursor = Cursor::Hidden;
        static Timer timer{};

        ImGuiIO &io = ImGui::GetIO();
        IM_ASSERT(io.Fonts->IsBuilt()
                  && "Font atlas not built! It is generally built by the renderer backend. Missing "
                     "call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

        // Setup display size (every frame to accommodate for window resizing)
        ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        PlatformWindow *pSurface = (PlatformWindow *)io.BackendPlatformUserData;

        io.DisplaySize = ImVec2((float)(pSurface->GetWidth()), (float)(pSurface->GetHeight()));

        // Setup time step
        io.DeltaTime = timer.elapsed();

        // Update OS mouse position
        ImGui_ImplSurface_UpdateMousePos();

        // Update OS mouse cursor with the cursor requested by imgui
        ImGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
        if (lastCursor != (Cursor)mouse_cursor)
        {
            lastCursor = (Cursor)mouse_cursor;
            ImGui_ImplSurface_UpdateMouseCursor();
        }

        timer.reset();
    }

    void ImGuiHandler::ImGui_ImplIRenderer_Draw()
    {
        ZoneScopedN("ImGuiHandler::Render");

        ImDrawData *pDrawData = ImGui::GetDrawData();
        IRenderer *pRenderer = GetEngine()->GetRenderer();
        Camera2D *pCamera = GetEngine()->GetCamera2D();

        pRenderer->SetCurrentTarget("renderer://backbuffer");

        pRenderer->SetDepthFunc(D3D::DepthFunc::Always, false);
        pRenderer->SetCulling(D3D::Cull::None, false);
        pRenderer->SetBlend(true, false);

        if (pDrawData->DisplaySize.x <= 0.0f || pDrawData->DisplaySize.y <= 0.0f) return;

        if (!m_VertexBuffer || g_VertexBufferSize < pDrawData->TotalVtxCount)
        {
            if (m_VertexBuffer) m_VertexBuffer->Delete();
            g_VertexBufferSize = pDrawData->TotalVtxCount + 5000;

            RenderBufferDesc desc;
            desc.DataLen = g_VertexBufferSize * sizeof(ImDrawVert);
            desc.Type = RenderBufferType::Vertex;
            desc.Usage = RenderBufferUsage::Dynamic;
            desc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

            m_VertexBuffer = RenderBuffer::Create(desc);
        }

        if (!m_IndexBuffer || g_IndexBufferSize < pDrawData->TotalIdxCount)
        {
            if (m_IndexBuffer) m_IndexBuffer->Delete();
            g_IndexBufferSize = pDrawData->TotalIdxCount + 10000;

            RenderBufferDesc desc;
            desc.DataLen = g_IndexBufferSize * sizeof(ImDrawIdx);
            desc.Type = RenderBufferType::Index;
            desc.Usage = RenderBufferUsage::Dynamic;
            desc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

            m_IndexBuffer = RenderBuffer::Create(desc);
        }

        ImDrawVert *pVerices = (ImDrawVert *)m_VertexBuffer->GetData();
        ImDrawIdx *pIndices = (ImDrawIdx *)m_IndexBuffer->GetData();

        for (int i = 0; i < pDrawData->CmdListsCount; i++)
        {
            const ImDrawList *pDrawList = pDrawData->CmdLists[i];
            memcpy(pVerices, pDrawList->VtxBuffer.Data, pDrawList->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(pIndices, pDrawList->IdxBuffer.Data, pDrawList->IdxBuffer.Size * sizeof(ImDrawIdx));

            pVerices += pDrawList->VtxBuffer.Size;
            pIndices += pDrawList->IdxBuffer.Size;
        }

        m_VertexBuffer->UnmapData();
        m_IndexBuffer->UnmapData();

        glm::mat4 mvp = pCamera->GetMatrix();
        m_VertexConstantBuffer->SetData(&mvp[0][0], sizeof(glm::mat4));
        glm::vec2 clipOff = { pDrawData->DisplayPos.x, pDrawData->DisplayPos.y };

        pRenderer->UseShader(m_VertexShader);
        pRenderer->UseShader(m_PixelShader);

        pRenderer->UseConstantBuffer(m_VertexConstantBuffer, RenderBufferTarget::Vertex, 0);
        pRenderer->UseVertexBuffer(m_VertexBuffer, &kImGuiInputLayout);
        pRenderer->UseIndexBuffer(m_IndexBuffer, false);

        uint32_t vertexOff = 0;
        uint32_t indexOff = 0;

        for (int i = 0; i < pDrawData->CmdListsCount; i++)
        {
            const ImDrawList *pDrawList = pDrawData->CmdLists[i];

            for (auto &cmd : pDrawList->CmdBuffer)
            {
                if (cmd.UserCallback)
                {
                    if (cmd.UserCallback == ImDrawCallback_ResetRenderState)
                    {
                        LOG_ERROR("Setup Render state!!!");
                    }
                    else
                    {
                        cmd.UserCallback(pDrawList, &cmd);
                    }
                }
                else
                {
                    glm::vec2 clipMin(cmd.ClipRect.x - clipOff.x, cmd.ClipRect.y - clipOff.y);
                    glm::vec2 clipMax(cmd.ClipRect.z - clipOff.x, cmd.ClipRect.w - clipOff.y);
                    if (clipMax.x < clipMin.x || clipMax.y < clipMin.y) continue;

                    // pRenderer->SetScissor({ clipMin.x, clipMin.y, clipMax.x, clipMax.y });

                    TextureHandle texture = (TextureHandle)cmd.TextureId;
                    glm::vec4 mips = { texture->m_UsingMip, 0.f, 0.f, 0.f };

                    m_PixelConstantBuffer->SetData(&mips, sizeof(glm::vec4));
                    pRenderer->UseConstantBuffer(m_PixelConstantBuffer, RenderBufferTarget::Pixel, 0);
                    texture->Use();

                    pRenderer->DrawIndexed(cmd.ElemCount, cmd.IdxOffset + indexOff, cmd.VtxOffset + vertexOff);
                }
            }

            vertexOff += pDrawList->VtxBuffer.Size;
            indexOff += pDrawList->IdxBuffer.Size;
        }
    }

}  // namespace Lorr
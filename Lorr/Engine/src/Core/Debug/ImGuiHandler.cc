#include "ImGuiHandler.hh"

#include "Engine.hh"

#include "Utils/Timer.hh"

#include "Core/Window/IWindow.hh"

#include <imgui_impl_bgfx.h>

namespace Lorr
{
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
            pSurface->OnSetMousePosition(glm::vec2{io.MousePos.x, io.MousePos.y}, glm::vec2{io.MouseDelta.x, io.MouseDelta.y});
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

        ImGui::StyleColorsDark();

        ImGui_Implbgfx_Init(255);
        InitImGui(pEngine->GetWindow());
    }

    void ImGuiHandler::BeginFrame()
    {
        ZoneScoped;

        ImGui_Implbgfx_NewFrame();
        ImGui_ImplSurface_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiHandler::EndFrame()
    {
        ZoneScopedN("ImGuiHandler::EndFrame");

        ImGui::Render();
        ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
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
        main_viewport->PlatformHandle = (void *)pWindow->GetPlatformData().nwh;

        pWindow->OnSetKeyState.connect<&ImGui_ImplSurface_KeyPress>();
        pWindow->OnSetMouseState.connect<&ImGui_ImplSurface_MouseStateChange>();
        pWindow->OnChar.connect<&ImGui_ImplSurface_OnChar>();
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

}  // namespace Lorr
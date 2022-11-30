// Lab01 task
// uses code from ImGUI example main.cpp
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"version.lib")
#pragma comment(lib,"Setupapi.lib")


#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_dx11.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_syswm.h>

#include "MyMath.h"
#include "SDLWindow.h"
#include <DirectXMath.h>
#include "Box.h"
#include "Bindable.h"
#include "Camera.h"

void ImGUiKeyboard();

// Main code
int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    try {
        // Setup SDL
        // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
        // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to the latest version of SDL is recommended!)
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            MessageBoxA(nullptr, SDL_GetError(), "SDL Init Error", MB_OK | MB_ICONEXCLAMATION);
            return -1;
        }
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

            // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup window
        // Setup Platform/Renderer backends
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI);
        Window window("My new windoW", { 640,480 }, window_flags);
        // Setup project variables
        ImVec4 clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        Camera cam;
        Box box(window.Gfx(),
            { 1.0f,1.0f,0.5f },
            DirectX::XMFLOAT3(),
            { 0.0f,0.0f,0.0f},
            DirectX::XMFLOAT3(),
            { 0.0f,0.0f,0.0f });
;        ImVec4 vel,rot;
        float incr = 0.1f;
        // Main loop
        bool done = false;
        bool show_demo_window = false;
        bool is_wireframe = false;
        bool is_ortho = false;

        DirectX::XMFLOAT3 pos = {5.0f,0.0f,0.0f};

        while (!done)
        {
            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
            // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT)
                    done = true;
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == window.getID())
                    done = true;
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == window.getID())
                {
                    // Release all outstanding references to the swap chain's buffers before resizing.
                    //CleanupRenderTarget();
                    //g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
                    //CreateRenderTarget();
                }
                if (event.type == SDL_KEYDOWN)
                {
                    /* Check the SDLKey values and move change the coords */
                    switch (event.key.keysym.sym) {
                    case SDLK_r:
                        if (!is_ortho)
                            vel.x = -incr;
                        break;
                    case SDLK_f:
                        if (!is_ortho)
                            vel.x = incr;
                        break;
                    case SDLK_a:
                        vel.y = -incr;
                        break;
                    case SDLK_d:
                        vel.y = incr;
                        break;
                    case SDLK_w:
                        vel.z = incr;
                        break;
                    case SDLK_s:
                        vel.z = -incr;
                        break;

                    case SDLK_q:
                        if (is_wireframe)
                        {
                            window.Gfx().solid();
                            is_wireframe = false;
                        }
                        else
                        {
                            window.Gfx().wireframe();
                            is_wireframe = true;
                        }
                        break;
                    case SDLK_p:
                        if (is_ortho)
                        {
                            is_ortho = false;
                            window.Gfx().perspective();
                        }
                        break;
                    case SDLK_u:
                        if (!is_ortho)
                        {
                            is_ortho = true;
                            window.Gfx().orthographic();
                        }
                        break;
                    default:
                        break;
                    }
                }
                /* We must also use the SDL_KEYUP events to zero the x */
                /* and y velocity variables. But we must also be       */
                /* careful not to zero the velocities when we shouldn't*/
                if (event.type == SDL_KEYUP)
                {
                    switch (event.key.keysym.sym) {
                    case SDLK_r:
                        if (vel.x < 0)
                            vel.x = 0;
                        break;
                    case SDLK_f:
                        if (vel.x > 0)
                            vel.x = 0;
                        break;
                    case SDLK_a:
                        if (vel.y < 0)
                            vel.y = 0;
                        break;
                    case SDLK_d:
                        if (vel.y > 0)
                            vel.y = 0;
                        break;
                    case SDLK_w:
                        if (vel.z > 0)
                            vel.z = 0;
                        break;
                    case SDLK_s:
                        if (vel.z < 0)
                            vel.z = 0;
                        break;
                    default:
                        break;
                    }
                }

            }
            //update local cam pos
            pos.x = pos.x + vel.x;
            if (pos.x < 1.0f) pos.x = 1.0f;
            pos.y = NormalizeAngle(pos.y + vel.y);
            pos.z = NormalizeAngle(pos.z + vel.z);
            if (pos.z > M_PI / 2 || pos.z < -M_PI / 2)
                pos.z = sgn(pos.z) * M_PI / 2;
            //Set enviroment updates
            cam.SetPositionSph(pos);
            box.Update(1.0f / 60);
            window.Gfx().SetCamera(cam.GetMatrix());
            //render
            window.Gfx().ClearBuffer(clear_color);
            box.Draw(window.Gfx());
            // Start the Dear ImGui frame
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            cam.SpawnControlWindow();
#ifndef NDEBUG
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);
            {
                ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
                ImGui::ColorEdit4("Clear color", (float*)&clear_color, ImGuiColorEditFlags_NoInputs);
                ImGui::SameLine();
                if (ImGui::Button("reset position"))
                    pos = { 0,0,0 };
                ImGui::SliderFloat("velocity", &incr, 0.0000001f, 0.2f);
                ImGUiKeyboard();
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
#endif
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
            window.Gfx().EndFrame(); // Present with vsync
        }
        // Cleanup
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        SDL_Quit();
        return 0;
    }
    catch (const ChiliException& e)
    {
        MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
}
void ImGUiKeyboard() {

    const ImVec2 key_size = ImVec2(35.0f, 35.0f);
    const float  key_rounding = 3.0f;
    const ImVec2 key_face_size = ImVec2(25.0f, 25.0f);
    const ImVec2 key_face_pos = ImVec2(5.0f, 3.0f);
    const float  key_face_rounding = 2.0f;
    const ImVec2 key_label_pos = ImVec2(7.0f, 4.0f);
    const ImVec2 key_step = ImVec2(key_size.x - 1.0f, key_size.y - 1.0f);
    const float  key_row_offset = 9.0f;

    ImVec2 board_min = ImGui::GetCursorScreenPos();
    ImVec2 board_max = ImVec2(board_min.x + 3 * key_step.x + 2 * key_row_offset + 10.0f, board_min.y + 3 * key_step.y + 10.0f);
    ImVec2 start_pos = ImVec2(board_min.x + 5.0f - key_step.x, board_min.y);

    struct KeyLayoutData { int Row, Col; const char* Label; ImGuiKey Key; };
    const KeyLayoutData keys_to_display[] =
    {
        { 0, 0, "", ImGuiKey_Tab },      { 0, 1, "Q", ImGuiKey_Q }, { 0, 2, "W", ImGuiKey_W }, { 0, 3, "E", ImGuiKey_E }, { 0, 4, "R", ImGuiKey_R },
        { 1, 0, "", ImGuiKey_CapsLock }, { 1, 1, "A", ImGuiKey_A }, { 1, 2, "S", ImGuiKey_S }, { 1, 3, "D", ImGuiKey_D }, { 1, 4, "F", ImGuiKey_F },
        { 2, 0, "", ImGuiKey_LeftShift },{ 2, 1, "Z", ImGuiKey_Z }, { 2, 2, "X", ImGuiKey_X }, { 2, 3, "C", ImGuiKey_C }, { 2, 4, "V", ImGuiKey_V }
    };

    // Elements rendered manually via ImDrawList API are not clipped automatically.
    // While not strictly necessary, here IsItemVisible() is used to avoid rendering these shapes when they are out of view.
    ImGui::Dummy(ImVec2(board_max.x - board_min.x, board_max.y - board_min.y));
    if (ImGui::IsItemVisible())
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->PushClipRect(board_min, board_max, true);
        for (int n = 0; n < IM_ARRAYSIZE(keys_to_display); n++)
        {
            const KeyLayoutData* key_data = &keys_to_display[n];
            ImVec2 key_min = ImVec2(start_pos.x + key_data->Col * key_step.x + key_data->Row * key_row_offset, start_pos.y + key_data->Row * key_step.y);
            ImVec2 key_max = ImVec2(key_min.x + key_size.x, key_min.y + key_size.y);
            draw_list->AddRectFilled(key_min, key_max, IM_COL32(204, 204, 204, 255), key_rounding);
            draw_list->AddRect(key_min, key_max, IM_COL32(24, 24, 24, 255), key_rounding);
            ImVec2 face_min = ImVec2(key_min.x + key_face_pos.x, key_min.y + key_face_pos.y);
            ImVec2 face_max = ImVec2(face_min.x + key_face_size.x, face_min.y + key_face_size.y);
            draw_list->AddRect(face_min, face_max, IM_COL32(193, 193, 193, 255), key_face_rounding, ImDrawFlags_None, 2.0f);
            draw_list->AddRectFilled(face_min, face_max, IM_COL32(252, 252, 252, 255), key_face_rounding);
            ImVec2 label_min = ImVec2(key_min.x + key_label_pos.x, key_min.y + key_label_pos.y);
            draw_list->AddText(label_min, IM_COL32(64, 64, 64, 255), key_data->Label);
            if (ImGui::IsKeyDown(key_data->Key))
                draw_list->AddRectFilled(key_min, key_max, IM_COL32(255, 0, 0, 128), key_rounding);
        }
        draw_list->PopClipRect();
    }

}

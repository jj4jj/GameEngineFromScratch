#include "GuiSubPass.hpp"
#include "imgui/imgui.h"

using namespace My;

#include <chrono>

static void ShowStaticsOverlay(bool* p_open) {
    const float DISTANCE = 10.0f;
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    if (corner != -1)
    {
        ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
        window_flags |= ImGuiWindowFlags_NoMove;
    if (ImGui::Begin("Statics", p_open, window_flags))
    {
        static std::chrono::time_point<std::chrono::steady_clock> previous_frame_time; 
        std::chrono::duration<float, std::ratio<1, 120>> diff = std::chrono::steady_clock::now() - previous_frame_time;
        previous_frame_time = std::chrono::steady_clock::now();
        ImGui::Text("Current FPS: %.1f", 120.0f / diff.count());
        ImGui::Separator();
    }
    ImGui::End();
}

#include "GraphicsManager.hpp"
static void ShowTextureDebugOverlay(bool* p_open, Frame& frame) {
    const float DISTANCE = 10.0f;
    static int corner = 1;
    ImGuiIO& io = ImGui::GetIO();
    if (corner != -1)
    {
        ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    if (ImGui::Begin("Resources", p_open, window_flags)) {
        if (ImGui::CollapsingHeader("Textures")) {
            #if 0
            for (int32_t i = 0; i < frame.frameContext.globalShadowMapCount; i++) {
                ImGui::Image(&frame.frameContext.globalShadowMap, ImVec2(100.0f, 100.0f));
            }

            for (int32_t i = 0; i < frame.frameContext.shadowMapCount; i++) {
                ImGui::Image(&frame.frameContext.shadowMap, ImVec2(100.0f, 100.0f));
            }
            #endif

            auto brdf_lut = g_pGraphicsManager->GetTexture("BRDF_LUT");
            ImGui::Image(brdf_lut, ImVec2(100.0f, 100.0f));
        }
    }

    ImGui::End();
}

void GuiSubPass::Draw(Frame& frame) {
    if (ImGui::GetCurrentContext()) {
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
        static bool show_static_overlay = true;
        ShowStaticsOverlay(&show_static_overlay);
        static bool show_texture_debug_overlay = true;
        ShowTextureDebugOverlay(&show_texture_debug_overlay, frame);

        ImGui::Render();
    }
}
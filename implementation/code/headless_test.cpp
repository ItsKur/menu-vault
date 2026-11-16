#include "demo_menu.h"
#include "glass_sidebar_menu.h"
#include "hud_elements.h"
#include "gamesense_menu.h"
#include "popup_menus.h"
#include "fatality_visuals.h"

// popup_menus.h input script: right-click a row, New Hotkey, hover the new entry, click Key, press G,
// cycle Mode, flip Value, hover the other entry, click away, then hover a row for its tooltip.
static void PopupScript(ImGuiIO& io, int n) {
    struct Step { int from; float x, y; };
    static const Step steps[] = { { 0, 100, 89 }, { 10, 150, 178 }, { 30, 150, 144 }, { 50, 350, 144 }, { 80, 350, 178 },
                                  { 100, 350, 223 }, { 120, 150, 110 }, { 160, 60, 300 }, { 200, 60, 49 } };
    ImVec2 m(0, 0);
    for (const Step& s : steps) if (n >= s.from) m = ImVec2(s.x, s.y);
    io.AddMousePosEvent(m.x, m.y);
    io.AddMouseButtonEvent(1, n == 5);
    io.AddMouseButtonEvent(0, n == 20 || n == 60 || n == 90 || n == 110 || n == 180);
    io.AddKeyEvent(ImGuiKey_G, n == 70);
}
#include <stdio.h>
#include "backends/imgui_impl_null.h"
#include "backends/imgui_impl_null.cpp"

int main() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    menu::ApplyStyle();
    ImGui_ImplNullPlatform_Init();
    ImGui_ImplNullRender_Init();
    ImGuiIO& io = ImGui::GetIO();
    for (int n = 0; n < 60; n++) {
        ImGui_ImplNullPlatform_NewFrame();
        ImGui_ImplNullRender_NewFrame();
        // simulate a click on the second tab and a drag on the first slider area
        io.AddMousePosEvent(n < 30 ? 150.0f : 230.0f, n < 30 ? 30.0f : 120.0f);
        io.AddMouseButtonEvent(0, (n % 10) == 5);
        ImGui::NewFrame();
        menu::DrawDemoMenu();
        ImGui::Render();
    }
    printf("demo menu, vertices last frame: %d\n", ImGui::GetDrawData()->TotalVtxCount);

    // glass_sidebar_menu.h: click a rail tab, a sub-tab, a row's "..." popup, then open search
    glass::ApplyStyle();
    const ImVec2 clicks[] = { ImVec2(37, 115), ImVec2(330, 25), ImVec2(370, 66), ImVec2(691, 25) };
    for (int n = 0; n < 80; n++) {
        ImGui_ImplNullPlatform_NewFrame();
        ImGui_ImplNullRender_NewFrame();
        const ImVec2 m = clicks[(n / 20) % 4];
        io.AddMousePosEvent(m.x, m.y);
        io.AddMouseButtonEvent(0, (n % 20) == 10);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        glass::DrawGlassMenu();
        ImGui::Render();
    }
    printf("glass menu, vertices last frame: %d\n", ImGui::GetDrawData()->TotalVtxCount);

    // hud_elements.h: every HUD element over 120 frames (toasts spawn and slide, a keybind row leaves and returns)
    for (int n = 0; n < 120; n++) {
        ImGui_ImplNullPlatform_NewFrame();
        ImGui_ImplNullRender_NewFrame();
        io.DeltaTime = 1.0f / 30.0f;
        ImGui::NewFrame();
        hud::DrawHudDemo();
        ImGui::Render();
    }
    printf("hud demo, vertices last frame: %d\n", ImGui::GetDrawData()->TotalVtxCount);

    // gamesense_menu.h: click a rail tab, a checkbox, open a combo and pick an entry, drag a slider, open a swatch
    gs::ApplyStyle();
    const ImVec2 gs_clicks[] = { ImVec2(42, 111), ImVec2(130, 46), ImVec2(150, 66), ImVec2(150, 108), ImVec2(200, 158),
                                 ImVec2(321, 397) };
    for (int n = 0; n < 120; n++) {
        ImGui_ImplNullPlatform_NewFrame();
        ImGui_ImplNullRender_NewFrame();
        io.DeltaTime = 1.0f / 60.0f;
        const ImVec2 m = gs_clicks[(n / 20) % 6];
        io.AddMousePosEvent(m.x, m.y);
        io.AddMouseButtonEvent(0, (n % 20) == 10);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        gs::DrawGamesenseMenu();
        ImGui::Render();
    }
    printf("gamesense menu, vertices last frame: %d\n", ImGui::GetDrawData()->TotalVtxCount);

    // popup_menus.h: the hotkey menu, its submenu and key capture, then a docked tooltip
    for (int n = 0; n < 260; n++) {
        ImGui_ImplNullPlatform_NewFrame();
        ImGui_ImplNullRender_NewFrame();
        io.DeltaTime = 1.0f / 60.0f;
        PopupScript(io, n);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        pm::DrawPopupDemo();
        ImGui::Render();
    }
    printf("popup menus, vertices last frame: %d\n", ImGui::GetDrawData()->TotalVtxCount);

    // fatality_visuals.h: the in-world demo scene for 90 frames (bars drain, toasts slide in, arrows blink)
    for (int n = 0; n < 90; n++) {
        ImGui_ImplNullPlatform_NewFrame();
        ImGui_ImplNullRender_NewFrame();
        io.DeltaTime = 1.0f / 60.0f;
        ImGui::NewFrame();
        fv::DrawDemoScene();
        ImGui::Render();
    }
    printf("fatality visuals, vertices last frame: %d\n", ImGui::GetDrawData()->TotalVtxCount);

    // menu_widgets.h ComboRow + ColorRow: open the list and pick the third entry, open the picker, drag in its SV square
    menu::ApplyStyle();
    {
        int pick = 0;
        ImVec4 tint(0.2f, 0.4f, 0.8f, 1.0f);
        const char* items[] = { "Option 1", "Option 2", "Option 3" };
        bool picker_open = false;
        const ImVec2 rows_clicks[] = { ImVec2(230, 24), ImVec2(230, 88), ImVec2(270, 52), ImVec2(150, 100) };
        for (int n = 0; n < 70; n++) {
            ImGui_ImplNullPlatform_NewFrame();
            ImGui_ImplNullRender_NewFrame();
            io.DeltaTime = 1.0f / 60.0f;
            const ImVec2 m = rows_clicks[ImMin(n / 15, 3)];
            io.AddMousePosEvent(m.x, m.y);
            io.AddMouseButtonEvent(0, n == 5 || n == 20 || n == 35 || (n >= 50 && n < 55));
            ImGui::NewFrame();
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Always);
            ImGui::Begin("##rows", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
            menu::ComboRow("Mode", &pick, items, 3);
            menu::ColorRow("Tint", &tint);
            picker_open = ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId);
            ImGui::End();
            ImGui::Render();
        }
        const bool tint_changed = tint.x != 0.2f || tint.y != 0.4f || tint.z != 0.8f;
        printf("combo/color rows: picked %d, picker open %d, tint changed %d\n", pick, (int)picker_open, (int)tint_changed);
        if (pick != 2 || !picker_open || !tint_changed) { puts("FAIL: ComboRow/ColorRow"); return 1; }
    }

    ImGui_ImplNullRender_Shutdown();
    ImGui_ImplNullPlatform_Shutdown();
    ImGui::DestroyContext();
    puts("OK");
}

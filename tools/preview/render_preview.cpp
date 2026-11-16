// Headless CPU rasterizer for ImDrawData -> PPM, for preview images only.
#include "demo_menu.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "backends/imgui_impl_null.h"
#include "backends/imgui_impl_null.cpp"

static const int W = 900, H = 600, SS = 3, SW = W * SS, SH = H * SS;
static std::vector<float> fb(SW * SH * 3);

static void Blend(int x, int y, float r, float g, float b, float a) {
    float* p = &fb[(y * SW + x) * 3];
    p[0] = p[0] * (1 - a) + r * a; p[1] = p[1] * (1 - a) + g * a; p[2] = p[2] * (1 - a) + b * a;
}

static float SampleA(ImTextureData* t, float u, float v, float* rgb) {
    rgb[0] = rgb[1] = rgb[2] = 1.0f;
    if (!t || !t->Pixels) return 1.0f;
    int xx = ImClamp((int)(u * t->Width), 0, t->Width - 1), yy = ImClamp((int)(v * t->Height), 0, t->Height - 1);
    unsigned char* px = t->Pixels + (yy * t->Width + xx) * t->BytesPerPixel;
    float acc[4];
    if (t->BytesPerPixel == 1) { acc[0] = acc[1] = acc[2] = 1.0f; acc[3] = px[0] / 255.f; }
    else for (int k = 0; k < 4; k++) acc[k] = px[k] / 255.f;
    rgb[0] = acc[0]; rgb[1] = acc[1]; rgb[2] = acc[2];
    return acc[3];
}

static void Raster(ImDrawData* dd) {
    for (ImDrawList* dl : dd->CmdLists) {
        for (const ImDrawCmd& cmd : dl->CmdBuffer) {
            if (cmd.UserCallback) continue;
            ImTextureData* tex = cmd.TexRef._TexData;
            for (unsigned e = 0; e < cmd.ElemCount; e += 3) {
                ImVec2 P[3]; const ImDrawVert* v[3];
                for (int k = 0; k < 3; k++) { v[k] = &dl->VtxBuffer[cmd.VtxOffset + dl->IdxBuffer[cmd.IdxOffset + e + k]]; P[k] = ImVec2(v[k]->pos.x * SS, v[k]->pos.y * SS); }
                float minx = std::max({cmd.ClipRect.x * SS, 0.f, std::min({P[0].x, P[1].x, P[2].x})});
                float maxx = std::min({cmd.ClipRect.z * SS, (float)SW, std::max({P[0].x, P[1].x, P[2].x})});
                float miny = std::max({cmd.ClipRect.y * SS, 0.f, std::min({P[0].y, P[1].y, P[2].y})});
                float maxy = std::min({cmd.ClipRect.w * SS, (float)SH, std::max({P[0].y, P[1].y, P[2].y})});
                float area = (P[1].x - P[0].x) * (P[2].y - P[0].y) - (P[2].x - P[0].x) * (P[1].y - P[0].y);
                if (fabsf(area) < 1e-9f) continue;
                for (int y = (int)floorf(miny); y < (int)ceilf(maxy); y++)
                for (int x = (int)floorf(minx); x < (int)ceilf(maxx); x++) {
                    float px = x + 0.5f, py = y + 0.5f;
                    float w0 = ((P[1].x - px) * (P[2].y - py) - (P[2].x - px) * (P[1].y - py)) / area;
                    float w1 = ((P[2].x - px) * (P[0].y - py) - (P[0].x - px) * (P[2].y - py)) / area;
                    float w2 = 1 - w0 - w1;
                    // top-left-ish rule: include one shared edge, exclude the other, so fans don't double-blend
                    if (w0 < 0 || w1 < 0 || w2 < 0) continue;
                    if (w0 == 0 || w1 == 0 || w2 == 0) { if (area > 0 ? (w0 == 0) : (w2 == 0)) continue; }
                    float u = w0 * v[0]->uv.x + w1 * v[1]->uv.x + w2 * v[2]->uv.x;
                    float vv = w0 * v[0]->uv.y + w1 * v[1]->uv.y + w2 * v[2]->uv.y;
                    float trgb[3]; float ta = SampleA(tex, u, vv, trgb);
                    float col[4];
                    for (int k = 0; k < 4; k++) {
                        float c0 = ((v[0]->col >> (8 * k)) & 0xFF) / 255.f, c1 = ((v[1]->col >> (8 * k)) & 0xFF) / 255.f, c2 = ((v[2]->col >> (8 * k)) & 0xFF) / 255.f;
                        col[k] = w0 * c0 + w1 * c1 + w2 * c2;
                    }
                    float a = col[3] * ta;
                    if (a > 0) Blend(x, y, col[0] * trgb[0], col[1] * trgb[1], col[2] * trgb[2], a);
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    const char* regular = argc > 1 ? argv[1] : nullptr;
    const char* bold = argc > 2 ? argv[2] : nullptr;
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    menu::ApplyStyle();
    menu::Fonts fonts;
    if (regular && bold) { fonts = menu::LoadFonts(regular, bold); io.FontDefault = fonts.label; }
    ImGui_ImplNullPlatform_Init();
    ImGui_ImplNullRender_Init();
    for (int n = 0; n < 90; n++) {
        ImGui_ImplNullPlatform_NewFrame();
        ImGui_ImplNullRender_NewFrame();
        io.DisplaySize = ImVec2(W, H);
        io.DeltaTime = 1.0f / 60.0f;
        io.AddMousePosEvent(-100, -100);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(70, 60), ImGuiCond_Always);
        menu::DrawDemoMenu(regular ? &fonts : nullptr);
        ImGui::Render();
    }
    // backdrop: dark neutral
    for (int i = 0; i < SW * SH; i++) { fb[i*3] = 0.10f; fb[i*3+1] = 0.11f; fb[i*3+2] = 0.13f; }
    Raster(ImGui::GetDrawData());
    FILE* f = fopen("preview.ppm", "wb");
    fprintf(f, "P6 %d %d 255\n", W, H);
    for (int y = 0; y < H; y++) for (int x = 0; x < W; x++) for (int k = 0; k < 3; k++) {
        float acc = 0;
        for (int sy = 0; sy < SS; sy++) for (int sx = 0; sx < SS; sx++) acc += fb[((y * SS + sy) * SW + (x * SS + sx)) * 3 + k];
        fputc((int)(ImSaturate(acc / (SS * SS)) * 255.f + 0.5f), f);
    }
    fclose(f);
    ImGui_ImplNullRender_Shutdown(); ImGui_ImplNullPlatform_Shutdown();
    ImGui::DestroyContext();
    puts("rendered");
}

#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

static void BusMeter(void);
static void MasterOutMeter(void);

void ImGuiAdx::MixerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
    static int32_t selected_bus_index = 0;
    auto speaker_mapping = ADXRuntime::GetSpeakerMapping();
    auto [num_rendered_samples, num_rendered_sampling_rate] = ADXRuntime::GetNumRenderedSamples();
    auto [num_output_samples, num_output_sampling_rate] = ADXRuntime::GetNumOutputSamples();
    auto [result, bus_info] = ADXRuntime::GetBusInfo(selected_bus_index);
    auto info = ADXRuntime::GetPerformanceInfo();
    auto bus_names = ADXRuntime::GetBusNames();
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    ImGui::Begin("Mixer", is_open, ImGuiWindowFlags_NoSavedSettings);

    if (ImGui::BeginTabBar("TabBar##Mixer", tab_bar_flags)) {
        if (ImGui::BeginTabItem("Info")) {
            ImGui::SeparatorText("Config");
            if (ImGui::BeginTable("Table##Config", 2)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Speaker Mapping");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%s", ADXUtils::GetSpeakerMappingString(speaker_mapping));
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Rendered Samples");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%lld", num_rendered_samples);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Output Samples");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%lld", num_output_samples);
                ImGui::TableNextRow();
                ImGui::EndTable();
            }

            ImGui::SeparatorText("Performance");
            if (ImGui::BeginTable("Table##Performance", 2)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Last Process Time");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d (us)", info.last_process_time);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Last Process Interval");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d (us)", info.last_process_interval);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Last Process Samples");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d (us)", info.last_process_samples);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Max Process Time");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d (us)", info.max_process_time);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Max Process Interval");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d (us)", info.max_process_interval);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Max Process Samples");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d (us)", info.max_process_samples);
                ImGui::TableNextRow();
                ImGui::EndTable();
            }

            if (ImGui::Button("Reset")) {
                ADXRuntime::ResetPerformanceInfo();
            }

            ImGui::SeparatorText("Bus");
            if (!bus_names.empty()) {
                ImGuiUtils::Comboui("Bus", &selected_bus_index, &bus_names);
            }

            if (ImGui::BeginTable("Table##Bus", 2)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Bus Name");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%s", bus_info.name);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Bus Volume");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%.3f", bus_info.volume);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Bus Pan3D Volume");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%.3f", bus_info.pan3d_volume);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Bus Pan3D Spread");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%.3f", bus_info.pan3d_spread);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Bus Pan3D Distance");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%.3f", bus_info.pan3d_distance);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Bus Pan3D Angle");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%.3f", bus_info.pan3d_angle);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Bus Number Of AFXes");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d", bus_info.num_fxes);
                ImGui::TableNextRow();
                ImGui::EndTable();
            }

            ImGui::SeparatorText("Effect");
            if (bus_info.num_fxes > 0) {
                auto afx_names = ADXRuntime::GetAfxNames(selected_bus_index);
                if (ImGui::BeginTable("AFX Info Table", 2)) {
                    for (const auto& name : afx_names) {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "EffectName");
                        ImGui::TableSetColumnIndex(1); ImGui::Text("%s", name.c_str());
                    }
                    ImGui::EndTable();
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Meter")) {
            if (ADXRuntime::GetBusNames().empty()) {
                MasterOutMeter();
            } else {
                BusMeter();
            }
            ImGui::EndTabItem();
        }
    }
    ImGui::EndTabBar();


    ImGui::End();
}

static void BusMeter(void)
{
    int32_t max_channels;
    std::vector<float> rms_values, peak_values, peak_hold_values;
    float histogram_width = 100.0f;
    float histogram_height = 300.0f;
    float padding_histogram_width = 30.0f;
    ImFont* font = ImGui::GetFont();
    float base_size = ImGui::GetFontSize();
    auto bus_names = ADXRuntime::GetBusNames();

    for (auto i = 0; i < bus_names.size(); i++) {
        ImDrawList* draw_list;
        ImVec2 cursor_position;
        ImVec2 base;
        float font_scale;
        const char* bus_name = bus_names.at(i).c_str();
        auto [result, bus_info] = ADXRuntime::GetBusInfo(i);
        std::map<int32_t, std::string> meter_label;

        max_channels = criAtomExAsrRack_GetNumBusChannelsByName(CRIATOMEXASR_RACK_DEFAULT_ID, bus_name);
        rms_values.resize(max_channels);
        peak_values.resize(max_channels);
        peak_hold_values.resize(max_channels);

        criAtomExAsrRack_GetBusRmsLevelByName(CRIATOMEXASR_RACK_DEFAULT_ID, bus_name, rms_values.data(), max_channels);
        criAtomExAsrRack_GetBusPeakLevelByName(CRIATOMEXASR_RACK_DEFAULT_ID, bus_name, peak_values.data(), max_channels);
        criAtomExAsrRack_GetBusPeakHoldLevelByName(CRIATOMEXASR_RACK_DEFAULT_ID, bus_name, peak_hold_values.data(), max_channels);
        meter_label = ADXUtils::GetSpeakerMappingLabel(bus_info.speaker_mapping);

        /* パディングサイズを最大チャンネル数から求める */
        padding_histogram_width = max_channels * 10.0f;

        /* パディングによりヒストグラム間を調整 */
        ImGui::SameLine(0.0f, 50.0f);

        /* ヒストグラム描画位置の高さ方向を固定 */
        ImGui::SetCursorPosY(ImGui::GetWindowSize().y / 5.0f);

        /* 描画対象リストを作成 */
        draw_list = ImGui::GetWindowDrawList();

        /* グループ化して表示 */
        ImGui::BeginGroup();

        /* ヒストグラムの上に表示 */
        ImGui::SetCursorPosX(ImGui::GetCursorPosX());
        ImGui::Text("%s", bus_name);

        /* 現在のカーソル位置を取得 */
        cursor_position = ImGui::GetCursorScreenPos();
        base = ImGui::GetCursorScreenPos();

        /* チャンネル数に応じてフォントを 0.7～1.0 倍に縮小   */
        font_scale = ImClamp(1.0f - 0.05f * (max_channels - 1), 0.4f, 1.0f);

        for (auto ch = 0; ch < max_channels; ch++) {
            const char* label = meter_label[ch].c_str();
            /* 各バーの矩形領域*/
            ImVec2 bar_size{ histogram_width / max_channels, histogram_height };
            ImVec2 p0(base.x + ch * (bar_size.x + 6.0f), base.y);
            ImVec2 p1(p0.x + bar_size.x, p0.y + bar_size.y);
            ImVec2 fill0;
            ImVec2 label_sz = { ImGui::CalcTextSize(label).x * font_scale, ImGui::CalcTextSize(label).y * font_scale };
            float text_gap = 4.0f;
            float hight_rms, hight_peak, hight_peak_hold;
            float label_x = p0.x + (bar_size.x - label_sz.x) * 0.5f;
            float label_y = p1.y + text_gap;

            /* 背景 */
            draw_list->AddRectFilled(p0, p1, IM_COL32(40, 40, 40, 255), 3.0f);

            /* RMS（常時塗り） */
            hight_rms = ADXUtils::NormalizeDecibel(ADXUtils::LevelToDecibel(rms_values.at(ch))) * bar_size.y;
            fill0 = ImVec2(p0.x, p1.y - hight_rms);
            draw_list->AddRectFilled(fill0, p1, IM_COL32(0, 200, 0, 255), 3.0f, ImDrawFlags_RoundCornersBottom);

            /* Peak（赤線）*/
            hight_peak = ADXUtils::NormalizeDecibel(ADXUtils::LevelToDecibel(peak_values.at(ch))) * bar_size.y;
            draw_list->AddLine(ImVec2(p0.x, p1.y - hight_peak),
                ImVec2(p1.x, p1.y - hight_peak),
                IM_COL32(255, 100, 100, 255), 2.0f);

            /* Peak‑Hold（黄線）*/
            hight_peak_hold = ADXUtils::NormalizeDecibel(ADXUtils::LevelToDecibel(peak_hold_values.at(ch))) * bar_size.y;
            draw_list->AddLine(ImVec2(p0.x, p1.y - hight_peak_hold),
                ImVec2(p1.x, p1.y - hight_peak_hold),
                IM_COL32(255, 200, 40, 255), 2.0f);

            /* ラベル（中央揃え・縮小フォント） --------------------*/
            draw_list->AddText(font, base_size * font_scale,
                ImVec2(label_x, label_y),
                IM_COL32_WHITE, label);
        }
        /* パディング付きのヒストグラムを提示して次の描画位置を整える */
        ImGui::Dummy(ImVec2(histogram_width + padding_histogram_width, histogram_height));
        /* グループ化終了 */
        ImGui::EndGroup();
    }
}

static void MasterOutMeter(void)
{
    float rms_value, peak_value, peak_hold_value;
    float histogram_width = 100.0f;
    float histogram_height = 300.0f;
    float padding_histogram_width = 30.0f;
    ImFont* font = ImGui::GetFont();
    float base_size = ImGui::GetFontSize();
    ImDrawList* draw_list;
    ImVec2 cursor_position;
    ImVec2 base;
    float font_scale;
    std::map<int32_t, std::string> meter_label;
    CriAtomLevelInfo level_info;

    criAtomExAsrRack_GetLevelInfo(CRIATOMEXASR_RACK_DEFAULT_ID, &level_info);

    meter_label = ADXUtils::GetSpeakerMappingLabel(criAtomExAsrRack_GetSpeakerMapping(CRIATOMEXASR_RACK_DEFAULT_ID));

    /* パディングサイズを最大チャンネル数から求める */
    padding_histogram_width = level_info.num_channels * 10.0f;

    /* パディングによりヒストグラム間を調整 */
    ImGui::SameLine(0.0f, 50.0f);

    /* ヒストグラム描画位置の高さ方向を固定 */
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y / 5.0f);

    /* 描画対象リストを作成 */
    draw_list = ImGui::GetWindowDrawList();

    /* グループ化して表示 */
    ImGui::BeginGroup();

    /* ヒストグラムの上に表示 */
    ImGui::SetCursorPosX(ImGui::GetCursorPosX());
    ImGui::Text("%s", "MasterOut");

    /* 現在のカーソル位置を取得 */
    cursor_position = ImGui::GetCursorScreenPos();
    base = ImGui::GetCursorScreenPos();

    /* チャンネル数に応じてフォントを 0.7～1.0 倍に縮小   */
    font_scale = ImClamp(1.0f - 0.05f * (level_info.num_channels - 1), 0.4f, 1.0f);

    for (auto ch = 0; ch < level_info.num_channels; ch++) {
        const char* label = meter_label[ch].c_str();
        /* 各バーの矩形領域*/
        ImVec2 bar_size{ histogram_width / level_info.num_channels, histogram_height };
        ImVec2 p0(base.x + ch * (bar_size.x + 6.0f), base.y);
        ImVec2 p1(p0.x + bar_size.x, p0.y + bar_size.y);
        ImVec2 fill0;
        ImVec2 label_sz = { ImGui::CalcTextSize(label).x * font_scale, ImGui::CalcTextSize(label).y * font_scale };
        float text_gap = 4.0f;
        float hight_rms, hight_peak, hight_peak_hold;
        float label_x = p0.x + (bar_size.x - label_sz.x) * 0.5f;
        float label_y = p1.y + text_gap;

        rms_value = ADXUtils::NormalizeDecibel(level_info.rms_levels[ch]);
        peak_value = ADXUtils::NormalizeDecibel(level_info.peak_levels[ch]);
        peak_hold_value = ADXUtils::NormalizeDecibel(level_info.peak_hold_levels[ch]);

        /* 背景 */
        draw_list->AddRectFilled(p0, p1, IM_COL32(40, 40, 40, 255), 3.0f);

        /* RMS（常時塗り） */
        hight_rms = ADXUtils::NormalizeDecibel(ADXUtils::LevelToDecibel(rms_value)) * bar_size.y;
        fill0 = ImVec2(p0.x, p1.y - hight_rms);
        draw_list->AddRectFilled(fill0, p1, IM_COL32(0, 200, 0, 255), 3.0f, ImDrawFlags_RoundCornersBottom);

        /* Peak（赤線）*/
        hight_peak = ADXUtils::NormalizeDecibel(ADXUtils::LevelToDecibel(peak_value)) * bar_size.y;
        draw_list->AddLine(ImVec2(p0.x, p1.y - hight_peak),
            ImVec2(p1.x, p1.y - hight_peak),
            IM_COL32(255, 100, 100, 255), 2.0f);

        /* Peak‑Hold（黄線）*/
        hight_peak_hold = ADXUtils::NormalizeDecibel(ADXUtils::LevelToDecibel(peak_hold_value)) * bar_size.y;
        draw_list->AddLine(ImVec2(p0.x, p1.y - hight_peak_hold),
            ImVec2(p1.x, p1.y - hight_peak_hold),
            IM_COL32(255, 200, 40, 255), 2.0f);

        /* ラベル（中央揃え・縮小フォント） --------------------*/
        draw_list->AddText(font, base_size * font_scale,
            ImVec2(label_x, label_y),
            IM_COL32_WHITE, label);
    }
    /* パディング付きのヒストグラムを提示して次の描画位置を整える */
    ImGui::Dummy(ImVec2(histogram_width + padding_histogram_width, histogram_height));
    /* グループ化終了 */
    ImGui::EndGroup();
}

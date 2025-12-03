#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

void ImGuiAdx::InfoWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
    static int32_t selected_cue_index = 0;
    auto cue_names = ADXRuntime::GetCueNames();
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    ImGui::Begin("Info", is_open, ImGuiWindowFlags_NoSavedSettings);

    ImGui::SeparatorText("ACF Info");
    if (auto [result, info] = ADXRuntime::GetAcfInfo(); result) {
        if (ImGui::BeginTable("ACF Info Table", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Name");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", info.name);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Size");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.size);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Version");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.version);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of DSP Settings");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_dsp_settings);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Categories");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_categories);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Categories / Playback");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_categories_per_playback);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Reacts");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_reacts);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of AISAC Controls");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_aisac_controls);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Global AISAC Controls");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_global_aisacs);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Game Variables");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_game_variables);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Num Buses");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_buses);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Max Buses of DSP Bus Settings");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.max_buses_of_dsp_bus_settings);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Voice Limit Groups");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_voice_limit_groups);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of OutputPorts");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_output_ports);
            ImGui::TableNextRow();
            ImGui::EndTable();
        }
    }

    ImGui::SeparatorText("ACB Info");
    if (auto [result, info] = ADXRuntime::GetAcbInfo(); result) {
        if (ImGui::BeginTable("ACB Info Table", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Name");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", info.name);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Version");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.version);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Encoding of Char");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.character_encoding);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Volume");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%.3f", info.volume);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Cues");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.num_cues);
            ImGui::TableNextRow();
            ImGui::EndTable();
        }
    }

    ImGui::SeparatorText("Cue Info");
    if (!cue_names.empty()) {
        ImGuiUtils::Comboui("Cue", &selected_cue_index, &cue_names);
        if (auto [result, info] = ADXRuntime::GetCueInfo(cue_names.at(selected_cue_index).c_str()); result) {
            if (ImGui::BeginTable("Cue Info Table", 2)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "ID");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.id);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "name");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%s", info.name);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Type");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.type);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Volume");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%.3f", info.volume);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Length");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%lld", info.length);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "PanType");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%s", ADXUtils::GetPanTypeString(info.pan_type));
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Min Distance");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%.3f", info.pos3d_info.min_distance);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Max Distance");
                ImGui::TableSetColumnIndex(1); ImGui::Text("%.3f", info.pos3d_info.max_distance);
                ImGui::TableNextRow();
                ImGui::EndTable();
            }
        }
    }


    ImGui::End();
}

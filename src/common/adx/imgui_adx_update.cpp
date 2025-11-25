#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

static void VoicePoolWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
static void InfoWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
static void PlayerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);

void ImGuiAdx::Update(const ImVec2 size, const ImVec2 pos)
{
	static bool is_open_voicepool_window = false;
    static bool is_open_info_window = false;
	static bool is_open_player_window = false;

	/* 未初期化なら何もしない */
	if (!ADXRuntime::IsInitilaized()) {
		return;
	}

	ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

	ImGui::Begin("Update", nullptr, ImGuiWindowFlags_NoSavedSettings);

	ImGui::Text("Error : %s", ADXUtils::GetErrorMessage().c_str());
    if (ImGui::TreeNode("Info")) {
        if (ImGui::Button("Open")) {
            is_open_info_window = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            is_open_info_window = false;
        }
        if (is_open_info_window) {
            ImVec2 info_window_size { size.x + 200.0f, size.y };
            InfoWindow(info_window_size, ImGuiUtils::AddOffsetX(pos, 200.0f), &is_open_info_window);
        }
        ImGui::TreePop();
    }
	if (ImGui::TreeNode("VoicePool")) {
		if (ImGui::Button("Open")) {
			is_open_voicepool_window = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			is_open_voicepool_window = false;
		}
		if (is_open_voicepool_window) {
            auto vp_window_size = ImGuiUtils::AddOffset(size, 100.0f);
			VoicePoolWindow(vp_window_size, ImGuiUtils::AddOffsetX(pos, 300.0f), &is_open_voicepool_window);
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Player")) {
		if (ImGui::Button("Open")) {
			is_open_player_window = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			is_open_player_window = false;
		}
		if (is_open_player_window) {
            ImVec2 player_window_size { size.x + 200.0f, size.y };
			PlayerWindow(player_window_size, ImGuiUtils::AddOffsetX(pos, 200.0f), &is_open_player_window);
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

static void VoicePoolWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
    ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoHostExtendX;
	ImGui::SetNextWindowPos(ImGuiUtils::AddOffsetX(pos, 300.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
	ImGui::Begin("VoicePool", is_open, ImGuiWindowFlags_NoSavedSettings);
	std::vector<std::string> names;
	static int32_t selected_index;
	static VoiceType voice_type = VoiceType::Standard;
    std::vector<VoiceType> voice_types = {
		VoiceType::Standard,
		VoiceType::RawPcm,
		VoiceType::Wave
    };

	std::transform(voice_types.begin(), voice_types.end(), std::back_inserter(names), [](const VoiceType v) { return ADXUtils::GetVoiceTypeString(v); });
	ImGuiUtils::Comboui("VoiceType", &selected_index, &names);
	voice_type = voice_types.at(selected_index);

    if (ADXRuntime::vp.GetVoicePoolHn(voice_type) != NULL) {
        auto [num_used_voices, num_max_voices] = ADXRuntime::GetNumUsedVoicePools(voice_type);
        if (ImGui::BeginTable("Voice Pool Info Table", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Ptr");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%p", ADXRuntime::vp.GetVoicePoolHn(VoiceType::Standard));
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Used / Max");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%3d / %3d", num_used_voices, num_max_voices);
            ImGui::EndTable();
        }

        if (ImGui::BeginTable("Table##Voice Pool", 2, flags)) {
            ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);
            ImGui::TableSetupColumn("Ptr", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);
            ImGui::TableHeadersRow();
            for (int32_t i = 0; i < num_max_voices; i++) {
                auto vp_hn = ADXRuntime::vp.GetVoicePoolHn(voice_type);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%5d", i);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%5p", criAtomExVoicePool_GetPlayerHandle(vp_hn, i));
            }
            ImGui::EndTable();
        }
    }

	ImGui::End();
}

static void InfoWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
    ImGui::SetNextWindowPos(ImGuiUtils::AddOffsetX(pos, 300.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    ImGui::Begin("Info", is_open, ImGuiWindowFlags_NoSavedSettings);
    
    auto [result, acf_info] = ADXRuntime::GetAcfInfo();
    
    ImGui::SeparatorText("ACF Info");
    if (result) {
        if (ImGui::BeginTable("ACF Info Table", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Name");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", acf_info.name);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Size");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.size);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Version");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.version);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of DSP Settings");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_dsp_settings);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Categories");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_categories);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Categories / Playback");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_categories_per_playback);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Reacts");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_reacts);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of AISAC Controls");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_aisac_controls);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Global AISAC Controls");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_global_aisacs);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Game Variables");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_game_variables);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Num Buses");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_buses);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Max Buses of DSP Bus Settings");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.max_buses_of_dsp_bus_settings);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Voice Limit Groups");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_voice_limit_groups);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of OutputPorts");
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", acf_info.num_output_ports);
            ImGui::TableNextRow();
            ImGui::EndTable();
        }
    }
    
    ImGui::End();
}

static void PlayerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
	ImGui::SetNextWindowPos(ImGuiUtils::AddOffsetX(pos, 300.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
	ImGui::Begin("Player", is_open, ImGuiWindowFlags_NoSavedSettings);
	static int32_t selected_index = 0;
	std::vector<std::string> names;
	CriAtomExPlayerHn player;
    static bool is_auto_update = false;
    static bool is_loop = false;

	names.resize(ADXRuntime::player.num_players);
	for (int32_t i = 0; i < ADXRuntime::player.num_players; i++) {
		names.at(i) = "PlayerIndex : " + std::to_string(i);
	}
	ImGuiUtils::Comboui("Player", &selected_index, &names);

	player = ADXRuntime::player.GetPlayerHn(selected_index);
    
    ImGui::Separator();
	if (ImGui::BeginTable("Table##player", 2)) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Player Ptr");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%p", player);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Player Status");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%s", ADXUtils::GetPlayerStatusString(player).c_str());
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Player Samples");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%lld", criAtomExPlayer_GetTimeReal(player));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Playbacks");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%d", criAtomExPlayer_GetNumPlaybacks(player));
		ImGui::TableNextRow();
		ImGui::EndTable();
	}
    
    ImGui::Separator();
    if (ImGui::Button("Start")) {
        auto playback_id = criAtomExPlayer_Start(player);
        ADXRuntime::playback_ids.push_back(playback_id);
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        criAtomExPlayer_Stop(player);
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop without Release Time")) {
        criAtomExPlayer_StopWithoutReleaseTime(player);
    }
    ImGui::SameLine();
    if (ImGui::Button("Update")) {
        criAtomExPlayer_UpdateAll(player);
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        criAtomExPlayer_ResetParameters(player);
        criAtomExPlayer_UpdateAll(player);
    }
    if (ImGui::Button("Pause")) {
        criAtomExPlayer_Pause(player, CRI_TRUE);
    }
    ImGui::SameLine();
    if (ImGui::Button("Prepare")) {
        criAtomExPlayer_Prepare(player);
    }
    ImGui::SameLine();
    if (ImGui::Button("Resume")) {
        criAtomExPlayer_Resume(player, CRIATOMEX_RESUME_PREPARED_PLAYBACK);
    }
    ImGui::SameLine();
    ImGui::Checkbox("Auto Update", &is_auto_update);
    if (is_auto_update != false) {
        criAtomExPlayer_UpdateAll(player);
    }
    ImGui::SameLine();
    ImGui::Checkbox("Loop Play", &is_loop);
    if (is_loop != false) {
        criAtomExPlayer_LimitLoopCount(player, CRIATOMEXPLAYER_FORCE_LOOP);
    }
    ImGui::Separator();
    if (ImGui::TreeNode("Volume")) {
        static float volume = 1.0f;
        ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
        criAtomExPlayer_SetVolume(player, volume);
        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::TreeNode("Start Time")) {
        static ImU32 start_time = 0;
        const float drag_speed = 0.2f;
        const ImU32 u32_zero = 0, u32_fifty = 50;
        static bool drag_clamp = false;
        ImGui::Checkbox("Clamp integers to 0..50", &drag_clamp);
        ImGui::DragScalar("Start Time[ms]", ImGuiDataType_U32, &start_time, drag_speed, drag_clamp ? &u32_zero : NULL, drag_clamp ? &u32_fifty : NULL, "%u ms");
        criAtomExPlayer_SetStartTime(player, start_time);
        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::TreeNode("Pitch")) {
        static float pitch = 0.0f;
        ImGui::InputFloat("Pitch", &pitch);
        pitch = (pitch < 0.0f) ? 0.0f : pitch;
        pitch = (pitch > 192000) ? 192000 : pitch;
        criAtomExPlayer_SetPitch(player, pitch);
        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::TreeNode("Pan3d")) {
        static float pan3d_angle = 0.0f;
        static float pan3d_elevation = 0.0f;
        static float pan3d_distance = 1.0f;

        criAtomExPlayer_SetPanType(player, CRIATOMEX_PAN_TYPE_PAN3D);

        ImGui::SliderFloat("Pan3d Angle", &pan3d_angle, -180.0f, 180.0f);
        ImGui::SliderFloat("Pan3d Elevation", &pan3d_elevation, -180.0f, 180.0f);
        ImGui::SliderFloat("Pan3d Distance", &pan3d_distance, 0.0f, 1.0f);
        criAtomExPlayer_SetPan3dAngle(player, pan3d_angle);
#if (CRI_ATOM_VER_MINOR) >= (28)
        criAtomExPlayer_SetPan3dElevation(player, pan3d_elevation);
#endif
        criAtomExPlayer_SetPan3dInteriorDistance(player, pan3d_distance);

        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::TreeNode("AISAC")) {
        static int32_t aisac_cointrol_id = 0;
        static float aisac_control_value = 0.0f;
        ImGui::InputInt("Control ID", &aisac_cointrol_id);
        ImGui::SliderFloat("Control Value", &aisac_control_value, 0.0f, 1.0f);
        criAtomExPlayer_SetAisacControlById(player, aisac_cointrol_id, aisac_control_value);
        ImGui::TreePop();
    }
    
	ImGui::End();
}

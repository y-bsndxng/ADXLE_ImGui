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
            ImVec2 window_size{ size.x + 200.0f, size.y };
            InfoWindow(window_size, ImGuiUtils::AddOffsetX(pos, 500.0f), &is_open_info_window);
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
            auto window_size = ImGuiUtils::AddOffset(size, 100.0f);
			VoicePoolWindow(window_size, ImGuiUtils::AddOffsetX(pos, 500.0f), &is_open_voicepool_window);
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
            ImVec2 window_size{ size.x + 200.0f, size.y };
			PlayerWindow(window_size, ImGuiUtils::AddOffsetX(pos, 500.0f), &is_open_player_window);
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

static void InfoWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
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

static void VoicePoolWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
    ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoHostExtendX;
	ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
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

static void PlayerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
    ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoHostExtendX;
    static int32_t selected_cue_index = 0;
    static int32_t selected_player_index = 0;
    static bool is_auto_update = false;
    static bool is_loop = false;
    auto cue_names = ADXRuntime::GetCueNames();
    std::vector<std::string> player_names;
    CriAtomExPlayerHn player;
	ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
	ImGui::Begin("Player", is_open, ImGuiWindowFlags_NoSavedSettings);

    player_names.resize(ADXRuntime::player.num_players);
	for (int32_t i = 0; i < ADXRuntime::player.num_players; i++) {
        player_names.at(i) = "PlayerIndex : " + std::to_string(i);
	}
	ImGuiUtils::Comboui("Player", &selected_player_index, &player_names);

	player = ADXRuntime::player.GetPlayerHn(selected_player_index);
    
    ImGui::Separator();
	if (ImGui::BeginTable("Table##player", 2)) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Player Ptr");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%p", player);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Player Status");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%s", ADXUtils::GetPlayerStatusString(player));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Player Samples");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%lld", criAtomExPlayer_GetTimeReal(player));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Playbacks");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%d", criAtomExPlayer_GetNumPlaybacks(player));
		ImGui::TableNextRow();
		ImGui::EndTable();
	}
    
    if (!cue_names.empty()) {
        ImGuiUtils::Comboui("Cue", &selected_cue_index, &cue_names);
    }
    ImGui::Separator();
    if (ImGui::Button("Start")) {
        auto name = cue_names.at(selected_cue_index).c_str();
        criAtomExPlayer_SetCueName(player, ADXRuntime::acb_hn, name);
        ADXRuntime::playback_ids.push_back(criAtomExPlayer_Start(player));
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
    ImGui::Separator();
    if (ImGui::TreeNode("Playback")) {
        if (ImGui::BeginTable("PlaybackTable", 4, flags)) {
            ImGui::TableSetupColumn("Playback ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);
            ImGui::TableSetupColumn("Played Samples", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);
            ImGui::TableSetupColumn("Playback Status", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);
            ImGui::TableSetupColumn("Player Ptr", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);
            ImGui::TableHeadersRow();

            /* Playback Status の表示 */
            if (ADXRuntime::playback_ids.empty() == false) {
                for (auto playback_id_ptr = ADXRuntime::playback_ids.begin(); playback_id_ptr != ADXRuntime::playback_ids.end(); playback_id_ptr++) {
                    int32_t column = 0;
                    CriSint32 sampling_rate = 0;
                    CriSint64 played_samples = 0;
                    CriAtomExPlaybackStatus playback_status;
                    char buffer[256] = "";
                    const char* playback_status_name = "";
                    playback_status = criAtomExPlayback_GetStatus(*playback_id_ptr);
                    criAtomExPlayback_GetNumPlayedSamples(*playback_id_ptr, &played_samples, &sampling_rate);

                    switch (playback_status) {
                    case CRIATOMEXPLAYBACK_STATUS_PREP:
                        playback_status_name = "PREP";
                        break;
                    case CRIATOMEXPLAYBACK_STATUS_PLAYING:
                        playback_status_name = "PLAYING";
                        break;
                    case CRIATOMEXPLAYBACK_STATUS_REMOVED:
                        playback_status_name = "REMOVED";
                        break;
                    default:
                        playback_status_name = "UNKNOWN";
                        break;
                    }

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(column);
                    snprintf(buffer, sizeof(buffer), "%d", *playback_id_ptr);
                    ImGuiUtils::Rightalign(buffer);
                    column++;
                    ImGui::TableSetColumnIndex(column);
                    snprintf(buffer, sizeof(buffer), "%lld", played_samples);
                    ImGuiUtils::Rightalign(buffer);
                    column++;
                    ImGui::TableSetColumnIndex(column);
                    ImGuiUtils::Rightalign(playback_status_name);
                    column++;
                    ImGui::TableSetColumnIndex(column);
                    snprintf(buffer, sizeof(buffer), "%p", criAtomExPlayback_GetAtomPlayer(*playback_id_ptr));
                    ImGuiUtils::Rightalign(buffer);
                }
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }
    ImGui::Separator();
    
	ImGui::End();
}

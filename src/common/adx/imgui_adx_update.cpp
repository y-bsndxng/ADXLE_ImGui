#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

static void FileWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
static void InfoWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
static void VoicePoolWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
static void PlayerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
static void DataRequestCallback(void *obj, CriAtomExPlaybackId id, CriAtomPlayerHn player);
static void MixerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
static void BusMeter(void);
static void MasterOutMeter(void);

void ImGuiAdx::Update(const ImVec2 size, const ImVec2 pos)
{
    static bool is_open_file_window = false;
	static bool is_open_voicepool_window = false;
    static bool is_open_info_window = false;
	static bool is_open_player_window = false;
    static bool is_open_mixer_window = false;

	/* 未初期化なら何もしない */
	if (!ADXRuntime::IsInitilaized()) {
		return;
	}

	ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

	ImGui::Begin("Update", nullptr, ImGuiWindowFlags_NoSavedSettings);

	ImGui::Text("Error : %s", ADXUtils::GetErrorMessage().c_str());
    if (ImGui::TreeNode("File")) {
        if (ImGui::Button("Open")) {
            is_open_file_window = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            is_open_file_window = false;
        }
        if (is_open_file_window) {
            ImVec2 window_size{ size.x + 400.0f, size.y };
            FileWindow(window_size, ImGuiUtils::AddOffsetX(pos, 500.0f), &is_open_file_window);
        }
        ImGui::TreePop();
    }
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
    if (ImGui::TreeNode("Mixer")) {
        if (ImGui::Button("Open")) {
            is_open_mixer_window = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            is_open_mixer_window = false;
        }
        if (is_open_mixer_window) {
            ImVec2 window_size{ size.x + 200.0f, size.y };
            MixerWindow(window_size, ImGuiUtils::AddOffsetX(pos, 500.0f), &is_open_mixer_window);
        }
        ImGui::TreePop();
    }
	ImGui::End();

    criAtomEx_ExecuteMain();
}

static void FileWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
    constexpr int32_t path_length = 512;
    static char acf_file[path_length] = "";
    static char acb_file[path_length] = "";
    static char awb_file[path_length] = "";
    ImVec2 file_dialog_window_size = { 800, 600 };
    
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    ImGui::Begin("File", is_open, ImGuiWindowFlags_NoSavedSettings);
    
    ImGui::InputText("ACF File", acf_file, IM_ARRAYSIZE(acf_file));
    ImGui::SameLine();
    if (ImGui::Button("Open##ACF")) {
        IGFD::FileDialogConfig config;
        config.path = ImGuiUtils::GetCurrentPath();
        ImGui::SetNextWindowSize(file_dialog_window_size, ImGuiCond_Always);
        ImGuiFileDialog::Instance()->OpenDialog("ChooseACFFileDlgKey", "Choose File", ".acf", config);
    }
    ImGuiUtils::OpenDialog("ChooseACFFileDlgKey", acf_file);
    ImGui::SameLine();
    if (ImGui::Button("Clear##ACF")) {
        memset(&acf_file, 0, sizeof(acf_file));
    }

    ImGui::InputText("ACB File", acb_file, IM_ARRAYSIZE(acb_file));
    ImGui::SameLine();
    if (ImGui::Button("Open##ACB")) {
        IGFD::FileDialogConfig config;
        config.path = ImGuiUtils::GetCurrentPath();
        ImGui::SetNextWindowSize(file_dialog_window_size, ImGuiCond_Always);
        ImGuiFileDialog::Instance()->OpenDialog("ChooseACBFileDlgKey", "Choose File", ".acb", config);
    }
    ImGuiUtils::OpenDialog("ChooseACBFileDlgKey", acb_file);
    ImGui::SameLine();
    if (ImGui::Button("Clear##ACB")) {
        memset(&acb_file, 0, sizeof(acb_file));
    }

    ImGui::InputText("AWB File", awb_file, IM_ARRAYSIZE(awb_file));
    ImGui::SameLine();
    if (ImGui::Button("Open##AWB")) {
        IGFD::FileDialogConfig config;
        config.path = ImGuiUtils::GetCurrentPath();
        ImGui::SetNextWindowSize(file_dialog_window_size, ImGuiCond_Always);
        ImGuiFileDialog::Instance()->OpenDialog("ChooseAWBFileDlgKey", "Choose File", ".awb", config);
    }
    ImGuiUtils::OpenDialog("ChooseAWBFileDlgKey", awb_file);
    ImGui::SameLine();
    if (ImGui::Button("Clear##AWB")) {
        memset(&awb_file, 0, sizeof(awb_file));
    }
    
    if (ImGui::Button("Load")) {
        ADXRuntime::LoadFile(acf_file, awb_file, awb_file);
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
    ImGui::Separator();
    if (ImGui::Button("Start")) {
        if (!cue_names.empty()) {
            auto name = cue_names.at(selected_cue_index).c_str();
            criAtomExPlayer_SetCueName(player, ADXRuntime::acb_hn, name);
        }
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
    if (ImGui::TreeNode("Cue")) {
        if (!cue_names.empty()) {
            ImGuiUtils::Comboui("Cue", &selected_cue_index, &cue_names);
        }
        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::TreeNode("PCM")) {
		static float freq = 440.0f;
        static Player::DataRequestObj obj;
        static int32_t selected_noise_index;
        std::vector<std::string> noise_names;
        std::vector<NoiseType> noise_types = {
            NoiseType::Sin,
            NoiseType::White,
            NoiseType::Pink
        };

        std::transform(noise_types.begin(), noise_types.end(), std::back_inserter(noise_names), [](const NoiseType v) { return ADXUtils::GetNoiseTypeString(v); });
        ImGuiUtils::Comboui("NoiseType", &selected_noise_index, &noise_names);
        obj.noise_type = noise_types.at(selected_noise_index);
		ImGui::SliderFloat("input freq", &freq, 0.0f, 10000.0f);

		if (ImGui::Button("Set")) {
            obj.frequency = (int32_t)freq;
            obj.index = 0;
	        criAtomExPlayer_SetFormat(player, CRIATOMEX_FORMAT_RAW_PCM);
            criAtomExPlayer_SetNumChannels(player, obj.num_channels);
	        criAtomExPlayer_SetSamplingRate(player, obj.sampling_rate);
	        criAtomExPlayer_SetDataRequestCallback(player, DataRequestCallback, &obj);
            criAtomExPlayer_SetData(player, obj.buffer[obj.index].data(), obj.num_samples * obj.num_channels * sizeof(int16_t));
		}
		ImGui::SameLine();
		if (ImGui::Button("Unet")) {
            criAtomExPlayer_Stop(player);
	        criAtomExPlayer_SetDataRequestCallback(player, NULL, NULL);
		}

		ImGui::TreePop();
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
        criAtomExPlayer_SetPan3dElevation(player, pan3d_elevation);
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
    if (ADXRuntime::GetBusNames().size() > 0) {
        ImGui::Separator();
        if (ImGui::TreeNode("Set Bus Send Level Offset")) {
            auto bus_names = ADXRuntime::GetBusNames();
            static int32_t selected_bus_index = 0;
            static float bus_send_level = 0.0f;
            ImGuiUtils::Comboui("Player", &selected_player_index, &player_names);
            ImGui::SliderFloat("Bus Send Level", &bus_send_level, 0.0f, 1.0f);
            criAtomExPlayer_SetBusSendLevelOffsetByName(player, bus_names.at(selected_bus_index).c_str(), bus_send_level);
            ImGui::TreePop();
        }
    }
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

static void DataRequestCallback(void* obj, CriAtomExPlaybackId id, CriAtomPlayerHn player)
{
    Player::DataRequestObj* request_obj = (Player::DataRequestObj*)obj;
	float sin_step = 2.0f * 3.141592f * request_obj->frequency / request_obj->sampling_rate;
    std::vector<std::vector<int16_t>> buffer(request_obj->num_channels, std::vector<int16_t>(request_obj->num_samples, 0));
    float coefficient[7] = { 0 };
    
	UNUSED(id);
    
    request_obj->index++;
    request_obj->index %= 2;

    std::fill(request_obj->buffer[request_obj->index].begin(), request_obj->buffer[request_obj->index].end(), static_cast<int16_t>(0));
    ADXUtils::Deinterleave(request_obj->buffer[request_obj->index], buffer, request_obj->num_channels, request_obj->num_samples);

    for (auto i = 0; i < request_obj->num_samples; i++) {
        for (auto ch = 0; ch < request_obj->num_channels; ch++) {
            float pcm = 0.0f;
            switch (request_obj->noise_type) {
            case NoiseType::Sin:
                pcm = sinf(request_obj->offset);
                request_obj->offset += sin_step;
                break;
            case NoiseType::White:
                pcm = (float)rand();
                pcm = (sinf(2.0f) * 3.141592f * pcm) / RAND_MAX;
                break;
            case NoiseType::Pink:
                coefficient[0] = 0.99886f * coefficient[0] + 0.0555179f * pcm;
                coefficient[1] = 0.99332f * coefficient[1] + 0.0750759f * pcm;
                coefficient[2] = 0.96900f * coefficient[2] + 0.1538520f * pcm;
                coefficient[3] = 0.86650f * coefficient[3] + 0.3104856f * pcm;
                coefficient[4] = 0.55000f * coefficient[4] + 0.5329522f * pcm;
                coefficient[5] = -0.7616f * coefficient[5] - 0.0168980f * pcm;
                pcm = 1.1125f * 0.129f * (coefficient[0] + coefficient[1] + coefficient[2] + coefficient[3] + coefficient[4] + coefficient[5] + coefficient[6] + (rand() / ((float)RAND_MAX / 2.0f) - 1.0f) * 0.5362f);
                coefficient[6] = pcm * 0.115926f;
                break;
            default:
                break;
            }
            buffer[ch][i] = ADXUtils::FloatToInt16(pcm);
        }
    }
	
	request_obj->offset = fmodf(request_obj->offset, 2.0f * 3.141592f);
    ADXUtils::Interleave(buffer, request_obj->buffer[request_obj->index], request_obj->num_channels, request_obj->num_samples);
	criAtomPlayer_SetData(player, request_obj->buffer[request_obj->index].data(), request_obj->num_samples * request_obj->num_channels * sizeof(int16_t));
}

static void MixerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
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

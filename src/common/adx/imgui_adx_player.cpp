#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

static void DataRequestCallback(void* obj, CriAtomExPlaybackId id, CriAtomPlayerHn player);

void ImGuiAdx::PlayerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
    ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoHostExtendX;
    static int32_t selected_cue_index = 0;
    static int32_t selected_player_index = 0;
    static bool is_auto_update = false;
    static bool is_loop = false;
    static float freq = 220.0f;
    auto cue_names = ADXRuntime::GetCueNames();
    std::vector<std::string> player_names;
    CriAtomExPlayerHn player;
    static PlayerWrapper::DataRequestObj obj;
	ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
	ImGui::Begin("Player", is_open, ImGuiWindowFlags_NoSavedSettings);
    static bool is_cue = (!cue_names.empty()) ? true : false;

    player_names.resize(ADXRuntime::player_wrapper.num_players);
	for (int32_t i = 0; i < ADXRuntime::player_wrapper.num_players; i++) {
        player_names.at(i) = "PlayerIndex : " + std::to_string(i);
	}
	ImGuiUtils::Comboui("Player", &selected_player_index, &player_names);

	player = ADXRuntime::player_wrapper.GetPlayerHn(selected_player_index);
    
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
        if (is_cue) {
            auto cue_name = cue_names.at(selected_cue_index).c_str();
            criAtomExPlayer_SetCueName(player, ADXRuntime::acb_hn, cue_name);
        } else {
            obj.index = 0;
            criAtomExPlayer_SetFormat(player, CRIATOMEX_FORMAT_RAW_PCM);
            criAtomExPlayer_SetNumChannels(player, obj.num_channels);
            criAtomExPlayer_SetSamplingRate(player, obj.sampling_rate);
            criAtomExPlayer_SetDataRequestCallback(player, DataRequestCallback, &obj);
            std::fill(obj.buffer[obj.index].begin(), obj.buffer[obj.index].end(), static_cast<int16_t>(0));
            criAtomExPlayer_SetData(player, obj.buffer[obj.index].data(), obj.num_samples * obj.num_channels * sizeof(int16_t));
        }
        ADXRuntime::playback_ids.push_back(criAtomExPlayer_Start(player));
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        criAtomExPlayer_Stop(player);
        criAtomExPlayer_SetDataRequestCallback(player, NULL, NULL);
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop without Release Time")) {
        criAtomExPlayer_StopWithoutReleaseTime(player);
    }
    ImGui::SameLine();
    if (ImGui::Button("Update")) {
        ADXRuntime::player_wrapper.Update(selected_player_index);
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
        ADXRuntime::player_wrapper.Update(selected_player_index);
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
            is_cue = true;
        }
        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::TreeNode("PCM")) {
        static int32_t selected_noise_index;
        std::vector<std::string> noise_names;
        std::vector<NoiseType> noise_types = {
            NoiseType::Sin,
            NoiseType::White,
            NoiseType::Pink
        };

        std::transform(noise_types.begin(), noise_types.end(), std::back_inserter(noise_names), [](const NoiseType v) { return ADXUtils::GetNoiseTypeString(v); });
        ImGuiUtils::Comboui("NoiseType", &selected_noise_index, &noise_names);
		ImGui::SliderFloat("input freq", &freq, 0.0f, 1000.0f);
        obj.noise_type = noise_types.at(selected_noise_index);
        obj.frequency = freq;
        is_cue = false;

        for (auto i = 0; i < obj.enable_channels.size(); i++) {
            std::string label = "ch : " + std::to_string(i);
            bool enable_channle = obj.enable_channels.at(i);
            ImGui::PushID(i);
            if (ImGui::Checkbox(label.c_str(), &enable_channle)) {
                obj.enable_channels.at(i) = enable_channle;
            }
            ImGui::PopID();
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
        
        ADXRuntime::player_wrapper.SetPanType(selected_player_index, CRIATOMEX_PAN_TYPE_PAN3D);

        ImGui::SliderFloat("Pan3d Angle", &pan3d_angle, -180.0f, 180.0f);
        ImGui::SliderFloat("Pan3d Elevation", &pan3d_elevation, -180.0f, 180.0f);
        ImGui::SliderFloat("Pan3d Distance", &pan3d_distance, 0.0f, 1.0f);
        criAtomExPlayer_SetPan3dAngle(player, pan3d_angle);
        criAtomExPlayer_SetPan3dElevation(player, pan3d_elevation);
        criAtomExPlayer_SetPan3dInteriorDistance(player, pan3d_distance);

        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::TreeNode("3D Pos")) {
        static int32_t selected_orientation_index = 0;
        std::unordered_map<std::string, std::pair<CriAtomExVector, CriAtomExVector>> orientation_list = {
            {"Forward", {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}},
            {"Backward", {{0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}}},
            {"Left", {{-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}},
            {"Right", {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}},
            {"Up", {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}},
            {"Down", {{0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}},
        };
        std::vector<std::string> orientation_names;
        static CriAtomExVector source_pos { 0.0f, 0.0f, 1.0f };
        static CriAtomExVector listener_pos { 0.0f, 0.0f, 1.0f };
        static CriAtomExVector listener_front { 0.0f, 0.0f, 1.0f };
        static CriAtomExVector listener_top { 0.0f, 0.0f, 1.0f };
        static float min_dist = 0.0f, max_dist = 10.0f;

        std::transform(orientation_list.begin(), orientation_list.end(), std::back_inserter(orientation_names),[](const auto& v) { return v.first; });

        ADXRuntime::player_wrapper.SetPanType(selected_player_index, CRIATOMEX_PAN_TYPE_3D_POS);

        if (!cue_names.empty()) {
            auto [result, info] = ADXRuntime::GetCueInfo(cue_names.at(selected_cue_index).c_str());
            switch (info.pan_type) {
                case CRIATOMEX_PAN_TYPE_3D_POS:
                    min_dist = info.pos3d_info.min_distance;
                    max_dist = info.pos3d_info.max_distance;
                    break;
                default:
                    min_dist = 1.0f;
                    max_dist = 10.0f;
                    break;
            }
        }

        ImGui::SeparatorText("Source");
        ImGui::SliderFloat("source x", &source_pos.x, -1.0f * max_dist, max_dist);
        ImGui::SliderFloat("source y", &source_pos.y, -1.0f * max_dist, max_dist);
        ImGui::SliderFloat("source z", &source_pos.z, -1.0f * max_dist, max_dist);
        ImGui::InputFloat("Min Distance", &min_dist);
        ImGui::InputFloat("Max Distance", &max_dist);

        if (ImGui::Button("Random Source Position")) {
            source_pos.x = (float)rand() / RAND_MAX * max_dist;
            source_pos.y = (float)rand() / RAND_MAX * max_dist;
            source_pos.z = (float)rand() / RAND_MAX * max_dist;
        }
        if (ImGui::Button("Reset Source Position")) {
            source_pos.x = 0.0f;
            source_pos.y = 0.0f;
            source_pos.z = 1.0f;
        }
        
        ImGui::SeparatorText("Listener");
        ImGui::Text("Position");
        ImGui::SliderFloat("listener x", &listener_pos.x, -1.0f * max_dist, max_dist);
        ImGui::SliderFloat("listener y", &listener_pos.y, -1.0f * max_dist, max_dist);
        ImGui::SliderFloat("listener z", &listener_pos.z, -1.0f * max_dist, max_dist);
        
        ImGui::Text("Orientation");
        ImGuiUtils::Comboui("Orientation", &selected_orientation_index, &orientation_names);
        listener_front = orientation_list[orientation_names.at(selected_orientation_index)].first;
        listener_top = orientation_list[orientation_names.at(selected_orientation_index)].second;
        
        if (ImGui::Button("Random Listener Position")) {
            auto rand_index = rand() % orientation_names.size();
            listener_pos.x = (float)rand() / RAND_MAX * max_dist;
            listener_pos.y = (float)rand() / RAND_MAX * max_dist;
            listener_pos.z = (float)rand() / RAND_MAX * max_dist;
            selected_orientation_index = static_cast<int32_t>(rand_index);
        }
        if (ImGui::Button("Reset Listener Position")) {
            listener_pos.x = 0.0f;
            listener_pos.y = 0.0f;
            listener_pos.z = 0.0f;
            selected_orientation_index = 0;
        }
        
        ADXRuntime::player_wrapper.SetSourcePosition(selected_player_index, source_pos);
        ADXRuntime::player_wrapper.SetListenerPosition(selected_player_index, listener_pos);
        ADXRuntime::player_wrapper.SetListenerOrientation(selected_player_index, listener_front, listener_top);
        
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

static void DataRequestCallback(void* obj, CriAtomExPlaybackId id, CriAtomPlayerHn player)
{
    PlayerWrapper::DataRequestObj* request_obj = (PlayerWrapper::DataRequestObj*)obj;
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
            if (!request_obj->enable_channels[ch]) {
                continue;
            }
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

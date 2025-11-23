#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

static void VoicePoolWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
static void PlayerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);

void ImGuiAdx::Update(const ImVec2 size, const ImVec2 pos)
{
	static bool is_open_voicepool_window = false;
	static bool is_open_player_window = false;

	/* 未初期化なら何もしない */
	if (!ADXRuntime::IsInitilaized()) {
		return;
	}

	ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

	ImGui::Begin("Update", nullptr, ImGuiWindowFlags_NoSavedSettings);

	ImGui::Text("Error : %s", ADXUtils::GetErrorMessage().c_str());
	if (ImGui::TreeNode("VoicePool")) {
		if (ImGui::Button("Open")) {
			is_open_voicepool_window = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			is_open_voicepool_window = false;
		}
		if (is_open_voicepool_window) {
			VoicePoolWindow(ImGuiUtils::AddOffset(size, 100.0f), ImGuiUtils::AddOffsetX(pos, 300.0f), &is_open_voicepool_window);
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
			PlayerWindow(ImGuiUtils::AddOffset(size, 100.0f), ImGuiUtils::AddOffsetX(pos, 300.0f), &is_open_player_window);
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

static void PlayerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
	ImGui::SetNextWindowPos(ImGuiUtils::AddOffsetX(pos, 300.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
	ImGui::Begin("Player", is_open, ImGuiWindowFlags_NoSavedSettings);
	static int32_t selected_index = 0;
	std::vector<std::string> names;
	CriAtomExPlayerHn player;

	names.resize(ADXRuntime::player.num_players);
	for (int32_t i = 0; i < ADXRuntime::player.num_players; i++) {
		names.at(i) = "PlayerIndex : " + std::to_string(i);
	}
	ImGuiUtils::Comboui("Player", &selected_index, &names);

	player = ADXRuntime::player.GetPlayerHn(selected_index);

	if (ImGui::BeginTable("Table##player", 2)) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Player Ptr");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%p", player);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Player Status");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%s", ADXUtils::GetPlayerStatusString(player).c_str());
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Player Samples");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%d", criAtomExPlayer_GetTimeReal(player));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Number of Playbacks");
		ImGui::TableSetColumnIndex(1); ImGui::Text("%d", criAtomExPlayer_GetNumPlaybacks(player));
		ImGui::TableNextRow();
		ImGui::EndTable();
	}

	ImGui::End();
}

#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

void ImGuiAdx::Update(const ImVec2 size, const ImVec2 pos)
{
    static bool is_open_file_window = false;
	static bool is_open_voicepool_window = false;
    static bool is_open_info_window = false;
	static bool is_open_player_window = false;
    static bool is_open_mixer_window = false;
    static bool is_enabled_binauralizer = false;
    ImVec2 sub_window_size{ 400.0f, 500.0f };
    ImVec2 sub_window_pos{ 600.0f, 200.0f };

	/* 未初期化なら何もしない */
	if (!ADXRuntime::IsInitilaized()) {
		return;
	}

	ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

	ImGui::Begin("Update", nullptr, ImGuiWindowFlags_NoSavedSettings);

	ImGui::Text("Error : %s", ADXUtils::GetErrorMessage().c_str());
    ImGui::Checkbox("Binauralizer", &is_enabled_binauralizer);
    if (is_enabled_binauralizer) {
        criAtomExAsr_EnableBinauralizer(CRI_TRUE);
    } else {
        criAtomExAsr_EnableBinauralizer(CRI_FALSE);
    }
    if (ImGui::TreeNode("File")) {
        if (ImGui::Button("Open")) {
            is_open_file_window = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            is_open_file_window = false;
        }
        if (is_open_file_window) {
            sub_window_size = ImGuiUtils::AddOffsetX(sub_window_size, 100.0f);
            FileWindow(sub_window_size, sub_window_pos, &is_open_file_window);
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
            sub_window_pos = ImGuiUtils::AddOffsetX(sub_window_pos, 50.0f);
            ImGuiAdx::InfoWindow(sub_window_size, sub_window_pos, &is_open_info_window);
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
            sub_window_pos = ImGuiUtils::AddOffsetX(sub_window_pos, 50.0f);
            ImGuiAdx::VoicePoolWindow(sub_window_size, sub_window_pos, &is_open_voicepool_window);
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
            sub_window_pos = ImGuiUtils::AddOffsetX(sub_window_pos, 70.0f);
            ImGuiAdx::PlayerWindow(sub_window_size, sub_window_pos, &is_open_player_window);
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
            sub_window_pos = ImGuiUtils::AddOffsetX(sub_window_pos, 50.0f);
            ImGuiAdx::MixerWindow(sub_window_size, sub_window_pos, &is_open_mixer_window);
        }
        ImGui::TreePop();
    }
	ImGui::End();

    criAtomEx_ExecuteMain();
}

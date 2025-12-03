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
            ImGuiAdx::InfoWindow(window_size, ImGuiUtils::AddOffsetX(pos, 500.0f), &is_open_info_window);
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
            ImGuiAdx::VoicePoolWindow(window_size, ImGuiUtils::AddOffsetX(pos, 500.0f), &is_open_voicepool_window);
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
            ImGuiAdx::PlayerWindow(window_size, ImGuiUtils::AddOffsetX(pos, 500.0f), &is_open_player_window);
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
            ImGuiAdx::MixerWindow(window_size, ImGuiUtils::AddOffsetX(pos, 500.0f), &is_open_mixer_window);
        }
        ImGui::TreePop();
    }
	ImGui::End();

    criAtomEx_ExecuteMain();
}

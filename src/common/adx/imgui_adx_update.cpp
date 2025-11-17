#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

void ImGuiAdx::Update(const ImVec2 size, const ImVec2 pos)
{
	static bool is_open_voicepool_window = false;
	/* 未初期化なら何もしない */
	if (!ADXRuntime::IsInitilaized()) {
		return;
	}

	ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

	ImGui::Begin("Update", nullptr, ImGuiWindowFlags_NoSavedSettings);

	if (ImGui::TreeNode("VoicePool")) {
		if (ImGui::Button("Open")) {
			is_open_voicepool_window = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			is_open_voicepool_window = false;
		}
		if (is_open_voicepool_window) {
			ImGui::SetNextWindowPos(ImGuiUtils::AddOffsetX(pos, 300.0f), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
			ImGui::Begin("VoicePool", &is_open_voicepool_window, ImGuiWindowFlags_NoSavedSettings);
			ImGui::End();
		}
		ImGui::TreePop();
	}

	ImGui::End();
}
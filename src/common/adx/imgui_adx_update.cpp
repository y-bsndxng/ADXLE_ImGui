#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

static void VoicePoolWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);

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
			VoicePoolWindow(ImGuiUtils::AddOffsetX(pos, 300.0f), size, &is_open_voicepool_window);
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

static void VoicePoolWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
	ImGui::SetNextWindowPos(ImGuiUtils::AddOffsetX(pos, 300.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
	ImGui::Begin("VoicePool", is_open, ImGuiWindowFlags_NoSavedSettings);

	if (ADXRuntime::vp.num_voicepools > 0) {
		auto [num_used_voices, num_max_voices] = ADXRuntime::GetNumUsedVoicePools(0);
		if (ImGui::BeginTable("Voice Pool Info Table", 2)) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Voice Pool Ptr");
			ImGui::TableSetColumnIndex(1); ImGui::Text("%p", ADXRuntime::vp.GetVoicePoolHn(0));
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); ImGui::Text("%s", "Used / Max");
			ImGui::TableSetColumnIndex(1); ImGui::Text("%3d / %3d", num_used_voices, num_max_voices);
			ImGui::EndTable();
		}
	}

	ImGui::End();
}

#include <imgui_adx_initilaize.h>
#include <imgui_utils.h>
#include <adx_utils.h>

void imgui_adx_initilaize(const ImVec2 size, const ImVec2 pos)
{
	constexpr int32_t path_length = 512;
	static char acf_file[path_length] = "";
	static char acb_file[path_length] = "";
	static char awb_file[path_length] = "";
	ImVec2 file_dialog_window_size = { 800, 600 };

	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);

	ImGui::Begin("Initialize", nullptr, ImGuiWindowFlags_NoSavedSettings);

	ImGui::InputText("ACF File", acf_file, IM_ARRAYSIZE(acf_file));
	ImGui::SameLine();
	if (ImGui::Button("Open##ACF")) {
		IGFD::FileDialogConfig config;
		config.path = std::filesystem::current_path().string();
		ImGui::SetNextWindowSize(file_dialog_window_size, ImGuiCond_Always);
		ImGuiFileDialog::Instance()->OpenDialog("ChooseACFFileDlgKey", "Choose File", ".acf", config);
	}
	imgui_open_dialog("ChooseACFFileDlgKey", acf_file);
	ImGui::SameLine();
	if (ImGui::Button("Clear##ACF")) {
		memset(&acf_file, 0, sizeof(acf_file));
	}

	ImGui::InputText("ACB File", acb_file, IM_ARRAYSIZE(acb_file));
	ImGui::SameLine();
	if (ImGui::Button("Open##ACB")) {
		IGFD::FileDialogConfig config;
		config.path = std::filesystem::current_path().string();
		ImGui::SetNextWindowSize(file_dialog_window_size, ImGuiCond_Always);
		ImGuiFileDialog::Instance()->OpenDialog("ChooseACBFileDlgKey", "Choose File", ".acb", config);
	}
	imgui_open_dialog("ChooseACBFileDlgKey", acb_file);
	ImGui::SameLine();
	if (ImGui::Button("Clear##ACB")) {
		memset(&acb_file, 0, sizeof(acb_file));
	}

	ImGui::InputText("AWB File", awb_file, IM_ARRAYSIZE(awb_file));
	ImGui::SameLine();
	if (ImGui::Button("Open##AWB")) {
		IGFD::FileDialogConfig config;
		config.path = std::filesystem::current_path().string();
		ImGui::SetNextWindowSize(file_dialog_window_size, ImGuiCond_Always);
		ImGuiFileDialog::Instance()->OpenDialog("ChooseAWBFileDlgKey", "Choose File", ".awb", config);
	}
	imgui_open_dialog("ChooseAWBFileDlgKey", awb_file);
	ImGui::SameLine();
	if (ImGui::Button("Clear##AWB")) {
		memset(&awb_file, 0, sizeof(awb_file));
	}

	ImGui::End();
}

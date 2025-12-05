#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

void ImGuiAdx::FileWindow(const ImVec2 size, const ImVec2 pos, bool* is_open)
{
    static char acf_file[PATH_LENGTH] = "";
    static char acb_file[PATH_LENGTH] = "";
    static char awb_file[PATH_LENGTH] = "";
    ImVec2 file_dialog_window_size =  ImGuiUtils::GetDialogWindowSize();

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
        ADXRuntime::LoadFile(acf_file, acb_file, awb_file);
    }
    

    ImGui::End();
}

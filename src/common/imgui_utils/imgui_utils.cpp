#include <imgui_utils.hpp>

std::string ImGuiUtils::GetCurrentPath()
{
	return std::filesystem::current_path().string();
}

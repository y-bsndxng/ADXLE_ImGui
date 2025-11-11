#include <imgui_adx_initilaize.h>
#include <adx_utils.h>

void imgui_adx_initilaize(const ImVec2 size, const ImVec2 pos)
{
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);

	ImGui::Begin("Initilaize");
	ImGui::End();
}

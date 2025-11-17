#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuiFileDialog.h>
#include <imgui_utils.hpp>

namespace ImGuiAdx {
	void Initilaize(const ImVec2 size, const ImVec2 pos);
	bool IsInitilaized(void);
	void Update(const ImVec2 size, const ImVec2 pos);
	void Finalize(void);
};

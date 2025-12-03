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
void FileWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
void InfoWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
void VoicePoolWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
void PlayerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
void MixerWindow(const ImVec2 size, const ImVec2 pos, bool* is_open);
};

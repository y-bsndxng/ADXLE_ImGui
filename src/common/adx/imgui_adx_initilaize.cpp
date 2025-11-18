#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

void ImGuiAdx::Initilaize(const ImVec2 size, const ImVec2 pos)
{
	constexpr int32_t path_length = 512;
	static char acf_file[path_length] = "";
	static char acb_file[path_length] = "";
	static char awb_file[path_length] = "";
	static const std::vector<CriAtomExThreadModel> thread_models{ CRIATOMEX_THREAD_MODEL_MULTI_WITH_SONICSYNC, CRIATOMEX_THREAD_MODEL_MULTI, CRIATOMEX_THREAD_MODEL_MULTI_USER_DRIVEN, CRIATOMEX_THREAD_MODEL_USER_MULTI, CRIATOMEX_THREAD_MODEL_SINGLE };
	static const std::vector<std::string> thread_models_items{ "MULTI_WITH_SONICSYNC", "MULTI", "USER_DRIVEN", "USER_MULTI", "SINGLE" };
	static int32_t selected_thread_model_index = 0;
	static int32_t num_sampling_rate = CRIATOM_DEFAULT_OUTPUT_SAMPLING_RATE;
	static int32_t num_voice = 64;
	static int32_t num_virtual_voice = 256;
	static int32_t num_player = 5;
	ImVec2 file_dialog_window_size = { 800, 600 };
	std::string ui_caption;

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
	ImGuiUtils::OpenDialog("ChooseACFFileDlgKey", acf_file);
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
	ImGuiUtils::OpenDialog("ChooseACBFileDlgKey", acb_file);
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
	ImGuiUtils::OpenDialog("ChooseAWBFileDlgKey", awb_file);
	ImGui::SameLine();
	if (ImGui::Button("Clear##AWB")) {
		memset(&awb_file, 0, sizeof(awb_file));
	}

	ImGui::Separator();
	ui_caption = "ThreadModel";
	ImGuiUtils::Comboui(&ui_caption, &selected_thread_model_index, &thread_models_items);
	ImGui::SliderInt("Number of Sampling Rate", &num_sampling_rate, 0, 192000);
	ImGui::SliderInt("Number of Voice", &num_voice, 1, 256);
	ImGui::SliderInt("Number of Virtual Voice", &num_virtual_voice, 1, 256);
	ImGui::SliderInt("Number of Player", &num_player, 1, 10);

	if (ImGui::Button("Initialize")) {
#if defined(XPT_TGT_PC) 
		CriAtomExConfig_WASAPI config;
#elif defined(XPT_TGT_MACOSX)
		CriAtomExConfig_MACOSX config;
#endif
		CriAtomExAcfRegistrationInfo acf_info;
		CriAtomExMonitorConfig monitor_config;

#if defined(XPT_TGT_PC) 
		criAtomEx_SetDefaultConfig_WASAPI(&config);
#elif defined(XPT_TGT_MACOSX)
		criAtomEx_SetDefaultConfig_MACOSX(&config);
#endif
		criAtomExMonitor_SetDefaultConfig(&monitor_config);

		/* エラーコールバック関数の登録 */
		criErr_SetCallback(ADXUtils::user_error_callback_func);
		/* メモリアロケータの登録 */
		criAtomEx_SetUserAllocator(ADXUtils::user_alloc_func, ADXUtils::user_free_func, NULL);

		/* ACF情報の設定 */
		acf_info.type = CRIATOMEX_ACF_LOCATION_INFO_TYPE_NAME;
		acf_info.info.name.binder = NULL;
		acf_info.info.name.path = acf_file;

		/* ACF ファイル名が空ならファイルなし初期化 */
		if (strlen(acf_file) == 0) {
			config.atom_ex.acf_info = NULL;
		} else {
			config.atom_ex.acf_info = &acf_info;
		}
		config.asr.output_sampling_rate = num_sampling_rate;
		config.hca_mx.max_sampling_rate = num_sampling_rate;
		config.atom_ex.max_virtual_voices = num_virtual_voice;
		config.atom_ex.thread_model = thread_models[selected_thread_model_index];

		/* デフォルト値の上書き */
		ADXRuntime::Initialize(&config, &monitor_config);
	}

	if (ImGuiAdx::IsInitilaized()) {
		CriAtomExStandardVoicePoolConfig config;

		criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&config);
		config.num_voices = num_voice;
		config.player_config.max_sampling_rate = num_sampling_rate;
		config.player_config.streaming_flag = CRI_TRUE;
		config.player_config.max_channels = criAtomExAsrRack_GetOutputChannels(CRIATOMEXASR_RACK_DEFAULT_ID);
		ADXRuntime::vp.CreateVoicePool(&config);
	}

	ImGui::End();
}

bool ImGuiAdx::IsInitilaized(void)
{
	return ADXRuntime::IsInitilaized();
}
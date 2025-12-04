#include <imgui_adx.h>
#include <adx_utils.h>
#include <adx_runtime.h>

void ImGuiAdx::Initilaize(const ImVec2 size, const ImVec2 pos)
{
	constexpr int32_t path_length = 512;
	static char acf_file[path_length] = "";
	static char acb_file[path_length] = "";
	static char awb_file[path_length] = "";
    const std::vector<CriAtomExThreadModel> thread_models{ CRIATOMEX_THREAD_MODEL_MULTI_WITH_SONICSYNC, CRIATOMEX_THREAD_MODEL_MULTI, CRIATOMEX_THREAD_MODEL_MULTI_USER_DRIVEN, CRIATOMEX_THREAD_MODEL_USER_MULTI, CRIATOMEX_THREAD_MODEL_SINGLE };
	static int32_t selected_thread_model_index = 0;
    const std::vector<std::string> thread_models_items{ "MULTI_WITH_SONICSYNC", "MULTI", "USER_DRIVEN", "USER_MULTI", "SINGLE" };
    const std::vector<CriAtomSpeakerMapping> speaker_mappings{ CRIATOM_SPEAKER_MAPPING_AUTO, CRIATOM_SPEAKER_MAPPING_MONO, CRIATOM_SPEAKER_MAPPING_STEREO, CRIATOM_SPEAKER_MAPPING_5_1, CRIATOM_SPEAKER_MAPPING_7_1, CRIATOM_SPEAKER_MAPPING_5_1_2, CRIATOM_SPEAKER_MAPPING_7_1_2, CRIATOM_SPEAKER_MAPPING_7_1_4, CRIATOM_SPEAKER_MAPPING_7_1_4_4 };
    const std::vector<std::string> speaker_mapping_items{ "AUTO", "MONO", "STEREO", "5_1", "7_1", "5_1_2", "7_1_2", "7_1_4", "7_1_4_4" };
    static int32_t selected_speaker_mapping_index = 0;
    const std::vector<CriAtomSoundRendererType> sound_renderer_types{ CRIATOM_SOUND_RENDERER_NATIVE, CRIATOM_SOUND_RENDERER_SPATIAL };
    const std::vector<std::string> sound_renderer_items{ "NATIVE", "SPATIAL" };
    static int32_t selected_sound_renderer_type_index = 0;
	static int32_t num_sampling_rate = CRIATOM_DEFAULT_OUTPUT_SAMPLING_RATE;
    static int32_t num_output_channels = CRIATOM_DEFAULT_OUTPUT_CHANNELS;
	static int32_t num_voice = 64;
	static int32_t num_virtual_voice = 256;
	static int32_t num_players = 5;
	ImVec2 file_dialog_window_size = { 1000, 600 };

    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

	ImGui::Begin("Initialize", nullptr, ImGuiWindowFlags_NoSavedSettings);

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

	ImGui::Separator();
    ImGuiUtils::Comboui("SpeakerMapping", &selected_speaker_mapping_index, &speaker_mapping_items);
    ImGuiUtils::Comboui("SoundRendererType", &selected_sound_renderer_type_index, &sound_renderer_items);
	ImGuiUtils::Comboui("ThreadModel", &selected_thread_model_index, &thread_models_items);
	ImGui::SliderInt("Number of Sampling Rate", &num_sampling_rate, 0, 192000);
    ImGui::SliderInt("Number of Output Channels", &num_output_channels, 2, 16);
	ImGui::SliderInt("Number of Voice", &num_voice, 1, 256);
	ImGui::SliderInt("Number of Virtual Voice", &num_virtual_voice, 1, 256);
    ImGui::InputInt("Number of Player", &num_players);

	if (ImGui::Button("Initialize")) {
		ADXRuntime::Config config;

		/* エラーコールバック関数の登録 */
		criErr_SetCallback(ADXUtils::UserErrorCallBackFunc);
		/* メモリアロケータの登録 */
		criAtomEx_SetUserAllocator(ADXUtils::UserAllocFunc, ADXUtils::UserFreeFunc, NULL);

		/* ACF情報の設定 */
		config.acf_info.type = CRIATOMEX_ACF_LOCATION_INFO_TYPE_NAME;
		config.acf_info.info.name.binder = NULL;
		config.acf_info.info.name.path = acf_file;

		/* ACF ファイル名が空ならファイルなし初期化 */
		if (strlen(acf_file) == 0) {
			config.specific_config.atom_ex.acf_info = NULL;
		} else {
			config.specific_config.atom_ex.acf_info = &config.acf_info;
		}
		config.specific_config.asr.output_sampling_rate = num_sampling_rate;
		config.specific_config.hca_mx.max_sampling_rate = num_sampling_rate;
		config.specific_config.atom_ex.max_virtual_voices = num_virtual_voice;
        config.specific_config.atom_ex.thread_model = thread_models.at(selected_thread_model_index);
        config.specific_config.asr.speaker_mapping = speaker_mappings.at(selected_speaker_mapping_index);
		config.specific_config.asr.sound_renderer_type = sound_renderer_types.at(selected_sound_renderer_type_index);
        config.specific_config.asr.output_channels = num_output_channels;

		/* 初期化 */
		ADXRuntime::Initialize(config);
	}

	if (ImGuiAdx::IsInitilaized()) {
        VoicePoolWrapper::Config voicepool_config;
		PlayerWrapper::Config player_config;

        voicepool_config.standard_config.num_voices = num_voice;
        voicepool_config.rawpcm_config.num_voices = num_voice;
        voicepool_config.wave_config.num_voices = num_voice;

        voicepool_config.standard_config.player_config.max_sampling_rate = num_sampling_rate;
        voicepool_config.rawpcm_config.player_config.max_sampling_rate = num_sampling_rate;
        voicepool_config.wave_config.player_config.max_sampling_rate = num_sampling_rate;

        voicepool_config.standard_config.player_config.max_channels = criAtomExAsrRack_GetOutputChannels(CRIATOMEXASR_RACK_DEFAULT_ID);
        voicepool_config.rawpcm_config.player_config.max_channels = criAtomExAsrRack_GetOutputChannels(CRIATOMEXASR_RACK_DEFAULT_ID);
        voicepool_config.wave_config.player_config.max_channels = criAtomExAsrRack_GetOutputChannels(CRIATOMEXASR_RACK_DEFAULT_ID);

        voicepool_config.standard_config.player_config.streaming_flag = CRI_TRUE;

		player_config.num_players = num_players;

        ADXRuntime::voicepool_wrapper.CreateVoicePool(voicepool_config);
		ADXRuntime::player_wrapper.Create(player_config);
        ADXRuntime::LoadFile(acb_file, awb_file);
	}

	ImGui::End();
}

bool ImGuiAdx::IsInitilaized(void)
{
	return ADXRuntime::IsInitilaized();
}

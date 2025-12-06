#include <adx_runtime.h>
ADXRuntime::Config::Config()
{
#if defined(XPT_TGT_PC)
    criAtomEx_SetDefaultConfig_WASAPI(&this->specific_config);
#elif defined(XPT_TGT_MACOSX)
    criAtomEx_SetDefaultConfig_MACOSX(&this->specific_config);
#endif
    criAtomExMonitor_SetDefaultConfig(&this->monitor_config);
    memset(&this->acf_info, 0, sizeof(CriAtomExAcfRegistrationInfo));
    criAtomDbas_SetDefaultConfig(&dbas_config);
    dbas_config.max_bps *= 2;
    dbas_config.max_streams *= 2;
    dbas_config.max_streams *= 2;
}

void ADXRuntime::Initialize(const ADXRuntime::Config& config)
{
#if defined(XPT_TGT_PC)
	criAtomEx_Initialize_WASAPI(&config.specific_config, NULL, 0);
#elif defined(XPT_TGT_MACOSX)
    criAtomEx_Initialize_MACOSX(&config.specific_config, NULL, 0);
#endif
    criAtomExMonitor_Initialize(&config.monitor_config, NULL, 0);
    
    /* 初期化成功なら以下を実行 */
    if (criAtomEx_IsInitialized() != CRI_FALSE) {
        ADXRuntime::dbas_id = criAtomExDbas_Create(&config.dbas_config, NULL, 0);
        criAtomEx_AttachPerformanceMonitor();
        criAtomEx_ResetPerformanceMonitor();
        criAtomExAsrRack_AttachLevelMeter(CRIATOMEXASR_RACK_DEFAULT_ID, NULL, NULL, 0);
    }
}

bool ADXRuntime::IsInitilaized(void)
{
	CriBool is_initilaized = criAtomEx_IsInitialized();

	if (is_initilaized == CRI_FALSE) {
		return false;
	} else {
		return true;
	}
}

void ADXRuntime::Finalize(void)
{
    criAtomExAsrRack_DetachLevelMeter(CRIATOMEXASR_RACK_DEFAULT_ID);
    ADXRuntime::voicepool_wrapper.DestroyAllVoicePool();
    ADXRuntime::player_wrapper.Destroy();
    ADXRuntime::playback_ids.clear();
    ADXRuntime::UnloadFile();
    
    criAtomExDbas_Destroy(ADXRuntime::dbas_id);
    criAtomExMonitor_Finalize();
#if defined(XPT_TGT_PC)
	criAtomEx_Finalize_WASAPI();
#elif defined(XPT_TGT_MACOSX)
	criAtomEx_Finalize_MACOSX();
#endif
}

void ADXRuntime::LoadFile(const char* acf_file, const char* acb_file, const char* awb_file)
{
    CriBool result = criAtomEx_RegisterAcfFile(NULL, acf_file, NULL, 0);
    
    if (result != CRI_FALSE) {
        ADXRuntime::UnloadFile();
        ADXRuntime::LoadFile(acb_file, awb_file);
    }
}

void ADXRuntime::LoadFile(const char* acb_file, const char* awb_file)
{
    CriAtomExAcbHn acb_hn_ = NULL;

    if (strlen(acb_file) == 0) {
        return;
    }

    if (ADXRuntime::acb_hn != NULL) {
        ADXRuntime::UnloadFile();
    }
    
    if (strlen(awb_file) > 0) {
        acb_hn_ = criAtomExAcb_LoadAcbFile(
            NULL, acb_file, NULL, awb_file, NULL, 0);
    } else {
        acb_hn_ = criAtomExAcb_LoadAcbFile(
            NULL, acb_file, NULL, NULL, NULL, 0);
    }
    
    if (strlen(acb_file) > 0) {
        assert(acb_hn_ != NULL);
    }
    
    ADXRuntime::acb_hn = acb_hn_;
}

void ADXRuntime::UnloadFile(void)
{
    if (ADXRuntime::acb_hn != NULL) {
        criAtomExAcb_Release(ADXRuntime::acb_hn);
        ADXRuntime::acb_hn = NULL;
    }
}

std::tuple<bool, CriAtomExAcfInfo> ADXRuntime::GetAcfInfo(void)
{
    CriAtomExAcfInfo info;
    bool result;
    
    if (criAtomExAcf_GetAcfInfo(&info) == CRI_FALSE) {
        result = false;
    } else {
        result = true;
    }
    return std::make_tuple(result, info);
}

std::tuple<bool, CriAtomExAcbInfo> ADXRuntime::GetAcbInfo(void)
{
    CriAtomExAcbInfo info { 0 };
    bool result = false;

    if (ADXRuntime::acb_hn == NULL) {
        return std::make_tuple(result, info);
    }
    
    if (criAtomExAcb_GetAcbInfo(ADXRuntime::acb_hn, &info) == CRI_FALSE) {
        result = false;
    } else {
        result = true;
    }
    return std::make_tuple(result, info);
}

std::tuple<bool, CriAtomExCueInfo> ADXRuntime::GetCueInfo(const char* name)
{
    CriAtomExCueInfo info{ 0 };
    bool result = false;
    
    if (criAtomExAcb_GetCueInfoByName(ADXRuntime::acb_hn, name, &info) == CRI_FALSE) {
        result = false;
    } else {
        result = true;
    }
    return std::make_tuple(result, info);
}

std::tuple<bool, CriAtomExAcfDspBusInfo> ADXRuntime::GetBusInfo(const int32_t bus_index)
{
    auto [result, acf_info] = ADXRuntime::GetAcfInfo();
    CriAtomExAcfDspBusInfo bus_info{ 0 };

    if (result) {
        CriSint32 num_buses = criAtomExAcf_GetNumBuses();

        if (num_buses > 0) {
            criAtomExAcf_GetDspBusInformation((CriUint16)bus_index, &bus_info);
        }
        return std::make_tuple(true, bus_info);
    }

    return std::make_tuple(false, bus_info);
}

std::vector<std::string> ADXRuntime::GetCueNames(void)
{
    auto [result, acb_info] = ADXRuntime::GetAcbInfo();
    std::vector<std::string> cue_names;
    
    if (result) {
        cue_names.resize(acb_info.num_cues);
        for (int32_t i = 0; i < acb_info.num_cues; i++) {
            cue_names.at(i) = criAtomExAcb_GetCueNameByIndex(acb_hn, i);
        }
    }

    return cue_names;
}

std::vector<std::string> ADXRuntime::GetBusNames(void)
{
    auto [result, acf_info] = ADXRuntime::GetAcfInfo();
    std::vector<std::string> names;

    if (result) {
        CriSint32 num_settings = criAtomExAcf_GetNumDspSettings();
        CriAtomExAcfDspSettingInfo setting_info;
        CriAtomExAcfDspBusInfo bus_info;

        if (num_settings < 0) {
            return names;
        }

        for (auto i = 0; i < num_settings; i++) {
            result = criAtomExAcf_GetDspSettingInformation(criAtomExAcf_GetDspSettingNameByIndex((CriUint16)i), &setting_info);
            if (result == CRI_FALSE) {
                return names;
            }
            names.resize(setting_info.num_buses);
            for (auto j = 0; j < setting_info.num_buses; j++) {
                criAtomExAcf_GetDspBusInformation((CriUint16)j, &bus_info);
                names.at(j) = bus_info.name;
            }
        }
    }

    return names;
}

std::vector<std::string> ADXRuntime::GetAfxNames(const CriAtomExAsrRackId rack_id, const int32_t bus_index)
{
    std::vector<std::string> names;
    CriSint32 num_buses = criAtomExAcf_GetNumBuses();
    
    if (num_buses > 0) {
        auto [result, bus_info] = ADXRuntime::GetBusInfo(bus_index);
        names.resize(bus_info.num_fxes);
        for (auto i = 0; i < bus_info.num_fxes; i++) {
            names.at(i) = criAtomExAsrRack_GetEffectName(rack_id, bus_info.name, i);
        }
    }

    return names;
}

std::vector<std::string> ADXRuntime::GetAfxNames(const int32_t bus_index)
{
    return ADXRuntime::GetAfxNames(CRIATOMEXASR_RACK_DEFAULT_ID, bus_index);
}

std::tuple<int32_t, int32_t> ADXRuntime::GetNumUsedVoicePools(VoiceType voice_type)
{
	CriSint32 num_used_voices = 0;
	CriSint32 num_max_voices = 0;

	criAtomExVoicePool_GetNumUsedVoices(ADXRuntime::voicepool_wrapper.GetVoicePoolHn(voice_type), &num_used_voices, &num_max_voices);

	return std::make_tuple(num_used_voices, num_max_voices);
}

CriAtomSpeakerMapping ADXRuntime::GetSpeakerMapping(void)
{
    return ADXRuntime::GetSpeakerMapping(CRIATOMEXASR_RACK_DEFAULT_ID);
}

CriAtomSpeakerMapping ADXRuntime::GetSpeakerMapping(const CriAtomExAsrRackId rack_id)
{
    return criAtomExAsrRack_GetSpeakerMapping(rack_id);
}

std::tuple<int64_t, int32_t> ADXRuntime::GetNumOutputSamples(void)
{
    return ADXRuntime::GetNumOutputSamples(CRIATOMEXASR_RACK_DEFAULT_ID);
}

std::tuple<int64_t, int32_t> ADXRuntime::GetNumOutputSamples(const CriAtomExAsrRackId rack_id)
{
    CriSint64 num_samples;
    CriSint32 num_sampling_rate;

    criAtomExAsrRack_GetNumOutputSamples(rack_id, &num_samples, &num_sampling_rate);
    
    return std::make_tuple(num_samples, num_sampling_rate);
}

std::tuple<int64_t, int32_t> ADXRuntime::GetNumRenderedSamples(void)
{
    return ADXRuntime::GetNumRenderedSamples(CRIATOMEXASR_RACK_DEFAULT_ID);
}

std::tuple<int64_t, int32_t> ADXRuntime::GetNumRenderedSamples(const CriAtomExAsrRackId rack_id)
{
    CriSint64 num_samples;
    CriSint32 num_sampling_rate;

    criAtomExAsrRack_GetNumRenderedSamples(rack_id, &num_samples, &num_sampling_rate);
    
    return std::make_tuple(num_samples, num_sampling_rate);
}

CriAtomExAsrRackPerformanceInfo ADXRuntime::GetPerformanceInfo(void)
{
    return ADXRuntime::GetPerformanceInfo(CRIATOMEXASR_RACK_DEFAULT_ID);
}

CriAtomExAsrRackPerformanceInfo ADXRuntime::GetPerformanceInfo(const CriAtomExAsrRackId rack_id)
{
    CriAtomExAsrRackPerformanceInfo info;
    criAtomExAsrRack_GetPerformanceInfo(rack_id, &info);
    return info;
}

void ADXRuntime::ResetPerformanceInfo(void)
{
    ADXRuntime::ResetPerformanceInfo(CRIATOMEXASR_RACK_DEFAULT_ID);
}

void ADXRuntime::ResetPerformanceInfo(const CriAtomExAsrRackId rack_id)
{
    criAtomExAsrRack_ResetPerformanceMonitor(rack_id);
}

VoicePoolWrapper::Config::Config()
{
    criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&this->standard_config);
    criAtomExVoicePool_SetDefaultConfigForRawPcmVoicePool(&this->rawpcm_config);
    criAtomExVoicePool_SetDefaultConfigForWaveVoicePool(&this->wave_config);
}

VoicePoolWrapper::VoicePoolWrapper()
{
    this->standard_voicepool_hn = NULL;
    this->rawpcm_voicepool_hn = NULL;
    this->wave_voicepool_hn = NULL;
}

void VoicePoolWrapper::CreateVoicePool(const VoicePoolWrapper::Config& config)
{
	this->standard_voicepool_hn = criAtomExVoicePool_AllocateStandardVoicePool(&config.standard_config, NULL, 0);
    this->rawpcm_voicepool_hn = criAtomExVoicePool_AllocateRawPcmVoicePool(&config.rawpcm_config, NULL, 0);
    this->wave_voicepool_hn = criAtomExVoicePool_AllocateWaveVoicePool(&config.wave_config, NULL, 0);
}

void VoicePoolWrapper::DestroyAllVoicePool(void)
{
    if (this->standard_voicepool_hn != NULL) {
        criAtomEx_FreeVoicePool(this->standard_voicepool_hn);
    }
    if (this->rawpcm_voicepool_hn != NULL) {
        criAtomEx_FreeVoicePool(this->rawpcm_voicepool_hn);
    }
    if (this->wave_voicepool_hn != NULL) {
        criAtomEx_FreeVoicePool(this->wave_voicepool_hn);
    }
}

CriAtomExVoicePoolHn VoicePoolWrapper::GetVoicePoolHn(const VoiceType voice_type)
{
    CriAtomExVoicePoolHn vp_hn = NULL;

    switch (voice_type) {
        case VoiceType::Standard:
            vp_hn = this->standard_voicepool_hn;
            break;
        case VoiceType::RawPcm:
            vp_hn = this->rawpcm_voicepool_hn;
            break;
        case VoiceType::Wave:
            vp_hn = this->wave_voicepool_hn;
            break;
        default:
            break;
    }

    return vp_hn;
}

PlayerWrapper::Config::Config() 
{
    this->num_players = 0;
    criAtomExPlayer_SetDefaultConfig(&this->player_config);
    criAtomEx3dSource_SetDefaultConfig(&this->source_config);
    criAtomEx3dListener_SetDefaultConfig(&this->listener_config);
    this->player_config.max_path = MAX_PATH_LENGTH;
}

PlayerWrapper::DataRequestObj::DataRequestObj()
{
    this->noise_type = NoiseType::Sin;
    this->index = 0;
    this->num_samples = 1024;
    this->sampling_rate = CRIATOM_DEFAULT_OUTPUT_SAMPLING_RATE;
    this->num_channels = criAtomExAsrRack_GetOutputChannels(CRIATOMEXASR_RACK_DEFAULT_ID);
    this->frequency = 0;
    this->offset = 0.0f;
    this->buffer[0].resize(this->num_channels * this->num_samples);
    this->buffer[1].resize(this->num_channels * this->num_samples);
    this->enable_channels.resize(this->num_channels);
    this->enable_channels.at(0) = true;
}

void PlayerWrapper::PlayerWrapper::Create(const PlayerWrapper::Config& config)
{
    this->num_players = config.num_players;
    this->players.resize(this->num_players);
    this->sources.resize(this->num_players);
    this->listeners.resize(this->num_players);

    for (int32_t i = 0; i < this->num_players; i++) {
        this->players.at(i) = criAtomExPlayer_Create(&config.player_config, NULL, 0);
        this->sources.at(i) = criAtomEx3dSource_Create(&config.source_config, NULL, 0);
        this->listeners.at(i) = criAtomEx3dListener_Create(&config.listener_config, NULL, 0);

        criAtomExPlayer_Set3dSourceHn(this->players.at(i), this->sources.at(i));
        criAtomExPlayer_Set3dListenerHn(this->players.at(i), this->listeners.at(i));
        criAtomExPlayer_UpdateAll(this->players.at(i));
    }

    assert((size_t)this->num_players == this->players.size());
    assert((size_t)this->num_players == this->sources.size());
    assert((size_t)this->num_players == this->listeners.size());
}

void PlayerWrapper::PlayerWrapper::Destroy(void)
{
    for (const auto& player : this->players) {
        criAtomExPlayer_Set3dSourceHn(player, NULL);
        criAtomExPlayer_Set3dListenerHn(player, NULL);
        criAtomExPlayer_UpdateAll(player);
        criAtomExPlayer_Destroy(player);
    }
    this->players.clear();

    for (const auto& source : this->sources) {
        criAtomEx3dSource_Destroy(source);
    }
    this->sources.clear();

    for (const auto& listener : this->listeners) {
        criAtomEx3dListener_Destroy(listener);
    }
    this->listeners.clear();
}

void PlayerWrapper::Update(const int32_t player_index)
{
    auto player = PlayerWrapper::GetPlayerHn(player_index);
    auto source = this->sources.at(player_index);
    auto listener = this->listeners.at(player_index);
    criAtomEx3dSource_Update(source);
    criAtomEx3dListener_Update(listener);
    criAtomExPlayer_UpdateAll(player);
}

void PlayerWrapper::SetPanType(const int32_t player_index, const CriAtomExPanType type)
{
    auto player = PlayerWrapper::GetPlayerHn(player_index);
    
    if (type == CRIATOMEX_PAN_TYPE_3D_POS) {
        auto source = this->sources.at(player_index);
        auto listener = this->listeners.at(player_index);
        criAtomExPlayer_Set3dSourceHn(player, source);
        criAtomExPlayer_Set3dListenerHn(player, listener);
    } else {
        criAtomExPlayer_Set3dSourceHn(player, NULL);
        criAtomExPlayer_Set3dListenerHn(player, NULL);
    }
}

void PlayerWrapper::SetSourcePosition(const int32_t player_index, const CriAtomExVector position)
{
    auto source = this->sources.at(player_index);
    criAtomEx3dSource_SetPosition(source, &position);
}

void PlayerWrapper::SetListenerPosition(const int32_t player_index, const CriAtomExVector position)
{
    auto listener = this->listeners.at(player_index);
    criAtomEx3dListener_SetPosition(listener, &position);
}

void PlayerWrapper::SetListenerOrientation(const int32_t player_index, const CriAtomExVector front, const CriAtomExVector top)
{
    auto listener = this->listeners.at(player_index);
    criAtomEx3dListener_SetOrientation(listener, &front, &top);
}

void PlayerWrapper::SetMinMaxDistance(const int32_t player_index, const float min_distance, const float max_distance)
{
    auto source = this->sources.at(player_index);
    criAtomEx3dSource_SetMinMaxDistance(source, min_distance, max_distance);
}

CriAtomExPlayerHn PlayerWrapper::GetPlayerHn(const int32_t player_index)
{
    auto player = this->players.at(player_index);

    assert(player != NULL);

    return player;
}

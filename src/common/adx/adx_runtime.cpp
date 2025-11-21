#include <adx_runtime.h>

void ADXRuntime::Initialize(const ADXRuntime::Config& config)
{
#if defined(XPT_TGT_PC)
	criAtomEx_Initialize_WASAPI(&config.specific_config, NULL, 0);
#elif defined(XPT_TGT_MACOSX)
    criAtomEx_Initialize_MACOSX(&config.specific_config, NULL, 0);
#endif
    criAtomExMonitor_Initialize(&config.monitor_config, NULL, 0);
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
    ADXRuntime::vp.DestroyAllVoicePool();

    criAtomExMonitor_Finalize();
#if defined(XPT_TGT_PC)
	criAtomEx_Finalize_WASAPI();
#elif defined(XPT_TGT_MACOSX)
	criAtomEx_Finalize_MACOSX();
#endif
}

std::tuple<int32_t, int32_t> ADXRuntime::GetNumUsedVoicePools(VoiceType voice_type)
{
	CriSint32 num_used_voices = 0;
	CriSint32 num_max_voices = 0;

	criAtomExVoicePool_GetNumUsedVoices(ADXRuntime::vp.GetVoicePoolHn(voice_type), &num_used_voices, &num_max_voices);

	return std::make_tuple(num_used_voices, num_max_voices);
}

VoicePool::Config::Config()
{
    criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&this->standard_config);
    criAtomExVoicePool_SetDefaultConfigForRawPcmVoicePool(&this->rawpcm_config);
    criAtomExVoicePool_SetDefaultConfigForWaveVoicePool(&this->wave_config);
}

VoicePool::VoicePool()
{
    this->standard_voicepool_hn = NULL;
    this->rawpcm_voicepool_hn = NULL;
    this->wave_voicepool_hn = NULL;
}

void VoicePool::CreateVoicePool(const VoicePool::Config& config)
{
	this->standard_voicepool_hn = criAtomExVoicePool_AllocateStandardVoicePool(&config.standard_config, NULL, 0);
    this->rawpcm_voicepool_hn = criAtomExVoicePool_AllocateRawPcmVoicePool(&config.rawpcm_config, NULL, 0);
    this->wave_voicepool_hn = criAtomExVoicePool_AllocateWaveVoicePool(&config.wave_config, NULL, 0);
}

void VoicePool::DestroyAllVoicePool(void)
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

CriAtomExVoicePoolHn VoicePool::GetVoicePoolHn(const VoiceType voice_type)
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

ADXRuntime::Config::Config()
{
#if defined(XPT_TGT_PC)
    criAtomEx_SetDefaultConfig_WASAPI(&this->specific_config);
#elif defined(XPT_TGT_MACOSX)
    criAtomEx_SetDefaultConfig_MACOSX(&this->specific_config);
#endif
    criAtomExMonitor_SetDefaultConfig(&this->monitor_config);
}

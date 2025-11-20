#include <adx_runtime.h>

#if defined(XPT_TGT_PC)
void ADXRuntime::Initialize(CriAtomExConfig_WASAPI* config)
{
	criAtomEx_Initialize_WASAPI(config, NULL, 0);

}
#elif defined(XPT_TGT_MACOSX)
void ADXRuntime::Initialize(CriAtomExConfig_MACOSX* config) 
{
    criAtomEx_Initialize_MACOSX(config, NULL, 0);
}
#endif

#if defined(XPT_TGT_PC)
void ADXRuntime::Initialize(CriAtomExConfig_WASAPI* config, CriAtomExMonitorConfig* monitor_config)
{
	ADXRuntime::Initialize(config);
	criAtomExMonitor_Initialize(monitor_config, NULL, 0);
}
#elif defined(XPT_TGT_MACOSX)
void ADXRuntime::Initialize(CriAtomExConfig_MACOSX* config, CriAtomExMonitorConfig* monitor_config)
{
	ADXRuntime::Initialize(config);
	criAtomExMonitor_Initialize(monitor_config, NULL, 0);
}
#endif


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

	criAtomExVoicePool_GetNumUsedVoices(ADXRuntime::vp.GetVoicePoolHn(VoiceType::Standard), &num_used_voices, &num_max_voices);

	return std::make_tuple(num_used_voices, num_max_voices);
}


void VoicePool::CreateVoicePool(CriAtomExStandardVoicePoolConfig* config)
{
	this->standard_voicepool_hn = criAtomExVoicePool_AllocateStandardVoicePool(config, NULL, 0);
}

void VoicePool::CreateVoicePool(CriAtomExRawPcmVoicePoolConfig* config)
{
    this->rawpcm_voicepool_hn = criAtomExVoicePool_AllocateRawPcmVoicePool(config, NULL, 0);
}

void VoicePool::CreateVoicePool(CriAtomExWaveVoicePoolConfig* config)
{
    this->wave_voicepool_hn = criAtomExVoicePool_AllocateWaveVoicePool(config, NULL, 0);
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
    CriAtomExVoicePoolHn vp_hn;

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


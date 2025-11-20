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

std::tuple<int32_t, int32_t> ADXRuntime::GetNumUsedVoicePools(const int32_t voicepool_index)
{
	CriSint32 num_used_voices = 0;
	CriSint32 num_max_voices = 0;

	criAtomExVoicePool_GetNumUsedVoices(ADXRuntime::vp.GetVoicePoolHn(voicepool_index), &num_used_voices, &num_max_voices);

	return std::make_tuple(num_used_voices, num_max_voices);
}

VoicePool::VoicePool()
{
	this->num_voicepools = 0;
}

void VoicePool::CreateVoicePool(CriAtomExStandardVoicePoolConfig* config)
{
	auto vp_hn = criAtomExVoicePool_AllocateStandardVoicePool(config, NULL, 0);
	this->voice_pools.push_back(vp_hn);
	this->num_voicepools = this->voice_pools.size();
}

void VoicePool::DestroyAllVoicePool(void)
{
	if (this->voice_pools.size() > 0) {
		for (const auto& vp_hn : this->voice_pools) {
			criAtomExVoicePool_Free(vp_hn);
		}
		this->voice_pools.clear();
	}
}

CriAtomExVoicePoolHn VoicePool::GetVoicePoolHn(int32_t index)
{
	return this->voice_pools.at(index);
}

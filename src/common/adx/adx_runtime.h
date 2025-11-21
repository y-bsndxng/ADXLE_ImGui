#pragma once

/* STD */
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <map>
#include <unordered_map>
#include <math.h>
#include <filesystem>
#include <tuple>

/* CRI Headers */
#include <cri_adx2le.h>

#if defined(XPT_TGT_PC)
#include <cri_le_atom_pc.h>
#include <cri_le_atom_wasapi.h>
#elif defined(XPT_TGT_MACOSX)
#include <cri_le_atom_macosx.h>
#endif

enum class VoiceType : std::uint8_t {
    Standard,
    RawPcm,
    Wave
};

class VoicePool {
public:
    VoicePool();
    ~VoicePool() {};
    void CreateVoicePool(CriAtomExStandardVoicePoolConfig* config);
    void CreateVoicePool(CriAtomExRawPcmVoicePoolConfig* config);
    void CreateVoicePool(CriAtomExWaveVoicePoolConfig* config);
    void DestroyAllVoicePool(void);
    CriAtomExVoicePoolHn GetVoicePoolHn(const VoiceType voice_type);
private:
    CriAtomExVoicePoolHn standard_voicepool_hn;
    CriAtomExVoicePoolHn rawpcm_voicepool_hn;
    CriAtomExVoicePoolHn wave_voicepool_hn;
};

struct Config {
public:
    Config();
    ~Config() {};
#if defined(XPT_TGT_PC)
    CriAtomExConfig_WASAPI specific_config;
#elif defined(XPT_TGT_MACOSX)
    CriAtomExConfig_MACOSX specific_config;
#endif
    CriAtomExMonitorConfig monitor_config;
};

namespace ADXRuntime {
	void Initialize(const Config& config);
	bool IsInitilaized(void);
	void Finalize(void);

	std::tuple<int32_t, int32_t> GetNumUsedVoicePools(const VoiceType voice_type);

	/* Runtime Object */
	inline VoicePool vp;
}

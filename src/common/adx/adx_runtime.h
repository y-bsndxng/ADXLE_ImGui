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

/* CRI Headers */
#include <cri_adx2le.h>

#if defined(XPT_TGT_PC)
#include <cri_le_atom_pc.h>
#include <cri_le_atom_wasapi.h>
#elif defined(XPT_TGT_MACOSX)
#include <cri_le_atom_macosx.h>
#endif

class VoicePool {
public:
    VoicePool() {};
    ~VoicePool() {};
    void CreateVoicePool(CriAtomExStandardVoicePoolConfig* config);
    void DestroyAllVoicePool(void);
private:
    std::vector<CriAtomExVoicePoolHn> voice_pools;
};

namespace ADXRuntime {
#if defined(XPT_TGT_PC)
	void Initialize(CriAtomExConfig_WASAPI* config);
#elif defined(XPT_TGT_MACOSX)
	void Initialize(CriAtomExConfig_MACOSX* config);
#endif
#if defined(XPT_TGT_PC)
	void Initialize(CriAtomExConfig_WASAPI* config, CriAtomExMonitorConfig* monitor_config);
#elif defined(XPT_TGT_MACOSX)
	void Initialize(CriAtomExConfig_MACOSX* config, CriAtomExMonitorConfig* monitor_config);
#endif

	bool IsInitilaized(void);
	void Finalize(void);

	/* Runtime Object */
	inline VoicePool vp;
}

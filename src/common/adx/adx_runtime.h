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
#include <cassert>

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
    struct Config {
        Config();
        ~Config() {};
        CriAtomExStandardVoicePoolConfig    standard_config;
        CriAtomExRawPcmVoicePoolConfig      rawpcm_config;
        CriAtomExWaveVoicePoolConfig        wave_config;
    };
    VoicePool();
    ~VoicePool() {};
    void CreateVoicePool(const VoicePool::Config& config);
    void DestroyAllVoicePool(void);
    CriAtomExVoicePoolHn GetVoicePoolHn(const VoiceType voice_type);
private:
    CriAtomExVoicePoolHn standard_voicepool_hn;
    CriAtomExVoicePoolHn rawpcm_voicepool_hn;
    CriAtomExVoicePoolHn wave_voicepool_hn;
};

class Player {
public:
    struct Config {
        Config();
        ~Config() {};
        int32_t num_players;
        CriAtomExPlayerConfig player_config;
        CriAtomEx3dSourceConfig source_config;
        CriAtomEx3dListenerConfig listener_config;
    };
    Player() {};
    ~Player() {};
    int32_t num_players = 0;
    void CreatePlayer(const Player::Config& config);
    void DestroyAllPlayer(void);
    CriAtomExPlayerHn GetPlayerHn(const int32_t& player_index);
private:
    std::vector<CriAtomExPlayerHn> players;
    std::vector<CriAtomEx3dSourceHn> sources;
    std::vector<CriAtomEx3dListenerHn> listeners;
};

namespace ADXRuntime {
    struct Config {
        Config();
        ~Config() {};
        #if defined(XPT_TGT_PC)
        CriAtomExConfig_WASAPI specific_config;
        #elif defined(XPT_TGT_MACOSX)
        CriAtomExConfig_MACOSX specific_config;
        #endif
        CriAtomExMonitorConfig monitor_config;
		CriAtomExAcfRegistrationInfo acf_info;
        CriAtomExDbasConfig dbas_config;
    };
	void Initialize(const ADXRuntime::Config& config);
	bool IsInitilaized(void);
	void Finalize(void);
    void LoadFile(const char* acb_file, const char* awb_file);
    void UnloadFile(void);

	std::tuple<int32_t, int32_t> GetNumUsedVoicePools(const VoiceType voice_type);

	/* Runtime Object */
	inline VoicePool vp;
    inline Player player;
    inline std::vector<CriAtomExPlaybackId> playback_ids;
    inline CriAtomExAcbHn acb_hn;
    inline CriAtomExDbasId dbas_id;
}

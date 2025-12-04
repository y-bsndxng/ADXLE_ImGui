#pragma once

/* STD */
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <math.h>
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

enum class NoiseType : std::uint8_t {
    Sin,
    White,
    Pink
};

class VoicePoolWrapper {
public:
    struct Config {
        Config();
        ~Config() {};
        CriAtomExStandardVoicePoolConfig    standard_config;
        CriAtomExRawPcmVoicePoolConfig      rawpcm_config;
        CriAtomExWaveVoicePoolConfig        wave_config;
    };
    VoicePoolWrapper();
    ~VoicePoolWrapper() {};
    void CreateVoicePool(const VoicePoolWrapper::Config& config);
    void DestroyAllVoicePool(void);
    CriAtomExVoicePoolHn GetVoicePoolHn(const VoiceType voice_type);
private:
    CriAtomExVoicePoolHn standard_voicepool_hn;
    CriAtomExVoicePoolHn rawpcm_voicepool_hn;
    CriAtomExVoicePoolHn wave_voicepool_hn;
};

class PlayerWrapper {
public:
    struct Config {
        Config();
        ~Config() {};
        int32_t num_players;
        CriAtomExPlayerConfig player_config;
        CriAtomEx3dSourceConfig source_config;
        CriAtomEx3dListenerConfig listener_config;
    };
    struct DataRequestObj {
        DataRequestObj();
        ~DataRequestObj() {};
        NoiseType noise_type;
        int32_t index;
        int32_t num_channels;
        int32_t num_samples;
        int32_t sampling_rate;
        float frequency;
        float offset;
        std::vector<int16_t> buffer[2];
        std::vector<bool> enable_channels;
    };
    PlayerWrapper() {};
    ~PlayerWrapper() {};
    int32_t num_players = 0;
    void Create(const PlayerWrapper::Config& config);
    void Destroy(void);
    void Update(const int32_t player_index);
    void SetPanType(const int32_t player_index, const CriAtomExPanType type);
    void SetSourcePosition(const int32_t player_index, const CriAtomExVector position);
    void SetListenerPosition(const int32_t player_index, const CriAtomExVector position);
    void SetListenerOrientation(const int32_t player_index, const CriAtomExVector front, const CriAtomExVector top);
    CriAtomExPlayerHn GetPlayerHn(const int32_t player_index);
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
void LoadFile(const char* acf_file, const char* acb_file, const char* awb_file);
void LoadFile(const char* acb_file, const char* awb_file);
void UnloadFile(void);
std::tuple<bool, CriAtomExAcfInfo> GetAcfInfo(void);
std::tuple<bool, CriAtomExAcbInfo> GetAcbInfo(void);
std::tuple<bool, CriAtomExCueInfo> GetCueInfo(const char* name);
std::tuple<bool, CriAtomExAcfDspBusInfo> GetBusInfo(const int32_t bus_index);
std::vector<std::string> GetCueNames(void);
std::vector<std::string> GetBusNames(void);
std::vector<std::string> GetAfxNames(const CriAtomExAsrRackId rack_id, const int32_t bus_index);
std::vector<std::string> GetAfxNames(const int32_t bus_index);
std::tuple<int32_t, int32_t> GetNumUsedVoicePools(const VoiceType voice_type);
CriAtomSpeakerMapping GetSpeakerMapping(void);
CriAtomSpeakerMapping GetSpeakerMapping(const CriAtomExAsrRackId rack_id);
std::tuple<int64_t, int32_t> GetNumOutputSamples(void);
std::tuple<int64_t, int32_t> GetNumOutputSamples(const CriAtomExAsrRackId rack_id);
std::tuple<int64_t, int32_t> GetNumRenderedSamples(void);
std::tuple<int64_t, int32_t> GetNumRenderedSamples(const CriAtomExAsrRackId rack_id);
CriAtomExAsrRackPerformanceInfo GetPerformanceInfo(void);
CriAtomExAsrRackPerformanceInfo GetPerformanceInfo(const CriAtomExAsrRackId rack_id);
void ResetPerformanceInfo(void);
void ResetPerformanceInfo(const CriAtomExAsrRackId rack_id);

/* Runtime Object */
inline VoicePoolWrapper voicepool_wrapper;
inline PlayerWrapper player_wrapper;
inline std::vector<CriAtomExPlaybackId> playback_ids;
inline CriAtomExAcbHn acb_hn;
inline CriAtomExDbasId dbas_id;
}

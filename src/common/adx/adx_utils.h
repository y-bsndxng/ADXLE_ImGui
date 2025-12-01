#pragma once

#include <cstdio>
/* CRI Headers */
#include <cri_adx2le.h>

#include <adx_runtime.h>

#define UNUSED(arg) if(arg == arg) {}

namespace ADXUtils {
void UserErrorCallBackFunc(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray);
std::string GetErrorMessage(void);
void* UserAllocFunc(void* obj, CriUint32 size);
void UserFreeFunc(void* obj, void* ptr);
const char* GetNoiseTypeString(const NoiseType noise_type);
const char* GetVoiceTypeString(const VoiceType voice_type);
const char* GetPlayerStatusString(const CriAtomExPlayerHn player);
const char* GetPanTypeString(const CriAtomExPanType pan_type);
const char* GetSpeakerMappingString(const CriAtomSpeakerMapping speaker_mapping);
std::map<int32_t, std::string> GetSpeakerMappingLabel(const CriAtomSpeakerMapping speaker_mapping);
float LevelToDecibel(float value);
float NormalizeDecibel(float decibel);
void Interleave(const std::vector<std::vector<int16_t>>& src, std::vector<int16_t>& dst, const int32_t max_channels, const int32_t max_samples);
void Deinterleave(const std::vector<int16_t>& src, std::vector<std::vector<int16_t>>& dst, const int32_t max_channels, const int32_t max_samples);
}

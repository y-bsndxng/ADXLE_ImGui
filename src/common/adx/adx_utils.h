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
	std::string GetVoiceTypeString(const VoiceType voice_type);
	std::string GetPlayerStatusString(const CriAtomExPlayerHn player);
}
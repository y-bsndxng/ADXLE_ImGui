#include <adx_runtime.h>

#if defined(XPT_TGT_PC)
bool ADXRuntime::Initialize(CriAtomExConfig_WASAPI& config)
{
	criAtomEx_Initialize_WASAPI(&config, NULL, 0);
	if (criAtomEx_IsInitialized() == CRI_FALSE) {
		return false;
	} else {
		return true;
	}
}
#elif defined(XPT_TGT_MACOSX)
bool ADXRuntime::Initialize(CriAtomExConfig_MACOSX& config) 
{
	criAtomEx_Initialize_MACOSX(&config, NULL, 0);
	if (criAtomEx_IsInitialized() == CRI_FALSE) {
		return false;
	} else {
		return true;
	}
}
#endif

#include <adx_runtime.h>

#if defined(XPT_TGT_PC)
void ADXRuntime::Initialize(CriAtomExConfig_WASAPI* config)
{
	criAtomEx_Initialize_WASAPI(config, NULL, 0);

}
#elif defined(XPT_TGT_MACOSX)
void ADXRuntime::Initialize(CriAtomExConfig_MACOSX* config) 
{
	criAtomEx_Initialize_MACOSX(&config, NULL, 0);
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
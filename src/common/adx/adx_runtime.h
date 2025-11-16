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

namespace ADXRuntime {
#if defined(XPT_TGT_PC)
	bool Initialize(CriAtomExConfig_WASAPI& config);
#elif defined(XPT_TGT_MACOSX)
	bool Initialize(CriAtomExConfig_MACOSX& config);
#endif
}
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
#include <cri_le_atom.h>
#include <cri_le_atom_asr.h>
#include <cri_le_atom_ex.h>
#include <cri_le_atom_ex_monitor.h>

#if defined(XPT_TGT_PC)
#include <cri_le_atom_pc.h>
#include <cri_le_atom_wasapi.h>
#elif defined(XPT_TGT_MACOSX)
#include <cri_le_atom_macosx.h>
#endif

#define UNUSED(arg) if(arg == arg) {}

inline void* user_alloc_func(void* obj, CriUint32 size);
inline void user_free_func(void* obj, void* ptr);
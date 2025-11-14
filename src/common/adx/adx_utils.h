#pragma once

#include <cstdio>
/* CRI Headers */
#include <cri_adx2le.h>

#define UNUSED(arg) if(arg == arg) {}

namespace ADXUtils {
	void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray);
	void* user_alloc_func(void* obj, CriUint32 size);
	void user_free_func(void* obj, void* ptr);
}
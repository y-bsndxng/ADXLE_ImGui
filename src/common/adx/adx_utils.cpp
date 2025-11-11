#include "adx_utils.h"

inline void* user_alloc_func(void* obj, CriUint32 size)
{
	void* ptr;
	UNUSED(obj);
	ptr = malloc(size);
	return ptr;
}

inline void user_free_func(void* obj, void* ptr)
{
	UNUSED(obj);
	free(ptr);
}
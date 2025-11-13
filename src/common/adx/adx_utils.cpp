#include <adx_utils.h>

void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray)
{
	const CriChar8* errmsg;

	UNUSED(parray);

	/* エラー文字列の表示 */
	errmsg = criErr_ConvertIdToMessage(errid, p1, p2);
	printf("%s\n", errmsg);

	return;
}

void* user_alloc_func(void* obj, CriUint32 size)
{
	void* ptr;
	UNUSED(obj);
	ptr = malloc(size);
	return ptr;
}

void user_free_func(void* obj, void* ptr)
{
	UNUSED(obj);
	free(ptr);
}
#include <adx_utils.h>

static std::string error_message = "";

void ADXUtils::UserErrorCallBackFunc(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray)
{
	const CriChar8* errmsg;

	UNUSED(parray);

	/* エラー文字列の表示 */
	errmsg = criErr_ConvertIdToMessage(errid, p1, p2);
	error_message = std::string(errmsg);

	return;
}

std::string ADXUtils::GetErrorMessage(void)
{
	return error_message;
}

void* ADXUtils::UserAllocFunc(void* obj, CriUint32 size)
{
	void* ptr;
	UNUSED(obj);
	ptr = malloc(size);
	return ptr;
}

void ADXUtils::UserFreeFunc(void* obj, void* ptr)
{
	UNUSED(obj);
	free(ptr);
}

std::string ADXUtils::GetVoiceTypeString(const VoiceType voice_type)
{
	std::string ret = "";

	switch (voice_type) {
	case VoiceType::Standard:
		ret = "Standard";
		break;
	case VoiceType::RawPcm:
		ret = "RawPcm";
		break;
	case VoiceType::Wave:
		ret = "Wave";
		break;
	default:
		break;
	}

	return ret;
}

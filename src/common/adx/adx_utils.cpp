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
	switch (voice_type) {
	case VoiceType::Standard:
		return "Standard";
	case VoiceType::RawPcm:
		return "RawPcm";
	case VoiceType::Wave:
		return "Wave";
	default:
        return "UNKNOWN";
	}
}

std::string ADXUtils::GetPlayerStatusString(const CriAtomExPlayerHn player)
{
    auto status = criAtomExPlayer_GetStatus(player);

    switch (status) {
    case CRIATOMEXPLAYER_STATUS_STOP:
        return "STOP";
    case CRIATOMEXPLAYER_STATUS_PREP:
        return "PREP";
    case CRIATOMEXPLAYER_STATUS_PLAYEND:
        return "PLAYEND";
    case CRIATOMEXPLAYER_STATUS_PLAYING:
        return "PLAYING";
    case CRIATOMEXPLAYER_STATUS_ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

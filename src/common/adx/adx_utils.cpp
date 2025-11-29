#include <adx_utils.h>

static std::string error_message = "";

void ADXUtils::UserErrorCallBackFunc(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray)
{
	const CriChar8* errmsg;

	UNUSED(parray);

	/* エラー文字列の表示 */
	errmsg = criErr_ConvertIdToMessage(errid, p1, p2);
	error_message = std::string(errmsg);
    printf("%s\n", errmsg);

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

const char* ADXUtils::GetVoiceTypeString(const VoiceType voice_type)
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

const char* ADXUtils::GetPlayerStatusString(const CriAtomExPlayerHn player)
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

const char* ADXUtils::GetPanTypeString(const CriAtomExPanType pan_type)
{
    switch (pan_type) {
    case CRIATOMEX_PAN_TYPE_PAN3D:
        return "PAN3D";
    case CRIATOMEX_PAN_TYPE_3D_POS:
        return "3D_POS";
    case CRIATOMEX_PAN_TYPE_AUTO:
        return "AUTO";
    default:
        return "UNKNOWN";
    }
}

const char* ADXUtils::GetSpeakerMappingString(const CriAtomSpeakerMapping speaker_mapping)
{
    switch (speaker_mapping) {
    case CRIATOM_SPEAKER_MAPPING_AUTO:
        return "AUTO";
    case CRIATOM_SPEAKER_MAPPING_MONO:
        return "MONO";
    case CRIATOM_SPEAKER_MAPPING_STEREO:
        return "STEREO";
    case CRIATOM_SPEAKER_MAPPING_5_1:
        return "5_1";
    case CRIATOM_SPEAKER_MAPPING_7_1:
        return "7_1";
    case CRIATOM_SPEAKER_MAPPING_5_1_2:
        return "5_1_2";
    case CRIATOM_SPEAKER_MAPPING_7_1_2:
        return "7_1_2";
    case CRIATOM_SPEAKER_MAPPING_7_1_4:
        return "7_1_4";
    case CRIATOM_SPEAKER_MAPPING_7_1_4_4:
        return "7_1_4_4";
    case CRIATOM_SPEAKER_MAPPING_AMBISONICS_1P:
        return "AMBISONICS_1P";
    case CRIATOM_SPEAKER_MAPPING_AMBISONICS_2P:
        return "AMBISONICS_2P";
    case CRIATOM_SPEAKER_MAPPING_AMBISONICS_3P:
        return "AMBISONICS_3P";
    case CRIATOM_SPEAKER_MAPPING_OBJECT:
        return "OBJECT";
    default:
        return "UNKNOWN";
    }
}

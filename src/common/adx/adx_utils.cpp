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

std::map<int32_t, std::string> ADXUtils::GetSpeakerMappingLabel(const CriAtomSpeakerMapping speaker_mapping)
{
    std::map<int32_t, std::string> meter_label;
    
    switch (speaker_mapping) {
    case CRIATOM_SPEAKER_MAPPING_OBJECT:
        meter_label = {
            {0, "0"}, {1, "1"}, {2, "2"}, {3, "3"}, {4, "4"}, {5, "5"}, {6, "6"}, {7, "7"},
            {8, "8"}, {9, "9"}, {10, "10"}, {11, "11"}, {12, "12"}, {13, "13"}, {14, "14"}, {15, "15"},
        };
        break;
    case CRIATOM_SPEAKER_MAPPING_AMBISONICS_1P:
    case CRIATOM_SPEAKER_MAPPING_AMBISONICS_2P:
    case CRIATOM_SPEAKER_MAPPING_AMBISONICS_3P:
        meter_label = {
            {0, "W"}, {1, "X"}, {2, "Y"}, {3, "Z"}, {4, "R"}, {5, "S"}, {6, "T"}, {7, "U"},
            {8, "V"}, {9, "K"}, {10, "L"}, {11, "M"}, {12, "N"}, {13, "O"}, {14, "P"}, {15, "Q"},
        };
        break;
    case CRIATOM_SPEAKER_MAPPING_7_1_4_4:
        meter_label = {
            {0, "L"}, {1, "R"}, {2, "C"}, {3, "LFE"}, {4, "SL"}, {5, "SR"}, {6, "BL"}, {7, "BR"},
            {8, "TFL"}, {9, "TFR"}, {10, "TBL"}, {11, "TBR"}, {12, "BFL"}, {13, "BFR"}, {14, "BBL"}, {15, "BBR"},
        };
        break;
    case CRIATOM_SPEAKER_MAPPING_7_1_4:
        meter_label = {
            {0, "L"}, {1, "R"}, {2, "C"}, {3, "LFE"}, {4, "SL"}, {5, "SR"}, {6, "BL"}, {7, "BR"},
            {8, "TFL"}, {9, "TFR"}, {10, "TBL"}, {11, "TBR"}, {12, "BFL"},
        };
        break;
    case CRIATOM_SPEAKER_MAPPING_7_1_2:
        meter_label = {
            {0, "L"}, {1, "R"}, {2, "C"}, {3, "LFE"}, {4, "SL"}, {5, "SR"}, {6, "BL"}, {7, "BR"},
            {8, "TL"}, {9, "TR"},
        };
        break;
    case CRIATOM_SPEAKER_MAPPING_5_1_2:
        meter_label = {
            {0, "L"}, {1, "R"}, {2, "C"}, {3, "LFE"}, {4, "SL"}, {5, "SR"}, {6, "TL"}, {7, "TR"},
        };
        break;
    case CRIATOM_SPEAKER_MAPPING_MONO:
        meter_label = {
            {0, "C"},
        };
        break;
    case CRIATOM_SPEAKER_MAPPING_AUTO:
        meter_label = {
            {0, "L"}, {1, "R"}, {2, "C"}, {3, "LFE"}, {4, "SL"}, {5, "SR"}, {6, "BL"}, {7, "BR"},
            {8, "TFL"}, {9, "TFR"}, {10, "TBL"}, {11, "TBR"}, {12, "BFL"}, {13, "BFR"}, {14, "BBL"}, {15, "BBR"},
        };
        break;
    default:
        meter_label = {
            {0, "L"}, {1, "R"}, {2, "C"}, {3, "LFE"}, {4, "SL"}, {5, "SR"}, {6, "BL"}, {7, "BR"},
            {8, "TFL"}, {9, "TFR"}, {10, "TBL"}, {11, "TBR"}, {12, "BFL"}, {13, "BFR"}, {14, "BBL"}, {15, "BBR"},
        };
        break;
    }
    
    return meter_label;
}

float ADXUtils::LevelToDecibel(float value)
{
    float decibel = 0.0f;

    if (value <= 0.0f) {
        decibel = -96.0f;
    } else {
        decibel = 20.0f * log10f(value);
    }

    return decibel;
}

float ADXUtils::NormalizeDecibel(float decibel)
{
    float min_db = -96.0f;
    return std::clamp((decibel - min_db) / (0.0f - min_db), 0.0f, 1.0f);
}


#pragma once

typedef enum _tagENeptuneDevType
{
	NEPTUNE_DEV_TYPE_UNKNOWN	= -1,	// unknown camera
	NEPTUNE_DEV_TYPE_GIGE		= 0,	// GigE camera
	NEPTUNE_DEV_TYPE_1394		= 1,	// 1394 camera
	NEPTUNE_DEV_TYPE_USB3		= 2		// USB3 Vision camera
} ENeptuneDevType;


typedef enum _tagENeptuneDevAccess 
{
	NEPTUNE_DEV_ACCESS_UNKNOWN		= -1,
	NEPTUNE_DEV_ACCESS_EXCLUSIVE	= 0,
	NEPTUNE_DEV_ACCESS_CONTROL		= 1,
	NEPTUNE_DEV_ACCESS_MONITOR		= 2
} ENeptuneDevAccess ;


typedef struct _tagNEPTUNE_CAM_INFO {
	_char_t				strVendor[MAX_STRING_LENGTH];	// camera vendor name
	_char_t				strModel[MAX_STRING_LENGTH];	// camera model name
	_char_t				strSerial[MAX_STRING_LENGTH];	// camera serial number
	_char_t				strUserID[MAX_STRING_LENGTH];	// user ID
	_char_t				strIP[MAX_STRING_LENGTH];		// IP address(GigE camera)
	_char_t				strMAC[MAC_LENGTH];				// MAC address(GigE camera)
	_char_t				strSubnet[MAX_STRING_LENGTH];	// Subnet address(GigE camera)
	_char_t				strGateway[MAX_STRING_LENGTH];	// Gateway address(GigE camera)
	_char_t				strCamID[MAX_STRING_LENGTH];	// unique Camera ID

	_tagNEPTUNE_CAM_INFO()
	{
		memset(strVendor, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
		memset(strModel, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
		memset(strSerial, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
		memset(strUserID, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
		memset(strIP, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
		memset(strMAC, 0, sizeof(_char_t)*MAC_LENGTH);
		memset(strSubnet, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
		memset(strGateway, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
		memset(strCamID, 0, sizeof(_char_t)*MAX_STRING_LENGTH);
	}
} NEPTUNE_CAM_INFO, *PNEPTUNE_CAM_INFO;



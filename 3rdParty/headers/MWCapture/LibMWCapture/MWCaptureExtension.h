////////////////////////////////////////////////////////////////////////////////
// CONFIDENTIAL and PROPRIETARY software of Magewell Electronics Co., Ltd.
// Copyright (c) 2011-2014 Magewell Electronics Co., Ltd. (Nanjing) 
// All rights reserved.
// This copyright notice MUST be reproduced on all authorized copies.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(push)
#pragma warning(disable : 4201)
#include <ks.h>
#pragma warning(pop)

#include "MWCommon.h"
#include "MWIEC60958.h"
#include "MWSMPTE.h"
#include "MWHDMIPackets.h"

#pragma pack(push)
#pragma pack(1)

////////////////////////////////////////////////////////////////////////////////
// Magewell Capture Extensions

typedef enum _MWCAP_KSPROPERTY_EXTENSION {
	// Product informations
	MWCAP_KSPROPERTY_CHANNEL_INFO,								// G, 	Prop: KSPROPERTY,								Data: MWCAP_CHANNEL_INFO
	MWCAP_KSPROPERTY_FAMILY_INFO,								// G, 	Prop: KSPROPERTY,								Data: MWCAP_PRO_CAPTURE_INFO, MWCAP_VALUE_CAPTURE_INFO

	MWCAP_KSPROPERTY_VIDEO_CAPS,								// G, 	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_CAPS
	MWCAP_KSPROPERTY_AUDIO_CAPS,								// G,	Prop: KSPROPERTY,								Data: MWCAP_AUDIO_CAPS

	MWCAP_KSPROPERTY_DEVICE_INSTANCE_ID,						// G,	Prop: KSPROPERTY,								Data: WCHAR[_MAX_PATH]

	// Firmware upgrade
	MWCAP_KSPROPERTY_FIRMWARE_STORAGE,							// G, 	Prop: KSPROPERTY,								Data: MWCAP_FIRMWARE_STORAGE
	MWCAP_KSPROPERTY_FIRMWARE_ERASE,							// S,	Prop: KSPROPERTY,								Data: MWCAP_FIRMWARE_ERASE
	MWCAP_KSPROPERTY_FIRMWARE_DATA,								// G/S, Prop: MWCAP_KSPROPERTY_DWORD_S, 				Data: BYTE[]

	// Device misc controls
	MWCAP_KSPROPERTY_LED_MODE,									// S,	Prop: KSPROPERTY,								Data: DWORD
	MWCAP_KSPROPERTY_POST_RECONFIG,								// S,	Prop: KSPROPERTY,								Data: DWORD

	// Hardware timer
	MWCAP_KSPROPERTY_TIME,										// G/S, Prop: KSPROPERTY,								Data: LONGLONG
	MWCAP_KSPROPERTY_TIME_REGULATION,							// S,	Prop: KSPROPERTY,								Data: LONGLONG

	MWCAP_KSPROPERTY_TIMER_REGISTRATION,						// G,	Prop: MWCAP_KSPROPERTY_PTR64_S,					Data: MWCAP_PTR64
	MWCAP_KSPROPERTY_TIMER_DEREGISTRATION,						// S,	Prop: KSPROPERTY,								Data: MWCAP_PTR64
	MWCAP_KSPROPERTY_TIMER_EXPIRE_TIME,							// S,	Prop: KSPROPERTY,								Data: MWCAP_TIMER_EXPIRE_TIME

	// Notifications
	MWCAP_KSPROPERTY_NOTIFY_REGISTRATION,						// G,	Prop: MWCAP_KSPROPERTY_NOTIFY_REGISTRATION_S,	Data: MWCAP_PTR64
	MWCAP_KSPROPERTY_NOTIFY_DEREGISTRATION,						// S,	Prop: KSPROPERTY,								Data: MWCAP_PTR64
	MWCAP_KSPROPERTY_NOTIFY_STATUS,								// G,	Prop: MWCAP_KSPROPERTY_PTR64_S,					Data: ULONGLONG
	MWCAP_KSPROPERTY_NOTIFY_ENABLE,								// S,	Prop: MWCAP_KSPROPERTY_PTR64_S,					Data: ULONGLONG

	// Video frame information
	MWCAP_KSPROPERTY_VIDEO_BUFFER_INFO,							// G,   Prop: KSPROPERTY,								Data: MWCAP_VIDEO_BUFFER_INFO
	MWCAP_KSPROPERTY_VIDEO_FRAME_INFO,							// G,   Prop: MWCAP_KSPROPERTY_DWORD_S,					Data: MWCAP_VIDEO_FRAME_INFO

	// Video capture
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_OPEN,						// S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_CAPTURE_OPEN
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_FRAME,						// S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_CAPTURE_FRAME
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_STATUS,						// G,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_CAPTURE_STATUS
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_CLOSE,						// S,	Prop: KSPROPERTY,								Data: None

	// Audio capture
	MWCAP_KSPROPERTY_AUDIO_CAPTURE_OPEN,						// S,	Prop: KSPROPERTY,								Data: None
	MWCAP_KSPROPERTY_AUDIO_CAPTURE_FRAME,						// G,	Prop: KSPROPERTY,								Data: MWCAP_AUDIO_CAPTURE_FRAME
	MWCAP_KSPROPERTY_AUDIO_CAPTURE_CLOSE,						// S,	Prop: KSPROPERTY,								Data: None

	// Input source
	MWCAP_KSPROPERTY_VIDEO_INPUT_SOURCE_COUNT,					// G,	Prop: KSPROPERTY, 								Data: DWORD
	MWCAP_KSPROPERTY_VIDEO_INPUT_SOURCE_ARRAY,					// G,	Prop: KSPROPERTY,								Data: DWORD[]

	MWCAP_KSPROPERTY_AUDIO_INPUT_SOURCE_COUNT,					// G,	Prop: KSPROPERTY, 								Data: DWORD
	MWCAP_KSPROPERTY_AUDIO_INPUT_SOURCE_ARRAY,					// G,	Prop: KSPROPERTY,								Data: DWORD[]

	MWCAP_KSPROPERTY_INPUT_SOURCE_SCAN,							// G/S,	Prop: KSPROPERTY,								Data: BOOLEAN
	MWCAP_KSPROPERTY_INPUT_SOURCE_SCAN_STATE,					// G,	Prop: KSPROPERTY,								Data: BOOLEAN
	MWCAP_KSPROPERTY_AV_INPUT_SOURCE_LINK,						// G/S,	Prop: KSPROPERTY,								Data: BOOLEAN
	MWCAP_KSPROPERTY_VIDEO_INPUT_SOURCE,						// G/S,	Prop: KSPROPERTY,								Data: DWORD
	MWCAP_KSPROPERTY_AUDIO_INPUT_SOURCE,						// G/S, Prop: KSPROPERTY,								Data: DWORD

	// EDID
	MWCAP_KSPROPERTY_EDID,										// G/S,	Prop: KSPROPERTY,								Data: BYTE[]

	// Signal status
	MWCAP_KSPROPERTY_INPUT_SPECIFIC_STATUS,						// G,   Prop: KSPROPERTY,								Data: MWCAP_INPUT_SPECIFIC_STATUS
	MWCAP_KSPROPERTY_VIDEO_SIGNAL_STATUS,						// G,   Prop: KSPROPERTY,								Data: MWCAP_VIDEO_SIGNAL_STATUS
	MWCAP_KSPROPERTY_AUDIO_SIGNAL_STATUS,						// G,	Prop: KSPROPERTY,								Data: MWCAP_AUDIO_SIGNAL_STATUS

	// HDMI status
	MWCAP_KSPROPERTY_HDMI_INFOFRAME_VALID,						// G,	Prop: KSPROPERTY,								Data: DWORD
	MWCAP_KSPROPERTY_HDMI_INFOFRAME_PACKET,						// G,	Prop: MWCAP_KSPROPERTY_DWORD_S,					Data: HDMI_INFOFRAME_PACKET

	// Video processing
	MWCAP_KSPROPERTY_VIDEO_INPUT_ASPECT_RATIO,					// G/S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_ASPECT_RATIO
	MWCAP_KSPROPERTY_VIDEO_INPUT_COLOR_FORMAT,					// G/S, Prop: KSPROPERTY,								Data: MWCAP_VIDEO_COLOR_FORMAT
	MWCAP_KSPROPERTY_VIDEO_INPUT_QUANTIZATION_RANGE,			// G/S, Prop: KSPROPERTY,								Data: MWCAP_VIDEO_QUANTIZATION_RANGE

	// Video processing (NOT functional for DirectKS)
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_CONNECTION_FORMAT,			// G,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_CONNECTION_FORMAT
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_PROCESS_PRESET,				// G/S, Prop: KSPROPERTY,								Data: MWCAP_VIDEO_PROCESS_SETTINGS
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_PROCESS_SETTINGS,			// G/S, Prop: KSPROPERTY,								Data: MWCAP_VIDEO_PROCESS_SETTINGS
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_OSD_PRESET,					// G/S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_OSD_SETTINGS
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_OSD_SETTINGS,				// G/S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_OSD_SETTINGS
	MWCAP_KSPROPERTY_VIDEO_CAPTURE_OSD_IMAGE,					// G/S, Prop: KSPROPERTY,								Data: MWCAP_VIDEO_OSD_IMAGE

	MWCAP_KSPROPERTY_VIDEO_PREVIEW_CONNECTION_FORMAT,			// G,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_CONNECTION_FORMAT
	MWCAP_KSPROPERTY_VIDEO_PREVIEW_PROCESS_PRESET,				// G/S, Prop: KSPROPERTY,								Data: MWCAP_VIDEO_PROCESS_SETTINGS
	MWCAP_KSPROPERTY_VIDEO_PREVIEW_PROCESS_SETTINGS,			// G/S, Prop: KSPROPERTY,								Data: MWCAP_VIDEO_PROCESS_SETTINGS
	MWCAP_KSPROPERTY_VIDEO_PREVIEW_OSD_PRESET,					// G/S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_OSD_SETTINGS
	MWCAP_KSPROPERTY_VIDEO_PREVIEW_OSD_SETTINGS,				// G/S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_OSD_SETTINGS
	MWCAP_KSPROPERTY_VIDEO_PREVIEW_OSD_IMAGE,					// G/S, Prop: KSPROPERTY,								Data: MWCAP_VIDEO_OSD_IMAGE

	// Temperature
	MWCAP_KSPROPERTY_CORE_TEMPERATURE,							// G,	Prop: KSPROPERTY,								Data: LONG	(in 0.1 deg C)

	// Image upload, can be use as source image or OSD image
	MWCAP_KSPROPERTY_VIDEO_CREATE_IMAGE,						// G,	Prop: MWCAP_KSPROPERTY_VIDEO_CREATE_IMAGE_S,	Data: MWCAP_PTR64
	MWCAP_KSPROPERTY_VIDEO_OPEN_IMAGE,							// G,	Prop: MWCAP_KSPROPERTY_PTR64_S,					Data: LONG
	MWCAP_KSPROPERTY_VIDEO_CLOSE_IMAGE,							// G,	Prop: MWCAP_KSPROPERTY_PTR64_S,					Data: LONG
	MWCAP_KSPROPERTY_VIDEO_UPLOAD_IMAGE,						// S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_UPLOAD_IMAGE

	// VGA/Component timings
	MWCAP_KSPROPERTY_VIDEO_AUTO_H_ALIGN,						// G/S, Prop: KSPROPERTY,								Data: BOOLEAN
	MWCAP_KSPROPERTY_VIDEO_SAMPLING_PHASE,						// G/S, Prop: KSPROPERTY,								Data: BYTE (0-31, 255 triger auto adjust once)
	MWCAP_KSPROPERTY_VIDEO_SAMPLING_PHASE_AUTO,					// G/S, Prop: KSPROPERTY,								Data: BOOLEAN
	MWCAP_KSPROPERTY_VIDEO_TIMING,								// S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_TIMING
	MWCAP_KSPROPERTY_VIDEO_PREFERRED_TIMING_ARRAY,				// G,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_TIMING[MWCAP_VIDEO_MAX_NUM_PREFERRED_TIMINGS]
	MWCAP_KSPROPERTY_VIDEO_CUSTOM_TIMING,						// S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_CUSTOM_TIMING
	MWCAP_KSPROPERTY_VIDEO_CUSTOM_TIMING_COUNT,					// G,	Prop: KSPROPERTY,								Data: DWORD
	MWCAP_KSPROPERTY_VIDEO_CUSTOM_TIMING_ARRAY,					// G/S, Prop: KSPROPERTY,								Data: MWCAP_VIDEO_CUSTOM_TIMING[]
	MWCAP_KSPROPERTY_VIDEO_CUSTOM_RESOLUTION_COUNT,				// G,	Prop: KSPROPERTY,								Data: DWORD
	MWCAP_KSPROPERTY_VIDEO_CUSTOM_RESOLUTION_ARRAY,				// G/S, Prop: KSPROPERTY,								Data: SIZE[]

	// Video capture enhancement
	MWCAP_KSPROPERTY_VIDEO_PIN_BUFFER,							// S,	Prop: KSPROPERTY,								Data: MWCAP_VIDEO_PIN_BUFFER
	MWCAP_KSPROPERTY_VIDEO_UNPIN_BUFFER							// S,	Prop: KSPROPERTY,								Data: MWCAP_PTR64
} MWCAP_KSPROPERTY_EXTENSION;

////////////////////////////////////////////////////////////////////////////////
// Magewell Video Capture Filter Extension Property Set

#define STATIC_MWCAP_PROPSETID_FILTER \
    0x3a45b930, 0xff5c, 0x42aa, 0xa5, 0xa2, 0x43, 0x77, 0x1b, 0x93, 0xeb, 0xef
DEFINE_GUIDSTRUCT("3A45B930-FF5C-42AA-A5A2-43771B93EBEF", MWCAP_PROPSETID_FILTER);
#define MWCAP_PROPSETID_FILTER DEFINE_GUIDNAMED(MWCAP_PROPSETID_FILTER)

////////////////////////////////////////////////////////////////////////////////
// Magewell Video Capture Pro Family Specific Extension Property Set

#define STATIC_MWCAP_PROPSETID_PRO \
    0x3a45b931, 0xff5c, 0x42aa, 0xa5, 0xa2, 0x43, 0x77, 0x1b, 0x93, 0xeb, 0xef
DEFINE_GUIDSTRUCT("3A45B931-FF5C-42AA-A5A2-43771B93EBEF", MWCAP_PROPSETID_PRO);
#define MWCAP_PROPSETID_PRO DEFINE_GUIDNAMED(MWCAP_PROPSETID_PRO)

////////////////////////////////////////////////////////////////////////////////
// Magewell Video Capture HDMI Extension Property Set

#define STATIC_MWCAP_PROPSETID_HDMI \
    0x3a45b932, 0xff5c, 0x42aa, 0xa5, 0xa2, 0x43, 0x77, 0x1b, 0x93, 0xeb, 0xef
DEFINE_GUIDSTRUCT("3A45B932-FF5C-42AA-A5A2-43771B93EBEF", MWCAP_PROPSETID_HDMI);
#define MWCAP_PROPSETID_HDMI DEFINE_GUIDNAMED(MWCAP_PROPSETID_HDMI)

////////////////////////////////////////////////////////////////////////////////
// Magewell Video Capture Timing Extension Property Set

#define STATIC_MWCAP_PROPSETID_TIMING \
    0x3a45b933, 0xff5c, 0x42aa, 0xa5, 0xa2, 0x43, 0x77, 0x1b, 0x93, 0xeb, 0xef
DEFINE_GUIDSTRUCT("3A45B933-FF5C-42AA-A5A2-43771B93EBEF", MWCAP_PROPSETID_TIMING);
#define MWCAP_PROPSETID_TIMING DEFINE_GUIDNAMED(MWCAP_PROPSETID_TIMING)

////////////////////////////////////////////////////////////////////////////////
// Magewell Video Capture OSD Extension Property Set

#define STATIC_MWCAP_PROPSETID_OSD \
    0x3a45b934, 0xff5c, 0x42aa, 0xa5, 0xa2, 0x43, 0x77, 0x1b, 0x93, 0xeb, 0xef
DEFINE_GUIDSTRUCT("3A45B934-FF5C-42AA-A5A2-43771B93EBEF", MWCAP_PROPSETID_OSD);
#define MWCAP_PROPSETID_OSD DEFINE_GUIDNAMED(MWCAP_PROPSETID_OSD)

////////////////////////////////////////////////////////////////////////////////
// Magewell Video Capture Video Extension Property Set

#define STATIC_MWCAP_PROPSETID_VIDEO \
    0x3a45b935, 0xff5c, 0x42aa, 0xa5, 0xa2, 0x43, 0x77, 0x1b, 0x93, 0xeb, 0xef
DEFINE_GUIDSTRUCT("3A45B935-FF5C-42AA-A5A2-43771B93EBEF", MWCAP_PROPSETID_VIDEO);
#define MWCAP_PROPSETID_VIDEO DEFINE_GUIDNAMED(MWCAP_PROPSETID_VIDEO)

////////////////////////////////////////////////////////////////////////////////
// Data structs

// Common definitions
typedef VOID * __ptr64								MWCAP_PTR64;
														                                            	
#define INPUT_SOURCE(type, index)					(((type) << 8) | ((index) & 0xFF))
#define INPUT_TYPE(source)							((source) >> 8)
#define INPUT_INDEX(source)							((source) & 0xFF)
													
typedef enum _MWCAP_PRODUCT_ID {
	MWCAP_PRODUCT_ID_PRO_CAPTURE_AIO				= 0x00000102,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_DVI				= 0x00000103,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_HDMI				= 0x00000104,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_SDI				= 0x00000105,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_DUAL_SDI			= 0x00000106,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_DUAL_DVI			= 0x00000107,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_DUAL_HDMI			= 0x00000108,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_QUAD_SDI			= 0x00000109,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_QUAD_HDMI			= 0x00000110,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_MINI_HDMI			= 0x00000111,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_HDMI_4K			= 0x00000112,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_MINI_SDI			= 0x00000113,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_AIO_4K_PLUS		= 0x00000114,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_HDMI_4K_PLUS		= 0x00000115,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_DVI_4K				= 0x00000116,
	MWCAP_PRODUCT_ID_PRO_CAPTURE_AIO_4K				= 0x00000117,

	MWCAP_PRODUCT_ID_ECO_CAPTURE_OCTA_SDI			= 0x00000150
} MWCAP_PRODUCT_ID;

// A/V input type & source                      
typedef enum _MWCAP_VIDEO_INPUT_TYPE {          
	MWCAP_VIDEO_INPUT_TYPE_NONE						= 0x00,
	MWCAP_VIDEO_INPUT_TYPE_HDMI						= 0x01,
	MWCAP_VIDEO_INPUT_TYPE_VGA						= 0x02,
	MWCAP_VIDEO_INPUT_TYPE_SDI						= 0x04,
	MWCAP_VIDEO_INPUT_TYPE_COMPONENT				= 0x08,
	MWCAP_VIDEO_INPUT_TYPE_CVBS						= 0x10,
	MWCAP_VIDEO_INPUT_TYPE_YC						= 0x20
} MWCAP_VIDEO_INPUT_TYPE;	

typedef enum _MWCAP_AUDIO_INPUT_TYPE {	
	MWCAP_AUDIO_INPUT_TYPE_NONE						= 0x00,
	MWCAP_AUDIO_INPUT_TYPE_HDMI						= 0x01,
	MWCAP_AUDIO_INPUT_TYPE_SDI						= 0x02,
	MWCAP_AUDIO_INPUT_TYPE_LINE_IN					= 0x04,
	MWCAP_AUDIO_INPUT_TYPE_MIC_IN					= 0x08
} MWCAP_AUDIO_INPUT_TYPE;							
													
typedef enum _MWCAP_PCIE_LINK_TYPE {				
	MWCAP_PCIE_LINK_GEN_1							= 0x01,
	MWCAP_PCIE_LINK_GEN_2							= 0x02,
	MWCAP_PCIE_LINK_GEN_3							= 0x04,
	MWCAP_PCIE_LINK_GEN_4							= 0x08
} MWCAP_PCIE_LINK_TYPE;								
													
typedef enum _MWCAP_VIDEO_TIMING_TYPE {					
	MWCAP_VIDEO_TIMING_NONE							= 0x00000000,
	MWCAP_VIDEO_TIMING_LEGACY						= 0x00000001,
	MWCAP_VIDEO_TIMING_DMT							= 0x00000002,
	MWCAP_VIDEO_TIMING_CEA							= 0x00000004,
	MWCAP_VIDEO_TIMING_GTF							= 0x00000008,
	MWCAP_VIDEO_TIMING_CVT							= 0x00000010,
	MWCAP_VIDEO_TIMING_CVT_RB						= 0x00000020,
	MWCAP_VIDEO_TIMING_FAILSAFE						= 0x00002000
} MWCAP_VIDEO_TIMING_TYPE;

typedef enum _MWCAP_VIDEO_COLOR_FORMAT {         	
	MWCAP_VIDEO_COLOR_FORMAT_UNKNOWN				= 0x00,
	MWCAP_VIDEO_COLOR_FORMAT_RGB					= 0x01,
	MWCAP_VIDEO_COLOR_FORMAT_YUV601					= 0x02,
	MWCAP_VIDEO_COLOR_FORMAT_YUV709					= 0x03,
	MWCAP_VIDEO_COLOR_FORMAT_YUV2020				= 0x04,
	MWCAP_VIDEO_COLOR_FORMAT_YUV2020C				= 0x05				// Constant luminance, not supported yet.
} MWCAP_VIDEO_COLOR_FORMAT;							 	
														
typedef enum _MWCAP_VIDEO_QUANTIZATION_RANGE {			
	MWCAP_VIDEO_QUANTIZATION_UNKNOWN				= 0x00,
	MWCAP_VIDEO_QUANTIZATION_FULL					= 0x01, 			// Black level: 0, White level: 255/1023/4095/65535
	MWCAP_VIDEO_QUANTIZATION_LIMITED				= 0x02				// Black level: 16/64/256/4096, White level: 235(240)/940(960)/3760(3840)/60160(61440)
} MWCAP_VIDEO_QUANTIZATION_RANGE;					
													
typedef enum _MWCAP_VIDEO_SATURATION_RANGE {		
	MWCAP_VIDEO_SATURATION_UNKNOWN					= 0x00,
	MWCAP_VIDEO_SATURATION_FULL						= 0x01, 			// Min: 0, Max: 255/1023/4095/65535
	MWCAP_VIDEO_SATURATION_LIMITED					= 0x02, 			// Min: 16/64/256/4096, Max: 235(240)/940(960)/3760(3840)/60160(61440)
	MWCAP_VIDEO_SATURATION_EXTENDED_GAMUT			= 0x03  			// Min: 1/4/16/256, Max: 254/1019/4079/65279
} MWCAP_VIDEO_SATURATION_RANGE;						

typedef enum _MWCAP_VIDEO_FRAME_TYPE {
	MWCAP_VIDEO_FRAME_2D							= 0x00,
	MWCAP_VIDEO_FRAME_3D_TOP_AND_BOTTOM_FULL		= 0x01,
	MWCAP_VIDEO_FRAME_3D_TOP_AND_BOTTOM_HALF		= 0x02,
	MWCAP_VIDEO_FRAME_3D_SIDE_BY_SIDE_FULL			= 0x03,
	MWCAP_VIDEO_FRAME_3D_SIDE_BY_SIDE_HALF			= 0x04
} MWCAP_VIDEO_FRAME_TYPE;

typedef enum _MWCAP_VIDEO_DEINTERLACE_MODE {
	MWCAP_VIDEO_DEINTERLACE_WEAVE					= 0x00,
	MWCAP_VIDEO_DEINTERLACE_BLEND					= 0x01,
	MWCAP_VIDEO_DEINTERLACE_TOP_FIELD				= 0x02,
	MWCAP_VIDEO_DEINTERLACE_BOTTOM_FIELD			= 0x03
} MWCAP_VIDEO_DEINTERLACE_MODE;

typedef enum _MWCAP_VIDEO_ASPECT_RATIO_CONVERT_MODE {
	MWCAP_VIDEO_ASPECT_RATIO_IGNORE					= 0x00,
	MWCAP_VIDEO_ASPECT_RATIO_CROPPING				= 0x01,
	MWCAP_VIDEO_ASPECT_RATIO_PADDING				= 0x02
} MWCAP_VIDEO_ASPECT_RATIO_CONVERT_MODE;

typedef enum _MWCAP_VIDEO_SYNC_TYPE {
	VIDEO_SYNC_ALL									= 0x07,
	VIDEO_SYNC_HS_VS								= 0x01,
	VIDEO_SYNC_CS									= 0x02,
	VIDEO_SYNC_EMBEDDED								= 0x04
} MWCAP_VIDEO_SYNC_TYPE;

typedef struct _MWCAP_VIDEO_SYNC_INFO	 {
	BYTE											bySyncType;
	BOOLEAN											bHSPolarity;
	BOOLEAN											bVSPolarity;
	BOOLEAN											bInterlaced;
	DWORD											dwFrameDuration;
	WORD											wVSyncLineCount;
	WORD											wFrameLineCount;
} MWCAP_VIDEO_SYNC_INFO;

typedef struct _MWCAP_VIDEO_TIMING {
	DWORD											dwType;
	DWORD											dwPixelClock;
	BOOLEAN											bInterlaced;
	BYTE											bySyncType;
	BOOLEAN											bHSPolarity;
	BOOLEAN											bVSPolarity;
	WORD											wHActive;
	WORD											wHFrontPorch;
	WORD											wHSyncWidth;
	WORD											wHBackPorch;
	WORD											wVActive;
	WORD											wVFrontPorch;
	WORD											wVSyncWidth;
	WORD											wVBackPorch;
} MWCAP_VIDEO_TIMING;

typedef struct _MWCAP_VIDEO_TIMING_SETTINGS {
	WORD											wAspectX;
	WORD											wAspectY;
	WORD											x;
	WORD											y;
	WORD											cx;
	WORD											cy;
	WORD											cxTotal;
	BYTE											byClampPos;
} MWCAP_VIDEO_TIMING_SETTINGS;

typedef struct _MWCAP_KSPROPERTY_DWORD_S {
	KSPROPERTY										Property;
	DWORD											dwParameter;	
} MWCAP_KSPROPERTY_DWORD_S;							  

typedef struct _MWCAP_KSPROPERTY_PTR64_S {
	KSPROPERTY										Property;
	MWCAP_PTR64										pvParameter;
} MWCAP_KSPROPERTY_PTR64_S;

typedef struct _MWCAP_DWORD_PARAMETER_RANGE {		
	DWORD											dwMin;
	DWORD											dwMax;
	DWORD											dwStep;
	DWORD											dwDefault;
} MWCAP_DWORD_PARAMETER_RANGE;						
													
#define MWCAP_DWORD_PARAMETER_FLAG_AUTO				0x01
													
typedef struct _MWCAP_DWORD_PARAMETER_VALUE {		
	DWORD											dwFlags;
	DWORD											dwValue;
} MWCAP_DWORD_PARAMETER_VALUE;                  

// Product informations
typedef struct _MWCAP_CHANNEL_INFO {		
	WORD											wFamilyID;
	WORD											wProductID;
	CHAR											chHardwareVersion;
	BYTE											byFirmwareID;
	DWORD											dwFirmwareVersion;
	DWORD											dwDriverVersion;
	CHAR											szFamilyName[MW_FAMILY_NAME_LEN];
	CHAR											szProductName[MW_PRODUCT_NAME_LEN];
	CHAR											szFirmwareName[MW_FIRMWARE_NAME_LEN];
	CHAR											szBoardSerialNo[MW_SERIAL_NO_LEN];
	BYTE											byBoardIndex;
	BYTE											byChannelIndex;
} MWCAP_CHANNEL_INFO;

typedef struct _MWCAP_PRO_CAPTURE_INFO {
	BYTE											byPCIBusID;
	BYTE											byPCIDevID;
	BYTE											byLinkType;
	BYTE											byLinkWidth;
	BYTE											byBoardIndex;
	WORD											wMaxPayloadSize;
	WORD											wMaxReadRequestSize;
	DWORD											cbTotalMemorySize;
	DWORD											cbFreeMemorySize;
} MWCAP_PRO_CAPTURE_INFO;

typedef struct _MWCAP_VIDEO_CAPS {
	DWORD											dwCaps;
	WORD											wMaxInputWidth;
	WORD											wMaxInputHeight;
	WORD											wMaxOutputWidth;
	WORD											wMaxOutputHeight;
} MWCAP_VIDEO_CAPS;

typedef struct _MWCAP_AUDIO_CAPS {
	DWORD											dwCaps;
} MWCAP_AUDIO_CAPS;                       

// Firmware upgrade
typedef struct _MWCAP_FIRMWARE_STORAGE {
	DWORD											cbStorage;
	DWORD											cbEraseBlock;
	DWORD											cbProgramBlock;
	DWORD											cbHeaderOffset;
} MWCAP_FIRMWARE_STORAGE;

typedef struct _MWCAP_FIRMWARE_ERASE {
	DWORD											cbOffset;
	DWORD											cbErase;
} MWCAP_FIRMWARE_ERASE;

// Device misc controls
typedef enum _MWCAP_LED_MODE {
	MWCAP_LED_AUTO									= 0x00000000,
	MWCAP_LED_OFF									= 0x80000000,
	MWCAP_LED_ON									= 0x80000001,
	MWCAP_LED_BLINK									= 0x80000002,
	MWCAP_LED_DBL_BLINK								= 0x80000003,
	MWCAP_LED_BREATH								= 0x80000004
} MWCAP_LED_MODE;

// Signal status
typedef struct _MWCAP_SDI_SPECIFIC_STATUS {
	SDI_TYPE										sdiType;
	SDI_SCANNING_FORMAT								sdiScanningFormat;
	SDI_BIT_DEPTH									sdiBitDepth;
	SDI_SAMPLING_STRUCT								sdiSamplingStruct;
	BOOLEAN											bST352DataValid;
	DWORD											dwST352Data;
} MWCAP_SDI_SPECIFIC_STATUS;

typedef struct _MWCAP_HDMI_VIDEO_TIMING {
	BOOLEAN											bInterlaced;
	DWORD											dwFrameDuration;
	WORD											wHSyncWidth;
	WORD											wHFrontPorch;
	WORD											wHBackPorch;
	WORD											wHActive;
	WORD											wHTotalWidth;
	WORD											wField0VSyncWidth;
	WORD											wField0VFrontPorch;
	WORD											wField0VBackPorch;
	WORD											wField0VActive;
	WORD											wField0VTotalHeight;
	WORD											wField1VSyncWidth;
	WORD											wField1VFrontPorch;
	WORD											wField1VBackPorch;
	WORD											wField1VActive;
	WORD											wField1VTotalHeight;
} MWCAP_HDMI_VIDEO_TIMING;

typedef struct _MWCAP_HDMI_SPECIFIC_STATUS {
	BOOLEAN											bHDMIMode;
	BOOLEAN											bHDCP;
	BYTE											byBitDepth;
	HDMI_PXIEL_ENCODING								pixelEncoding;
	BYTE											byVIC;
	BOOLEAN											bITContent;
	BOOLEAN											b3DFormat;
	BYTE											by3DStructure;
	BYTE											bySideBySideHalfSubSampling;
	MWCAP_HDMI_VIDEO_TIMING							videoTiming;
} MWCAP_HDMI_SPECIFIC_STATUS;

typedef struct _MWCAP_COMPONENT_SPECIFIC_STATUS {
	MWCAP_VIDEO_SYNC_INFO							syncInfo;
	BOOLEAN											bTriLevelSync;
	MWCAP_VIDEO_TIMING								videoTiming;			// Not valid for custom video timing
	MWCAP_VIDEO_TIMING_SETTINGS						videoTimingSettings;
} MWCAP_COMPONENT_SPECIFIC_STATUS;

typedef enum _MWCAP_SD_VIDEO_STANDARD {
	MWCAP_SD_VIDEO_NONE,
	MWCAP_SD_VIDEO_NTSC_M,
	MWCAP_SD_VIDEO_NTSC_433,
	MWCAP_SD_VIDEO_PAL_M,
	MWCAP_SD_VIDEO_PAL_60,
	MWCAP_SD_VIDEO_PAL_COMBN,
	MWCAP_SD_VIDEO_PAL_BGHID,
	MWCAP_SD_VIDEO_SECAM,
	MWCAP_SD_VIDEO_SECAM_60
} MWCAP_SD_VIDEO_STANDARD;

typedef struct _MWCAP_CVBS_YC_SPECIFIC_STATUS {
	MWCAP_SD_VIDEO_STANDARD							standard;
	BOOLEAN											b50Hz;
} MWCAP_CVBS_YC_SPECIFIC_STATUS;

typedef struct _MWCAP_INPUT_SPECIFIC_STATUS {
	BOOLEAN											bValid;
	DWORD											dwVideoInputType;
	union {
		MWCAP_SDI_SPECIFIC_STATUS					sdiStatus;
		MWCAP_HDMI_SPECIFIC_STATUS					hdmiStatus;
		MWCAP_COMPONENT_SPECIFIC_STATUS				vgaComponentStatus;
		MWCAP_CVBS_YC_SPECIFIC_STATUS				cvbsYcStatus;
	};
} MWCAP_INPUT_SPECIFIC_STATUS;

typedef enum _MWCAP_VIDEO_SIGNAL_STATE {
	MWCAP_VIDEO_SIGNAL_NONE,						// No signal detectd
	MWCAP_VIDEO_SIGNAL_UNSUPPORTED,					// Video signal status not valid
	MWCAP_VIDEO_SIGNAL_LOCKING,						// Video signal status valid but not locked yet
	MWCAP_VIDEO_SIGNAL_LOCKED						// Every thing OK
} MWCAP_VIDEO_SIGNAL_STATE;

typedef struct _MWCAP_VIDEO_SIGNAL_STATUS {
	MWCAP_VIDEO_SIGNAL_STATE						state;
	int												x;
	int												y;
	int												cx;
	int												cy;
	int												cxTotal;
	int												cyTotal;
	BOOLEAN											bInterlaced;
	DWORD											dwFrameDuration;
	int												nAspectX;
	int												nAspectY;
	BOOLEAN											bSegmentedFrame;
	MWCAP_VIDEO_FRAME_TYPE							frameType;
	MWCAP_VIDEO_COLOR_FORMAT						colorFormat;
	MWCAP_VIDEO_QUANTIZATION_RANGE					quantRange;
	MWCAP_VIDEO_SATURATION_RANGE					satRange;
} MWCAP_VIDEO_SIGNAL_STATUS;

typedef struct _MWCAP_AUDIO_SIGNAL_STATUS {
	WORD											wChannelValid;
	BOOLEAN											bLPCM;
	BYTE											cBitsPerSample;
	DWORD											dwSampleRate;
	BOOLEAN											bChannelStatusValid;
	IEC60958_CHANNEL_STATUS							channelStatus;
} MWCAP_AUDIO_SIGNAL_STATUS;

// Hardware timer
typedef struct _MWCAP_TIMER_EXPIRE_TIME {
	MWCAP_PTR64										pvTimer;
	LONGLONG										llExpireTime;
} MWCAP_TIMER_EXPIRE_TIME;

// Notifications
#define MWCAP_NOTIFY_INPUT_SORUCE_START_SCAN		0x0001ULL
#define MWCAP_NOTIFY_INPUT_SORUCE_STOP_SCAN			0x0002ULL
#define MWCAP_NOTIFY_INPUT_SORUCE_SCAN_CHANGE		0x0003ULL

#define MWCAP_NOTIFY_VIDEO_INPUT_SOURCE_CHANGE		0x0004ULL
#define MWCAP_NOTIFY_AUDIO_INPUT_SOURCE_CHANGE		0x0008ULL

// MWCAP_KSPROPERTY_INPUT_SPECIFIC_STATUS
#define MWCAP_NOTIFY_INPUT_SPECIFIC_CHANGE			0x0010ULL

// MWCAP_KSPROPERTY_VIDEO_SIGNAL_STATUS
#define MWCAP_NOTIFY_VIDEO_SIGNAL_CHANGE			0x0020ULL

// MWCAP_KSPROPERTY_AUDIO_SIGNAL_STATUS
#define MWCAP_NOTIFY_AUDIO_SIGNAL_CHANGE			0x0040ULL

// MWCAP_KSPROPERTY_VIDEO_FRAME_INFO
#define MWCAP_NOTIFY_VIDEO_FIELD_BUFFERING			0x0080ULL
#define MWCAP_NOTIFY_VIDEO_FRAME_BUFFERING			0x0100ULL
#define MWCAP_NOTIFY_VIDEO_FIELD_BUFFERED			0x0200ULL
#define MWCAP_NOTIFY_VIDEO_FRAME_BUFFERED			0x0400ULL
#define MWCAP_NOTIFY_VIDEO_SMPTE_TIME_CODE			0x0800ULL

// MWCAP_KSPROPERTY_AUDIO_CAPTURE_FRAME
#define MWCAP_NOTIFY_AUDIO_FRAME_BUFFERED			0x1000ULL
#define MWCAP_NOTIFY_AUDIO_INPUT_RESET				0x2000ULL

// MWCAP_KSPROPERTY_VIDEO_SAMPLING_PHASE
#define MWCAP_NOTIFY_VIDEO_SAMPLING_PHASE_CHANGE	0x4000ULL

// MWCAP_KSPROPERTY_HDMI_INFOFRAME_VALID, MWCAP_KSPROPERTY_HDMI_INFOFRAME_PACKET
#define MWCAP_NOTIFY_HDMI_INFOFRAME_AVI				(1ULL << (32 + MWCAP_HDMI_INFOFRAME_ID_AVI))
#define MWCAP_NOTIFY_HDMI_INFOFRAME_AUDIO			(1ULL << (32 + MWCAP_HDMI_INFOFRAME_ID_AUDIO))
#define MWCAP_NOTIFY_HDMI_INFOFRAME_SPD				(1ULL << (32 + MWCAP_HDMI_INFOFRAME_ID_SPD))
#define MWCAP_NOTIFY_HDMI_INFOFRAME_MS				(1ULL << (32 + MWCAP_HDMI_INFOFRAME_ID_MS))
#define MWCAP_NOTIFY_HDMI_INFOFRAME_VS				(1ULL << (32 + MWCAP_HDMI_INFOFRAME_ID_VS))
#define MWCAP_NOTIFY_HDMI_INFOFRAME_ACP				(1ULL << (32 + MWCAP_HDMI_INFOFRAME_ID_ACP))
#define MWCAP_NOTIFY_HDMI_INFOFRAME_ISRC1			(1ULL << (32 + MWCAP_HDMI_INFOFRAME_ID_ISRC1))
#define MWCAP_NOTIFY_HDMI_INFOFRAME_ISRC2			(1ULL << (32 + MWCAP_HDMI_INFOFRAME_ID_ISRC2))
#define MWCAP_NOTIFY_HDMI_INFOFRAME_GAMUT			(1ULL << (32 + MWCAP_HDMI_INFOFRAME_ID_GAMUT))

typedef struct _MWCAP_KSPROPERTY_NOTIFY_REGISTRATION_S {
	KSPROPERTY										Property;
	MWCAP_PTR64										hEvent;
	ULONGLONG										ullEnableBits;
} MWCAP_KSPROPERTY_NOTIFY_REGISTRATION_S;

// Video frame information
#define MWCAP_MAX_VIDEO_FRAME_COUNT					8

typedef struct _MWCAP_SMPTE_TIMECODE {
	BYTE 											byFrames;
	BYTE											bySeconds;
	BYTE											byMinutes;
	BYTE											byHours;
} MWCAP_SMPTE_TIMECODE;

typedef enum _MWCAP_VIDEO_FRAME_STATE {
	MWCAP_VIDEO_FRAME_STATE_INITIAL,
	MWCAP_VIDEO_FRAME_STATE_F0_BUFFERING,
	MWCAP_VIDEO_FRAME_STATE_F1_BUFFERING,
	MWCAP_VIDEO_FRAME_STATE_BUFFERED
} MWCAP_VIDEO_FRAME_STATE;

typedef struct _MWCAP_VIDEO_BUFFER_INFO {
	DWORD											cMaxFrames;

	BYTE											iNewestBuffering;
	BYTE											iBufferingFieldIndex;

	BYTE											iNewestBuffered;
	BYTE											iBufferedFieldIndex;

	BYTE											iNewestBufferedFullFrame;
	DWORD											cBufferedFullFrames;
} MWCAP_VIDEO_BUFFER_INFO;

typedef struct _MWCAP_VIDEO_FRAME_INFO {
	MWCAP_VIDEO_FRAME_STATE							state;

	BOOLEAN											bInterlaced;
	BOOLEAN											bSegmentedFrame;
	BOOLEAN											bTopFieldFirst;
	BOOLEAN											bTopFieldInverted;

	int												cx;
	int												cy;
	int												nAspectX;
	int												nAspectY;

	LONGLONG										allFieldStartTimes[2];
	LONGLONG										allFieldBufferedTimes[2];
	MWCAP_SMPTE_TIMECODE							aSMPTETimeCodes[2];
} MWCAP_VIDEO_FRAME_INFO;

// Video capture
typedef struct _MWCAP_VIDEO_CAPTURE_OPEN {
	MWCAP_PTR64										hEvent;
} MWCAP_VIDEO_CAPTURE_OPEN;

#define MWCAP_VIDEO_MAX_NUM_OSD_RECTS				4

#define MWCAP_VIDEO_FRAME_ID_NEWEST_BUFFERED		(-1)
#define MWCAP_VIDEO_FRAME_ID_NEWEST_BUFFERING		(-2)
#define MWCAP_VIDEO_FRAME_ID_NEXT_BUFFERED			(-3)
#define MWCAP_VIDEO_FRAME_ID_NEXT_BUFFERING			(-4)

#define MWCAP_VIDEO_FRAME_ID_EMPTY					(-100)		// Used in MWCAP_VIDEO_CAPTURE_STATUS

#define MWCAP_VIDEO_PROCESS_FLIP					0x00000001
#define MWCAP_VIDEO_PROCESS_MIRROR					0x00000002

typedef enum _MWCAP_VIDEO_PLANE_ADDRESS_TYPE {
	MWCAP_VIDEO_PLANE_VIRTUAL_ADDRESS,
	MWCAP_VIDEO_PLANE_PHYSICAL_ADDRESS
} MWCAP_VIDEO_PLANE_ADDRESS_TYPE;

typedef struct _MWCAP_VIDEO_FLANE_BUFFER
{
	DWORD											cbBuffer;
	DWORD											cbStride;

	union {
		MWCAP_PTR64									pvVirtualAddress;
		LARGE_INTEGER								liPhysicalAddress;
	};
} MWCAP_VIDEO_PLANE_BUFFER;

typedef struct _MWCAP_VIDEO_CAPTURE_FRAME {
	// Processing parameters
	DWORD											dwFOURCC;
	WORD											cx;
	WORD											cy;
	int												nAspectX;
	int												nAspectY;
	MWCAP_VIDEO_COLOR_FORMAT						colorFormat;
	MWCAP_VIDEO_QUANTIZATION_RANGE					quantRange;
	MWCAP_VIDEO_SATURATION_RANGE					satRange;

	SHORT											sContrast;			// [50, 200]
	SHORT											sBrightness;		// [-100, 100]
	SHORT											sSaturation;		// [0, 200]
	SHORT											sHue;				// [-90, 90]

	RECT											rectSource;
	RECT											rectTarget;

	MWCAP_VIDEO_DEINTERLACE_MODE					deinterlaceMode;
	MWCAP_VIDEO_ASPECT_RATIO_CONVERT_MODE			aspectRatioConvertMode;
	
	// Source
	char											iSrcFrame;

	// OSD (within rectTarget and [0,0-cx,cy))
	MWCAP_PTR64										pOSDImage;
	RECT											aOSDRects[MWCAP_VIDEO_MAX_NUM_OSD_RECTS];
	int												cOSDRects;

	// Buffer parameters
	/*
	MWCAP_VIDEO_PLANE_ADDRESS_TYPE					addressType;
	int												cPlaneBuffers;
	MWCAP_VIDEO_PLANE_BUFFER						aPlaneBuffers[3];
	*/

	BOOLEAN											bPhysicalAddress;
	union {
		MWCAP_PTR64									pvFrame;
		LARGE_INTEGER								liPhysicalAddress;
	};

	DWORD											cbFrame;
	DWORD											cbStride;

	BOOLEAN											bBottomUp;

	WORD											cyPartialNotify;		// 0: Not use partial notify

	DWORD											dwProcessSwitchs;		// MWCAP_VIDEO_PROCESS_xx

	// Context
	MWCAP_PTR64										pvContext;
} MWCAP_VIDEO_CAPTURE_FRAME;

typedef struct _MWCAP_VIDEO_CAPTURE_STATUS {
	MWCAP_PTR64										pvContext;

	BOOLEAN											bPhysicalAddress;
	union {
		MWCAP_PTR64									pvFrame;
		LARGE_INTEGER								liPhysicalAddress;
	};

	int												iFrame;
	BOOLEAN											bFrameCompleted;
	WORD											cyCompleted;
	WORD											cyCompletedPrev;
} MWCAP_VIDEO_CAPTURE_STATUS;

// Audio capture
#define MWCAP_AUDIO_FRAME_SYNC_CODE					0xFECA0357
#define MWCAP_AUDIO_SAMPLES_PER_FRAME				192
#define MWCAP_AUDIO_MAX_NUM_CHANNELS				8

// Audio samples are 32bits wide, cBitsPerSample of high bits are valid
// Sample layout: 0L, 1L, 2L, 3L, 0R, 1R, 2R, 3R
typedef struct _MWCAP_AUDIO_CAPTURE_FRAME {
	DWORD											cFrameCount;
	DWORD											iFrame;
	DWORD											dwSyncCode;
	DWORD											dwReserved;
	LONGLONG										llTimestamp;
	DWORD											adwSamples[MWCAP_AUDIO_SAMPLES_PER_FRAME * MWCAP_AUDIO_MAX_NUM_CHANNELS];
} MWCAP_AUDIO_CAPTURE_FRAME;

// HDMI status
typedef enum _MWCAP_HDMI_INFOFRAME_ID {
	MWCAP_HDMI_INFOFRAME_ID_AVI,
	MWCAP_HDMI_INFOFRAME_ID_AUDIO,
	MWCAP_HDMI_INFOFRAME_ID_SPD,
	MWCAP_HDMI_INFOFRAME_ID_MS,
	MWCAP_HDMI_INFOFRAME_ID_VS,
	MWCAP_HDMI_INFOFRAME_ID_ACP,
	MWCAP_HDMI_INFOFRAME_ID_ISRC1,
	MWCAP_HDMI_INFOFRAME_ID_ISRC2,
	MWCAP_HDMI_INFOFRAME_ID_GAMUT,
	MWCAP_HDMI_INFOFRAME_COUNT
} MWCAP_HDMI_INFOFRAME_ID;

typedef enum _MWCAP_HDMI_INFOFRAME_MASK {
	MWCAP_HDMI_INFOFRAME_MASK_AVI					= (1 << MWCAP_HDMI_INFOFRAME_ID_AVI),
	MWCAP_HDMI_INFOFRAME_MASK_AUDIO					= (1 << MWCAP_HDMI_INFOFRAME_ID_AUDIO),
	MWCAP_HDMI_INFOFRAME_MASK_SPD					= (1 << MWCAP_HDMI_INFOFRAME_ID_SPD),
	MWCAP_HDMI_INFOFRAME_MASK_MS					= (1 << MWCAP_HDMI_INFOFRAME_ID_MS),
	MWCAP_HDMI_INFOFRAME_MASK_VS					= (1 << MWCAP_HDMI_INFOFRAME_ID_VS),
	MWCAP_HDMI_INFOFRAME_MASK_ACP					= (1 << MWCAP_HDMI_INFOFRAME_ID_ACP),
	MWCAP_HDMI_INFOFRAME_MASK_ISRC1					= (1 << MWCAP_HDMI_INFOFRAME_ID_ISRC1),
	MWCAP_HDMI_INFOFRAME_MASK_ISRC2					= (1 << MWCAP_HDMI_INFOFRAME_ID_ISRC2),
	MWCAP_HDMI_INFOFRAME_MASK_GAMUT					= (1 << MWCAP_HDMI_INFOFRAME_ID_GAMUT)
} MWCAP_HDMI_INFOFRAME_MASK;

typedef struct _MWCAP_VIDEO_ASPECT_RATIO {
	int												nAspectX;
	int												nAspectY;
} MWCAP_VIDEO_ASPECT_RATIO;

typedef struct _MWCAP_VIDEO_CONNECTION_FORMAT {
	// Valid flag
	BOOLEAN											bConnected;
	
	// Basic information
	LONG											cx;
	LONG											cy;
	DWORD											dwFrameDuration;
	DWORD											dwFOURCC;

	// Preferred parameters
	int												nAspectX;
	int												nAspectY;
	MWCAP_VIDEO_COLOR_FORMAT						colorFormat;
	MWCAP_VIDEO_QUANTIZATION_RANGE					quantRange;
	MWCAP_VIDEO_SATURATION_RANGE					satRange;
} MWCAP_VIDEO_CONNECTION_FORMAT;

typedef struct _MWCAP_VIDEO_PROCESS_SETTINGS {
	DWORD											dwProcessSwitchs;		// MWCAP_VIDEO_PROCESS_xx
	RECT											rectSource;
	int												nAspectX;
	int												nAspectY;
	BOOLEAN											bLowLatency;
	MWCAP_VIDEO_COLOR_FORMAT						colorFormat;
	MWCAP_VIDEO_QUANTIZATION_RANGE					quantRange;
	MWCAP_VIDEO_SATURATION_RANGE					satRange;
	MWCAP_VIDEO_DEINTERLACE_MODE					deinterlaceMode;
	MWCAP_VIDEO_ASPECT_RATIO_CONVERT_MODE			aspectRatioConvertMode;
} MWCAP_VIDEO_PROCESS_SETTINGS;

#define MWCAP_VIDEO_MAX_NUM_PREFERRED_TIMINGS		8

typedef struct _MWCAP_KSPROPERTY_VIDEO_CREATE_IMAGE_S {
	KSPROPERTY										Property;
	WORD											cx;
	WORD											cy;
} MWCAP_KSPROPERTY_VIDEO_CREATE_IMAGE_S;

typedef struct _MWCAP_VIDEO_UPLOAD_IMAGE {
	// Destination parameters
	MWCAP_PTR64										pvDestImage;
	MWCAP_VIDEO_COLOR_FORMAT						cfDest;
	WORD											xDest;
	WORD											yDest;
	WORD											cxDest;
	WORD											cyDest;

	MWCAP_VIDEO_QUANTIZATION_RANGE					quantRangeDest;
	MWCAP_VIDEO_SATURATION_RANGE					satRangeDest;

	// Source parameters
	BOOLEAN											bSrcPhysicalAddress;
	union {
		MWCAP_PTR64									pvSrcFrame;
		LARGE_INTEGER								liSrcPhysicalAddress;
	};

	DWORD											cbSrcFrame;
	DWORD											cbSrcStride;

	WORD											cxSrc;
	WORD											cySrc;
	BOOLEAN											bSrcBottomUp;
	BOOLEAN											bSrcPixelAlpha;
	BOOLEAN											bSrcPixelXBGR;
} MWCAP_VIDEO_UPLOAD_IMAGE;

typedef struct _MWCAP_VIDEO_OSD_SETTINGS {
	BOOLEAN											bEnable;
	WCHAR											wszPNGFilePath[_MAX_PATH];
} MWCAP_VIDEO_OSD_SETTINGS;

typedef struct _MWCAP_VIDEO_OSD_IMAGE {
	MWCAP_PTR64										pvOSDImage;
	RECT											aOSDRects[MWCAP_VIDEO_MAX_NUM_OSD_RECTS];
	int												cOSDRects;
} MWCAP_VIDEO_OSD_IMAGE;

typedef struct _MWCAP_VIDEO_CUSTOM_TIMING {
	MWCAP_VIDEO_SYNC_INFO							syncInfo;
	MWCAP_VIDEO_TIMING_SETTINGS						videoTimingSettings;
} MWCAP_VIDEO_CUSTOM_TIMING;

typedef struct _MWCAP_VIDEO_PIN_BUFFER {
	MWCAP_PTR64										pvBuffer;
	DWORD											cbBuffer;
} MWCAP_VIDEO_PIN_BUFFER;

#pragma pack(pop)

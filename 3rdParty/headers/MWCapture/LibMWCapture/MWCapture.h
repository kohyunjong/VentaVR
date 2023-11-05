
#pragma once

#ifdef LIBMWCAPTURE_EXPORTS

#ifdef _LIBMWCAPTUREDLL
#define LIBMWCAPTURE_API __declspec(dllexport)
#else
#define LIBMWCAPTURE_API 
#endif

#else

#ifdef _LIBMWCAPTUREDLL
#define LIBMWCAPTURE_API __declspec(dllimport)
#else
#define LIBMWCAPTURE_API 
#endif

#endif

#include <Windows.h>
#include <tchar.h>

#include "..\MWFOURCC.h"
#include "..\MWSDKCommon.h"
#include "MWCaptureExtension.h"

#ifdef __cplusplus

extern "C"
{
#endif

typedef enum MW_RESULT {
	MW_SUCCEEDED = 0x00,
	MW_FAILED,
	MW_INVALID_PARAMS,
};

DECLARE_HANDLE(HCHANNEL);

DECLARE_MWHANDLE64(HTIMER);

DECLARE_MWHANDLE64(HNOTIFY);

DECLARE_MWHANDLE64(HOSD);

MW_RESULT
LIBMWCAPTURE_API
MWGetVersion(
	BYTE*							pbyMaj,
	BYTE*							pbyMin,
	WORD*							pwBuild
	);

BOOL
LIBMWCAPTURE_API
MWCaptureInitInstance(
	);

void
LIBMWCAPTURE_API
MWCaptureExitInstance(
	);

MW_RESULT
LIBMWCAPTURE_API
MWRefreshDevice(
	);

int 
LIBMWCAPTURE_API
MWGetChannelCount(
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetChannelInfoByIndex(
	int								nIndex,
	MWCAP_CHANNEL_INFO *			pChannelInfo
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetFamilyInfoByIndex(
	int								nIndex,
	LPVOID							pFamilyInfo,
	DWORD							dwSize
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetDevicePath(
	int								nIndex,
	WCHAR*							pDevicePath
);

// Channel
HCHANNEL
LIBMWCAPTURE_API
MWOpenChannel(
	int								nBoardValue,
	int								nChannelIndex
	);

HCHANNEL
LIBMWCAPTURE_API
MWOpenChannelByPath(
	const WCHAR*					pszDevicePath
	);

void
LIBMWCAPTURE_API
MWCloseChannel(
	HCHANNEL						hChannel
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetChannelInfo(
	HCHANNEL						hChannel,
	MWCAP_CHANNEL_INFO *			pChannelInfo
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetFamilyInfo(
	HCHANNEL						hChannel,
	LPVOID							pFamilyInfo,
	DWORD							dwSize
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoCaps(
	HCHANNEL						hChannel,
	MWCAP_VIDEO_CAPS*				pVideoCaps
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetAudioCaps(
	HCHANNEL						hChannel,
	MWCAP_AUDIO_CAPS*				pAudioCaps
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoInputSourceArray(
	HCHANNEL						hChannel,
	DWORD*							pdwInputSource,
	DWORD*							pdwInputCount
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetAudioInputSourceArray(
	HCHANNEL						hChannel,
	DWORD*							pdwInputSource,
	DWORD*							pdwInputCount
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetInputSourceScan(
	HCHANNEL 						hChannel,
	BOOLEAN*						pbScan
	);

MW_RESULT
LIBMWCAPTURE_API
MWSetInputSourceScan(
	HCHANNEL 						hChannel,
	BOOLEAN							bScan
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetAVInputSourceLink(
	HCHANNEL 						hChannel,
	BOOLEAN*						pbLink
	);

MW_RESULT
LIBMWCAPTURE_API
MWSetAVInputSourceLink(
	HCHANNEL 						hChannel,
	BOOLEAN							bLink
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoInputSource(
	HCHANNEL						hChannel,
	DWORD*							pdwSource
	);

MW_RESULT
LIBMWCAPTURE_API
MWSetVideoInputSource(
	HCHANNEL						hChannel,
	DWORD							dwSource
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetAudioInputSource(
	HCHANNEL						hChannel,
	DWORD*							pdwSource
	);

MW_RESULT
LIBMWCAPTURE_API
MWSetAudioInputSource(
	HCHANNEL						hChannel,
	DWORD							dwSource
	);

// EDID
MW_RESULT
LIBMWCAPTURE_API
MWGetEDID(
	HCHANNEL						hChannel,
	BYTE*							pbyData,
	ULONG*							pulSize
	);

MW_RESULT
LIBMWCAPTURE_API
MWSetEDID(
	HCHANNEL						hChannel,
	BYTE*							pbyData,
	ULONG							ulSize
	);

// Signal Status
MW_RESULT
LIBMWCAPTURE_API
MWGetInputSpecificStatus(
	HCHANNEL						hChannel,
	MWCAP_INPUT_SPECIFIC_STATUS *	pInputStatus
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoSignalStatus(
	HCHANNEL						hChannel,
	MWCAP_VIDEO_SIGNAL_STATUS *		pSignalStatus
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetAudioSignalStatus(
	HCHANNEL						hChannel,
	MWCAP_AUDIO_SIGNAL_STATUS *		pSignalStatus
	);

// HDMI InfoFrame
MW_RESULT
LIBMWCAPTURE_API
MWGetHDMIInfoFrameValidFlag(
	HCHANNEL						hChannel,
	DWORD*							pdwValidFlag
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetHDMIInfoFramePacket(
	HCHANNEL						hChannel,
	MWCAP_HDMI_INFOFRAME_ID			id,
	HDMI_INFOFRAME_PACKET*			pPacket
	);

// Device Clock
MW_RESULT
LIBMWCAPTURE_API
MWGetDeviceTime(
	HCHANNEL						hChannel,
	LONGLONG*						pllTime
	);

MW_RESULT
LIBMWCAPTURE_API
MWSetDeviceTime(
	HCHANNEL						hChannel,
	LONGLONG						llTime
	);

MW_RESULT
LIBMWCAPTURE_API
MWRegulateDeviceTime(
	HCHANNEL						hChannel,
	LONGLONG						llTime
	);

// Timer Event
HTIMER
LIBMWCAPTURE_API
MWRegisterTimer(
	HCHANNEL						hChannel,
	HANDLE							hEvent
	);

MW_RESULT
LIBMWCAPTURE_API
MWUnregisterTimer(
	HCHANNEL 						hChannel,
	HTIMER							hTimer
	);

MW_RESULT
LIBMWCAPTURE_API
MWScheduleTimer(
	HCHANNEL 						hChannel,
	HTIMER							hTimer,
	LONGLONG						llExpireTime
	);

// Notify Event
HNOTIFY
LIBMWCAPTURE_API
MWRegisterNotify(
	HCHANNEL 						hChannel,
	HANDLE							hEvent,
	DWORD							dwEnableBits
	);

MW_RESULT
LIBMWCAPTURE_API
MWUnregisterNotify(
	HCHANNEL 						hChannel,
	HNOTIFY							hNotify
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetNotifyStatus(
	HCHANNEL 						hChannel,
	HNOTIFY							hNotify,
	ULONGLONG*						pullStatus
	);


// Video Capture
MW_RESULT
LIBMWCAPTURE_API
MWStartVideoCapture(
	HCHANNEL 						hChannel,
	HANDLE							hEvent
	);	

MW_RESULT
LIBMWCAPTURE_API
MWStopVideoCapture(
	HCHANNEL 						hChannel
	);


MW_RESULT
LIBMWCAPTURE_API
MWPinVideoBuffer(
	HCHANNEL 						hChannel,
	LPBYTE							pbFrame,
	DWORD							cbFrame
	);

MW_RESULT
LIBMWCAPTURE_API
MWUnpinVideoBuffer(
	HCHANNEL 						hChannel,
	LPBYTE							pbFrame
	);

MW_RESULT
LIBMWCAPTURE_API
MWCaptureVideoFrameToVirtualAddress(
	HCHANNEL 						hChannel,
	int								iFrame,
	LPBYTE							pbFrame,
	DWORD							cbFrame,
	DWORD							cbStride,
	BOOLEAN							bBottomUp,
	MWCAP_PTR64						pvContext,
	DWORD							dwFOURCC,
	int								cx,
	int								cy
	);

MW_RESULT
LIBMWCAPTURE_API
MWCaptureVideoFrameToPhysicalAddress(
	HCHANNEL 						hChannel,
	int								iFrame,
	LONGLONG						llFrameAddress,
	DWORD							cbFrame,
	DWORD							cbStride,
	BOOLEAN							bBottomUp,
	MWCAP_PTR64						pvContext,
	DWORD							dwFOURCC,
	int								cx,
	int								cy
	);

MW_RESULT
LIBMWCAPTURE_API
MWCaptureVideoFrameWithOSDToVirtualAddress(
	HCHANNEL 						hChannel,
	int								iFrame,
	LPBYTE							pbFrame,
	DWORD							cbFrame,
	DWORD							cbStride,
	BOOLEAN							bBottomUp,
	MWCAP_PTR64						pvContext,
	DWORD							dwFOURCC,
	int								cx,
	int								cy,
	HOSD							hOSDImage,
	const RECT *					pOSDRects,
	int								cOSDRects
	);

MW_RESULT
LIBMWCAPTURE_API
MWCaptureVideoFrameWithOSDToPhysicalAddress(
	HCHANNEL 						hChannel,
	int								iFrame,
	LONGLONG						llFrameAddress,
	DWORD							cbFrame,
	DWORD							cbStride,
	BOOLEAN							bBottomUp,
	MWCAP_PTR64						pvContext,
	DWORD							dwFOURCC,
	int								cx,
	int								cy,
	HOSD							hOSDImage,
	const RECT *					pOSDRects,
	int								cOSDRects
	);

MW_RESULT
LIBMWCAPTURE_API
MWCaptureVideoFrameToVirtualAddressEx(
	HCHANNEL 						hChannel,
	int								iFrame,
	LPBYTE							pbFrame,
	DWORD							cbFrame,
	DWORD							cbStride,
	BOOLEAN							bBottomUp,
	MWCAP_PTR64						pvContext,
	DWORD							dwFOURCC,
	int								cx,
	int								cy,
	DWORD							dwProcessSwitchs,
	int								cyParitalNotify,
	HOSD							hOSDImage,
	const RECT *					pOSDRects,
	int								cOSDRects,
	SHORT							sContrast,
	SHORT							sBrightness,
	SHORT							sSaturation,
	SHORT							sHue,
	MWCAP_VIDEO_DEINTERLACE_MODE			deinterlaceMode,
	MWCAP_VIDEO_ASPECT_RATIO_CONVERT_MODE	aspectRatioConvertMode,
	const RECT *							pRectSrc,
	const RECT *							pRectDest,
	int										nAspectX,
	int										nAspectY,
	MWCAP_VIDEO_COLOR_FORMAT				colorFormat,
	MWCAP_VIDEO_QUANTIZATION_RANGE			quantRange,
	MWCAP_VIDEO_SATURATION_RANGE			satRange
	);

MW_RESULT
LIBMWCAPTURE_API
MWCaptureVideoFrameToPhysicalAddressEx(
	HCHANNEL 						hChannel,
	int								iFrame,
	LONGLONG						llFrameAddress,
	DWORD							cbFrame,
	DWORD							cbStride,
	BOOLEAN							bBottomUp,
	MWCAP_PTR64						pvContext,
	DWORD							dwFOURCC,
	int								cx,
	int								cy,
	DWORD							dwProcessSwitchs,
	int								cyParitalNotify,
	HOSD							hOSDImage,
	const RECT *					pOSDRects,
	int								cOSDRects,
	SHORT							sContrast,
	SHORT							sBrightness,
	SHORT							sSaturation,
	SHORT							sHue,
	MWCAP_VIDEO_DEINTERLACE_MODE			deinterlaceMode,
	MWCAP_VIDEO_ASPECT_RATIO_CONVERT_MODE	aspectRatioConvertMode,
	const RECT *							pRectSrc,
	const RECT *							pRectDest,
	int										nAspectX,
	int										nAspectY,
	MWCAP_VIDEO_COLOR_FORMAT				colorFormat,
	MWCAP_VIDEO_QUANTIZATION_RANGE			quantRange,
	MWCAP_VIDEO_SATURATION_RANGE			satRange
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoBufferInfo(
	HCHANNEL 						hChannel,
	MWCAP_VIDEO_BUFFER_INFO *		pVideoBufferInfo
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoFrameInfo(
	HCHANNEL 						hChannel,
	BYTE							i,
	MWCAP_VIDEO_FRAME_INFO*			pVideoFrameInfo
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoCaptureStatus(
	HCHANNEL 						hChannel,
	MWCAP_VIDEO_CAPTURE_STATUS *	pStatus
	);

// Audio Capture
MW_RESULT
LIBMWCAPTURE_API
MWStartAudioCapture(
	HCHANNEL 						hChannel
	);	

MW_RESULT
LIBMWCAPTURE_API
MWStopAudioCapture(
	HCHANNEL 						hChannel
	);
	
MW_RESULT
LIBMWCAPTURE_API
MWCaptureAudioFrame(
	HCHANNEL 						hChannel,
	MWCAP_AUDIO_CAPTURE_FRAME*	pAudioCaptureFrame
	);

// Video processing
MW_RESULT
LIBMWCAPTURE_API
MWSetVideoInputAspectRatio(
	HCHANNEL 						hChannel,
	int								nAspectX,
	int								nAspectY
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoInputAspectRatio(
	HCHANNEL 						hChannel,
	int*							pnAspectX,
	int*							pnAspectY
	);

MW_RESULT
LIBMWCAPTURE_API
MWSetVideoInputColorFormat(
	HCHANNEL 						hChannel,
	MWCAP_VIDEO_COLOR_FORMAT		colorFormat
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoInputColorFormat(
	HCHANNEL 						hChannel,
	MWCAP_VIDEO_COLOR_FORMAT *		pColorFormat
	);

MW_RESULT
LIBMWCAPTURE_API
MWSetVideoInputQuantizationRange(
	HCHANNEL 						hChannel,
	MWCAP_VIDEO_QUANTIZATION_RANGE	quantRange
	);

MW_RESULT
LIBMWCAPTURE_API
MWGetVideoInputQuantizationRange(
	HCHANNEL 						hChannel,
	MWCAP_VIDEO_QUANTIZATION_RANGE* pQuantRange
	);

// LED Mode
MW_RESULT
LIBMWCAPTURE_API
MWSetLEDMode(
	HCHANNEL 						hChannel,
	DWORD							dwMode
	);

// Upgrade Firmware
MW_RESULT
LIBMWCAPTURE_API
MWGetFirmwareStorageInfo(
	HCHANNEL 						hChannel,
	MWCAP_FIRMWARE_STORAGE *		pFirmwareStorageInfo
	);

MW_RESULT
LIBMWCAPTURE_API
MWEraseFirmwareData(
	HCHANNEL 						hChannel,
	DWORD							cbOffset,
	DWORD							cbErase
	);

MW_RESULT
LIBMWCAPTURE_API
MWReadFirmwareData(
	HCHANNEL 						hChannel,
	DWORD							cbOffset,
	BYTE *							pbyData,
	DWORD							cbToRead,
	DWORD *							pcbRead
	);

MW_RESULT
LIBMWCAPTURE_API
MWWriteFirmwareData(
	HCHANNEL 						hChannel,
	DWORD							cbOffset,
	BYTE *							pbyData,
	DWORD							cbData
	);

MW_RESULT
LIBMWCAPTURE_API
MWSetPostReconfig(
	HCHANNEL 						hChannel,
	DWORD							dwDelayMS
	);

// OSD
HOSD
LIBMWCAPTURE_API
MWCreateImage(
	HCHANNEL 						hChannel,
	int								cx,
	int								cy
	);

MW_RESULT
LIBMWCAPTURE_API
MWOpenImage(
	HCHANNEL 						hChannel,
	HOSD							hImage,
	LONG*							plRet
	);

MW_RESULT
LIBMWCAPTURE_API
MWCloseImage(
	HCHANNEL 						hChannel,
	HOSD							hImage,
	LONG*							plRet
	);

MW_RESULT
LIBMWCAPTURE_API
MWUploadImageFromVirtualAddress(
	HCHANNEL 						hChannel,
	HOSD							hImage,
	MWCAP_VIDEO_COLOR_FORMAT		cfDest,
	MWCAP_VIDEO_QUANTIZATION_RANGE	quantRangeDest,
	MWCAP_VIDEO_SATURATION_RANGE	satRangeDest,
	WORD							xDest,
	WORD							yDest,
	WORD							cxDest,
	WORD							cyDest,
	MWCAP_PTR64						pvSrcFrame,
	DWORD							cbSrcFrame,
	DWORD							cbSrcStride,
	WORD							cxSrc,
	WORD							cySrc,
	BOOLEAN							bSrcBottomUp,
	BOOLEAN							bSrcPixelAlpha,
	BOOLEAN							bSrcPixelXBGR
	);

MW_RESULT
LIBMWCAPTURE_API
MWUploadImageFromPhysicalAddress(
	HCHANNEL 						hChannel,
	HOSD							hImage,
	MWCAP_VIDEO_COLOR_FORMAT		cfDest,
	MWCAP_VIDEO_QUANTIZATION_RANGE	quantRangeDest,
	MWCAP_VIDEO_SATURATION_RANGE	satRangeDest,
	WORD							xDest,
	WORD							yDest,
	WORD							cxDest,
	WORD							cyDest,
	LONGLONG						llSrcFrameAddress,
	DWORD							cbSrcFrame,
	DWORD							cbSrcStride,
	WORD							cxSrc,
	WORD							cySrc,
	BOOLEAN							bSrcBottomUp,
	BOOLEAN							bSrcPixelAlpha,
	BOOLEAN							bSrcPixelXBGR
	);

MW_RESULT
LIBMWCAPTURE_API
MWCreateExtendObject(
	HCHANNEL						hChannel,
	GUID							guid,
	LPVOID*							ppObject
	);

#ifdef __cplusplus
}
#endif

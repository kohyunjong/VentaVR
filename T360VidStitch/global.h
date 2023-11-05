#pragma once

#define MIN_APP_WIDTH	1920
#define MIN_APP_HEIGHT	1080

#define NEWLOADSCREENGQP 50
#define LOADRESENTSCREENGQP 25
#define MAX_SOURCE		8

#define TIMER_STARTAPP					0x0FA0
#define TIMER_STARTSCREENGL				0x0FA1
#define TIMER_STARTRECODE				0x0FA2
#define TIMER_SNAPSHOTDONE				0x0FA3
#define TIMER_CALIBRATIONDONE			0x0FA4
#define TIMER_STARTENCODINGWAIT			0x0FA5
#define TIMER_ENDENCODINGWAIT			0x0FA6
#define TIMER_STREAMSTARTWAIT			0x0FA7
#define TIMER_STREAMENDWAIT				0x0FA8
#define TIMER_OUTPUTSTARTWAIT			0x0FA9
#define TIMER_OUTPUTENDWAIT				0x0FAA
#define TIMER_CHECKCALIBRATION			0x0FAB
#define TIMER_FIRSTUPDATEPANO_RIGHT		0x0FAC
#define TIMER_FUPDATEPANO_RIGHTCHECK	0x0FAD
#define TIMER_INTO_CENTERSCREEN			0x0FAE
#define TIMER_CALIBRATIONFAIL			0x0FAF
#define TIMER_MASKSAVEDONE				0x0FB0
#define TIMER_VODSTARTWAIT				0x0FB1
#define TIMER_VODENDWAIT				0x0FB2
#define TIMER_ADDINGOVERLAY				0x0FB3

#define COLOR_DIALOG_TEXT				RGB(236,235,215)
#define COLOR_DIALOG_BK					RGB(37,36,40)
#define COLOR_DIALOG_NOMAL_BK			RGB(43,42,47)
#define COLOR_DIALOG_GRAY_TEXT			RGB(89,88,92)
#define COLOR_DIALOG_GRAY_BK			RGB(43,42,47)
#define COLOR_CONFIG_NOMAL_BK			RGB(31, 30, 33)
#define COLOR_LOAD_DIALOG_BK			RGB(53, 52, 57)

#define COLOR_SELECTED_CMBBOX			GetSysColor(COLOR_HIGHLIGHT)
#define COLOR_LOAD_DIALOG_BK			RGB(34,34,34)
#define COLOR_LOAD_RECENTCONFI_BK		RGB(40,40,40)
#define COLOR_LOAD_RECENTCONFI_LINE		RGB(109,111,115)


#define WM_UPDATESTRINGFPS   WM_USER+999
#define WM_CLOSEEDITINPUTSTEREO   WM_USER+1000
#define WM_CLOSEEDITINPUT   WM_USER+1001
#define WM_CLOSECOLORDLG   WM_USER+1002

enum vdstScreenCenter
{
	vdstNewLoad,				// New Load Configuration Screen
	vdstSourceView,				// Source View Screen
	vdstPanoramaView,			// Panorama View Screen
	vdstInteractiveView,		// Interactive View Screen
	vdstScreenView,
	vdstConfiguView,			// Configuration View Screen
	vdstLeftControlView,			// Left Button View Screen
	vdstRightSourceView,
	vdstRightPanoramaView,
	vdstRightInteracView,
	vdstRightCalibView,
	vdstRightOverlayView,				// for Overlay
	vdstRightOverlayListView,				// for OverlayList
	vdstRightAdvanceView,
	vdstRightAdvanceAlignView,
	vdstRightAdvanceUpdateView,
	vdstRightAdvanceManualView,
	vdstRightManualCalibView,			//Manual Calibration
	vdstBottomSourceView,
	vdstBottomPanoramaView,
	vdstBottomManualClibView,
	vdstBottomInteractiveView,
	vdstScreenMax               // Max Screen Count
};

enum vdstScrrenPosition
{
	vdstLeft,			// Left 
	vdstRight,			// Right
	vdstBottom,			// Bottom
	vdstCenter			// Center
};

enum vdstPaneScreen
{
	vdstBlankScreen,			// Hide All
	vdstSourceScreen,			
	vdstPanoramaScreen,			
	InteractiveView
};

enum vdstToastType
{
	vdstToastHideAll,			// Hide All
	vdstToastHdd,			// Hdd
	vdstToastStream,		// Streaming
	vdstToastOutput,		// Output
	vdstToastProgress,		// Progress
	vdstToastWarping,		// makeWarping
	vdstToastFail,			// Fail
	vdstToastDone			// Done
};

enum vdstRecStatus
{
	recReady,			
	recDoing,			
	recFinish
};

enum vdstFontType
{
	Aria10,			// 10 size
	TAria11,		// Thin / 11 size 
	Aria11,			// 11 size
	Aria12,			// 12 size
	BAria12,		// Bold / 12 size
	BAria13			// Bold / 13 size
};

#define COLOR_RED		0
#define COLOR_GREEN		1
#define COLOR_BLUE		2
#define LINE_COLOR_RED		RGB(255,0,0)
#define LINE_COLOR_GREEN	RGB(0,255,0)
#define LINE_COLOR_BLUE		RGB(0,0,255)



/*Quram Video360 API Example*/
#define PANORAMA_RREVIEW_FULL		0x01
#define PANORAMA_RREVIEW_UP			0x02
#define PANORAMA_RREVIEW_MID		0x03
#define PANORAMA_RREVIEW_DOWN		0x04

#define MODE_DRAW_SOURCE 0
#define MODE_DRAW_OUTPUT 1
#define MODE_DRAW_INTERACTIVE 2
#define MODE_DRAW_MANUAL_CALIB 3

#define MODE_PTS_LOAD

//юс╫ц??
const int WIDGET_W = 1268;
const int WIDGET_H = 620;

const int IMG_W = 1440;
const int IMG_H = 1080;

#define		NOMALSKIN  _T("\\baseskin.skin")
#define		SUBSKIN  _T("\\subskin.skin")

//170411 temp
#define SUBMODE_OFF					0x00
#define SUBMODE_ADVANCE_UPDATE		0x01
//#define SUBMODE_ADVANCE_MASK		0x02
#define SUBMODE_ADVANCE_STEP		0x03
#define SUBMODE_EDIT_ROTATION		0x04
#define SUBMODE_MANUAL_CALIB_MASK	0x05
#define SUBMODE_REGIONAL_WARPING	0x06

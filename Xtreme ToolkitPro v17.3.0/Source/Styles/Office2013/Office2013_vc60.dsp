# Microsoft Developer Studio Project File - Name="Office2013" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Office2013 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Office2013.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Office2013.mak" CFG="Office2013 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Office2013 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release/vc60"
# PROP BASE Intermediate_Dir "Release/vc60"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release/vc60"
# PROP Intermediate_Dir "Release/vc60"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XTPRESOURCEARSA_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XTPRESOURCE_EXPORTS" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../" /d "NDEBUG" /d "_AFXDLL" /d "_XTP_INCLUDE_VERSION"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 /nodefaultlib /out:"../Office2013.dll" /opt:nowin98 /noentry
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build
TargetPath=\SVN\ToolkitPro\Source\Styles\Office2013.dll
TargetName=Office2013
InputPath=\SVN\ToolkitPro\Source\Styles\Office2013.dll
SOURCE="$(InputPath)"

BuildCmds= \
	if exist "$(TargetPath)" copy "$(TargetPath)" "../../../bin/vc60/Styles" \
	if exist "$(TargetPath)" copy "$(TargetPath)" "../../../Samples/ActiveX/Styles" \
	

"../../../bin/vc60/Styles/$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"../../../Samples/ActiveX/Styles/$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build
# Begin Target

# Name "Office2013 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Office2013.rc
# End Source File
# End Group
# Begin Group "Image Files"

# PROP Default_Filter ".png;.bmp"
# Begin Group "png"

# PROP Default_Filter "*.png"
# Begin Source File

SOURCE=.\res\RibbonBackCalligraphyLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCalligraphyLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCalligraphyLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCalligraphyMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCalligraphyMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCalligraphyMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCalligraphySmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCalligraphySmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCalligraphySmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCirclesAndStripesLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCirclesAndStripesLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCirclesAndStripesLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCirclesAndStripesMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCirclesAndStripesMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCirclesAndStripesMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCirclesAndStripesSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCirclesAndStripesSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCirclesAndStripesSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCircuitLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCircuitLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCircuitLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCircuitMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCircuitMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCircuitMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCircuitSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCircuitSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCircuitSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCloudsLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCloudsLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCloudsLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCloudsMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCloudsMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCloudsMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCloudsSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCloudsSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackCloudsSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleCirclesLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleCirclesLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleCirclesLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleCirclesMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleCirclesMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleCirclesMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleCirclesSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleCirclesSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleCirclesSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleDiamondsLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleDiamondsLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleDiamondsLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleDiamondsMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleDiamondsMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleDiamondsMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleDiamondsSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleDiamondsSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackDoodleDiamondsSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackGeometryLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackGeometryLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackGeometryLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackGeometryMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackGeometryMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackGeometryMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackGeometrySmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackGeometrySmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackGeometrySmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackLunchBoxLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackLunchBoxLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackLunchBoxLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackLunchBoxMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackLunchBoxMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackLunchBoxMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackLunchBoxSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackLunchBoxSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackLunchBoxSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSchoolSuppliesLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSchoolSuppliesLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSchoolSuppliesLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSchoolSuppliesMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSchoolSuppliesMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSchoolSuppliesMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSchoolSuppliesSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSchoolSuppliesSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSchoolSuppliesSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSpringLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSpringLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSpringLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSpringMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSpringMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSpringMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSpringSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSpringSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackSpringSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStarsLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStarsLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStarsLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStarsMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStarsMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStarsMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStarsSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStarsSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStarsSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStrawsLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStrawsLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStrawsLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStrawsMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStrawsMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStrawsMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStrawsSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStrawsSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackStrawsSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackTreeRingsLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackTreeRingsLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackTreeRingsLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackTreeRingsMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackTreeRingsMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackTreeRingsMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackTreeRingsSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackTreeRingsSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackTreeRingsSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackUnderWaterLarge.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackUnderWaterLargeDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackUnderWaterLargeLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackUnderWaterMedium.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackUnderWaterMediumDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackUnderWaterMediumLight.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackUnderWaterSmall.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackUnderWaterSmallDark.png
# End Source File
# Begin Source File

SOURCE=.\res\RibbonBackUnderWaterSmallLight.png
# End Source File
# Begin Source File

SOURCE=.\res\ShortcutBarGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\ShortcutBarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\ShortcutBarItems.png
# End Source File
# Begin Source File

SOURCE=.\res\ShortcutBarNavigateItem.png
# End Source File
# Begin Source File

SOURCE=.\res\TimeLineViewAppointmentIcon.png
# End Source File
# Begin Source File

SOURCE=.\Styles\Office2013\res\TimeLineViewAppointmentIcon.png
# End Source File
# End Group
# Begin Group "bmp"

# PROP Default_Filter "*.bmp"
# Begin Source File

SOURCE=.\res\CalendarEventGlyphs.bmp
# End Source File
# End Group
# End Group
# Begin Group "Ini Files"

# PROP Default_Filter ".ini"
# Begin Source File

SOURCE=.\Office2013Access.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013AccessGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013AccessGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013Excel.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013ExcelGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013ExcelGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OneNote.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OneNoteGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OneNoteGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013Outlook.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OutlookGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013OutlookGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PowerPoint.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PowerPointGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PowerPointGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013Publisher.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PublisherGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013PublisherGrayLight.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013Word.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013WordGrayDark.ini
# End Source File
# Begin Source File

SOURCE=.\Office2013WordGrayLight.ini
# End Source File
# End Group
# Begin Group "Zip Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Styles\Office2013\XAMLStyle.zip
# End Source File
# End Group
# End Target
# End Project

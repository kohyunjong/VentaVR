# Microsoft Developer Studio Project File - Name="VisualStudio2015" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VisualStudio2015 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisualStudio2015.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisualStudio2015.mak" CFG="VisualStudio2015 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisualStudio2015 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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
# ADD LINK32 /nologo /dll /machine:I386 /nodefaultlib /out:"../VisualStudio2015.dll" /opt:nowin98 /noentry
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build
TargetPath=\SVN\ToolkitPro\Source\Styles\VisualStudio2015.dll
TargetName=VisualStudio2015
InputPath=\SVN\ToolkitPro\Source\Styles\VisualStudio2015.dll
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

# Name "VisualStudio2015 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\VisualStudio2015.rc
# End Source File
# End Group
# Begin Group "Image Files"

# PROP Default_Filter ".png;.bmp"
# Begin Group "png"

# PROP Default_Filter ""
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
# End Group
# Begin Group "bmp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\CalendarEventGlyphs.bmp
# End Source File
# End Group
# End Group
# Begin Group "Ini Files"

# PROP Default_Filter ".ini"
# Begin Source File

SOURCE=.\VisualStudio2015Blue.ini
# End Source File
# Begin Source File

SOURCE=.\VisualStudio2015Dark.ini
# End Source File
# Begin Source File

SOURCE=.\VisualStudio2015Light.ini
# End Source File
# End Group
# Begin Group "Zip Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XamlStyle.zip
# End Source File
# End Group
# End Target
# End Project

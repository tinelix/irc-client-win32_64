# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=TinelixIRC - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to TinelixIRC - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TinelixIRC - Win32 Release" && "$(CFG)" !=\
 "TinelixIRC - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "TinelixIRC.mak" CFG="TinelixIRC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TinelixIRC - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TinelixIRC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "TinelixIRC - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "TinelixIRC - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\tlx_irc.exe"

CLEAN : 
	-@erase ".\Release\tlx_irc.exe"
	-@erase ".\Release\IRCChatPage.obj"
	-@erase ".\Release\TinelixIRC.pch"
	-@erase ".\Release\StdAfx.obj"
	-@erase ".\Release\ConnectionManagerDialog.obj"
	-@erase ".\Release\MainWindow.obj"
	-@erase ".\Release\SettingsDialog.obj"
	-@erase ".\Release\StatisticsDialog.obj"
	-@erase ".\Release\AboutDialog.obj"
	-@erase ".\Release\TinelixIRC.obj"
	-@erase ".\Release\EditDialog.obj"
	-@erase ".\Release\TinelixIRC.res"
	-@erase ".\Release\ParserSettingsDialog.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)/TinelixIRC.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
RSC_PROJ=/l 0x419 /fo"$(INTDIR)/TinelixIRC.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TinelixIRC.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/tlx_irc.exe"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/tlx_irc.pdb" /machine:I386 /out:"$(OUTDIR)/tlx_irc.exe" 
LINK32_OBJS= \
	"$(INTDIR)/IRCChatPage.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/ConnectionManagerDialog.obj" \
	"$(INTDIR)/MainWindow.obj" \
	"$(INTDIR)/SettingsDialog.obj" \
	"$(INTDIR)/StatisticsDialog.obj" \
	"$(INTDIR)/AboutDialog.obj" \
	"$(INTDIR)/TinelixIRC.obj" \
	"$(INTDIR)/EditDialog.obj" \
	"$(INTDIR)/ParserSettingsDialog.obj" \
	"$(INTDIR)/TinelixIRC.res"

"$(OUTDIR)\tlx_irc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TinelixIRC - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\tlx_irc.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\TinelixIRC.pch"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\tlx_irc.exe"
	-@erase ".\Debug\StatisticsDialog.obj"
	-@erase ".\Debug\SettingsDialog.obj"
	-@erase ".\Debug\MainWindow.obj"
	-@erase ".\Debug\IRCChatPage.obj"
	-@erase ".\Debug\EditDialog.obj"
	-@erase ".\Debug\ConnectionManagerDialog.obj"
	-@erase ".\Debug\TinelixIRC.obj"
	-@erase ".\Debug\AboutDialog.obj"
	-@erase ".\Debug\StdAfx.obj"
	-@erase ".\Debug\TinelixIRC.res"
	-@erase ".\Debug\ParserSettingsDialog.obj"
	-@erase ".\Debug\tlx_irc.ilk"
	-@erase ".\Debug\tlx_irc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)/TinelixIRC.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
RSC_PROJ=/l 0x419 /fo"$(INTDIR)/TinelixIRC.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TinelixIRC.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/tlx_irc.exe"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/tlx_irc.pdb" /debug /machine:I386 /out:"$(OUTDIR)/tlx_irc.exe" 
LINK32_OBJS= \
	"$(INTDIR)/StatisticsDialog.obj" \
	"$(INTDIR)/SettingsDialog.obj" \
	"$(INTDIR)/MainWindow.obj" \
	"$(INTDIR)/IRCChatPage.obj" \
	"$(INTDIR)/EditDialog.obj" \
	"$(INTDIR)/ConnectionManagerDialog.obj" \
	"$(INTDIR)/TinelixIRC.obj" \
	"$(INTDIR)/AboutDialog.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/ParserSettingsDialog.obj" \
	"$(INTDIR)/TinelixIRC.res"

"$(OUTDIR)\tlx_irc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "TinelixIRC - Win32 Release"
# Name "TinelixIRC - Win32 Debug"

!IF  "$(CFG)" == "TinelixIRC - Win32 Release"

!ELSEIF  "$(CFG)" == "TinelixIRC - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\TinelixIRC.cpp
DEP_CPP_TINEL=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\MainWindow.h"\
	".\IRCChatPage.h"\
	".\StatisticsDialog.h"\
	

"$(INTDIR)\TinelixIRC.obj" : $(SOURCE) $(DEP_CPP_TINEL) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainWindow.cpp

!IF  "$(CFG)" == "TinelixIRC - Win32 Release"

DEP_CPP_MAINW=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\MainWindow.h"\
	".\ConnectionManagerDialog.h"\
	".\StatisticsDialog.h"\
	".\SettingsDialog.h"\
	".\AboutDialog.h"\
	".\IRCChatPage.h"\
	

"$(INTDIR)\MainWindow.obj" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


!ELSEIF  "$(CFG)" == "TinelixIRC - Win32 Debug"

DEP_CPP_MAINW=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\MainWindow.h"\
	".\ConnectionManagerDialog.h"\
	".\SettingsDialog.h"\
	".\AboutDialog.h"\
	".\IRCChatPage.h"\
	".\StatisticsDialog.h"\
	
NODEP_CPP_MAINW=\
	".\HISTORY"\
	".\*(strrchr(h_pcp2, '\')+1)='\0';"\
	

"$(INTDIR)\MainWindow.obj" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "TinelixIRC - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/TinelixIRC.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TinelixIRC.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "TinelixIRC - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)/TinelixIRC.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TinelixIRC.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TinelixIRC.rc
DEP_RSC_TINELI=\
	".\res\idr_tray.ico"\
	".\res\ico00001.ico"\
	".\res\idr_main.ico"\
	".\res\TinelixIRC.rc2"\
	

"$(INTDIR)\TinelixIRC.res" : $(SOURCE) $(DEP_RSC_TINELI) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IRCChatPage.cpp
DEP_CPP_IRCCH=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\IRCChatPage.h"\
	

"$(INTDIR)\IRCChatPage.obj" : $(SOURCE) $(DEP_CPP_IRCCH) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ConnectionManagerDialog.cpp
DEP_CPP_CONNE=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\ConnectionManagerDialog.h"\
	".\MainWindow.h"\
	".\EditDialog.h"\
	".\IRCChatPage.h"\
	".\StatisticsDialog.h"\
	

"$(INTDIR)\ConnectionManagerDialog.obj" : $(SOURCE) $(DEP_CPP_CONNE)\
 "$(INTDIR)" "$(INTDIR)\TinelixIRC.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditDialog.cpp

!IF  "$(CFG)" == "TinelixIRC - Win32 Release"

DEP_CPP_EDITD=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\EditDialog.h"\
	

"$(INTDIR)\EditDialog.obj" : $(SOURCE) $(DEP_CPP_EDITD) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


!ELSEIF  "$(CFG)" == "TinelixIRC - Win32 Debug"

DEP_CPP_EDITD=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\EditDialog.h"\
	
NODEP_CPP_EDITD=\
	".\Ќельз€ создавать профиль с таким именем, так как это им€ уже зарезервировано дл€ настроек клиента"\
	

"$(INTDIR)\EditDialog.obj" : $(SOURCE) $(DEP_CPP_EDITD) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SettingsDialog.cpp
DEP_CPP_SETTI=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\SettingsDialog.h"\
	".\MainWindow.h"\
	".\ParserSettingsDialog.h"\
	".\IRCChatPage.h"\
	".\StatisticsDialog.h"\
	

"$(INTDIR)\SettingsDialog.obj" : $(SOURCE) $(DEP_CPP_SETTI) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\AboutDialog.cpp
DEP_CPP_ABOUT=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\AboutDialog.h"\
	

"$(INTDIR)\AboutDialog.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StatisticsDialog.cpp
DEP_CPP_STATI=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\StatisticsDialog.h"\
	

"$(INTDIR)\StatisticsDialog.obj" : $(SOURCE) $(DEP_CPP_STATI) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ParserSettingsDialog.cpp
DEP_CPP_PARSE=\
	".\StdAfx.h"\
	".\TinelixIRC.h"\
	".\ParserSettingsDialog.h"\
	

"$(INTDIR)\ParserSettingsDialog.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"\
 "$(INTDIR)\TinelixIRC.pch"


# End Source File
# End Target
# End Project
################################################################################

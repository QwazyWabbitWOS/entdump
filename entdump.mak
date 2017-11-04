# Microsoft Developer Studio Generated NMAKE File, Based on entdump.dsp
!IF "$(CFG)" == ""
CFG=entdump - Win32 Debug
!MESSAGE No configuration specified. Defaulting to entdump - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "entdump - Win32 Release" && "$(CFG)" != "entdump - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "entdump.mak" CFG="entdump - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "entdump - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "entdump - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "entdump - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\entdump.exe"


CLEAN :
	-@erase "$(INTDIR)\entdump.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\entdump.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\entdump.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\entdump.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\entdump.pdb" /machine:I386 /out:"$(OUTDIR)\entdump.exe" 
LINK32_OBJS= \
	"$(INTDIR)\entdump.obj"

"$(OUTDIR)\entdump.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "entdump - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\entdump.exe" "$(OUTDIR)\entdump.bsc"


CLEAN :
	-@erase "$(INTDIR)\entdump.obj"
	-@erase "$(INTDIR)\entdump.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\entdump.bsc"
	-@erase "$(OUTDIR)\entdump.exe"
	-@erase "$(OUTDIR)\entdump.ilk"
	-@erase "$(OUTDIR)\entdump.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\entdump.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\entdump.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\entdump.sbr"

"$(OUTDIR)\entdump.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\entdump.pdb" /debug /machine:I386 /out:"$(OUTDIR)\entdump.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\entdump.obj"

"$(OUTDIR)\entdump.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("entdump.dep")
!INCLUDE "entdump.dep"
!ELSE 
!MESSAGE Warning: cannot find "entdump.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "entdump - Win32 Release" || "$(CFG)" == "entdump - Win32 Debug"
SOURCE=.\entdump.c

!IF  "$(CFG)" == "entdump - Win32 Release"


"$(INTDIR)\entdump.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "entdump - Win32 Debug"


"$(INTDIR)\entdump.obj"	"$(INTDIR)\entdump.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 


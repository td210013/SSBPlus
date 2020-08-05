# Microsoft Developer Studio Project File - Name="CppUnitTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=CppUnitTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CppUnitTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CppUnitTest.mak" CFG="CppUnitTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CppUnitTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "CppUnitTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CppUnitTest - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\cppunit\include" /I "..\..\cppunit\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UNICODE" /D "_AFXDLL" /D "CPPUNIT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 cppunit6.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ESAInterventionXmlLib.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\cppunit\LIB" /libpath:"..\ESAInterventionXmlLib\Release"

!ELSEIF  "$(CFG)" == "CppUnitTest - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\cppunit\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UNICODE" /D "_AFXDLL" /D "CPPUNIT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cppunit6d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ESAInterventionXmlLib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\cppunit\LIB" /libpath:"..\ESAInterventionXmlLib\Debug"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "CppUnitTest - Win32 Release"
# Name "CppUnitTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CppUnitTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CppUnitTest.rc
# End Source File
# Begin Source File

SOURCE=.\ESAI_Media_TestSuite.cpp
# End Source File
# Begin Source File

SOURCE=.\ESAI_Media_TestSuite.h
# End Source File
# Begin Source File

SOURCE=.\ESAI_MediaEvent_TestSuite.cpp
# End Source File
# Begin Source File

SOURCE=.\ESAI_MediaEvent_TestSuite.h
# End Source File
# Begin Source File

SOURCE=.\ESAI_MediaEvents_TestSuite.cpp
# End Source File
# Begin Source File

SOURCE=.\ESAI_MediaEvents_TestSuite.h
# End Source File
# Begin Source File

SOURCE=.\ESAI_MediaList_TestSuite.cpp
# End Source File
# Begin Source File

SOURCE=.\ESAI_MediaList_TestSuite.h
# End Source File
# Begin Source File

SOURCE=.\ESAI_Position_TestSuite.cpp
# End Source File
# Begin Source File

SOURCE=.\ESAI_Position_TestSuite.h
# End Source File
# Begin Source File

SOURCE=.\ESAI_WeightReference_TestSuite.cpp
# End Source File
# Begin Source File

SOURCE=.\ESAI_WeightReference_TestSuite.h
# End Source File
# Begin Source File

SOURCE=.\ESAInterventionXml_TestSuite.cpp
# End Source File
# Begin Source File

SOURCE=.\ESAInterventionXml_TestSuite.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Template.cpp
# End Source File
# Begin Source File

SOURCE=.\Template.h
# End Source File
# Begin Source File

SOURCE=.\Template_TestSuite.cpp
# End Source File
# Begin Source File

SOURCE=.\Template_TestSuite.h
# End Source File
# Begin Source File

SOURCE=.\UnitTestBase.cpp
# End Source File
# Begin Source File

SOURCE=.\UnitTestBase.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CppUnitTest.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

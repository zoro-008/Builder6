# Microsoft Developer Studio Project File - Name="HVSystem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HVSystem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HVSystem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HVSystem.mak" CFG="HVSystem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HVSystem - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HVSystem - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HVSystem - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "COMPILING_KFITTINGLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /stack:0x1f3fc18 /subsystem:windows /machine:I386 /out:"Release/TCosmos3DualAuto_Vision.exe"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "HVSystem - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /stack:0x1f3fc18 /subsystem:windows /map /debug /machine:I386 /out:"Debug/TCosmos3DualAuto_Vision.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "HVSystem - Win32 Release"
# Name "HVSystem - Win32 Debug"
# Begin Group "Resource Files[T Cosmos2 양산용 ADLINK]"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bmp\tabbutton\3dModeld_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\3dmodeln_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button80x40\80x40d.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button80x40\80x40i.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button80x40\80x40m.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button80x40\80x40n.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button80x40\80x40u.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\acenterSetd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\acenterSetn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\addd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\addn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\addu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\Ang_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\aokn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\aon.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\aopd.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\applyd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\applyn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\applyu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\auto.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibModeBtn\auto\AutoD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibModeBtn\auto\AutoN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibModeBtn\auto\AutoU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\axis_sta.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\bt26x26disable_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\bt26x26enable_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\bt26x26mask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_locate\btn_located_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_locate\btn_locatemask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_locate\btn_locaten_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_locate\btn_locateu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_poweroff\btn_powerd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_poweroff\btn_poweri_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_poweroff\btn_powermask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_poweroff\btn_powern_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_poweroff\btn_poweru_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btnaddd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnaddd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btnaddn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnaddn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btnaddu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnaddu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btncanceld_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btncanceln_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btncancelu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btndeleted_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btndeleted_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btndeleten_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btndeleten_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btndeleteu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btndeleteu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CTRLBTN_EXIT\btnmask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btnmask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnmask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btnmodifyd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnmodifyd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btnmodifyn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnmodifyn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setup_window\btnmodifyu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnmodifyu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnokd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnokn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\teach_window\btnoku_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_end\btnpmtendd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_end\btnpmtendi_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_end\btnpmtendn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_end\btnpmtendu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_save\btnpmtsaved_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_save\btnpmtsavei_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_save\btnpmtsaven_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_save\btnpmtsaveu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_start\btnpmtstartd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_start\btnpmtstarti_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_start\btnpmtstartmask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_start\btnpmtstartn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt_start\btnpmtstartu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibModeBtn\CalibMask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_okcancel80x30\canceld_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibWindow\CANCELD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okcancel100x50\canceld_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_okcancel80x30\canceln_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibWindow\CANCELN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okcancel100x50\canceln_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibWindow\CANCELU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okcancel100x50\cancelu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\cdatad_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\Cdatan_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_STATUS\Check_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_DOWN\CONTROL_AXIS_DOWND_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_DOWN\CONTROL_AXIS_DOWNN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_DOWN\CONTROL_AXIS_DOWNU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_LEFT\CONTROL_AXIS_LEFTD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_LEFT\CONTROL_AXIS_LEFTN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_LEFT\CONTROL_AXIS_LEFTU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_RIGHT\CONTROL_AXIS_RIGHTD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_RIGHT\CONTROL_AXIS_RIGHTN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_RIGHT\CONTROL_AXIS_RIGHTU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CONTROL_AXIS_RRTM.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_STOP\CONTROL_AXIS_STOPD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_STOP\CONTROL_AXIS_STOPN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_STOP\CONTROL_AXIS_STOPU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_LEFT_ROTATE\CONTROL_AXIS_ULRD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_LEFT_ROTATE\CONTROL_AXIS_ULRI_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_LEFT_ROTATE\CONTROL_AXIS_ULRN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_LEFT_ROTATE\CONTROL_AXIS_ULRU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_UP\CONTROL_AXIS_UPD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_UP\CONTROL_AXIS_UPN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_UP\CONTROL_AXIS_UPU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_RIGHT_ROTATE\CONTROL_AXIS_URRD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_RIGHT_ROTATE\CONTROL_AXIS_URRI_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_RIGHT_ROTATE\CONTROL_AXIS_URRN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_RIGHT_ROTATE\CONTROL_AXIS_URRU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_ZDOWN\CONTROL_AXIS_ZDOWND_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_ZDOWN\CONTROL_AXIS_ZDOWNN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_ZDOWN\CONTROL_AXIS_ZDOWNU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_ZUP\CONTROL_AXIS_ZUPD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_ZUP\CONTROL_AXIS_ZUPN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_ZUP\CONTROL_AXIS_ZUPU.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_align\ctlalignd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_align\ctlaligni_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_align\ctlalignn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_align\ctlalignu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_auto\ctlautod_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_auto\ctlautoi_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_auto\ctlauton_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_auto\ctlautou_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_align\ctlbuttonmask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt\ctlbuttonmask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CTRLBTN_EXIT\ctlbuttonmask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_cal\ctlcald_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_cal\ctlcali_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_cal\ctlcaln_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_cal\ctlcalu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_grab\ctlgrabd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_grab\ctlgrabi_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_grab\ctlgrabn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_grab\ctlgrabu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_Inspect\ctlinspectd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_Inspect\ctlinspecti_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_Inspect\ctlinspectn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_Inspect\ctlinspectu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_key\ctlkeyd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_key\ctlkeyi_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_key\ctlkeyn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_key\ctlkeyu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_live\ctllived_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_live\ctllivei_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_live\ctlliven_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_live\ctlliveu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_mask\ctlmaskd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_mask\ctlmaski_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_mask\ctlmaskn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_mask\ctlmasku_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt\ctlpmtd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt\ctlpmti_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt\ctlpmtn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_pmt\ctlpmtu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_reset\ctlresetd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_reset\ctlreseti_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_reset\ctlresetn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_reset\ctlresetu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_rsave\ctlrsaved.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_rsave\ctlrsavei.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_rsave\ctlrsaven.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_rsave\ctlrsaveu.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_save\ctlsaved_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_save\ctlsavei_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_save\ctlsaven_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_save\ctlsaveu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_setup\ctlsetupd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_setup\ctlsetupi_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_setup\ctlsetupn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_setup\ctlsetupu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_stop\ctlstopd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_stop\ctlstopi_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_stop\ctlstopn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_stop\ctlstopu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_teach\ctlteachd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_teach\ctlteachi_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_teach\ctlteachn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_teach\ctlteachu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_test\ctltestd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_test\ctltesti_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_test\ctltestn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_test\ctltestu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\datad_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\datan_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_default\defaultd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_default\defaultn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\deleted_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\deleten_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\deleteu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Downd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Downn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Downu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\edit_btnmask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\etcd.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\etcn.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CTRLBTN_EXIT\exitd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CTRLBTN_EXIT\exiti_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CTRLBTN_EXIT\exitn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CTRLBTN_EXIT\exitu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\fittingd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\fittingn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\fittingu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\gcap_button\gcapd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\gcap_button\gcapm_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\gcap_button\gcapn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\hanralogo_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\hanralogomask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\hhd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\hhlsmask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\hhn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\hhu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\hng_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\hok_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\homed_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\homen_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\homeu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\hsd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\hsmask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\hsn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\hsu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HVSystem.ico
# End Source File
# Begin Source File

SOURCE=.\res\HVSystem.rc2
# End Source File
# Begin Source File

SOURCE=.\res\HVSystemDoc.ico
# End Source File
# Begin Source File

SOURCE=.\bmp\Motion_Window\IO_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\IO_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\IO\iomask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\IO\iooff_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\IO\ioon_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\Motion_Window\IOUn.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\IOUn.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\IOWindowBtn.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\IOWindowBtnAPC.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\IOWindowBtnAPCClicked.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\IOWindowBtnAPCMask.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\IOWindowBtnAPCPower.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\IOWindowBtnClick.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\IOWindowBtnMask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\NEWLED\ledbuttonm_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\NEWLED\ledbuttonO_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\NEWLED\ledbuttonX_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Leftd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Leftn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Leftu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\LGlogo\lglogomask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\lhd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\lhn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\lhu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_link90x44\Linkd.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_link90x44\Linkn.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\lng_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\LGlogo\logon.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\lokn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\EDITBOX\manual.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibModeBtn\manual\ManuD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibModeBtn\manual\ManuN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibModeBtn\manual\ManuU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\Masd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\mask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_link90x44\mask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\mask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_master\mask100x50.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okcancel100x50\mask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\Masn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_master\masterd.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_master\mastern.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\modifyd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\modifyn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\modifyu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\Motion_Window\Motion_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\Motion_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\motiond_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\motionm_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\motionn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\Motion_Window\MotionUn.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\MotionUn.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_okcancel80x30\okancelmask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_okcancel80x30\okd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibWindow\OKD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okcancel100x50\okd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_okcancel80x30\okn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibWindow\OKN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okcancel100x50\okn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\CalibWindow\OKU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okcancel100x50\oku_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\passwdd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\passwdn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\pmt_cell\pmtcelld_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\pmt_cell\pmtcelli_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\pmt_cell\pmtcellmask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\pmt_cell\pmtcelln_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\png_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\pok_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\IO\READD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\IO\READN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_MODE\redledd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_MODE\redledn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\rightd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\rightn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\rightu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\rng_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\roisetd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\roisetm_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\roisetn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\roisetu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\rok_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_3dsave\save_mask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_3dsave\saved_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\saved_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_3dsave\saven_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\saven_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\saveparamd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\saveparamn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\saveparamu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\saveu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabsetupwindow\setupbntmask_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setupwindow\SetupBtnMask.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setupwindow\SetupCancelD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setupwindow\SetupCancelN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setupwindow\SetupCancelU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\setupdatad_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\tabbutton\setupdatan_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setupwindow\SetupOKD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setupwindow\SetupOKN_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\setupwindow\SetupOKU_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\sng_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\sok_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Stopd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Stopn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Stopu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_subbeam\subd.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_subbeam\subm.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\button_subbeam\subn.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\title_sysmenu\sysmenucheck_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\title_sysmenu\sysmenud_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\title_sysmenu\sysmenun_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\title_sysmenu\sysmenun_mask.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\titlebar_TableLeft.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\titlebar_TableM.bmp
# End Source File
# Begin Source File

SOURCE=.\guiimage\titlebar_TableRight.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\tng_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\okng\tokn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\AXIS_STATUS\Uncheck_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Upd_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Upn_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\arrow\Upu_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\IO\WriteD_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\IO\WriteM_256.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp\IO\WriteN_256.bmp
# End Source File
# End Group
# Begin Group "Generated Files[T Cosmos2 양산용 ADLINK]"

# PROP Default_Filter "*.cpp;*.h"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DialogMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\HVSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\HVSystem.rc
# End Source File
# Begin Source File

SOURCE=.\HVSystemDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\HVSystemView.cpp
# End Source File
# Begin Source File

SOURCE=.\HVSystemView_Comm.cpp
# End Source File
# Begin Source File

SOURCE=.\LogUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DialogMessage.h
# End Source File
# Begin Source File

SOURCE=.\External.h
# End Source File
# Begin Source File

SOURCE=.\GlobalVar.h
# End Source File
# Begin Source File

SOURCE=.\HVSystem.h
# End Source File
# Begin Source File

SOURCE=.\HVSystemDefine.h
# End Source File
# Begin Source File

SOURCE=.\HVSystemDoc.h
# End Source File
# Begin Source File

SOURCE=.\HVSystemView.h
# End Source File
# Begin Source File

SOURCE=.\LogUnit.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\PAIX\NMC410S.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Group
# Begin Group "GUI Files[T Cosmos2 양산용 ADLINK]"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=..\GuiSrc\BmpRgn.h
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\DisplayToGDI.cpp
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\DisplayToGDI.h
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\ListCtrlStyled.cpp
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\ListCtrlStyled.h
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\StaticSkin.cpp
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\StaticSkin.h
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\xSkinButton.cpp
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\xSkinButton.h
# End Source File
# End Group
# Begin Group "Process Module(DLL)"

# PROP Default_Filter "*.lib; *.h"
# Begin Group "DLL Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\KFitting.h
# End Source File
# Begin Source File

SOURCE=..\include\KProcess.h
# End Source File
# Begin Source File

SOURCE=..\include\KRBlob.h
# End Source File
# Begin Source File

SOURCE=..\include\KThresh.h
# End Source File
# End Group
# End Group
# Begin Group "Window Files[T Cosmos2 양산용 ADLINK]"

# PROP Default_Filter "*.cpp;*.h"
# Begin Group "Cpp Source Files"

# PROP Default_Filter "*.cpp"
# Begin Group "Display class"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=.\HProjection.cpp
# End Source File
# Begin Source File

SOURCE=.\HProjection.h
# End Source File
# Begin Source File

SOURCE=.\ImgDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgDisplay.h
# End Source File
# Begin Source File

SOURCE=.\ImgDisplay2.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgDisplay2.h
# End Source File
# Begin Source File

SOURCE=.\ImgDisplay3.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgDisplay3.h
# End Source File
# Begin Source File

SOURCE=.\VProjection.cpp
# End Source File
# Begin Source File

SOURCE=.\VProjection.h
# End Source File
# End Group
# Begin Group "Passwd files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PasswdInput.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswdInput.h
# End Source File
# Begin Source File

SOURCE=.\PasswdModify.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswdModify.h
# End Source File
# End Group
# Begin Group "GUI window files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BottomBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BottomBar.h
# End Source File
# Begin Source File

SOURCE=.\CtrlWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlWindow.h
# End Source File
# Begin Source File

SOURCE=.\TitleBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleBar.h
# End Source File
# Begin Source File

SOURCE=.\xSkinButtonEx.cpp
# End Source File
# Begin Source File

SOURCE=.\xSkinButtonEx.h
# End Source File
# End Group
# Begin Group "TabWindow"

# PROP Default_Filter ""
# Begin Group "Setting files"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=.\EtcWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\EtcWindow.h
# End Source File
# Begin Source File

SOURCE=.\SetupWin.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupWin.h
# End Source File
# End Group
# Begin Group "3D related files"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=.\3DModelWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\3DModelWindow.h
# End Source File
# Begin Source File

SOURCE=.\OpenGl3D.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGl3D.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\TabWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\TabWindow.h
# End Source File
# End Group
# Begin Group "Io"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IoWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\IoWindow.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AngleWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\AngleWindow.h
# End Source File
# Begin Source File

SOURCE=.\CounterWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CounterWindow.h
# End Source File
# Begin Source File

SOURCE=.\CurrentData.cpp
# End Source File
# Begin Source File

SOURCE=.\CurrentData.h
# End Source File
# Begin Source File

SOURCE=.\DataDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DataDisplay.h
# End Source File
# Begin Source File

SOURCE=.\DisplayComm.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayComm.h
# End Source File
# Begin Source File

SOURCE=.\MoveArea.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveArea.h
# End Source File
# Begin Source File

SOURCE=.\OkNgWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\OkNgWindow.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# End Group
# Begin Group "Process Module(Src)"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=.\DataSequence.h
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\LineProc.cpp
# End Source File
# Begin Source File

SOURCE=..\GuiSrc\LineProc.h
# End Source File
# Begin Source File

SOURCE=.\LineRoi.cpp
# End Source File
# Begin Source File

SOURCE=.\LineRoi.h
# End Source File
# Begin Source File

SOURCE=.\PointRoi.cpp
# End Source File
# Begin Source File

SOURCE=.\PointRoi.h
# End Source File
# Begin Source File

SOURCE=.\QArray.h
# End Source File
# Begin Source File

SOURCE=.\ROISet.cpp
# End Source File
# Begin Source File

SOURCE=.\ROISet.h
# End Source File
# End Group
# Begin Group "RS232 Control"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=.\CommThread.cpp
# End Source File
# Begin Source File

SOURCE=.\CommThread.h
# End Source File
# End Group
# Begin Group "Motion Files[T3DualAuto]"

# PROP Default_Filter ""
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DialogTabMBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTabMBase.h
# End Source File
# Begin Source File

SOURCE=.\DialogTabMIO.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTabMIO.h
# End Source File
# Begin Source File

SOURCE=.\DialogTabMMotion.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTabMMotion.h
# End Source File
# Begin Source File

SOURCE=.\MotionDefine.h
# End Source File
# End Group
# Begin Group "INI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\iniPara.cpp
# End Source File
# Begin Source File

SOURCE=.\iniPara.h
# End Source File
# Begin Source File

SOURCE=.\SystemData.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemData.h
# End Source File
# End Group
# End Group
# Begin Group "IMITech"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CameraControl.cpp
# End Source File
# Begin Source File

SOURCE=.\CameraControl.h
# End Source File
# Begin Source File

SOURCE=.\ImiTech.cpp
# End Source File
# Begin Source File

SOURCE=.\ImiTech.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\test.txt
# End Source File
# End Target
# End Project

; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=ConnectionManagerDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TinelixIRC.h"

ClassCount=10
Class1=IRCClient

ResourceCount=11
Resource2=IDR_MAINMENU
Resource1=IDR_MAINFRAME
Class2=MainWindow
Resource3=IDD_STATISTICSDLG (English (U.S.))
Class3=IRCChatPage
Resource4=IDD_IRCCHATPAGE (English (U.S.))
Resource5=IDD_CONNMAN (English (U.S.))
Class4=ConnectionManagerDialog
Class5=EditDialog
Resource6=IDD_EDITTEXT (English (U.S.))
Resource7=IDD_ABOUTDLG (English (U.S.))
Class6=SettingsDialog
Class7=AboutDialog
Resource8=IDD_MAINWINDOW (English (U.S.))
Class8=StatisticsDialog
Resource9=IDD_SETTINGSDLG (English (U.S.))
Class9=ParserSettingsDialog
Resource10=IDD_PARSER_S (English (U.S.))
Class10=InfoMessageDialog
Resource11=IDD_INFOMSG (English (U.S.))

[CLS:IRCClient]
Type=0
HeaderFile=TinelixIRC.h
ImplementationFile=TinelixIRC.cpp
Filter=N
LastObject=IDC_CONNECTION_PROGRESS

[DLG:IDD_MAINWINDOW (English (U.S.))]
Type=1
Class=MainWindow
ControlCount=2
Control1=IDC_IRC_CHAT_TABS,SysTabControl32,1342177280
Control2=IDC_STATUSBAR_TEXT,static,1342308352

[CLS:MainWindow]
Type=0
HeaderFile=MainWindow.h
ImplementationFile=MainWindow.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=MainWindow

[CLS:IRCChatPage]
Type=0
HeaderFile=IRCChatPage.h
ImplementationFile=IRCChatPage.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO1
VirtualFilter=dWC

[DLG:IDD_IRCCHATPAGE (English (U.S.))]
Type=1
Class=IRCChatPage
ControlCount=3
Control1=IDC_SOCKMSGS,edit,1352730692
Control2=IDC_MSGTEXT,edit,1484849280
Control3=IDC_SENDMSG,button,1476460544

[MNU:IDR_MAINMENU]
Type=1
Class=MainWindow
Command1=ID_FILE_CONNECT
Command2=ID_FILE_STATISTICS
Command3=ID_FILE_QUIT
Command4=ID_VIEW_SETTINGS
Command5=ID_HELP_ABOUT
CommandCount=5

[DLG:IDD_CONNMAN (English (U.S.))]
Type=1
Class=ConnectionManagerDialog
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LANGLABEL,static,1342308352
Control4=IDC_LANGCOMBO1,combobox,1344340227
Control5=IDC_PROFILES_GROUP,button,1342177287
Control6=IDC_PROFILELIST,listbox,1352728835
Control7=IDC_ADD_PROFILE_BTN,button,1342242816
Control8=IDC_CONNECT_PROFILE_BTN,button,1476460544
Control9=IDC_CHANGE_PROFILE_BTN,button,1476460544
Control10=IDC_DELETE_PROFILE_BTn,button,1476460544

[CLS:ConnectionManagerDialog]
Type=0
HeaderFile=ConnectionManagerDialog.h
ImplementationFile=ConnectionManagerDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=ConnectionManagerDialog
VirtualFilter=dWC

[CLS:EditDialog]
Type=0
HeaderFile=EditDialog.h
ImplementationFile=EditDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_EDITTEXT (English (U.S.))]
Type=1
Class=EditDialog
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ENTERTEXTEDIT,edit,1350631552
Control4=IDC_ENTERTEXTLABEL,static,1342308352

[DLG:IDD_SETTINGSDLG (English (U.S.))]
Type=1
Class=SettingsDialog
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LANGLABEL,static,1342308352
Control4=IDC_LANGCOMBO1,combobox,1344340227
Control5=IDC_SAVE_MSG_HISTORY,button,1342242819
Control6=IDC_CHANGE_MSG_FONT,button,1342242816
Control7=IDC_RESTARTREQUIRED,static,1342308352
Control8=IDC_MINIMIZE_TO_TRAY,button,1342242819
Control9=IDC_INTERFACE_GROUP,button,1342177287
Control10=IDC_FUNCTIONAL_GROUP,button,1342177287
Control11=IDC_APPEARANCE_GROUP,button,1342177287
Control12=IDC_PARSER_GROUP,button,1342177287
Control13=IDC_PARSER_LOAD,button,1342242816
Control14=IDC_PARSER_SETTINGS,button,1342242816

[CLS:SettingsDialog]
Type=0
HeaderFile=SettingsDialog.h
ImplementationFile=SettingsDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_APPEARANCE_GROUP
VirtualFilter=dWC

[DLG:IDD_ABOUTDLG (English (U.S.))]
Type=1
ControlCount=10
Control1=IDC_STATIC,static,1342177283
Control2=IDOK,button,1342373889
Control3=IDC_APPNAME,static,1342308352
Control4=IDC_COPYRIGHT,static,1342308352
Control5=IDC_WIN32_EDITION,static,1342308352
Control6=IDC_TECHINFO,button,1342177287
Control7=IDC_WINVER_LABEL,static,1342308352
Control8=IDC_WINVER,static,1342308352
Control9=IDC_LICENSE_NOTIFICATION,static,1342308353
Control10=IDC_GITHUB_REPO_LINK,static,1342308354

[CLS:AboutDialog]
Type=0
HeaderFile=AboutDialog.h
ImplementationFile=AboutDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_STATISTICSDLG (English (U.S.))]
Type=1
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDC_TRAFFIC_GROUP,button,1342177287
Control3=IDC_SENDED_LABEL,static,1342308352
Control4=IDC_SENDED_LABEL2,static,1342308352
Control5=IDC_RECIEVED_LABEL,static,1342308352
Control6=IDC_RECIEVED_LABEL2,static,1342308352
Control7=IDC_TOTAL_LABEL,static,1342308352
Control8=IDC_TOTAL_LABEL2,static,1342308352

[CLS:StatisticsDialog]
Type=0
HeaderFile=StatisticsDialog.h
ImplementationFile=StatisticsDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=StatisticsDialog

[DLG:IDD_PARSER_S (English (U.S.))]
Type=1
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PARSER_GROUP,button,1342177287
Control4=IDC_SHOW_MSGTIME,button,1342242819
Control5=IDC_TIMESTAMPS_POS,combobox,1344340227

[CLS:ParserSettingsDialog]
Type=0
HeaderFile=ParserSettingsDialog.h
ImplementationFile=ParserSettingsDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_SHOW_MSGTIME
VirtualFilter=dWC

[DLG:IDD_INFOMSG (English (U.S.))]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_INFOMSG_TEXT,static,1342308352
Control3=IDC_DONT_SHOW,button,1342242819

[CLS:InfoMessageDialog]
Type=0
HeaderFile=InfoMessageDialog.h
ImplementationFile=InfoMessageDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK


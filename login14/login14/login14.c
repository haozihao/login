/*login2
 首先弹出一个对话框，要求输入用户名和密码，由于没有输入键盘，此次用户名和密码都为空，
然后进入主菜单，此处主菜单未添加具体控件 
2012-5-31     */
/*login3
在login2的基础上，添加输入用户名和密码的软件盘，并实现输入功能。添加头文件“mycoolbar.c” 
注意在主函数中添加InitMiniGUTExt函数，否则输入键盘无法显示。
2012-6-4
*/
/*login4
在login3的基础上，给主菜单添加四个按键，并实现按下“使用说明”时弹出一个对话框，显示使用说明内容。添加头文件“hel"。
2012-6-4
*/
/*login5
在前面的基础上，实现按下"氨氮检测"时弹出一个对话框，并绘制一组正弦曲线，添加头文件“NhBox.c”.
2012-6-7
*/
/*login6
在login5的基础上，在“氨氮检测”对话框下添加“参考光”和“样品光两个按钮，并加入起始波长和终止波长以及数字输入
酷工具栏！
2012-6-10
*/
/*login7
在login6的基础上，在”氨氮检测“对话框下添加坐标背景，绘制正弦曲线或者其他曲线
2012-6-14
*/
/*login8
在前面的基础上，编写步进电机和tlc2543的驱动程序，并将adc值转光强值，并作为纵坐标
绘图
加入斩波器。。。未实现阻塞，一开启就无法关闭。
*/
/*login9
加入三路泵，编写驱动和操作界面，实现开和关...
*/
/*login10
2012-12-10
transplant SQLite and achieve the function of data save.
*/
/*login11
2013-1-3
去掉数据库支持，改用输出TXT格式的文件
去掉之前使用成功的第二线程。
使用minigui自带的定时器不能输出方波，考虑使用内核定时器。
加入内核定时器，用于对卤钨灯斩波,斩波频率10Hz....
*/
/*login12
2013-6-17
修改光谱扫描界面，去掉光谱显示，改为显示三个特征波长的电压值
增加热插拔功能
*/
/*
2-13-8-12
直接从login12升级而来，之前的版本电机扫描方向反了，从现在开始，改变电机的转动方向，电机驱动不需做更改，只需要将电机控制信号的1和3，2和4调换过来即可。
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
//wave头文件
#include <unistd.h>
#include <math.h>
#include <error.h>
#include <pthread.h>
#include <sys/ipc.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <minigui/mgext.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "stepper.h" 
#include "lopper.h"
#include "pump.h"
#include "mycoolbar.c"  //用户名密码输入
#include "help.c"     //使用说明
#include "NhBox.c"    //氨氮检测
#include "SampleBox.c"  //样品管理
#include "LogBox.c" //数据管理

//#include <sqlite3.h>

#define IDC_EUSER 401
#define IDC_EPASS 402
#define IDC_BUTTON_SAMPLE 100
#define IDC_BUTTON_LOG 101
#define IDC_BUTTON_NH  102
#define IDC_BUTTON_HELP 103

#define IDC_BUTTON_HELP_CHD 104
HWND hMainWnd;
HWND hWndTmp;
//sqlite3 *db=NULL;
//char *errmsg=NULL;
//int rc;
//char *sql2 = NULL;

static CTRLDATA CtrlInitData[]=
{
	{
		"static",
		WS_VISIBLE|SS_SIMPLE,
		30,10,240,16,
		IDC_STATIC,
		"请输入用户名和密码",
		0	
	},
	{
		"static",
		WS_VISIBLE|SS_SIMPLE,
		10,40,60,16,
		IDC_STATIC,
		"用户名：",
		0
	},
	{
		"static",
		WS_VISIBLE|SS_SIMPLE,
		10,80,60,16,
		IDC_STATIC,
		"密码：",
		0
	},
	{
		"edit",
		WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP,
		70,40,130,25,
		IDC_EUSER,
		"",
		0
	},
	{
		"edit",
                WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP|ES_PASSWORD,
                70,80,130,25,
                IDC_EPASS,
                "",
                0

	},
	{
		"button",
		WS_VISIBLE|WS_TABSTOP|BS_DEFPUSHBUTTON,
		60,240,80,25,
		IDOK,
		"确定",
		0
	}
};
static DLGTEMPLATE MyDlg =
{
	WS_BORDER | WS_CAPTION,
	WS_EX_NONE,
	10,10,220,300,
	"登录",
	0,0,
	sizeof(CtrlInitData) / sizeof(CTRLDATA),NULL,
	0
};

static char *g_user="";
static char *g_pass="";

static BOOL CheckUser(char *user,char *pass)
{
	if(strcmp(user,g_user)==0)
	{
		if(strcmp(pass,g_pass)==0)
			return(TRUE);
		else
			return(FALSE);
	}

	return(FALSE);	
}
static int MyDlgProc(HWND hDlg,int message,WPARAM wParam,LPARAM lParam)
{
	char user[22];
	char pass[22];
	int xTmp,yTmp;
	int code,id;
	switch (message){
	case MSG_INITDIALOG:
		create_coolbar_num(hDlg);
		create_coolbar_qwe(hDlg);
		create_coolbar_asd(hDlg);
		create_coolbar_zxc(hDlg);
		hWndTmp=GetDlgItem(hDlg,IDC_EUSER);
		return 1;
	
	case MSG_LBUTTONDOWN:
		xTmp=LOWORD(lParam);
		yTmp=HIWORD(lParam);
		if((xTmp>=70) && (xTmp<200))
		{
			if((yTmp>=40)&& (yTmp<65))
				hWndTmp=GetDlgItem(hDlg,IDC_EUSER);
			else if((yTmp>=80)&&(yTmp<105))
				hWndTmp=GetDlgItem(hDlg,IDC_EPASS);		
		}
		break;
	case MSG_COMMAND:
		id=LOWORD(wParam);
		code = HIWORD(wParam);
//		switch(LOWORD(id))
		switch(id)
		{
//			case IDC_EUSER:
//			case IDC_EPASS:
//				if(HIWORD(wParam) !=EN_ENTER)
//					break;
			case IDOK:
				GetWindowText(GetDlgItem(hDlg,IDC_EUSER),user,22);
				GetWindowText(GetDlgItem(hDlg,IDC_EPASS),pass,22);
				if(CheckUser(user,pass))
				{
					EndDialog(hDlg,wParam);
					DestroyAllControls(hDlg);
				}
				else
				{
					MessageBox(hDlg,"用户名或密码错误","输入错误",MB_OKCANCEL|MB_ICONHAND);
				}
				break;
			case IDC_CB_NUM:
				PostMessage(hWndTmp,MSG_CHAR,'0'+code,0);
				break;
			case IDC_CB_QWE:
				PostMessage (hWndTmp, MSG_CHAR, caption2[code][0], 0);
				break;
			case IDC_CB_ASD:
				PostMessage (hWndTmp, MSG_CHAR, caption3[code][0], 0);
				break;
			case IDC_CB_ZXC:
				if(code != 9)
                   		 PostMessage (hWndTmp, MSG_CHAR, caption4[code][0], 0);
				else 
				PostMessage(hWndTmp,MSG_SETTEXT,0,(LPARAM)pStr);
               			break;
			default:
				break;
		}
		break;
	default:
		break;
	}
	return(DefaultDialogProc(hDlg,message,wParam,lParam));
}

static char *hello_str ="欢迎登录红外光谱检测系统";
static int LoginWinProc(HWND hWnd,int message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	int xTmp,yTmp;
	HWND hWndHelp;
	HWND hButton1,hButton2,hButton3,hButton4;

	switch (message){
		case MSG_CREATE:
			hButton1 = CreateWindow (CTRL_BUTTON,
				"样品管理",
				WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
				IDC_BUTTON_SAMPLE,
				50,60,120,30,
				hWnd,
				0);
			hButton2 = CreateWindow (CTRL_BUTTON,
				"数据管理",
				WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
				IDC_BUTTON_LOG,
				50,120,120,30,
				hWnd,
				0);
			hButton3 = CreateWindow (CTRL_BUTTON,
				"光谱检测",
				WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
				IDC_BUTTON_NH,
				50,180,120,30,
				hWnd,
				0);
			hButton4 = CreateWindow (CTRL_BUTTON,
                                "使用说明",     
                                WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
				IDC_BUTTON_HELP,
			        50,240,120,30,
                                hWnd,
                                0);
			break;
		case MSG_PAINT:
			hdc = BeginPaint(hWnd);
			TextOut(hdc,20,20,hello_str);
			EndPaint(hWnd,hdc);
			break;
		case MSG_COMMAND:
			switch(wParam)
			{
				case IDC_BUTTON_HELP:
					HelpBox(hWnd);
					break;
				case IDC_BUTTON_NH:
					NhBox(hWnd);
					break;
				case IDC_BUTTON_SAMPLE:
		 			SampleBox(hWnd);
					break;
				case IDC_BUTTON_LOG:
//		      			SaveNum(hWnd);//用于测试 
					break;
				default:	
					break;
			}
			break;
		case MSG_CLOSE:
			DestroyMainWindow(hWnd);
			PostQuitMessage(hWnd);
			break;
		default:
			return(DefaultMainWinProc(hWnd,message,wParam,lParam));
	}
	return 0;			
}

void* MainWindowThread(void* data)
{
	MSG Msg;
	HWND hMainWnd;
	MAINWINCREATE CreateInfo;
	
    	CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    	CreateInfo.dwExStyle = WS_EX_NONE;
   	CreateInfo.spCaption = "红外测油仪";
    	CreateInfo.hMenu = 0;
    	CreateInfo.hCursor = GetSystemCursor(0);
    	CreateInfo.hIcon = 0;
    	CreateInfo.MainWindowProc = LoginWinProc;
    	CreateInfo.lx = 0;
    	CreateInfo.ty = 0;
    	CreateInfo.rx = 240;
    	CreateInfo.by = 320;
    	CreateInfo.iBkColor = COLOR_lightwhite;
    	CreateInfo.dwAddData = 0;
    	CreateInfo.hHosting = HWND_DESKTOP;
    
   	hMainWnd = CreateMainWindow (&CreateInfo);
    
    	if (hMainWnd == HWND_INVALID)
        	return NULL;

    	ShowWindow(hMainWnd, SW_SHOWNORMAL);

    	while (GetMessage(&Msg, hMainWnd)) {
        	TranslateMessage(&Msg);
        	DispatchMessage(&Msg);
    	}

    	MainWindowThreadCleanup (hMainWnd);

    	return NULL;
}
int MiniGUIMain(int argc,const char *argv[])
{
        pid_t p_database;
        pthread_t thread;
        if (!InitMiniGUIExt()) {
            return 2;
        }   // 若没有这一if语句，则酷工具栏无法显示
//用户名和密码显示窗口
	MyDlg.controls=CtrlInitData;
	DialogBoxIndirectParam(&MyDlg,HWND_DESKTOP,MyDlgProc,0L);
        
	CreateThreadForMainWindow(&thread,NULL,MainWindowThread,0);//主线程
        return 0;
}

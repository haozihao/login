/*login2
 ���ȵ���һ���Ի���Ҫ�������û��������룬����û��������̣��˴��û��������붼Ϊ�գ�
Ȼ��������˵����˴����˵�δ��Ӿ���ؼ� 
2012-5-31     */
/*login3
��login2�Ļ����ϣ���������û��������������̣���ʵ�����빦�ܡ����ͷ�ļ���mycoolbar.c�� 
ע���������������InitMiniGUTExt������������������޷���ʾ��
2012-6-4
*/
/*login4
��login3�Ļ����ϣ������˵�����ĸ���������ʵ�ְ��¡�ʹ��˵����ʱ����һ���Ի�����ʾʹ��˵�����ݡ����ͷ�ļ���hel"��
2012-6-4
*/
/*login5
��ǰ��Ļ����ϣ�ʵ�ְ���"�������"ʱ����һ���Ի��򣬲�����һ���������ߣ����ͷ�ļ���NhBox.c��.
2012-6-7
*/
/*login6
��login5�Ļ����ϣ��ڡ�������⡱�Ի�������ӡ��ο��⡱�͡���Ʒ��������ť����������ʼ��������ֹ�����Լ���������
�Ṥ������
2012-6-10
*/
/*login7
��login6�Ļ����ϣ��ڡ�������⡰�Ի�����������걳���������������߻�����������
2012-6-14
*/
/*login8
��ǰ��Ļ����ϣ���д���������tlc2543���������򣬲���adcֵת��ǿֵ������Ϊ������
��ͼ
����ն����������δʵ��������һ�������޷��رա�
*/
/*login9
������·�ã���д�����Ͳ������棬ʵ�ֿ��͹�...
*/
/*login10
2012-12-10
transplant SQLite and achieve the function of data save.
*/
/*login11
2013-1-3
ȥ�����ݿ�֧�֣��������TXT��ʽ���ļ�
ȥ��֮ǰʹ�óɹ��ĵڶ��̡߳�
ʹ��minigui�Դ��Ķ�ʱ�������������������ʹ���ں˶�ʱ����
�����ں˶�ʱ�������ڶ�±�ٵ�ն��,ն��Ƶ��10Hz....
*/
/*login12
2013-6-17
�޸Ĺ���ɨ����棬ȥ��������ʾ����Ϊ��ʾ�������������ĵ�ѹֵ
�����Ȳ�ι���
*/
/*
2-13-8-12
ֱ�Ӵ�login12����������֮ǰ�İ汾���ɨ�跽���ˣ������ڿ�ʼ���ı�����ת�����򣬵���������������ģ�ֻ��Ҫ����������źŵ�1��3��2��4�����������ɡ�
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
//waveͷ�ļ�
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
#include "mycoolbar.c"  //�û�����������
#include "help.c"     //ʹ��˵��
#include "NhBox.c"    //�������
#include "SampleBox.c"  //��Ʒ����
#include "LogBox.c" //���ݹ���

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
		"�������û���������",
		0	
	},
	{
		"static",
		WS_VISIBLE|SS_SIMPLE,
		10,40,60,16,
		IDC_STATIC,
		"�û�����",
		0
	},
	{
		"static",
		WS_VISIBLE|SS_SIMPLE,
		10,80,60,16,
		IDC_STATIC,
		"���룺",
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
		"ȷ��",
		0
	}
};
static DLGTEMPLATE MyDlg =
{
	WS_BORDER | WS_CAPTION,
	WS_EX_NONE,
	10,10,220,300,
	"��¼",
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
					MessageBox(hDlg,"�û������������","�������",MB_OKCANCEL|MB_ICONHAND);
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

static char *hello_str ="��ӭ��¼������׼��ϵͳ";
static int LoginWinProc(HWND hWnd,int message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	int xTmp,yTmp;
	HWND hWndHelp;
	HWND hButton1,hButton2,hButton3,hButton4;

	switch (message){
		case MSG_CREATE:
			hButton1 = CreateWindow (CTRL_BUTTON,
				"��Ʒ����",
				WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
				IDC_BUTTON_SAMPLE,
				50,60,120,30,
				hWnd,
				0);
			hButton2 = CreateWindow (CTRL_BUTTON,
				"���ݹ���",
				WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
				IDC_BUTTON_LOG,
				50,120,120,30,
				hWnd,
				0);
			hButton3 = CreateWindow (CTRL_BUTTON,
				"���׼��",
				WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
				IDC_BUTTON_NH,
				50,180,120,30,
				hWnd,
				0);
			hButton4 = CreateWindow (CTRL_BUTTON,
                                "ʹ��˵��",     
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
//		      			SaveNum(hWnd);//���ڲ��� 
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
   	CreateInfo.spCaption = "���������";
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
        }   // ��û����һif��䣬��Ṥ�����޷���ʾ
//�û�����������ʾ����
	MyDlg.controls=CtrlInitData;
	DialogBoxIndirectParam(&MyDlg,HWND_DESKTOP,MyDlgProc,0L);
        
	CreateThreadForMainWindow(&thread,NULL,MainWindowThread,0);//���߳�
        return 0;
}

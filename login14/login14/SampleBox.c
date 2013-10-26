#define IDC_PUMP1     121    // 样品
#define IDC_PUMP1_ON  122
#define IDC_PUMP1_OFF 123
#define IDC_PUMP2     124     //指示液
#define IDC_PUMP2_ON  125
#define IDC_PUMP2_OFF 126
#define IDC_PUMP3     127      //清水冲洗
#define IDC_PUMP3_ON  128
#define IDC_PUMP3_OFF 129
#define IDC_PUMP4     130      //排液泵
#define IDC_PUMP4_ON  131
#define IDC_PUMP4_OFF 132
int state1,state2,state3,state4,state5,state6,state7,state8;  //四路蠕动泵的八个状态
static CTRLDATA CtrlSampleFiles[] =
{
	{
		CTRL_STATIC,
		WS_VISIBLE|SS_GROUPBOX|BS_CHECKED,
		20,20,80,90,
		IDC_PUMP1,
		"待测液泵",
		0
	},
	{
		CTRL_BUTTON,
		BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
		25,40,55,30,
		IDC_PUMP1_ON,
		"开",
		0		
	},
	{
		CTRL_BUTTON,
		BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
		25,75,55,30,
		IDC_PUMP1_OFF,
		"关",
		0
	},

	{
		CTRL_STATIC,
		WS_VISIBLE|BS_CHECKED|SS_GROUPBOX,
		120,20,80,90,
		IDC_PUMP2,
		"指示液泵",
		0
	},
	{
		CTRL_BUTTON,
		BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
		125,40,55,30,
		IDC_PUMP2_ON,
		"开",
		0		
	},
	{
		CTRL_BUTTON,
		BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
		125,75,55,30,
		IDC_PUMP2_OFF,
		"关",
		0
	},
	 {
                CTRL_STATIC,
                WS_VISIBLE|BS_CHECKED|SS_GROUPBOX,
                20,120,80,90,
                IDC_PUMP3,
                "清水泵",
                0
        },
        {
                CTRL_BUTTON,
                BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
                25,140,55,30,
                IDC_PUMP3_ON,
                "开",
                0
        },
        {
                CTRL_BUTTON,
                BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
                25,175,55,30,
                IDC_PUMP3_OFF,
                "关",
                0
        },
	{
                CTRL_STATIC,
                WS_VISIBLE|BS_CHECKED|SS_GROUPBOX,
                120,120,80,90,
                IDC_PUMP4,
                "排液泵",
                0
        },
	{
                CTRL_BUTTON,
                BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
                125,140,55,30,
                IDC_PUMP4_ON,
                "开",
                0
        },
	{
                CTRL_BUTTON,
                BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
                125,175,55,30,
                IDC_PUMP4_OFF,
                "关",
                0
        },

 /*	{
                CTRL_BUTTON,
                WS_VISIBLE|BS_DEFPUSHBUTTON,
                40,220,60,30,
                IDOK,
                "确定",
                0
        },  */
        {
                CTRL_BUTTON,
                WS_VISIBLE|BS_DEFPUSHBUTTON,
                80,215,70,30,
                IDCANCEL,
                "返回",
                0
        },

};
static DLGTEMPLATE DlgSampleFiles =
{
	WS_BORDER|WS_CAPTION,
	WS_EX_NONE,
	20,20,220,280,
	"样品管理",
	0,0,
	sizeof(CtrlSampleFiles)/sizeof(CTRLDATA),NULL,
	0
};

static int SampleFileProc(HWND hDlg,int message,WPARAM wParam,LPARAM lParam)
{
	char buff[8];
	int i = 0;
	 int fdPump;
//打开蠕动泵
        fdPump=open("/dev/pump",0);
        if(fdPump<0)
        {
                perror("open pump device");
                exit(1);
        }

	switch(message)
	{
		case MSG_INITDIALOG:
			return 1;
		case MSG_COMMAND:
			switch(wParam)
			{
				case IDC_PUMP1_ON:
				     	ioctl(fdPump,  MEMDEV1_ON, 0);
					MessageBox(hDlg,"正在加入待测液，请稍后...","提示",MB_OKCANCEL);
					break;
				case IDC_PUMP1_OFF:
					ioctl(fdPump,  MEMDEV1_OFF, 0);
					MessageBox(hDlg,"待测液滴加完毕，请进行其他操作！","提示",MB_OKCANCEL);
				     	break;
				case IDC_PUMP2_ON:
					ioctl(fdPump,  MEMDEV2_ON, 0);
					MessageBox(hDlg,"正在加入指示液，请稍后...","提示",MB_OKCANCEL);
					break;
                                case IDC_PUMP2_OFF:
					ioctl(fdPump,  MEMDEV2_OFF, 0);
					MessageBox(hDlg,"指示液滴加完毕，请进行其他操作！","提示",MB_OKCANCEL);
					break;
				case IDC_PUMP3_ON:
                                        ioctl(fdPump,  MEMDEV3_ON, 0);
                                        MessageBox(hDlg,"正在冲洗，请稍后...","提示",MB_OKCANCEL);
                                        break;
                                case IDC_PUMP3_OFF:
                                        ioctl(fdPump,  MEMDEV3_OFF, 0);
                                        MessageBox(hDlg,"冲洗完毕，请进行其他操作！","提示",MB_OKCANCEL);
                                        break;
				case IDC_PUMP4_ON:
                                        ioctl(fdPump,  MEMDEV4_ON, 0);
                                        MessageBox(hDlg,"正在排除废液，请稍后...","提示",MB_OKCANCEL);
                                        break;
                                case IDC_PUMP4_OFF:
                                        ioctl(fdPump,  MEMDEV4_OFF, 0);
                                        MessageBox(hDlg,"废液排除完毕，请进行其他操作！","提示",MB_OKCANCEL);
                                        break;
				case IDCANCEL:
					EndDialog(hDlg,wParam);
					DestroyAllControls(hDlg);
					break;
				default:
					break;
		}
		break;
	default:
                break;
	
	}
	close(fdPump);
	return DefaultDialogProc(hDlg,message,wParam,lParam);
}
static void SampleBox(HWND hWnd)
{
	DlgSampleFiles.controls = CtrlSampleFiles;
	DialogBoxIndirectParam(&DlgSampleFiles,hWnd,SampleFileProc,0L);
//	return 0;
}

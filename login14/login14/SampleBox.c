#define IDC_PUMP1     121    // ��Ʒ
#define IDC_PUMP1_ON  122
#define IDC_PUMP1_OFF 123
#define IDC_PUMP2     124     //ָʾҺ
#define IDC_PUMP2_ON  125
#define IDC_PUMP2_OFF 126
#define IDC_PUMP3     127      //��ˮ��ϴ
#define IDC_PUMP3_ON  128
#define IDC_PUMP3_OFF 129
#define IDC_PUMP4     130      //��Һ��
#define IDC_PUMP4_ON  131
#define IDC_PUMP4_OFF 132
int state1,state2,state3,state4,state5,state6,state7,state8;  //��·�䶯�õİ˸�״̬
static CTRLDATA CtrlSampleFiles[] =
{
	{
		CTRL_STATIC,
		WS_VISIBLE|SS_GROUPBOX|BS_CHECKED,
		20,20,80,90,
		IDC_PUMP1,
		"����Һ��",
		0
	},
	{
		CTRL_BUTTON,
		BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
		25,40,55,30,
		IDC_PUMP1_ON,
		"��",
		0		
	},
	{
		CTRL_BUTTON,
		BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
		25,75,55,30,
		IDC_PUMP1_OFF,
		"��",
		0
	},

	{
		CTRL_STATIC,
		WS_VISIBLE|BS_CHECKED|SS_GROUPBOX,
		120,20,80,90,
		IDC_PUMP2,
		"ָʾҺ��",
		0
	},
	{
		CTRL_BUTTON,
		BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
		125,40,55,30,
		IDC_PUMP2_ON,
		"��",
		0		
	},
	{
		CTRL_BUTTON,
		BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
		125,75,55,30,
		IDC_PUMP2_OFF,
		"��",
		0
	},
	 {
                CTRL_STATIC,
                WS_VISIBLE|BS_CHECKED|SS_GROUPBOX,
                20,120,80,90,
                IDC_PUMP3,
                "��ˮ��",
                0
        },
        {
                CTRL_BUTTON,
                BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
                25,140,55,30,
                IDC_PUMP3_ON,
                "��",
                0
        },
        {
                CTRL_BUTTON,
                BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
                25,175,55,30,
                IDC_PUMP3_OFF,
                "��",
                0
        },
	{
                CTRL_STATIC,
                WS_VISIBLE|BS_CHECKED|SS_GROUPBOX,
                120,120,80,90,
                IDC_PUMP4,
                "��Һ��",
                0
        },
	{
                CTRL_BUTTON,
                BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
                125,140,55,30,
                IDC_PUMP4_ON,
                "��",
                0
        },
	{
                CTRL_BUTTON,
                BS_NOTIFY| WS_VISIBLE|BS_PUSHBUTTON|BS_CHECKED,
                125,175,55,30,
                IDC_PUMP4_OFF,
                "��",
                0
        },

 /*	{
                CTRL_BUTTON,
                WS_VISIBLE|BS_DEFPUSHBUTTON,
                40,220,60,30,
                IDOK,
                "ȷ��",
                0
        },  */
        {
                CTRL_BUTTON,
                WS_VISIBLE|BS_DEFPUSHBUTTON,
                80,215,70,30,
                IDCANCEL,
                "����",
                0
        },

};
static DLGTEMPLATE DlgSampleFiles =
{
	WS_BORDER|WS_CAPTION,
	WS_EX_NONE,
	20,20,220,280,
	"��Ʒ����",
	0,0,
	sizeof(CtrlSampleFiles)/sizeof(CTRLDATA),NULL,
	0
};

static int SampleFileProc(HWND hDlg,int message,WPARAM wParam,LPARAM lParam)
{
	char buff[8];
	int i = 0;
	 int fdPump;
//���䶯��
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
					MessageBox(hDlg,"���ڼ������Һ�����Ժ�...","��ʾ",MB_OKCANCEL);
					break;
				case IDC_PUMP1_OFF:
					ioctl(fdPump,  MEMDEV1_OFF, 0);
					MessageBox(hDlg,"����Һ�μ���ϣ����������������","��ʾ",MB_OKCANCEL);
				     	break;
				case IDC_PUMP2_ON:
					ioctl(fdPump,  MEMDEV2_ON, 0);
					MessageBox(hDlg,"���ڼ���ָʾҺ�����Ժ�...","��ʾ",MB_OKCANCEL);
					break;
                                case IDC_PUMP2_OFF:
					ioctl(fdPump,  MEMDEV2_OFF, 0);
					MessageBox(hDlg,"ָʾҺ�μ���ϣ����������������","��ʾ",MB_OKCANCEL);
					break;
				case IDC_PUMP3_ON:
                                        ioctl(fdPump,  MEMDEV3_ON, 0);
                                        MessageBox(hDlg,"���ڳ�ϴ�����Ժ�...","��ʾ",MB_OKCANCEL);
                                        break;
                                case IDC_PUMP3_OFF:
                                        ioctl(fdPump,  MEMDEV3_OFF, 0);
                                        MessageBox(hDlg,"��ϴ��ϣ����������������","��ʾ",MB_OKCANCEL);
                                        break;
				case IDC_PUMP4_ON:
                                        ioctl(fdPump,  MEMDEV4_ON, 0);
                                        MessageBox(hDlg,"�����ų���Һ�����Ժ�...","��ʾ",MB_OKCANCEL);
                                        break;
                                case IDC_PUMP4_OFF:
                                        ioctl(fdPump,  MEMDEV4_OFF, 0);
                                        MessageBox(hDlg,"��Һ�ų���ϣ����������������","��ʾ",MB_OKCANCEL);
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

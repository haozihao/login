#define IDC_SET_USER   120
static CTRLDATA CtrlSetFiles[] =
{
	{
		CTRL_BUTTON,
		WS_VISIBLE|BS_DEFPUSHBUTTON,
		50,50,120,30,
		IDC_SET_USER,
		"用户管理",
		0		
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE|BS_DEFPUSHBUTTON,
		80,220,60,30,
		IDCANCEL,
		"返回",
		0
	},
};
static DLGTEMPLATE DlgSetFiles =
{
	WS_BORDER|WS_CAPTION,
	WS_EX_NONE,
	20,20,220,280,
	"系统设置",
	0,0,
	sizeof(CtrlSetFiles)/sizeof(CTRLDATA),NULL,
	0
};

static int SetFileProc(HWND hDlg,int message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case MSG_INITDIALOG:
			return 1;
		case MSG_COMMAND:
			switch(wParam)
				case IDCANCEL:
					EndDialog(hDlg,wParam);
					DestroyAllControls(hDlg);
					break;
				default:
					break;
			break;
	}
	return DefaultDialogProc(hDlg,message,wParam,lParam);
}
static void SetBox(HWND hWnd)
{
	DlgSetFiles.controls = CtrlSetFiles;
	DialogBoxIndirectParam(&DlgSetFiles,hWnd,SetFileProc,0L);
//	return 0;
}


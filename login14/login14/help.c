/*help file*/
static CTRLDATA CtrlHelpFiles[] =
{
	{
		CTRL_STATIC,
		WS_VISIBLE|SS_LEFT,
		30,30,170,150,
		IDC_STATIC,
		"Welcome to use this system.If you have any different in using it,you can read this text,and maybe you can find the solution!Now let's go",
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
static DLGTEMPLATE DlgHelpFiles =
{
	WS_BORDER|WS_CAPTION,
	WS_EX_NONE,
	20,20,220,280,
	"使用说明",
	0,0,
	sizeof(CtrlHelpFiles)/sizeof(CTRLDATA),NULL,
	0
};

static int HelpFileProc(HWND hDlg,int message,WPARAM wParam,LPARAM lParam)
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
static void HelpBox(HWND hWnd)
{
	DlgHelpFiles.controls = CtrlHelpFiles;
	DialogBoxIndirectParam(&DlgHelpFiles,hWnd,HelpFileProc,0L);
//	return 0;
}


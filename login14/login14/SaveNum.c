//#include <sqlite3.h>
#define IDC_FILENAME  119
HWND hWndTmp;
/*********************************************************
sqlite3 *db1=NULL;
char *errmsg1=NULL;
int rc1;
char *sql3=NULL;   //用于更改表名
*********************************************************/
static CTRLDATA CtrlSaveFiles[] = 
{
	{
		"static",
		WS_VISIBLE|SS_SIMPLE,
		30,20,240,16,
		IDC_STATIC,
		"文件另存为",
		0	
	},
	{
		"edit",
                WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP,
                50,50,130,25,
                IDC_FILENAME,
                "",
                0
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE|WS_TABSTOP|BS_DEFPUSHBUTTON,
		60,240,45,30,
		IDOK,
		"确定",
		0
	},
	{
		CTRL_BUTTON,
		WS_VISIBLE|WS_TABSTOP|BS_DEFPUSHBUTTON,
		130,240,45,30,
		IDCANCEL,
		"取消",
		0
	}
};
static DLGTEMPLATE DlgSaveFiles =
{
        WS_BORDER | WS_CAPTION,
        WS_EX_NONE,
        10,10,220,300,
	"文件保存",
	0,0,
        sizeof(CtrlSaveFiles) / sizeof(CTRLDATA),NULL,
        0
};

static int SaveFileProc(HWND hDlg,int message,WPARAM wParam,LPARAM lParam)
{
	char pathname[30]={0};
	char name[20];
	int xTmp,yTmp;
	int code,id;
	pid_t p_ret;
	switch (message){
	case MSG_INITDIALOG:
		create_coolbar_num(hDlg);
		create_coolbar_qwe(hDlg);
		create_coolbar_asd(hDlg);
		create_coolbar_zxc(hDlg);
		hWndTmp=GetDlgItem(hDlg,IDC_FILENAME);
/*		p_ret=vfork();
		if(p_ret==-1)
		{
			perror("vfork()");
			exit(0);
		}
		else if(p_ret==0) //表示子进程
		{
			printf("this is child. pid of child is: %d\n",getpid());
			if(execlp("mount","mount","/dev/sda[a-z][0-9]","/mnt/udisk",NULL)<0)
				perror("execlp mount");
			printf("mount success!");
		}
		else 
		{
			printf("this is father.pid of child is: %d\n",p_ret);
			waitpid(p_ret,NULL,0);
			printf("mount child is over\n");
		}  */
		return 1;
	case MSG_LBUTTONDOWN:
                xTmp=LOWORD(lParam);
                yTmp=HIWORD(lParam);
                if((xTmp>=50) && (xTmp<180))
                {
                        if((yTmp>=50)&& (yTmp<75))
                                hWndTmp=GetDlgItem(hDlg,IDC_FILENAME);
                }
                break;
	case MSG_COMMAND:
		id=LOWORD(wParam);
		code=HIWORD(wParam);
		switch(id)
		{
			case IDC_FILENAME:
				if(HIWORD(wParam) !=EN_ENTER)
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
                        case IDOK:
                                GetWindowText(GetDlgItem(hDlg,IDC_FILENAME),name,30);
				strcpy(pathname,"/mnt/udisk/");
				strcat(pathname,name);
				strcat(pathname,".txt");
/*                                if(strlen(name)==0)
				{
					MessageBox(hDlg,"文件名不能为空","输入有误",MB_OK|MB_ICONHAND); 
					break;
				}   */
		//		sprintf(name,%f.txt,);
/*********************************************************************************
				rc1 = sqlite3_open("login.db",&db1);
				if(rc1){
					fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db1));
				}
				sql3 = sqlite3_mprintf("alter table login_table rename to %s",name);
				sqlite3_exec(db1,sql3,0,0,&errmsg1);
				sqlite3_free(sql3);	
				sqlite3_close(db1);
//				rename("login.db","/mnt/udisk/login.db");
********************************************************************************/
				 
				 
				p_ret=vfork();
                                if(p_ret == -1)
                                {
                                        perror("vfork()");
                                        exit(0);
                                }
                                else if(p_ret == 0)
                                {
				//	if(execlp("cp","cp","/tmp/login.txt","/mnt/udisk/",NULL)<0)
                                        if(execlp("cp","cp","/tmp/login.txt",pathname,NULL)<0)
                                        	perror("excelp mv login.db");
                                        
                                }
                                else
                                {
                                        waitpid(p_ret,NULL,0);
                                        MessageBox(hDlg,"保存成功","文件保存",MB_OK);
                                }
                                sync();   //一定要执行这一步，否则热插拔会出问题
				/*执行umount命令卸载U盘*/
/*				p_ret=vfork();
				if(p_ret==-1)
				{
					perror("vfork()");
					exit(0);
				}
				else if(p_ret==0) //表示子进程
				{
					printf("this is child.pid of child is: %d\n",getpid());
					if(execlp("umount","umount","/mnt/udisk",NULL)<0)
						perror("execlp mount");
					printf("umount success!");
				}
				else 
				{
					printf("this is father.pid of child is: %d\n",p_ret);
					waitpid(p_ret,NULL,0);
					printf("umount child is over\n");
				}
*/
				EndDialog(hDlg,wParam);
                                DestroyAllControls(hDlg);
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
	return DefaultDialogProc(hDlg,message,wParam,lParam);
}
static void SaveNum(HWND hWnd)
{
        DlgSaveFiles.controls = CtrlSaveFiles;
        DialogBoxIndirectParam(&DlgSaveFiles,hWnd,SaveFileProc,0L);
//      return 0;
}

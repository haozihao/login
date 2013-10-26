/*将wave_data改成二维数组，运行NhPaint十次 
修改校准起始点为3300 2013-8-14*/
#include "SaveNum.c"
#define IDC_CANKAO   113
#define IDC_YANGPING 114
#define IDC_START    118
#define IDC_SAVE     119
#define IDC_WAVEONE  210  //用于显示2930波数电压结果
#define IDC_WAVETWO  211  //用于显示2960波数电压结果
#define IDC_WAVETHREE  212  //用于显示3030波数电压结果
#define IDC_RESULT     213  //用于显示浓度值

#define X0  2             //坐标基准点，X0-Y0对应左上角起始点，X1-Y1对应长和宽
#define Y0 30
#define X1 130
#define Y1 150

#define VOL_REF  5350  //AD参考电压
#define WAVE_INTERVAL 1 //相邻采样点的波长间隔

#define WAVEORIGIN  3000    //扫描基准点
#define WAVESTART  3400     //开始扫描点
#define WAVEEND    3600     //结束扫描点
#define TIMES   10      //连续扫描次数

#define WAVEONE  212    //2930波数对应3413nm，起始扫描点是3201
#define WAVETWO  177   //2960波数对应3378nm，起始扫描点是3201
#define WAVETHREE  100    //3030波数对应3300nm，起始扫描点是3201

static char WaveOneNum[10];  //保存三个特定波长的电压值
static char WaveTwoNum[10];
static char WaveThreeNum[10];

int WaveCalibra=1; //初始化步进电机，将其运转到起始位置
int WaveStepNow; //步进电机走的步数，起始波长处为0步

static char ad_tx_buf[]={0xac,0xac};
static char ad_rx_buf[]={'0','0'};
static int ad_rx_data;    //用于保存AD值
static float ad_v_data;  //用于保存电压值
static int WaveNow=WAVEORIGIN;
static const char *pStr1 = ".";

int fdLopper;  //斩波器驱动
/*********************数据保存*******************/
#define WAVE_LENGTH  (WAVEEND-WAVESTART+10)     //波长范围存储单元
struct wave_struct
{
        int wave;
        float light;
};
struct wave_struct wave_data[TIMES][WAVE_LENGTH];  //暂存电压数据
//struct wave_struct wave_save[TIMES][WAVE_LENGTH];   //最终电压数据
static int wave_index=0;   //数据保存的游标
/**************************************************************************************/
static CTRLDATA CtrlNhFiles[] =
{
	{

		CTRL_STATIC,
		WS_VISIBLE|SS_GROUPBOX|BS_CHECKED,
		X0,Y0,X1,Y1,
		IDC_STATIC,
		"检测结果",
		0		
	},
	{

                CTRL_STATIC,
                WS_VISIBLE|BS_CHECKED,
                X0+8,Y0+30,X1-90,Y1-120,
                IDC_STATIC,
                "A2930",
                0
        },
	{

                CTRL_SLEDIT,
                WS_VISIBLE|WS_BORDER|SS_BLACKFRAME,
	        X0+53,Y0+30,X1-70,Y1-130,
                IDC_WAVEONE,
                "",
                0
        },

	{

                CTRL_STATIC,
                WS_VISIBLE|BS_CHECKED,
                X0+8,Y0+65,X1-70,Y1-130,
                IDC_STATIC,
                "A2960",
                0
 	},
	{

                CTRL_SLEDIT,
		WS_VISIBLE|WS_BORDER|SS_BLACKFRAME,
                X0+53,Y0+65,X1-70,Y1-130,
                IDC_WAVETWO,
                "",
                0
        },
	{

                CTRL_STATIC,
                WS_VISIBLE|BS_CHECKED,
                X0+8,Y0+100,X1-90,Y1-120,
                IDC_STATIC,
                "A3030",
                0
        },
	{

                CTRL_SLEDIT,
               	WS_VISIBLE|WS_BORDER|SS_BLACKFRAME,
                X0+53,Y0+100,X1-70,Y1-130,
                IDC_WAVETHREE,
                "",
                0
        },
	{

                CTRL_STATIC,
                WS_VISIBLE|SS_GROUPBOX|BS_CHECKED,
                X0+133,Y0,X1-30,Y1,
                IDC_STATIC,
                "浓度",
                0
        },
	{

                CTRL_STATIC,
                WS_VISIBLE,
                X0+138,Y0+30,X1-45,Y1-130,
                IDC_STATIC,
                "当前浓度为：",
                0
        },
	{

                CTRL_SLEDIT,
                WS_VISIBLE|WS_BORDER|SS_BLACKFRAME,
                X0+143,Y0+58,X1-80,Y1-125,
                IDC_RESULT,
                "",
                0
        },
	{

                CTRL_STATIC,
                WS_VISIBLE,
                X0+196,Y0+60,X1-90,Y1-125,
                IDC_STATIC,
                "mg/L",
                0
        },
	{
                CTRL_BUTTON,
                WS_VISIBLE|BS_DEFPUSHBUTTON,
                X0+143,Y0+100,X1-50,Y1-120,
                IDC_SAVE,
                "保存结果",
                0
        },

	{
		CTRL_BUTTON,
		WS_VISIBLE|BS_DEFPUSHBUTTON,
		40,260,60,30,
		IDC_START,
		"确定",
		0
	},	
	{
		CTRL_BUTTON,
		WS_VISIBLE|BS_DEFPUSHBUTTON,
		120,260,60,30,
		IDCANCEL,
		"返回",
		0
	},
};
static DLGTEMPLATE DlgNhFiles =
{
	WS_BORDER|WS_CAPTION,
	WS_EX_NONE,
	0,0,240,320,
	"浓度检测",
	0,0,
	sizeof(CtrlNhFiles)/sizeof(CTRLDATA),NULL,
	0
};
//初始化光谱仪，使其到达起始扫描点
void WaveCalibrate(HWND hWnd)
{
//        int tmpWaveLen = WAVESTART-WAVEORIGIN;
        int fdStepper;
        fdStepper=open("/dev/stepper",0);
        if(fdStepper<0)
        {
                perror("open stepper device");
                exit(1);
        }
        ioctl(fdStepper,STEPPER_IOCSTA,0);
        read(fdStepper,NULL,0);
        printf("Stepper reach %d nm!\n",WAVEORIGIN);  //到达基准点
        //打开斩波器
/*      fdLopper=open("/dev/lopper",0);
        if(fdLopper<0)
        {
             perror("open lopper device");
             exit(1);
        }
        ioctl(fdLopper,LOPPER_ON,0);  //斩波器开始工作,预热卤钨灯。
	ioctl(fdStepper,STEPPER_IOCFWD,tmpWaveLen);
        read(fdStepper,NULL,0);
	printf("Stepper reach %d nm!\n",WAVESTART);   //到达开始采样的起始点
	ioctl(fdLopper,LOPPER_OFF,0);  //斩波器停止工作 */
        close(fdStepper);  
//	close(fdLopper);
}

/*将wave_data数据写入指定文件*/
void write_to_file(void)
{
	FILE *fp_bin;
	FILE *fp_str;
	int i,j;
	size_t length;
	printf("wave_index:%d\n",wave_index);
/*	for(j=0;j<wave_index;j++)
        {
                for(i=1;i<TIMES;i++)
                {
                        wave_data[0][j].light += wave_data[i][j].light;

                }
                wave_data[0][j].light = wave_data[0][j].light/TIMES;
	}
	fp_bin = fopen("/tmp/login_bin.txt","w+b");
	if(fp_bin == NULL)
		perror("fopen login_bin.txt error");
	length = fwrite(wave_data[0],sizeof(wave_data[0][0]),sizeof(wave_data[0])/sizeof(wave_data[0][0]),fp_bin);
	if(length != sizeof(wave_data[0])/sizeof(wave_data[0][0]))
		perror("fwrite error");
	fclose(fp_bin);
	printf("Complete the bin file write!\n");   */
	fp_str = fopen("/tmp/login.txt","w+b");
	if(fp_str == NULL)
                perror("fopen login.txt error");
	for(j=0;j<wave_index;j++)
	{
		for(i=6;i<TIMES;i++)
		{
			wave_data[0][j].light += wave_data[i][j].light*(i-6+1)*0.1; //7,8,9,10次扫描权重为1：2：3：4。	
		}
	//	wave_data[0][j].light = (wave_data[0][j].light)/(TIMES-6);
		/* 格式化输出参考《UNIX环境高级编程》121页 */
		fprintf(fp_str,"%-10d",wave_data[0][j].wave);  
		fprintf(fp_str,"%-10.2f\n",wave_data[0][j].light);
	}
	fclose(fp_str);
	printf("Complete the str file write!\n");
}
int NHPaint(HWND hWnd){
	int fdStepper,fdADC;
	char buff[8];
	int WaveStart,WaveEnd;
	int WaveNext;
	int WaveStepLen;
	int WaveStepNum; //步进电机要走的步数
	float light_rx;
	int i;
	HDC hdc;

	WaveStart=WAVESTART;WaveEnd=WAVEEND;  
	printf("Start Wave is :%d\n",WaveStart);
	printf("End Wave is :%d\n",WaveEnd);

	//打开步进电机设备文件
	fdStepper=open("/dev/stepper",0);
	if(fdStepper<0)
	{
		perror("open stepper device");
		exit(1);	
	}
	//打开ADC设备文件
	fdADC=open("/dev/mini2440_spi0",O_RDWR);
	if(fdADC<0)
	{
		perror("open spi device");
		exit(1);	
	}
        //打开斩波器
        fdLopper=open("/dev/lopper",0);
        if(fdLopper<0)
        {
             perror("open lopper device");
             exit(1);
        }
        ioctl(fdLopper,LOPPER_ON,0);  //斩波器开始工作
	for(i=0;i<TIMES;i++)
	{	
		wave_index = 0;
		printf("WaveNow=%d\n",WaveNow);
		if(WaveNow!=WaveStart)
		{
		if(WaveNow>WaveStart)
		{
			WaveStepLen=WaveNow-WaveStart;
			ioctl(fdStepper,STEPPER_IOCBACK,WaveStepLen);
			read(fdStepper,NULL,0);	
		}
		if(WaveNow<WaveStart)
		{
			WaveStepLen=WaveStart-WaveNow;
			ioctl(fdStepper,STEPPER_IOCFWD,WaveStepLen);
			read(fdStepper,NULL,0);	
		}
		WaveNow=WaveStart;
		WaveStepNow = 0;	
		}
		write(fdADC,ad_tx_buf,sizeof(ad_tx_buf)/sizeof(ad_tx_buf[0]));
		read(fdADC,ad_rx_buf,sizeof(ad_rx_buf)/sizeof(ad_rx_buf[0]));
	
		while(WaveNow < WaveEnd)
		{
		WaveNext=WaveNow+WAVE_INTERVAL; //下一次采样点的波长位置
		WaveStepLen=WaveNext-WaveStart; //波长间隔
		WaveStepNum=WaveStepLen-WaveStepNow; //步进电机从当前位置到下一位置要走的步数
		ioctl(fdStepper,STEPPER_IOCFWD,WaveStepNum);
		read(fdStepper,NULL,0);

		WaveNow = WaveNext;
		WaveStepNow=WaveStepLen;
		
		write(fdADC,ad_tx_buf,sizeof(ad_tx_buf)/sizeof(ad_tx_buf[0]));
		read(fdADC,ad_rx_buf,sizeof(ad_rx_buf)/sizeof(ad_rx_buf[0]));
		ad_rx_data=((int)ad_rx_buf[0])<<8;
		ad_rx_data|=ad_rx_buf[1]; //得到AD值
		ad_v_data = (float)ad_rx_data*VOL_REF/4096.0; //得到电压，用于存储
		light_rx = ad_v_data/(0.0138*WaveNow-3.95); //?????得到光强值，单位nw
		ad_rx_data = (int)light_rx;
		printf("WaveNow = %d,ad_v_data = %.2f \n",WaveNow,ad_v_data);

             	/*将波长和光强信息暂存在wave_data数组中,第一次扫描数据不保存*/
		wave_data[i][wave_index].wave=WaveNow;
             	if(i < 6)
             	{
			wave_data[i][wave_index].light =0.000000;
		}
		else
		{
			wave_data[i][wave_index].light =ad_v_data;	
		}
		wave_index++;
		}
	}
	ioctl(fdLopper,LOPPER_OFF,0);  //斩波器停止工作
//	write_to_file(); //将wave_data数据写入指定文件
	close(fdLopper);
	close(fdStepper);
	close(fdADC);
	return 0;	
}
/*将检测结果显示到LCD屏幕上*/
void WriteToScreen(HWND hDlg)
{
	sprintf(WaveOneNum,"%.2f",wave_data[0][WAVEONE].light);
        SetWindowText(GetDlgItem(hDlg,IDC_WAVEONE),WaveOneNum);
        sprintf(WaveTwoNum,"%.2f",wave_data[0][WAVETWO].light);
        SetWindowText(GetDlgItem(hDlg,IDC_WAVETWO),WaveTwoNum);
       	sprintf(WaveThreeNum,"%.2f",wave_data[0][WAVETHREE].light);
       	SetWindowText(GetDlgItem(hDlg,IDC_WAVETHREE),WaveThreeNum);
	
}
static int NhFileProc(HWND hDlg,int message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	int code1,id1;
	switch(message)
	{
		case MSG_INITDIALOG:
			return 1;
		case MSG_COMMAND:
			id1 = LOWORD(wParam);
			code1 = HIWORD(wParam);
			switch(id1)
			{
				case IDC_START:
					if(WaveCalibra)
					{
						WaveCalibrate(hDlg);
						WaveCalibra=0;
					}
					wave_index = 0; //将波长游标初始化为0
					NHPaint(hDlg);
					write_to_file(); //将wave_data数据写入指定文件
					WriteToScreen(hDlg);
					break;
				case IDC_SAVE:
		      			SaveNum(hDlg);//用于测试 
					break;
				default:
					break;
			}	
		default:
			break;
	}
	return DefaultDialogProc(hDlg,message,wParam,lParam);

}
static void NhBox(HWND hWnd)
{
	DlgNhFiles.controls = CtrlNhFiles;
	DialogBoxIndirectParam(&DlgNhFiles,hWnd,NhFileProc,0L);
}


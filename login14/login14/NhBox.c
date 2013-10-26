/*��wave_data�ĳɶ�ά���飬����NhPaintʮ�� 
�޸�У׼��ʼ��Ϊ3300 2013-8-14*/
#include "SaveNum.c"
#define IDC_CANKAO   113
#define IDC_YANGPING 114
#define IDC_START    118
#define IDC_SAVE     119
#define IDC_WAVEONE  210  //������ʾ2930������ѹ���
#define IDC_WAVETWO  211  //������ʾ2960������ѹ���
#define IDC_WAVETHREE  212  //������ʾ3030������ѹ���
#define IDC_RESULT     213  //������ʾŨ��ֵ

#define X0  2             //�����׼�㣬X0-Y0��Ӧ���Ͻ���ʼ�㣬X1-Y1��Ӧ���Ϳ�
#define Y0 30
#define X1 130
#define Y1 150

#define VOL_REF  5350  //AD�ο���ѹ
#define WAVE_INTERVAL 1 //���ڲ�����Ĳ������

#define WAVEORIGIN  3000    //ɨ���׼��
#define WAVESTART  3400     //��ʼɨ���
#define WAVEEND    3600     //����ɨ���
#define TIMES   10      //����ɨ�����

#define WAVEONE  212    //2930������Ӧ3413nm����ʼɨ�����3201
#define WAVETWO  177   //2960������Ӧ3378nm����ʼɨ�����3201
#define WAVETHREE  100    //3030������Ӧ3300nm����ʼɨ�����3201

static char WaveOneNum[10];  //���������ض������ĵ�ѹֵ
static char WaveTwoNum[10];
static char WaveThreeNum[10];

int WaveCalibra=1; //��ʼ�����������������ת����ʼλ��
int WaveStepNow; //��������ߵĲ�������ʼ������Ϊ0��

static char ad_tx_buf[]={0xac,0xac};
static char ad_rx_buf[]={'0','0'};
static int ad_rx_data;    //���ڱ���ADֵ
static float ad_v_data;  //���ڱ����ѹֵ
static int WaveNow=WAVEORIGIN;
static const char *pStr1 = ".";

int fdLopper;  //ն��������
/*********************���ݱ���*******************/
#define WAVE_LENGTH  (WAVEEND-WAVESTART+10)     //������Χ�洢��Ԫ
struct wave_struct
{
        int wave;
        float light;
};
struct wave_struct wave_data[TIMES][WAVE_LENGTH];  //�ݴ��ѹ����
//struct wave_struct wave_save[TIMES][WAVE_LENGTH];   //���յ�ѹ����
static int wave_index=0;   //���ݱ�����α�
/**************************************************************************************/
static CTRLDATA CtrlNhFiles[] =
{
	{

		CTRL_STATIC,
		WS_VISIBLE|SS_GROUPBOX|BS_CHECKED,
		X0,Y0,X1,Y1,
		IDC_STATIC,
		"�����",
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
                "Ũ��",
                0
        },
	{

                CTRL_STATIC,
                WS_VISIBLE,
                X0+138,Y0+30,X1-45,Y1-130,
                IDC_STATIC,
                "��ǰŨ��Ϊ��",
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
                "������",
                0
        },

	{
		CTRL_BUTTON,
		WS_VISIBLE|BS_DEFPUSHBUTTON,
		40,260,60,30,
		IDC_START,
		"ȷ��",
		0
	},	
	{
		CTRL_BUTTON,
		WS_VISIBLE|BS_DEFPUSHBUTTON,
		120,260,60,30,
		IDCANCEL,
		"����",
		0
	},
};
static DLGTEMPLATE DlgNhFiles =
{
	WS_BORDER|WS_CAPTION,
	WS_EX_NONE,
	0,0,240,320,
	"Ũ�ȼ��",
	0,0,
	sizeof(CtrlNhFiles)/sizeof(CTRLDATA),NULL,
	0
};
//��ʼ�������ǣ�ʹ�䵽����ʼɨ���
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
        printf("Stepper reach %d nm!\n",WAVEORIGIN);  //�����׼��
        //��ն����
/*      fdLopper=open("/dev/lopper",0);
        if(fdLopper<0)
        {
             perror("open lopper device");
             exit(1);
        }
        ioctl(fdLopper,LOPPER_ON,0);  //ն������ʼ����,Ԥ��±�ٵơ�
	ioctl(fdStepper,STEPPER_IOCFWD,tmpWaveLen);
        read(fdStepper,NULL,0);
	printf("Stepper reach %d nm!\n",WAVESTART);   //���￪ʼ��������ʼ��
	ioctl(fdLopper,LOPPER_OFF,0);  //ն����ֹͣ���� */
        close(fdStepper);  
//	close(fdLopper);
}

/*��wave_data����д��ָ���ļ�*/
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
			wave_data[0][j].light += wave_data[i][j].light*(i-6+1)*0.1; //7,8,9,10��ɨ��Ȩ��Ϊ1��2��3��4��	
		}
	//	wave_data[0][j].light = (wave_data[0][j].light)/(TIMES-6);
		/* ��ʽ������ο���UNIX�����߼���̡�121ҳ */
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
	int WaveStepNum; //�������Ҫ�ߵĲ���
	float light_rx;
	int i;
	HDC hdc;

	WaveStart=WAVESTART;WaveEnd=WAVEEND;  
	printf("Start Wave is :%d\n",WaveStart);
	printf("End Wave is :%d\n",WaveEnd);

	//�򿪲�������豸�ļ�
	fdStepper=open("/dev/stepper",0);
	if(fdStepper<0)
	{
		perror("open stepper device");
		exit(1);	
	}
	//��ADC�豸�ļ�
	fdADC=open("/dev/mini2440_spi0",O_RDWR);
	if(fdADC<0)
	{
		perror("open spi device");
		exit(1);	
	}
        //��ն����
        fdLopper=open("/dev/lopper",0);
        if(fdLopper<0)
        {
             perror("open lopper device");
             exit(1);
        }
        ioctl(fdLopper,LOPPER_ON,0);  //ն������ʼ����
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
		WaveNext=WaveNow+WAVE_INTERVAL; //��һ�β�����Ĳ���λ��
		WaveStepLen=WaveNext-WaveStart; //�������
		WaveStepNum=WaveStepLen-WaveStepNow; //��������ӵ�ǰλ�õ���һλ��Ҫ�ߵĲ���
		ioctl(fdStepper,STEPPER_IOCFWD,WaveStepNum);
		read(fdStepper,NULL,0);

		WaveNow = WaveNext;
		WaveStepNow=WaveStepLen;
		
		write(fdADC,ad_tx_buf,sizeof(ad_tx_buf)/sizeof(ad_tx_buf[0]));
		read(fdADC,ad_rx_buf,sizeof(ad_rx_buf)/sizeof(ad_rx_buf[0]));
		ad_rx_data=((int)ad_rx_buf[0])<<8;
		ad_rx_data|=ad_rx_buf[1]; //�õ�ADֵ
		ad_v_data = (float)ad_rx_data*VOL_REF/4096.0; //�õ���ѹ�����ڴ洢
		light_rx = ad_v_data/(0.0138*WaveNow-3.95); //?????�õ���ǿֵ����λnw
		ad_rx_data = (int)light_rx;
		printf("WaveNow = %d,ad_v_data = %.2f \n",WaveNow,ad_v_data);

             	/*�������͹�ǿ��Ϣ�ݴ���wave_data������,��һ��ɨ�����ݲ�����*/
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
	ioctl(fdLopper,LOPPER_OFF,0);  //ն����ֹͣ����
//	write_to_file(); //��wave_data����д��ָ���ļ�
	close(fdLopper);
	close(fdStepper);
	close(fdADC);
	return 0;	
}
/*���������ʾ��LCD��Ļ��*/
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
					wave_index = 0; //�������α��ʼ��Ϊ0
					NHPaint(hDlg);
					write_to_file(); //��wave_data����д��ָ���ļ�
					WriteToScreen(hDlg);
					break;
				case IDC_SAVE:
		      			SaveNum(hDlg);//���ڲ��� 
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


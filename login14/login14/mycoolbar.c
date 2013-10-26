#define ITEM_NUM 10
#define IDC_CB_NUM      200
#define IDC_CB_QWE	201 
#define IDC_CB_ASD	202 
#define IDC_CB_ZXC	203 
static const char *pStr = "";
static const char *caption1[] = 
{"0 ","1 ","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 "};

static const char *caption2[] =
{"q ","w ","e ","r ","t ","y ","u ","i ","o ","p "};


static const char *caption3[] =
{"a ","s ","d ","f ","g ","h ","j ","k ","l "};

static const char *caption4[] =
{"z ","x ","c ","v ","b ","n ","m ",",",";","del"};

static void create_coolbar_num(HWND hWnd)
{
	HWND cb;
	COOLBARITEMINFO item;
	int i;
	
	cb = CreateWindow (CTRL_COOLBAR,
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			IDC_CB_NUM,
			50,120,132,20,
			hWnd,
			0
			);
	item.ItemType = TYPE_TEXTITEM;
	item.Bmp = NULL;
	item.dwAddData = 0;
	for (i = 0; i < ITEM_NUM; i++) {
		item.insPos = i;
		item.id = i;
		item.Caption = caption1[i];
		item.ItemHint = NULL;
		SendMessage (cb, CBM_ADDITEM, 0, (LPARAM)&item);
	}
}

static void create_coolbar_qwe (HWND hWnd)
{
    HWND cb;
    COOLBARITEMINFO item;
    int i;

    cb = CreateWindow (CTRL_COOLBAR,
                    "",
                    WS_CHILD | WS_VISIBLE | WS_BORDER, 
                    IDC_CB_QWE,
                   50, 142, 132, 20,
                    hWnd,
                    0);

    item.ItemType = TYPE_TEXTITEM;
    item.Bmp = NULL;
    item.dwAddData = 0;
    for (i = 0; i < ITEM_NUM; i++) {
	item.insPos = i;
	item.id = i;
	item.Caption = caption2[i];
	item.ItemHint = NULL;
	SendMessage (cb, CBM_ADDITEM, 0, (LPARAM)&item);
    }
}

static void create_coolbar_asd (HWND hWnd)
{
    HWND cb;
    COOLBARITEMINFO item;
    int i;

    cb = CreateWindow (CTRL_COOLBAR,
                    "",
                    WS_CHILD | WS_VISIBLE | WS_BORDER, 
                    IDC_CB_ASD,
                   50, 164, 132, 20,
                    hWnd,
                    0);

    item.ItemType = TYPE_TEXTITEM;
    item.Bmp = NULL;
    item.dwAddData = 0;
    for (i = 0; i < ITEM_NUM; i++) {
	item.insPos = i;
	item.id = i;
	item.Caption = caption3[i];
	item.ItemHint = NULL;
	SendMessage (cb, CBM_ADDITEM, 0, (LPARAM)&item);
    }
}

static void create_coolbar_zxc (HWND hWnd)
{
    HWND cb;
    COOLBARITEMINFO item;
    int i;

    cb = CreateWindow (CTRL_COOLBAR,
                    "",
                    WS_CHILD | WS_VISIBLE | WS_BORDER, 
                    IDC_CB_ZXC,
                   50, 186, 132, 20,
                    hWnd,
                    0);

    item.ItemType = TYPE_TEXTITEM;
    item.Bmp = NULL;
    item.dwAddData = 0;
    for (i = 0; i < ITEM_NUM; i++) {
	item.insPos = i;
	item.id = i;
	item.Caption = caption4[i];
	item.ItemHint = NULL;
	SendMessage (cb, CBM_ADDITEM, 0, (LPARAM)&item);
    }
}

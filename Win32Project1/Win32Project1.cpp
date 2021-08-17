#include "stdafx.h"
#include "Lmz.h"

//gif
int num = 0, num2 = 0, selectnum=0,selectpos=0 ,beijingnum=0;
//玩家，boss
Player lm = Player();
Boss ll = Boss();
Boss curboss = Boss();
int bossid;
//自机子弹
SingleLinkList<Shot> lm_s;
int firecount = 0;
//掉落物
SingleLinkList<Drop> Dr_L;
int dropcount = 0;
//得分
int score = 0;
//符卡位置
int SCcount = 1;
Card *curC;
C1_1_1 c111;
C1_1_2 c112;
C1_1_3 c113;
C1_1_4 c114;
C1_1_5 c115;
C1_1_6 c116;
C1_1_7 c117;
C1_1_8 c118;
C1_1_9 c119;
//特效
int tenwan = 0;
bool SCchange = false;
int lihuishowy = -1000;
int lihuishowx = -1000;
int lihuishowspeed=1;
//结束
int endflag = 0;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				MyPaint_gif(HDC hdc);
void                paint(HDC hdc);
void                paint_start(HDC hdc);
void                paint_end(HDC hdc);
void                paint_gameover(HDC hdc);
void				DanmakuAction();
void				ShotAction();
void				DropAction();
void				pengzhuang();
void                SCselect();

//#pragma comment(linker,"/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	//MSG msg;

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (guan == 1){
				tNow = GetTickCount();
				if (tNow - tPre >= 15)
				{
					//cout << tNow - tPre<<endl;
					lm.move();
					curboss.move(rx, ry);
					ShotAction();
					DropAction();
					DanmakuAction();
					paint(hdc);
				}
			}
		}
	}
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "canvas";
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	HBITMAP bmp;
	hInst = hInstance;

	hWnd = CreateWindow("canvas", "Windows", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd, 100, 100, 960, 720, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	RECT recWin, recCli;
	GetWindowRect(hWnd, &recWin);
	GetClientRect(hWnd, &recCli);
	int newWidth = 960 + (recWin.right - recWin.left) - (recCli.right - recCli.left);
	int newHeight = 720 + (recWin.bottom - recWin.top) - (recCli.bottom - recCli.top);
	MoveWindow(hWnd, 100, 100, newWidth, newHeight, FALSE);

	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);
	tempdc = CreateCompatibleDC(hdc);
	bmp = CreateCompatibleBitmap(hdc, 960, 720);

	SelectObject(mdc, bmp);
	hbmp = (HBITMAP)LoadImage(NULL, "stage1.bmp", IMAGE_BITMAP, 765, 800, LR_LOADFROMFILE);
	hbmp2 = (HBITMAP)LoadImage(NULL, "stage2.bmp", IMAGE_BITMAP, 600, 849, LR_LOADFROMFILE);
	start_b = (HBITMAP)LoadImage(NULL, "start.bmp", IMAGE_BITMAP, 2406, 1080, LR_LOADFROMFILE);
	title_b = (HBITMAP)LoadImage(NULL, "title.bmp", IMAGE_BITMAP, 198, 75, LR_LOADFROMFILE);
	select_b = (HBITMAP)LoadImage(NULL, "select.bmp", IMAGE_BITMAP, 364, 166, LR_LOADFROMFILE);
	pl_b = (HBITMAP)LoadImage(NULL, "pl00.bmp", IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);
	kuang_b = (HBITMAP)LoadImage(NULL, "kuang.bmp", IMAGE_BITMAP, 960, 720, LR_LOADFROMFILE);
	lian_b = (HBITMAP)LoadImage(NULL, "lian.bmp", IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);
	shot_b = (HBITMAP)LoadImage(NULL, "shot.bmp", IMAGE_BITMAP, 76, 260, LR_LOADFROMFILE);
	hp_b = (HBITMAP)LoadImage(NULL, "xuetiao.bmp", IMAGE_BITMAP, 520, 8, LR_LOADFROMFILE);
	hp2_b = (HBITMAP)LoadImage(NULL, "xuetiao2.bmp", IMAGE_BITMAP, 520, 8, LR_LOADFROMFILE);
	pd_b = (HBITMAP)LoadImage(NULL, "panding.bmp", IMAGE_BITMAP, 10, 10, LR_LOADFROMFILE);
	cname_b = (HBITMAP)LoadImage(NULL, "cname.bmp", IMAGE_BITMAP, 446, 160, LR_LOADFROMFILE);
	cback_b = (HBITMAP)LoadImage(NULL, "back.bmp", IMAGE_BITMAP, 256, 36, LR_LOADFROMFILE);
	tenwan_b = (HBITMAP)LoadImage(NULL, "tenwan.bmp", IMAGE_BITMAP, 960, 720, LR_LOADFROMFILE);
	r_h = (HBITMAP)LoadImage(NULL, "red_heart.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
	p_h = (HBITMAP)LoadImage(NULL, "pink_heart.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
	g_r = (HBITMAP)LoadImage(NULL, "green_rice.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	y_d = (HBITMAP)LoadImage(NULL, "yellow_dayu.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
	r_d = (HBITMAP)LoadImage(NULL, "red_dayu.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
	b_d = (HBITMAP)LoadImage(NULL, "blue_dayu.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
	g_d = (HBITMAP)LoadImage(NULL, "green_dayu.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
	r_l = (HBITMAP)LoadImage(NULL, "red_light.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	b_l = (HBITMAP)LoadImage(NULL, "blue_light.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	b_z = (HBITMAP)LoadImage(NULL, "blue_zhongyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	c_z = (HBITMAP)LoadImage(NULL, "cyan_zhongyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	b_dao = (HBITMAP)LoadImage(NULL, "blue_dao.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
	c_dao = (HBITMAP)LoadImage(NULL, "cyan_dao.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
	r_g = (HBITMAP)LoadImage(NULL, "red_guangyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	g_g = (HBITMAP)LoadImage(NULL, "green_guangyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	y_g = (HBITMAP)LoadImage(NULL, "yellow_guangyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	b_g = (HBITMAP)LoadImage(NULL, "blue_guangyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	p_g = (HBITMAP)LoadImage(NULL, "pink_guangyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	c_g = (HBITMAP)LoadImage(NULL, "cyan_guangyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	o_g = (HBITMAP)LoadImage(NULL, "orange_guangyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	d_g = (HBITMAP)LoadImage(NULL, "darkred_guangyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	w_g = (HBITMAP)LoadImage(NULL, "white_guangyu.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	w_s = (HBITMAP)LoadImage(NULL, "white_snow.bmp", IMAGE_BITMAP, 8, 8, LR_LOADFROMFILE);
	gift = (HBITMAP)LoadImage(NULL, "pink_gift.bmp", IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
	lihui1_b = (HBITMAP)LoadImage(NULL, "lihui_1.bmp", IMAGE_BITMAP, 450, 400, LR_LOADFROMFILE);
	lihui2_b = (HBITMAP)LoadImage(NULL, "lihui_2.bmp", IMAGE_BITMAP, 848, 1200, LR_LOADFROMFILE);
	Pdian_b = (HBITMAP)LoadImage(NULL, "dian_P.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	Fdian_b = (HBITMAP)LoadImage(NULL, "dian_F.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	Ldian_b = (HBITMAP)LoadImage(NULL, "dian_L.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	ascii_b = (HBITMAP)LoadImage(NULL, "ascii.bmp", IMAGE_BITMAP, 208, 16, LR_LOADFROMFILE);
	bosspos_b = (HBITMAP)LoadImage(NULL, "bosspos.bmp", IMAGE_BITMAP, 38, 16, LR_LOADFROMFILE);
	heart_bottom_b = (HBITMAP)LoadImage(NULL, "heart_bottom.bmp", IMAGE_BITMAP, 256, 32, LR_LOADFROMFILE);
	heart_up_b = (HBITMAP)LoadImage(NULL, "heart_up.bmp", IMAGE_BITMAP, 256, 32, LR_LOADFROMFILE);
	end1_b = (HBITMAP)LoadImage(NULL, "end1.bmp", IMAGE_BITMAP, 960, 720, LR_LOADFROMFILE);
	end2_b = (HBITMAP)LoadImage(NULL, "end2.bmp", IMAGE_BITMAP, 960, 720, LR_LOADFROMFILE);
	end3_b = (HBITMAP)LoadImage(NULL, "end3.bmp", IMAGE_BITMAP, 960, 720, LR_LOADFROMFILE);
	SetTimer(hWnd, 1, 100, NULL);//人物动图定时器
	SetTimer(hWnd, 2, 120, NULL);//子弹发射定时器
	SetTimer(hWnd, 3, 16, NULL);//子弹碰撞定时器
	SetTimer(hWnd, 4, 5000, NULL);//Boss移动定时器
	SetTimer(hWnd, 5, 300, NULL);//开始界面显示定时器
	SetTimer(hWnd, 6, 300, NULL);//结束界面显示定时器
	SetTimer(hWnd, 7, 100, NULL);//失败界面显示定时器
	curboss = ll;//第一关boss
	bossid = 1;
	curC = &c111;
	mciSendString("play 1-1.wma repeat", NULL, 0, NULL);
//	mciSendString("play SE.mp3", NULL, 0, NULL);

	

	return TRUE;
}
void MyPaint_gif(HDC hdc)
{
	if ((lm.dleft == 1) && (lm.dright == 1)){
		num2 = 0;
		if (num == 7)
			num = 0;
	}
	else if ((lm.dleft == 1) && (lm.dright == 0)){
		num2 = 48;
		if (num == 7)
			num = 3;
	}
	else if ((lm.dleft == 0) && (lm.dright == 1)){
		num2 = 96;
		if (num == 7)
			num = 3;
	}
	else{
		num2 = 0;
		if (num == 7)
			num = 0;
	}
	//012 345 678 91011
	if (curboss.numlian == 11){
		curboss.numlian = 9;
		curboss.cnumlian = -1;
	}
	if (curboss.numlian == 0){
		curboss.numlian = 2;
		curboss.cnumlian = 1;
	}
	num++;
	curboss.numlian += curboss.cnumlian;
}
//jjjjjjjjjjjjjjkjjjup  kkkkkkkkk
void paint(HDC hdc)
{
	//游戏框左上48，24，右下625，696
	//显示背景
	SelectObject(bufdc, hbmp);
	TransparentBlt(mdc, 48, 24, 577, 672, bufdc, 188, 24, 577, 672, RGB(3, 3, 3));
	if (SCcount == 8 || SCcount == 9){
		if (beijingnum != 672){
			beijingnum += 3;
		}
		SelectObject(bufdc, hbmp2);
		TransparentBlt(mdc, 48, 24, 577, beijingnum, bufdc, 0, 50, 577, beijingnum, RGB(1, 1, 1));
	}
	//显示人物
	SelectObject(bufdc, pl_b);
	TransparentBlt(mdc, lm.sx, lm.sy, 48, 72, bufdc, num * 32, num2, 32, 48, RGB(125, 255, 0));

	//显示人物射击
	SelectObject(bufdc, shot_b);
	for (int i = 0; i < firecount; i++){
		Shot tempS;
		tempS = lm_s.getValue(i);
		TransparentBlt(mdc, tempS.sx, tempS.sy, 76, 65, bufdc, 0, (tempS.level) * 65, 76, 65, RGB(125, 255, 0));
	}
	//显示掉落物
	for (int i = 0; i < dropcount; i++){
		Drop tempDr;
		tempDr = Dr_L.getValue(i);
		if (tempDr.type == 1){
			SelectObject(bufdc, Pdian_b);
		}
		if (tempDr.type == 2){
			SelectObject(bufdc, Fdian_b);
		}
		if (tempDr.type == 3){
			SelectObject(bufdc, Ldian_b);
			TransparentBlt(mdc, tempDr.sx - 12, tempDr.sy - 12, 48, 48, bufdc, 0, 0, 16, 16, RGB(0, 0, 0));
		}
		else{
			TransparentBlt(mdc, tempDr.sx, tempDr.sy, 24, 24, bufdc, 0, 0, 16, 16, RGB(0, 0, 0));
		}
	}
	//显示弹幕
	for (int i = 0; i < (curC->dcount); i++){
		Danmaku tempD;
		tempD = curC->c_L.getValue(i);
		TransparentBlt(mdc, tempD.sx, tempD.sy, tempD.length*tempD.showrate, tempD.length*tempD.showrate, tempD.dcro, 0, 0, tempD.length, tempD.length, RGB(254, 254, 254));
	//	DeleteDC(tempD.dcro);
	}

	//显示立绘
	if (SCchange){
		lihuishowx = 700;
		lihuishowy = -550;
		SCchange = false;
	}
	if (lihuishowy > 100 && lihuishowy < 200){
		lihuishowspeed = 1;
	}
	else {
		lihuishowspeed = 7;
	}
	lihuishowy += lihuishowspeed;
	lihuishowx -= lihuishowspeed;
	if (SCcount != 8){
		SelectObject(bufdc, lihui1_b);
		TransparentBlt(mdc, lihuishowx, lihuishowy, 450, 400, bufdc, 0, 0, 450, 400, RGB(0, 0, 0));
	}
	else
	{
		SelectObject(bufdc, lihui2_b);
		TransparentBlt(mdc, lihuishowx, lihuishowy-30, 424, 600, bufdc, 0, 0, 848, 1200, RGB(0, 0, 0));
	}
	//显示框
	SelectObject(bufdc, kuang_b);
	TransparentBlt(mdc, 0, 0, 960, 720, bufdc, 0, 0, 960, 720, RGB(1, 1, 1));
	//显示判定点
	if (lm.low == true){
		SelectObject(bufdc, pd_b);
		TransparentBlt(mdc, lm.x - 7, lm.y - 7, 14, 14, bufdc, 0, 0, 10, 10, RGB(125, 255, 0));
	}
	//显示boss
	SelectObject(bufdc, lian_b);
	TransparentBlt(mdc, curboss.sx, curboss.sy, 96, 96, bufdc, (curboss.numlian / 3) * 64, curboss.movegif * 64, 64, 64, RGB(1, 1, 1));
	if (SCcount != 9){
		if (SCcount != 8){
			SelectObject(bufdc, hp_b);
			if (curboss.hp % 1000 == 0){
				BitBlt(mdc, 55, 32, 520, 8, bufdc, 0, 0, SRCCOPY);
			}
			else{
				BitBlt(mdc, 55, 32, int(curboss.hp % 1000 * 1.0 / 1000 * 520), 8, bufdc, 0, 0, SRCCOPY);
			}
		}
		else
		{
			SelectObject(bufdc, hp2_b);
			BitBlt(mdc, 55, 32, int(curboss.hp * 1.0 / 288 * 520), 8, bufdc, 0, 0, SRCCOPY);
		}
	}
	SelectObject(bufdc, bosspos_b);
	TransparentBlt(mdc, curboss.x - 19, 696, 38, 16, bufdc, 0, 0, 38, 16, RGB(0, 0, 0));
	//显示符卡名称
	if (SCcount % 2 == 0){
		SelectObject(bufdc, cback_b);
		TransparentBlt(mdc, 370, 38, 256, 36, bufdc, 0, 2, 256, 34, RGB(255, 255, 255));
		SelectObject(bufdc, cname_b);
		TransparentBlt(mdc, 390, 45, 223, 20, bufdc, 0, (SCcount / 2 - 1) * 40, 446, 40, RGB(255, 255, 255));
	}
	//显示特效
	if (SCcount == 4){
		if (tenwan != 720){
			tenwan = tenwan + 3;
		}
		SelectObject(bufdc, tenwan_b);
		TransparentBlt(mdc, 0, 0, 960, tenwan, bufdc, 0, 0, 960, tenwan, RGB(255, 255, 255));
	}
	//显示得分
	score += 10;
	SelectObject(bufdc, ascii_b);
	TransparentBlt(mdc, 720, 55, 24, 24, bufdc, (score / 100000000) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 742, 55, 24, 24, bufdc, (score / 10000000) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 764, 55, 24, 24, bufdc, (score / 1000000) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 786, 55, 24, 24, bufdc, (score / 100000) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 808, 55, 24, 24, bufdc, (score / 10000) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 830, 55, 24, 24, bufdc, (score / 1000) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 852, 55, 24, 24, bufdc, (score / 100) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 874, 55, 24, 24, bufdc, (score / 10) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 896, 55, 24, 24, bufdc, score % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	//显示power
	TransparentBlt(mdc, 750, 220, 24, 24, bufdc, (lm.power/100) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 765, 220, 24, 24, bufdc, 176, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 780, 220, 24, 24, bufdc, (lm.power/10) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 800, 220, 24, 24, bufdc, (lm.power) % 10 * 16, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 820, 220, 24, 24, bufdc, 160, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 840, 220, 24, 24, bufdc, 64, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 855, 220, 24, 24, bufdc, 176, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 870, 220, 24, 24, bufdc, 0, 0, 16, 16, RGB(254, 254, 254));
	TransparentBlt(mdc, 890, 220, 24, 24, bufdc, 0, 0, 16, 16, RGB(254, 254, 254));
	//显示生命值
	SelectObject(bufdc, heart_bottom_b);
	TransparentBlt(mdc, 650, 130, 256, 32, bufdc, 0, 0, 256, 32, RGB(1, 1, 1));
	SelectObject(bufdc, heart_up_b);
	TransparentBlt(mdc, 650, 130, lm.life*32, 32, bufdc, 0, 0, lm.life*32, 32, RGB(1, 1, 1));
	//前面两个参数决定在窗体中的位置,第三第四决定缩放大小
	BitBlt(hdc, 0, 0, 960, 720, mdc, 0, 0, SRCCOPY);
	tPre = GetTickCount();
}
void paint_start(HDC hdc)
{
	//游戏框左上48，24，右下625，696
	//显示背景
	SelectObject(bufdc, start_b);
	TransparentBlt(mdc, 0, 0, 960, 720, bufdc, 50, 0, 1440, 1080, RGB(1, 1, 1));
	//显示标题
	SelectObject(bufdc, title_b);
	TransparentBlt(mdc, 70, 50, 198 * 2, 75 * 2, bufdc, 0, 0, 198, 75, RGB(1, 1, 1));
	//显示选项
	SelectObject(bufdc, select_b);
	TransparentBlt(mdc, 370, 450, 182, 83, bufdc, selectnum % 2 * 182, selectpos % 2 * 83, 182, 83, RGB(255, 255, 255));
	//前面两个参数决定在窗体中的位置,第三第四决定缩放大小
	BitBlt(hdc, 0, 0, 960, 720, mdc, 0, 0, SRCCOPY);
	selectnum++;
}
void paint_end(HDC hdc)
{
	if (endflag == 0){
		SelectObject(bufdc, end1_b);
		TransparentBlt(mdc, 0, 0, 960, 720, bufdc, 0, 0, 960, 720, RGB(1, 1, 1));
	}
	if (endflag == 1){
		SelectObject(bufdc, end2_b);
		TransparentBlt(mdc, 0, 0, 960, 720, bufdc, 0, 0, 960, 720, RGB(1, 1, 1));
	}
	BitBlt(hdc, 0, 0, 960, 720, mdc, 0, 0, SRCCOPY);
}
void paint_gameover(HDC hdc)
{
	SelectObject(bufdc, end3_b);
	TransparentBlt(mdc, 0, 0, 960, 720, bufdc, 0, 0, 960, 720, RGB(1, 1, 123));
	BitBlt(hdc, 0, 0, 960, 720, mdc, 0, 0, SRCCOPY);
}
void DanmakuAction(){
	//待优化：别用虚函数
	//弹幕移动
	SCselect();
	int x = 0;
	Danmaku tempD;
	curC->action(lm,curboss);
	for (int i = 0; i < (curC->dcount); i++){
		tempD = curC->c_L.getValue(i);
		x = tempD.move();
		if (tempD.xz_active){
			rodan(tempD, tempD.angle);
			tempD.xz_active = false;
		}
		curC->c_L.getValue(i) = tempD;
		if (x == 1){
			DeleteDC(curC->c_L.getValue(i).dcro);
			curC->c_L.pop(i);
			curC->dcount--;
			i--;
		}
	}
}
void ShotAction(){
	//id
	int x=0;
	int temp = 0;
	for (int i = 0; i < firecount; i++){
		x = lm_s.getValue(i).move(curboss);
		//boss处理
		if (x == 1)
		{
			lm_s.pop(i);
			firecount--;
			//根据火力改变扣血
			//待完善：击中boss特效
			if (!curC->bosswudi){
				score += 50;
				temp = curboss.hit(lm.power);
			}
			i--;
		}
		//版边处理
		if (x == 2)
		{
			lm_s.pop(i);
			firecount--;
			i--;
		}
	}
	if (temp != SCcount&&temp != 0){
		if (temp != 9){
			if (temp % 2 == 0){
				SCchange = true;
				mciSendString("play SE_charge.wav", NULL, 0, NULL);
				score += 300000;
			}
			else{
				score += 500000;
				for (int i = 0; i < 20; i++){
					Drop tempDr = Drop(curboss, 1);
					Dr_L.append(tempDr);
					dropcount++;
				}
				for (int i = 0; i < 50; i++){
					Drop tempDr = Drop(curboss, 2);
					Dr_L.append(tempDr);
					dropcount++;
				}
				Drop tempDr = Drop(curboss, 3);
				Dr_L.append(tempDr);
				dropcount++;
			}
			if (temp != 1){
				mciSendString("play SE_SCpo2.wav", NULL, 0, NULL);
			}
			if (temp == 8){
				mciSendString("stop 1-2.wma", NULL, 0, NULL);
				mciSendString("play 1-3.wma repeat", NULL, 0, NULL);
			}
		}
		else
		{
			mciSendString("play SE_SCpo.wav", NULL, 0, NULL);
			mciSendString("stop 1-3.wma", NULL, 0, NULL);
			mciSendString("play 1-4.wma repeat", NULL, 0, NULL);
		}
		//待优化：消去上关的子弹
		SCcount = temp;
	}
}
void DropAction(){
	int x = 0;
	Drop tempDr;
	for (int i = 0; i < dropcount; i++){
		tempDr = Dr_L.getValue(i);
		if (lm.y < 300){
			tempDr.xi_r = 1000;
		}
		x = tempDr.move(lm);
		//获得处理
		if (x == 1){
			if (tempDr.type == 1){
				lm.power += 5;
				if (lm.power > 400){
					lm.power = 400;
				}
			}
			else if (tempDr.type == 2){
				score += 5000;
			}
			else{
				mciSendString("play SE_extend.wav", NULL, 0, NULL);
				lm.life += 1;
			}
			x = 3;
		}
		//吸引处理
		if (x == 2){
			tempDr.angle = (180 / 3.14)*atan2((lm.y - tempDr.y), (lm.x - tempDr.x)) + 90;
			tempDr.speed = 10;
		}
		Dr_L.getValue(i) = tempDr;
		//版边处理
		if (x == 3){
			Dr_L.pop(i);
			dropcount--;
			i--;
		}
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (guan)
	{
	case 0:
		switch (message)
		{
		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_UP:
				mciSendString("play SE_ok.wav", NULL, 0, NULL);
				selectpos++;
				break;
			case VK_DOWN:
				mciSendString("play SE_ok.wav", NULL, 0, NULL);
				selectpos++;
				break;
			case VK_RIGHT:
				mciSendString("play SE_ok.wav", NULL, 0, NULL);
				selectpos++;
				break;
			case VK_LEFT:
				mciSendString("play SE_ok.wav", NULL, 0, NULL);
				selectpos++;
				break;
			case 'Z':
				if (selectpos % 2 == 0){
					guan = 1;
					mciSendString("play SE_ok.wav", NULL, 0, NULL);
					mciSendString("stop 1-1.wma", NULL, 0, NULL);
					mciSendString("play 1-2.wma repeat", NULL, 0, NULL);
				}
				else
				{
					SendMessage(hWnd, WM_DESTROY, 0, 0);
				}
				break;
			}
			break;
		case WM_TIMER:
			switch (wParam)
			{
			case 5:
				paint_start(hdc);
				break;
			}
			break;
		case WM_DESTROY:
			DeleteDC(mdc);
			DeleteDC(bufdc);
			DeleteDC(tempdc);
			DeleteObject(hbmp);
			ReleaseDC(hWnd, hdc);
			DeleteObject(pl_b);
			DeleteObject(bg_b);
			DeleteObject(kuang_b);
			DeleteObject(lian_b);
			DeleteObject(shot_b);
			DeleteObject(hp_b);
			DeleteObject(hp2_b);
			DeleteObject(pd_b);
			DeleteObject(tenwan_b);
			DeleteObject(lihui1_b);
			DeleteObject(lihui2_b);
			DeleteObject(start_b);
			DeleteObject(title_b);
			DeleteObject(select_b);
			DeleteObject(Pdian_b);
			DeleteObject(Fdian_b);
			DeleteObject(Ldian_b);
			DeleteObject(r_h);
			DeleteObject(p_h);
			DeleteObject(g_r);
			DeleteObject(y_d);
			DeleteObject(r_d);
			DeleteObject(b_d);
			DeleteObject(g_d);
			DeleteObject(r_l);
			DeleteObject(b_l);
			DeleteObject(b_z);
			DeleteObject(c_z);
			DeleteObject(b_dao);
			DeleteObject(c_dao);
			DeleteObject(r_g);
			DeleteObject(g_g);
			DeleteObject(y_g);
			DeleteObject(b_g);
			DeleteObject(p_g);
			DeleteObject(c_g);
			DeleteObject(o_g);
			DeleteObject(d_g);
			DeleteObject(w_g);
			DeleteObject(w_s);
			DeleteObject(gift);
			DeleteObject(cname_b);
			DeleteObject(cback_b);
			DeleteObject(ascii_b);
			DeleteObject(bosspos_b);
			DeleteObject(heart_bottom_b);
			DeleteObject(heart_up_b);
			DeleteObject(end1_b);
			DeleteObject(end2_b);
			DeleteObject(end3_b);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			KillTimer(hWnd, 5);
			KillTimer(hWnd, 6);
			KillTimer(hWnd, 7);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case 1:
		switch (message)
		{
			//待优化：移动控制外置
		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_UP:
				lm.moveup();
				break;
			case VK_DOWN:
				lm.movedown();
				break;
			case VK_RIGHT:
				lm.moveright();
				break;
			case VK_LEFT:
				lm.moveleft();
				break;
			case 'Z':
				lm.fire = true;
				break;
			case VK_SHIFT:
				lm.disu();
				break;
			}
			break;
		case WM_KEYUP:
			switch (wParam)
			{
			case VK_UP:
				lm.cmoveup();
				break;
			case VK_DOWN:
				lm.cmovedown();
				break;
			case VK_RIGHT:
				lm.cmoveright();
				break;
			case VK_LEFT:
				lm.cmoveleft();
				break;
			case 'Z':
				lm.fire = false;
				break;
			case VK_SHIFT:
				lm.cdisu();
				break;
			}
			break;
		case WM_TIMER:
			switch (wParam)
			{
			case 1:
				MyPaint_gif(hdc);
				break;
			case 2:
				if (lm.fire == 1){
					Shot tempS = Shot(lm);
					lm_s.append(tempS);
					firecount++;
				}
				break;
			case 3:
				lm.wudi--;
				if (lm.wudi < 0){
					pengzhuang();
				}
				break;
			case 4:
				if (curC->move_active){
					rx = rand() % (400 - 250 + 1) + 250;
					ry = rand() % (300 - 100 + 1) + 100;
				}
				break;
			}
			break;
		case WM_DESTROY:
			DeleteDC(mdc);
			DeleteDC(bufdc);
			DeleteDC(tempdc);
			DeleteObject(hbmp);
			ReleaseDC(hWnd, hdc);
			DeleteObject(pl_b);
			DeleteObject(bg_b);
			DeleteObject(kuang_b);
			DeleteObject(lian_b);
			DeleteObject(shot_b);
			DeleteObject(hp_b);
			DeleteObject(hp2_b);
			DeleteObject(pd_b);
			DeleteObject(tenwan_b);
			DeleteObject(lihui1_b);
			DeleteObject(lihui2_b);
			DeleteObject(start_b);
			DeleteObject(title_b);
			DeleteObject(select_b);
			DeleteObject(Pdian_b);
			DeleteObject(Fdian_b);
			DeleteObject(Ldian_b);
			DeleteObject(r_h);
			DeleteObject(p_h);
			DeleteObject(g_r);
			DeleteObject(y_d);
			DeleteObject(r_d);
			DeleteObject(b_d);
			DeleteObject(g_d);
			DeleteObject(r_l);
			DeleteObject(b_l);
			DeleteObject(b_z);
			DeleteObject(c_z);
			DeleteObject(b_dao);
			DeleteObject(c_dao);
			DeleteObject(r_g);
			DeleteObject(g_g);
			DeleteObject(y_g);
			DeleteObject(b_g);
			DeleteObject(p_g);
			DeleteObject(c_g);
			DeleteObject(o_g);
			DeleteObject(d_g);
			DeleteObject(w_g);
			DeleteObject(w_s);
			DeleteObject(gift);
			DeleteObject(cname_b);
			DeleteObject(cback_b);
			DeleteObject(ascii_b);
			DeleteObject(bosspos_b);
			DeleteObject(heart_bottom_b);
			DeleteObject(heart_up_b);
			DeleteObject(end1_b);
			DeleteObject(end2_b);
			DeleteObject(end3_b);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			KillTimer(hWnd, 5);
			KillTimer(hWnd, 6);
			KillTimer(hWnd, 7);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case 2:
		switch (message)
		{
		case WM_KEYDOWN:
			switch (wParam)
			{
			case 'Z':
				endflag++;
				if (endflag == 2){
					SendMessage(hWnd, WM_DESTROY, 0, 0);
				}
				mciSendString("play SE_ok.wav", NULL, 0, NULL);
				break;
			}
			break;
		case WM_TIMER:
			switch (wParam)
			{
			case 6:
				paint_end(hdc);
				break;
			}
			break;
		case WM_DESTROY:
			DeleteDC(mdc);
			DeleteDC(bufdc);
			DeleteDC(tempdc);
			DeleteObject(hbmp);
			ReleaseDC(hWnd, hdc);
			DeleteObject(pl_b);
			DeleteObject(bg_b);
			DeleteObject(kuang_b);
			DeleteObject(lian_b);
			DeleteObject(shot_b);
			DeleteObject(hp_b);
			DeleteObject(hp2_b);
			DeleteObject(pd_b);
			DeleteObject(tenwan_b);
			DeleteObject(lihui1_b);
			DeleteObject(lihui2_b);
			DeleteObject(start_b);
			DeleteObject(title_b);
			DeleteObject(select_b);
			DeleteObject(Pdian_b);
			DeleteObject(Fdian_b);
			DeleteObject(Ldian_b);
			DeleteObject(r_h);
			DeleteObject(p_h);
			DeleteObject(g_r);
			DeleteObject(y_d);
			DeleteObject(r_d);
			DeleteObject(b_d);
			DeleteObject(g_d);
			DeleteObject(r_l);
			DeleteObject(b_l);
			DeleteObject(b_z);
			DeleteObject(c_z);
			DeleteObject(b_dao);
			DeleteObject(c_dao);
			DeleteObject(r_g);
			DeleteObject(g_g);
			DeleteObject(y_g);
			DeleteObject(b_g);
			DeleteObject(p_g);
			DeleteObject(c_g);
			DeleteObject(o_g);
			DeleteObject(d_g);
			DeleteObject(w_g);
			DeleteObject(w_s);
			DeleteObject(gift);
			DeleteObject(cname_b);
			DeleteObject(cback_b);
			DeleteObject(ascii_b);
			DeleteObject(bosspos_b);
			DeleteObject(heart_bottom_b);
			DeleteObject(heart_up_b);
			DeleteObject(end1_b);
			DeleteObject(end2_b);
			DeleteObject(end3_b);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			KillTimer(hWnd, 5);
			KillTimer(hWnd, 6);
			KillTimer(hWnd, 7);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case 3:
		switch (message)
		{
		case WM_KEYDOWN:
			switch (wParam)
			{
			case 'Z':
				guan = 0;
				mciSendString("play SE_ok.wav", NULL, 0, NULL);
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			}
			break;
		case WM_TIMER:
			switch (wParam)
			{
			case 7:
				paint_gameover(hdc);
				break;
			}
			break;
		case WM_DESTROY:
			DeleteDC(mdc);
			DeleteDC(bufdc);
			DeleteDC(tempdc);
			DeleteObject(hbmp);
			ReleaseDC(hWnd, hdc);
			DeleteObject(pl_b);
			DeleteObject(bg_b);
			DeleteObject(kuang_b);
			DeleteObject(lian_b);
			DeleteObject(shot_b);

			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	return 0;
}
void pengzhuang(){
	Danmaku tempD;
	int hit = 0;
	for (int i = 0; i < (curC->dcount); i++){
		tempD = curC->c_L.getValue(i);
		//sqrt(pow((double)(p.x - x), 2) + pow((double)(p.y - y), 2)) <= r
		//(tempD.y>lm.y - tempD.r) && (tempD.y<lm.y + tempD.r) && (tempD.x>lm.x - tempD.r) && (tempD.x < lm.x + tempD.r)
		if ((tempD.y>lm.y - tempD.r) && (tempD.y<lm.y + tempD.r) && (tempD.x>lm.x - tempD.r) && (tempD.x < lm.x + tempD.r)){
			//击中玩家
			mciSendString("play SE_biu.wav", NULL, 0, NULL);
			lm.life -= 1;
			lm.wudi == 5;
			lm.power -= 50;
			if (lm.power < 100){
				lm.power = 100;
			}
			lm.x = 337;
			lm.y = 680;
			hit = 1;
		}
		if (lm.life == -1){
			mciSendString("stop 1-2.wma", NULL, 0, NULL);
			mciSendString("stop 1-3.wma", NULL, 0, NULL);
			mciSendString("play 1-5.wma repeat", NULL, 0, NULL);
			guan = 3;
		}
		if (hit == 1){
			for (int i = 0; i < (curC->dcount); i++){
				curC->c_L.getValue(i).movelife=1;
			}
			break;
		}
	}
}
void SCselect(){
	switch (SCcount)
	{
	case 1:
		curC = &c111;
		break;
	case 2:
		curC = &c112;
		break;
	case 3:
		curC = &c113;
		break;
	case 4:
		curC = &c114;
		break;
	case 5:
		curC = &c115;
		break;
	case 6:
		curC = &c116;
		break;
	case 7:
		curC = &c117;
		break;
	case 8:
		curC = &c118;
		break;
	case 9:
		curC = &c119;
		break;
	}
}
void rodan(Danmaku d, int angle)
{
	HBITMAP tempbmp = 0;
	if (d.t == Heart){
		//限制了弹幕的大小
		tempbmp = CreateCompatibleBitmap(hdc, 32, 32);
		switch (d.c)
		{
		case Red:
			SelectObject(tempdc, r_h);
			break;
		case Pink:
			SelectObject(tempdc, p_h);
			break;
		}
		SelectObject(d.dcro, tempbmp);
		//将dc后(dmBmp1)以(17,17)为坐标中心旋转angle角度后后画到dc前中，后4参数：源xy，源宽高
		//前2参数代表显示的坐标中心位置
		rotate(d.dcro, 16, 16, angle, tempdc, 0, 0, 34, 34, 0xfefefe);
	}
	if (d.t == Rice){
		tempbmp = CreateCompatibleBitmap(hdc, 16, 16);
		SelectObject(tempdc, g_r);
		SelectObject(d.dcro, tempbmp);
		rotate(d.dcro, 8, 8, angle, tempdc, 0, 0, 17, 17, 0xfefefe);
	}
	if (d.t == Dayu){
		tempbmp = CreateCompatibleBitmap(hdc, 64, 64);
		switch (d.c)
		{
		case Yellow:
			SelectObject(tempdc, y_d);
			break;
		case Red:
			SelectObject(tempdc, r_d);
			break;
		case Blue:
			SelectObject(tempdc, b_d);
			break;
		case Green:
			SelectObject(tempdc, g_d);
			break;
		}
		SelectObject(d.dcro, tempbmp);
		rotate(d.dcro, 32, 32, angle, tempdc, 0, 0, 65, 65, 0xfefefe);
	}
	if (d.t == Light){
		//限制了弹幕的大小
		tempbmp = CreateCompatibleBitmap(hdc, 16, 16);
		switch (d.c)
		{
		case Red:
			SelectObject(tempdc, r_l);
			break;
		case Blue:
			SelectObject(tempdc, b_l);
			break;
		}
		SelectObject(d.dcro, tempbmp);
		//将dc后(dmBmp1)以(17,17)为坐标中心旋转angle角度后后画到dc前中，后4参数：源xy，源宽高
		//前2参数代表显示的坐标中心位置
		rotate(d.dcro, 8, 8, angle, tempdc, 0, 0, 17, 17, 0xfefefe);
	}
	if (d.t == Zhongyu){
		//限制了弹幕的大小
		tempbmp = CreateCompatibleBitmap(hdc, 16, 16);
		switch (d.c)
		{
		case Blue:
			SelectObject(tempdc, b_z);
			break;
		case Cyan:
			SelectObject(tempdc, c_z);
			break;
		}
		SelectObject(d.dcro, tempbmp);
		//将dc后(dmBmp1)以(17,17)为坐标中心旋转angle角度后后画到dc前中，后4参数：源xy，源宽高
		//前2参数代表显示的坐标中心位置
		rotate(d.dcro, 8, 8, angle, tempdc, 0, 0, 17, 17, 0xfefefe);
	}
	if (d.t == Dao){
		//限制了弹幕的大小
		tempbmp = CreateCompatibleBitmap(hdc, 32, 32);
		switch (d.c)
		{
		case Blue:
			SelectObject(tempdc, b_dao);
			break;
		case Cyan:
			SelectObject(tempdc, c_dao);
			break;
		}
		SelectObject(d.dcro, tempbmp);
		//将dc后(dmBmp1)以(17,17)为坐标中心旋转angle角度后后画到dc前中，后4参数：源xy，源宽高
		//前2参数代表显示的坐标中心位置
		rotate(d.dcro, 16, 16, angle, tempdc, 0, 0, 34, 34, 0xfefefe);
	}
	if (d.t == Guangyu){
		//限制了弹幕的大小
		//r_g, g_g, y_g, b_g, p_g, c_g, o_g, d_g, w_g
		tempbmp = CreateCompatibleBitmap(hdc, 16, 16);
		switch (d.c)
		{
		case Red:
			SelectObject(tempdc, r_g);
			break;
		case Green:
			SelectObject(tempdc, g_g);
			break;
		case Yellow:
			SelectObject(tempdc, y_g);
			break;
		case Blue:
			SelectObject(tempdc, b_g);
			break;
		case Pink:
			SelectObject(tempdc, p_g);
			break;
		case Cyan:
			SelectObject(tempdc, c_g);
			break;
		case Orange:
			SelectObject(tempdc, o_g);
			break;
		case Darkred:
			SelectObject(tempdc, d_g);
			break;
		case White:
			SelectObject(tempdc, w_g);
			break;
		}
		SelectObject(d.dcro, tempbmp);
		//将dc后(dmBmp1)以(17,17)为坐标中心旋转angle角度后后画到dc前中，后4参数：源xy，源宽高
		//前2参数代表显示的坐标中心位置
		rotate(d.dcro, 8, 8, angle, tempdc, 0, 0, 17, 17, 0xfefefe);
	}
	if (d.t == Snow){
		tempbmp = CreateCompatibleBitmap(hdc, 8, 8);
		SelectObject(tempdc, w_s);
		SelectObject(d.dcro, tempbmp);
		rotate(d.dcro, 4, 4, angle, tempdc, 0, 0, 9, 9, 0xfefefe);
	}
	if (d.t == Gift){
		tempbmp = CreateCompatibleBitmap(hdc, 150, 150);
		SelectObject(tempdc, gift);
		SelectObject(d.dcro, tempbmp);
		rotate(d.dcro, 75, 75, angle, tempdc, 0, 0, 150, 150, 0xfefefe);
	}
	DeleteObject(tempbmp);
}
void rotate(HDC hdcDest, int xPos, int yPos, int angle, HDC hdcSrc, int xSrc, int ySrc, int srcWidth, int srcHeight, COLORREF col)
{
	POINT pt[3];
	POINT defPt[3];
	double notPI = 3.14 / 180;

	double thetS, thetC;
	//待优化：删除long
	pt[0].x = LONG(-srcWidth * 0.5);
	pt[0].y = LONG(-srcHeight * 0.5);

	pt[1].x = pt[0].x + srcWidth;
	pt[1].y = pt[0].y;

	pt[2].x = pt[0].x;
	pt[2].y = pt[0].y + srcHeight;

	thetS = sin(angle * notPI);
	thetC = cos(angle * notPI);
	defPt[0].x = LONG((pt[0].x * thetC - pt[0].y * thetS) + xPos);
	defPt[0].y = LONG((pt[0].x * thetS + pt[0].y * thetC) + yPos);

	defPt[1].x = LONG((pt[1].x * thetC - pt[1].y * thetS) + xPos);
	defPt[1].y = LONG((pt[1].x * thetS + pt[1].y * thetC) + yPos);

	defPt[2].x = LONG((pt[2].x * thetC - pt[2].y * thetS) + xPos);
	defPt[2].y = LONG((pt[2].x * thetS + pt[2].y * thetC) + yPos);

	HBRUSH hBrush = CreateSolidBrush(col);
	RECT rect;
	rect.left = rect.top = 0;
	rect.right = rect.left + srcWidth;
	rect.bottom = rect.top + srcHeight;
	FillRect(hdcDest, &rect, hBrush);
	DeleteObject(hBrush);

	PlgBlt(hdcDest, &defPt[0], hdcSrc, xSrc, ySrc, srcWidth, srcHeight, 0, 0, 0);
}

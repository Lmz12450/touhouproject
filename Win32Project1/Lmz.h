#ifndef LMZ_H_
#define LMZ_H_
#include <math.h>
#include <tchar.h>
#include "DSAA.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h> 
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"msimg32.lib")
using namespace std;
enum DType { Heart, Rice, Dayu, Light, Zhongyu, Dao, Guangyu, Snow ,Gift};
enum DColor { Red, Green, Yellow, Blue, Pink, Cyan, Orange, Darkred, White };
HINSTANCE hInst;
HBITMAP   hbmp,hbmp2;
HDC	      mdc, hdc, bufdc, tempdc;
DWORD	tPre, tNow;
//关卡
int guan = 0;
MSG msg;
//贴图
HBITMAP bg_b, pl_b, kuang_b, lian_b, shot_b, hp_b, hp2_b, pd_b, tenwan_b, lihui1_b, lihui2_b, start_b, title_b, select_b, Pdian_b, Fdian_b, Ldian_b;
HBITMAP r_h, p_h, g_r, y_d, r_d, b_d, g_d, r_l, b_l, b_z, c_z, b_dao, c_dao, r_g, g_g, y_g, b_g, p_g, c_g, o_g, d_g, w_g, w_s,gift;
HBITMAP cname_b, cback_b, ascii_b, bosspos_b, heart_bottom_b, heart_up_b, end1_b, end2_b, end3_b;
//boss移动
int rx = 337, ry = 200;

//待优化：引用？
class Player
{
	//游戏框左上48，24，右下625，696
public:
	int life;
	//上下移动速度，左右移动速度，斜方向移动速度
	int speedx, speedy, speedxy;
	int x, y;
	//showx,showy,人物大小48,72
	int sx, sy;
	int dup, ddown, dleft, dright;
	bool fire;
	bool low;
	int power;
	int wudi;
	Player()
	{
		life = 4;
		speedx = 6;
		speedy = 6;
		speedxy = 3;
		x = 337; y = 650;
		sx = x - 24; sy = y - 36;
		dup = 0;
		ddown = 0;
		dleft = 0;
		dright = 0;
		fire = false;
		low = false;
		power = 200;
		wudi = -1;
	}
	void cmoveright()
	{
		dright = 0;
	}
	void cmoveleft()
	{
		dleft = 0;
	}
	void cmoveup()
	{
		dup = 0;
	}
	void cmovedown()
	{
		ddown = 0;
	}
	void moveright()
	{
		dright = 1;
	}
	void moveleft()
	{
		dleft = 1;
	}
	void moveup()
	{
		dup = 1;
	}
	void movedown()
	{
		ddown = 1;
	}
	void move()
	{
		if ((dup || ddown) && (dright || dleft)){
			x = x + speedxy*dright - speedxy*dleft;
			y = y + speedxy*ddown - speedxy*dup;
		}
		else{
			x = x + speedx*dright - speedx*dleft;
			y = y + speedy*ddown - speedy*dup;
		}
		if (x < 72) {
			x = 72;
		}
		if (x > 601) {
			x = 601;
		}
		if (y > 660) {
			y = 660;
		}
		if (y < 60) {
			y = 60;
		}
		sx = x - 24; sy = y - 36;
	}
	void disu(){
		speedx = 3;
		speedy = 3;
		speedxy = 2;
		low = true;
	}
	void cdisu(){
		speedx = 6;
		speedy = 6;
		speedxy = 3;
		low = false;
	}
};
class Boss
{
public:
	int x, y;
	int sx, sy;
	int hp;
	int speed;
	int movegif;
	int numlian;
	int cnumlian;
	Boss(){
		x = 337;
		y = 200;
		sx = x - 48; sy = y - 48;
		hp = 4000;
		speed = 2;
		movegif = 0;
		numlian = 0;
		cnumlian = 1;
	}
	int hit(int damage){
		//扣血阶段
		if ((hp > 3288 && hp  <= 4000)){
			hp -= damage / 100 * 4;
		}
		if (hp  > 3000 && hp <= 3288){
			hp -= 4;
		}
		if ((hp > 2288 && hp <= 3000)){
			hp -= damage / 100 * 3;
		}
		if (hp  > 2000 && hp <= 2288){
			hp -= 1 + damage / 200;
		}
		if ((hp > 1288 && hp <= 2000)){
			hp -= damage / 100 * 3;
		}
		if (hp  > 1000 && hp <= 1288){
			hp -= damage / 100 * 1;
		}
		if ((hp > 288 && hp <= 1000)){
			hp -= damage / 100 * 1;
		}
		if (hp  > 0 && hp <= 288){
			hp -= 1;
		}
		if (hp<0){
			return 9;
		}
		if ((hp%1000 > 288&&hp%1000<=1000)){
			return (4 - hp / 1000) * 2 - 1;
		}
		else if (hp%1000 >0&&hp%1000 <= 288){
			return (4 - hp / 1000) * 2;
		}
		else if (hp % 1000 == 0){
			return (4 - hp / 1000) * 2 + 1;
		}
	}
	void move(int rx,int ry){
		int flag = 0;
		if (rx+6 < x){
			//向左移动
			x -= speed;
			sx = x - 48;
			movegif = 3;
		}
		else if (rx>x){
			x += speed;
			sx = x - 48;
			movegif = 1;
		}
		else{
			flag ++;
		}
		if (ry +6< y){
			//向上移动
			y -= speed;
			sy = y - 48;
		}
		else if (ry>y){
			y += speed;
			sy = y - 48;
		}
		else {
			flag ++;
		}
		if (flag == 2){
			if (movegif != 2){
				movegif = 0;
			}
		}
		if (movegif == 2 && numlian == 0){
			movegif = 0;
		}
	}
	void Fadangif(){
		movegif = 2;
		numlian = 1;
		cnumlian = 1;
	}
};
class Danmaku
{
public:
	double x, y;
	int sx, sy;
	int length;
	double showrate;
	int r;
	DType t;
	DColor c;
	int angle;
	int speed;
	int life;
	int movelife;
	bool xz_active;
	bool snow;
	//用于储存旋转后位图的hdc
	HDC dcro;
	Danmaku(){

	}
	Danmaku(HDC hdc,int xx,int yy,DType tt,DColor cc,int aa,int ss)
	{
		x = xx;
		y = yy;
		t = tt;
		snow = false;
		if (t == Heart){
			length = 32;
			showrate = 1;
			r = 12;
		}
		if (t == Rice){
			length = 16;
			showrate = 1.5;
			r = 7;
		}
		if (t == Dayu){
			length = 64;
			showrate = 1.5;
			r = 18;
		}
		if (t == Light){
			length = 16;
			showrate = 2;
			r = 7;
		}
		if (t == Zhongyu){
			length = 16;
			showrate = 1.5;
			r = 7;
		}
		if (t == Dao){
			length = 32;
			showrate = 1.5;
			r = 7;
		}
		if (t == Guangyu){
			length = 16;
			showrate = 1.5;
			r = 7;
		}
		if (t == Snow){
			length = 8;
			showrate = 1.5;
			r = 4;
			snow = true;
		}
		if (t == Gift){
			length = 150;
			showrate = 1;
			r = 1;
		}
		sx = x - length/2*showrate;
		sy = y - length/2*showrate;
		c = cc;
		angle = aa;
		speed = ss;
		life = 1;
		movelife = -1;
		xz_active = true;
		dcro = CreateCompatibleDC(hdc);
	}
	int move(){
		//旋转type
		//active
		double notPI = 3.14 / 180;
		x += speed*sin(angle*notPI);
		y -= speed*cos(angle*notPI);
		//雪（SC8）
		if (snow == true){
			if (rand() % 2 == 0)
				angle += 1;
			else
				angle -= 1;
			xz_active = true;
		}
		//加速度
		//if （accelerate!=0)
		//xzactive
		sx = x - length / 2 * showrate;
		sy = y - length / 2 * showrate;
		movelife--;
		if (movelife == 0){
			return 1;
		}
		if (690<y ||y<30 || x<55 || 620<x){
			//击中版边
			life--;
			if (life == 0){
				return 1;
			}
			else{
				if (690 < y){
					angle = 180 - angle;
					y = 690;
				}
				if (y < 30){
					angle = 180 - angle;
					y = 30;
				}
				if (x < 55 ){
					angle = -angle;
					x = 55;
				}
				if (620 < x){
					angle = -angle;
					x = 620;
				}
				xz_active = true;
			}
			return 0;
		}
		else return 0;
	}
	void setlife(int a){
		life = a;
	}

};
class Shot
{
public:
	int x, y;
	//火力贴图间隔65
	int level;
	int sx, sy;
	int speed;
	Shot(){

	}
	Shot(Player &p)
	{
		x = p.x;
		y = p.y - 100;
		level = p.power/100-1;
		speed = 10;
		sx = x - 38;
		sy = y;
	}
	int move(Boss &b){
		y -= speed;
		sy = y;
		if ((y>b.y - 15) && (y<b.y + 15) && (x>b.x - 40) && (x < b.x + 40)){
			return 1;
		}
		if (y < 20){
			return 2;
		}
		else return 3;
	}
};
class Drop
{
public:
	int x, y;
	int sx, sy;
	int speed;
	int xi_r;
	int get_r;
	int angle;
	//1代表P，2代表F，3代表L
	int type;
	Drop(){
		
	}
	Drop(Boss &b,int t){
		//求[a,b]之间的整数：rand()%(b-a+1)+a
		x = rand() % 200 + b.x - 100;
		y = rand() % 200 + b.y - 100;
		type = t;
		if (t == 3){
			x = b.x;
			y = b.y;
		}
		sx = x - 12;
		sy = y - 12;
		speed = 3;
		xi_r = 50;
		get_r = 10;
		angle = 180;
	}
	int move(Player &p){
		double notPI = 3.14 / 180;
		x += speed*sin(angle*notPI);
		y -= speed*cos(angle*notPI);
		sx = x - 8;
		sy = y - 8;
		if ((y>p.y - get_r) && (y<p.y + get_r) && (x>p.x - get_r) && (x < p.x + get_r)){
			//获得
			return 1;
		}
		if ((y>p.y - xi_r) && (y<p.y + xi_r) && (x>p.x - xi_r) && (x < p.x + xi_r)){
			//吸引
			return 2;
		}
		if (y > 696){
			//版边
			return 3;
		}
		else return 4;
	}
};
//旋转函数的声明，不知道放哪
void				rotate(HDC hdcDest, int xPos, int yPos, int angle, HDC hdcSrc, int xSrc, int ySrc, int srcWidth, int srcHeight, COLORREF col);
void                rodan(Danmaku d, int angle);
//定义一个函数为虚函数，不代表函数为不被实现的函数。

//定义他为虚函数是为了允许用基类的指针来调用子类的这个函数。

//定义一个函数为纯虚函数，才代表函数没有被实现。

//定义纯虚函数是为了实现一个接口，起到一个规范的作用，规范继承这个类的程序员必须实现这个函数。
class Card
{
public:
	int timeline = 0;
	int dcount = 0;
	bool move_active = false;
	bool bosswudi = true;
	SingleLinkList<Danmaku> c_L;
	Card(){}
	virtual void action(Player &p, Boss &b){}
};
class C1_1_1 :public Card
{
	int anglein = 0;
	int rollspeed = 3;
	int showcount = 0;
public:
	C1_1_1(){
		move_active = true;
	}
	//求[a,b]之间的整数：rand()%(b-a+1)+a
	//游戏框左上48，24，右下625，696
	void create(Boss &b, int anglein){
		int anglecount = anglein;
		int x = 0, y = 0;
		x = b.x;
		y = b.y;
		for (int i = 0; i < 15; i++){
			Danmaku d = Danmaku(hdc, x, y, Rice, Green, anglecount, 3);
			dcount++;
			c_L.append(d);
			anglecount += 24;
		}
	}
	void create_2(Boss &b, int anglein){
		int anglecount = anglein;
		int x = 0, y = 0;
		x = b.x;
		y = b.y;
		for (int i = 0; i < 15; i++) {
			Danmaku d = Danmaku(hdc, x, y, Heart, Red, anglecount, 3);
			dcount++;
			c_L.append(d);
			anglecount += 24;
		}
	}
	void action(Player &p, Boss &b){
		showcount++;
		if (showcount >= 80){
			bosswudi = false;
			timeline++;
			if (timeline < 50){
				if (timeline % 5 == 0){
					create(b, anglein);
					anglein += rollspeed;
				}
				if (timeline == 1){
					PlaySound("SE_dan3.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
				if (timeline == 35){
					PlaySound("SE_dan3.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			if (timeline == 50){
				create_2(b, anglein);
			}
			if (timeline == 80){
				timeline = 0;
				rollspeed = -rollspeed;
				anglein = rand() % (360);
			}
		}
	}
};
class C1_1_2 :public Card
{
public:
	int showcount = 0;
	C1_1_2(){
		rx = 337;
		ry = 200;
	}
	//求[a,b]之间的整数：rand()%(b-a+1)+a
	//游戏框左上48，24，右下625，696,x轴宽度577，y轴长度672
	void create(int x ,int hou){
		for (int i = 0; i < 45; i++) {
			if (i<x || i>x + 5){
				Danmaku d = Danmaku(hdc, i * 15, 29+hou*25, Rice, Green, 180, 3);
				dcount++;
				c_L.append(d);
			}
		}
	}
	void create_2(int x){
		int e = rand() % (3-0)+1;
		DColor c;
		c = (DColor)e;
		Danmaku d = Danmaku(hdc, (x+2.5)*15, 53, Dayu, c, 180, 3);
		dcount++;
		c_L.append(d);
	}
	void action(Player &p, Boss &b){
		showcount++;
		rx = 337;
		ry = 200;
		if (showcount >= 250){
			timeline++;
			bosswudi = false;
			int x = rand() % (35 - 5) + 5 + 1;
			if (timeline == 3){
				PlaySound("SE_dan5.wav", NULL, SND_FILENAME | SND_ASYNC);
				create_2(x);
				for (int hou = 0; hou < 3; hou++){
					create(x, hou);
				}
			}
			if (timeline == 100){
				timeline = 0;
			}
		}
	}
};
class C1_1_3 :public Card
{
public:
	int showcount=0;
	C1_1_3(){
		move_active = true;
	}
	//求[a,b]之间的整数：rand()%(b-a+1)+a
	//游戏框左上48，24，右下625，696,x轴宽度577，y轴长度672
	void create(){
		int anglecount = rand() % 360;
		int x = 0, y = 0;
		x = rand() % (600 - 100 + 1) + 100;
		y = rand() % (250 - 50 +1) + 50;
		for (int i = 0; i < 20; i++){
			Danmaku d = Danmaku(hdc, x, y, Heart, Pink, anglecount, 2);
			dcount++;
			c_L.append(d);
			anglecount += 18;
		}
	}
	void create_2(){
		int anglecount = rand() % 360;
		int x = 0, y = 0;
		x = rand() % (600 - 100 + 1) + 100;
		y = rand() % (250 - 55 + 1) + 55;
		for (int i = 0; i < 20; i++){
			Danmaku d = Danmaku(hdc, x, y, Heart, Red, anglecount, 2);
			dcount++;
			c_L.append(d);
			anglecount += 18;
		}

	}
	void action(Player &p, Boss &b){
		showcount++;
		if (showcount >= 120){
			timeline++;
			bosswudi = false;
			if (timeline < 40){
				if (timeline % 12 == 0){
					create();
				}
			}
			else if (timeline < 80){
				if (timeline % 12 == 0){
					create_2();
				}
			}
			if (timeline % 20 == 0){
				PlaySound("SE_dan4.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			if (timeline == 80){
				timeline = 0;
			}
		}
	}
};
class C1_1_4 :public Card
{
public:
	int showcount = 0;
	C1_1_4(){
		move_active = true;
	}
	//求[a,b]之间的整数：rand()%(b-a+1)+a
	//游戏框左上48，24，右下625，696,x轴宽度577，y轴长度672
	void create(Player &p, Boss &b){
		int angle = (180/3.14)*atan2((p.y - b.y), (p.x - b.x -20))+90;
		Danmaku d = Danmaku(hdc, b.x + 20, b.y, Heart, Pink, angle, 3);
		d.showrate = 1.5;
		d.setlife(5);
		dcount++;
		c_L.append(d);
		angle = (180 / 3.14)*atan2((p.y - b.y), (p.x - b.x + 20)) + 90;
		Danmaku d1 = Danmaku(hdc, b.x - 20, b.y, Heart, Pink, angle, 3);
		d1.showrate = 1.5;
		d1.setlife(5);
		dcount++;
		c_L.append(d1);
	}
	void create_2(Player &p, Boss &b){
		
		Danmaku d = Danmaku(hdc, b.x, b.y, Heart, Pink, 130, 3);
		d.showrate = 1.5;
		d.setlife(40);
		dcount++;
		c_L.append(d);
		Danmaku d1 = Danmaku(hdc, b.x, b.y, Heart, Pink, -130, 3);
		d1.showrate = 1.5;
		d1.setlife(40);
		dcount++;
		c_L.append(d1);
	}
	void create_3(){
		Danmaku tempD;
		for (int i = 0; i < dcount; i++){
			int e = rand() % (2 - 1 + 1) + 1;
			DColor c;
			if (e == 1)
				c = Red;
			else
				c = Blue;
			e = rand() % (120 -0 + 1);
			tempD = c_L.getValue(i);
			if (tempD.t == Heart){
				Danmaku d = Danmaku(hdc, tempD.x, tempD.y, Light, c, 120+tempD.angle+e, 1);
				d.movelife = 50;
				dcount++;
				c_L.append(d);
			}
		}
	}
	void action(Player &p, Boss &b){
		showcount++;
		if (showcount >= 250){
			timeline++;
			bosswudi = false;
			if (timeline == 1){
				b.Fadangif();
				PlaySound("SE_dan5.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			if (timeline == 31){
				create(p, b);
			}
			if (timeline == 270){
				b.Fadangif();
				PlaySound("SE_dan5.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			if (timeline == 300){
				create_2(p, b);
			}
			if (timeline % 5 == 0){
				create_3();
			}
			if (timeline == 600){
				timeline = 0;
			}
		}
	}
};
class C1_1_5 :public Card
{
public:
	int showcount = 0;
	C1_1_5(){
		;
	}
	//求[a,b]之间的整数：rand()%(b-a+1)+a
	//游戏框左上48，24，右下625，696,x轴宽度577，y轴长度672
	void create(int i){
		int angle;
		int x = 0, y = 0;
		x = rand() % ((80 + i * 3) - (49 + i * 3) + 1) +49 + i * 3;
		y = rand() % ((680 - i * 21) - (650 - i * 21) + 1) + 650 - i * 21;
		angle = rand() % (120 - 95 + 1) + 95;
		if (dcount % 3 == 0){
			Danmaku d = Danmaku(hdc, x, y, Zhongyu, Cyan, angle, 6);
			dcount++;
			c_L.append(d);
		}
		else{
			Danmaku d = Danmaku(hdc, x, y, Zhongyu, Blue, angle, 5);
			dcount++;
			c_L.append(d);
		}
	}
	void action(Player &p, Boss &b){
		showcount++;
		rx = 337;
		ry = 200;
		if (showcount >= 120){
			timeline++;
			bosswudi = false;
			if (timeline == 5){
				PlaySound("SE_dan3.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			if (timeline == 10){
				for (int i = 0; i < 30; i++){
					create(i);
				}

			}
			if (timeline == 20){
				mciSendString("play SE_charge.wav", NULL, 0, NULL);
				b.Fadangif();
			}
			if (timeline == 150){
				timeline = 0;
			}
		}
	}
};
class C1_1_6 :public Card
{
public:
	int anglecount = 0;
	int rollspeed = 3;
	int showcount = 0;
	C1_1_6(){
		;
	}
	//求[a,b]之间的整数：rand()%(b-a+1)+a
	//游戏框左上48，24，右下625，696,x轴宽度577，y轴长度672
	void create(Boss &b){
		for (int i = 0; i < 20; i++){
			int x = b.x + sin(i * 18 * 3.14 / 180) * 200;
			int y = b.y - cos(i * 18 * 3.14 / 180) * 200;
			int angle = 180 + i * 18;
			Danmaku d = Danmaku(hdc, x ,y, Dao, Blue, angle, 3);
			d.movelife = 65;
			dcount++;
			c_L.append(d);
		}
	}
	void create_2(Player &p){
		for (int i = 0; i < 10; i++){
			int x = p.x + sin(i * 36 * 3.14 / 180) * 95;
			int y = p.y - cos(i * 36 * 3.14 / 180) * 95;
			int angle = 180 + i * 36;
			Danmaku d = Danmaku(hdc, x, y, Dao, Cyan, angle, 1);
			dcount++;
			c_L.append(d);
		}
	}
	void create_3(Boss &b,int anglecount){
		for (int i = 0; i < 20; i++){
			Danmaku d = Danmaku(hdc, b.x, b.y, Heart, Red, anglecount, 3);
			dcount++;
			c_L.append(d);
			anglecount += 18;
		}
	}
	void create_4(Boss &b,int anglecount){
		for (int i = 0; i < 20; i++){
			Danmaku d = Danmaku(hdc, b.x, b.y, Heart, Pink, anglecount, 3);
			dcount++;
			c_L.append(d);
			anglecount += 18;
		}
	}
	void action(Player &p, Boss &b){
		showcount++;
		rx = 337;
		ry = 300;
		if (showcount >= 250){
			timeline++;
			bosswudi = false;
			if (timeline == 1){
				anglecount = rand() % 360;
				PlaySound("SE_dan6.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			if (timeline == 10){
				create(b);
				create_2(p);
			}
			if (timeline >= 80 && timeline <= 160){
				if (timeline % 7 == 0){
					create_3(b, anglecount);
				}
				if (timeline % 20 == 0){
					PlaySound("SE_dan4.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			if (timeline == 160){
				PlaySound("SE_dan6.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			if (timeline == 170){
				create(b);
				create_2(p);
			}
			if (timeline == 240){
				anglecount = rand() % 360;
			}
			if (timeline >= 240&& timeline <= 320){
				if (timeline % 7 == 0){
					create_4(b, anglecount);
				}
				if (timeline % 20 == 0){
					PlaySound("SE_dan4.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			
			if (timeline == 321){
				timeline = 0;
			}
		}
	}
};
class C1_1_7 :public Card
{
public:
	int x=20;
	int y=-10;
	int e = 0;
	int anglecount = 0;
	int showcount = 0;
	C1_1_7(){
		move_active = true;
	}
	//求[a,b]之间的整数：rand()%(b-a+1)+a
	//游戏框左上48，24，右下625，696,x轴宽度577，y轴长度672
	void create(int x,int y,int anglecount){
//		int e = rand() % (8 - 0 + 1);
		DColor c;
		c = (DColor)e;
		for (int i = 0; i < 15; i++){
			Danmaku d = Danmaku(hdc, x, y, Guangyu, c, anglecount, 1);
			dcount++;
			c_L.append(d);
			anglecount += 24;
		}
	}
	void action(Player &p, Boss &b){
		showcount++;
		if (showcount >= 120){
			timeline++;
			bosswudi = false;
			if (timeline == 20){
				PlaySound("SE_dan3.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			if (timeline == 30){
				x += 60;
				if (x == 680){
					x = 80;
				}
				y +=40;
				if (y == 430){
					y = 30;
				}
				anglecount = rand() % 360;
				create(x, y,anglecount);
				create(x+(338-x)*2, y,-anglecount);
				e++;
				if (e == 9){
					e = 0;
				}
			}
			if (timeline == 40){
				timeline = 0;
			}
		}
		
	}
};
class C1_1_8 :public Card
{
public:
	int showcount = 0;
	int p = 0;
	int s = 3;
	int s1_x=367,s1_y=280;
	int co = 0;
	int endtime = 0;
	C1_1_8(){
		;
	}
	//求[a,b]之间的整数：rand()%(b-a+1)+a
	//游戏框左上48，24，右下625，696,x轴宽度577，y轴长度672
	void create(){
		for (int i = 0; i < 15; i++){
			int x = rand() % (600 - 100 + 1) + 100;
			int y = rand() % (100 - 30 + 1) + 30;
			Danmaku d = Danmaku(hdc, x, y, Snow, White, 180, 1);
			dcount++;
			c_L.append(d);
		}
	}
	void create_2(){
		Danmaku d = Danmaku(hdc, 337, 130, Gift, Pink, 180, 1);
		d.movelife = 150;
		dcount++;
		c_L.append(d);
	}
	void create_s1_1(Player &p){
		Danmaku d = Danmaku(hdc, 337, 280, Heart, Pink, 180, 2);
		d.showrate = 8;
		d.r = 80;
		dcount++;
		c_L.append(d);
	}
	void create_s1_2(){
		Danmaku d = Danmaku(hdc, s1_x + cos(co*3.14 / 180) * 80, s1_y - 100, Light, Red, 0, 2);
		d.movelife = 100;
		d.showrate = 3;
		dcount++;
		c_L.append(d);
	}
	void create_s1_3(){
		Danmaku d = Danmaku(hdc, s1_x - cos(co*3.14 / 180) * 80, s1_y - 100, Light, Blue, 0, 2);
		d.movelife = 100;
		d.showrate = 3;
		dcount++;
		c_L.append(d);
	}
	void create_s2_1(int x, int hou){
		for (int i = 0; i < 45; i++) {
			if (i<x || i>x + 5){
				Danmaku d = Danmaku(hdc, i * 15, 29 + hou * 25+250, Rice, Green, 180, 2);
				dcount++;
				c_L.append(d);
			}
		}
		for (int i = 0; i < 45; i++) {
			if (i<x || i>x + 5){
				Danmaku d1 = Danmaku(hdc, i * 15, 29 + hou * 25 + 175, Rice, Green, 180, 2);
				dcount++;
				c_L.append(d1);
			}
		}
		for (int i = 0; i < 45; i++) {
			if (i<x || i>x + 5){
				Danmaku d2 = Danmaku(hdc, i * 15, 29 + hou * 25 + 100, Rice, Green, 180, 2);
				dcount++;
				c_L.append(d2);
			}
		}
	}
	void create_s2_2(int x){
		int e = rand() % (3 - 0) + 1;
		DColor c;
		c = (DColor)e;
		Danmaku d = Danmaku(hdc, (x + 2.5) * 15, 53+250, Dayu, c, 180, 4);
		dcount++;
		c_L.append(d);
		e = rand() % (3 - 0) + 1;
		c = (DColor)e;
		Danmaku d1 = Danmaku(hdc, (x + 2.5) * 15, 53 + 175, Dayu, c, 180, 4);
		dcount++;
		c_L.append(d1);
		e = rand() % (3 - 0) + 1;
		c = (DColor)e;
		Danmaku d2 = Danmaku(hdc, (x + 2.5) * 15, 53 + 100, Dayu, c, 180, 4);
		dcount++;
		c_L.append(d2);
	}
	void create_s3_1(Player &p, Boss &b){
		for (int i = 0; i < 20; i++){
			int x = 337 + sin(i * 18 * 3.14 / 180) * 150;
			int y = 280 - cos(i * 18 * 3.14 / 180) * 150;
			int angle = (180 / 3.14)*atan2((p.y - y), (p.x - x)) + 90;
			Danmaku d = Danmaku(hdc, x, y, Dao, Blue, angle, 3);
			dcount++;
			c_L.append(d);
		}
	}
	void action(Player &p, Boss &b){
		showcount++;
		rx = 337;
		ry = 120;
		if (showcount >= 250){
			bosswudi = false;
			timeline++;
			if (timeline % 100==0){
				create();
			}
			if (timeline == 180){
				mciSendString("play SE_charge.wav", NULL, 0, NULL);
				b.Fadangif();
			}
			if (timeline == 290){
				PlaySound("SE_dan3.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			if (timeline == 300){
				create_2();
			}
			if (timeline == 440){
				mciSendString("play SE_feidie.wav", NULL, 0, NULL);
			}
			if (timeline == 450){
				s = rand() % (3 - 1 + 1) + 1;
				if (s == 1){
					create_s1_1(p);
					s1_x = 347;
					s1_y = 280;
					co = 0;
					endtime = 650;
				}
				if (s == 2){
					int s2 = rand() % (35 - 5) + 5 + 1;
					create_s2_2(s2);
					for (int hou = 0; hou < 3; hou++){
						create_s2_1(s2, hou);
					}
					endtime = 500;
				}
				if (s == 3){
					create_s3_1(p,b);
					endtime = 450;
				}
			}
			if (s == 1){
				if (timeline > 450 && timeline < 650){
					s1_y += 2;
					co += 3;
					if (timeline % 3 == 0){
						create_s1_2();
						create_s1_3();
					}
				}
			}
			if (timeline == endtime){
				timeline = 0;
			}
		}
	}
};
class C1_1_9 :public Card
{
public:
	int showcount = 0;
	C1_1_9(){
		;
	}
	void action(Player &p, Boss &b){
		showcount++;
		if (showcount >= 200){
			guan = 2;
		}

	}
};
#endif
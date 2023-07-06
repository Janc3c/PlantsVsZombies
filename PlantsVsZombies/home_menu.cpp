#include "home_menu.h"

void startUI(int* just) {
	IMAGE imgBg2, imgMENU1, imgMENU2, imgMENU_exit1, imgMENU_exit2;
	loadimage(&imgBg2, "res/menu.png");
	loadimage(&imgMENU1, "res/menu1.png");
	loadimage(&imgMENU2, "res/menu2.png");
	loadimage(&imgMENU_exit1, "res/menu_quit1.png");
	loadimage(&imgMENU_exit2, "res/menu_quit2.png");
	int flag = 1, flag_exit = 1;
	mciSendString("open res/audio/bg.mp3 alias bg", 0, 0, 0); 	//���ű������ֲ��޸�����
	mciSendString("play bg repeat", 0, 0, 0);
	mciSendString("setaudio bg volume to 300", 0, 0, 0);
	while (1) {
		BeginBatchDraw();
		putimage(0, 0, &imgBg2);
		putimagePNG(475, 75, flag ? &imgMENU1 : &imgMENU2);
		putimagePNG(812, 508, flag_exit ? &imgMENU_exit1 : &imgMENU_exit2);
		ExMessage msg;
		if (peekmessage(&msg)) {
			if (msg.message == WM_MOUSEMOVE &&
				msg.x > 474 && msg.x < 474 + 331 &&
				msg.y>75 && msg.y < 75 + 140) {
				flag = 0;
			}//
			else if (!(msg.x > 474 && msg.x < 474 + 331 &&
				msg.y>75 && msg.y < 75 + 140 ||
				msg.x > 812 && msg.x < 812 + 47 &&
				msg.y>508 && msg.y < 508 + 27)) {
				flag = 1;
				flag_exit = 1;
			}//�ж�����Ƿ��ڿ�ʼͼƬ������
			else if (msg.message == WM_LBUTTONDOWN &&
				msg.x > 474 && msg.x < 474 + 331 &&
				msg.y>75 && msg.y < 75 + 140) {
				flag = 2;
			}//�ж�����Ƿ���ͼƬ��Χ�ڽ���һ���������
			else if (msg.message == WM_LBUTTONUP && flag == 2) {
				*just = 1;
				break;
			}//�ж��������Ƿ��ɿ�һ��
			else if (msg.message == WM_MOUSEMOVE &&
				msg.x > 812 && msg.x < 812 + 47 &&
				msg.y>508 && msg.y < 508 + 27) {
				flag_exit = 0;
			}//
			else if (msg.message == WM_LBUTTONDOWN &&
				msg.x > 812 && msg.x < 812 + 47 &&
				msg.y>508 && msg.y < 508 + 27) {
				flag_exit = 2;
			}//�ж�����Ƿ���ͼƬ��Χ�ڽ���һ���������
			else if (msg.message == WM_LBUTTONUP && flag_exit == 2) {
				*just = 2;
				break;
			}//�ж��������Ƿ��ɿ�һ��
		}
		EndBatchDraw();
	}
}


void menu(int* just2,int *flag) {
		ExMessage msg;
		if (peekmessage(&msg)) {
			if (msg.message == WM_MOUSEMOVE &&
				msg.x > 888 && msg.x < 1000 &&
				msg.y>0 && msg.y < 32) {
				*flag = 0;
				just2 = 0;
			}
			else if (!(msg.x > 888 && msg.x < 1000 &&
				msg.y>0 && msg.y < 32)) {
				*flag = 1;
			}//�ж�����Ƿ���ͼƬ������
			else if (msg.message == WM_LBUTTONDOWN &&
				msg.x > 888 && msg.x < 1000 &&
				msg.y>0 && msg.y < 32) {
				*flag = 2;
			}//�ж�����Ƿ���ͼƬ��Χ�ڽ���һ���������
			else if (msg.message == WM_LBUTTONUP && *flag == 2) {
				*just2 = 1;
			
			}//�ж��������Ƿ��ɿ�һ��
	}
}


void pause_page(int *just4,int *a) {
	ExMessage msg;
	if (peekmessage(&msg)) {
		if (msg.message == WM_MOUSEMOVE &&
			msg.x > 417 && msg.x < 417+241 &&
			msg.y>355 && msg.y < 355+46) {
			a[0] = 1;
			a[1] = a[2] = 0;
		}//����Ϸ
		else if (msg.message == WM_MOUSEMOVE && 
			msg.x > 417 && msg.x <417+241 &&
			msg.y>405 && msg.y < 405+46) {
			a[1] = 1;
			a[0] = a[2] = 0;
		}//���˵�
		else if (msg.message == WM_MOUSEMOVE && 
			msg.x > 370 && msg.x < 370+336 &&
			msg.y>478 && msg.y < 478+75) {
			a[2] = 1;
			a[1] = a[0] = 0;
		}//����
		else if (!(msg.x > 888 && msg.x < 1000 &&
			msg.y>0 && msg.y < 32|| 
			msg.x > 417 && msg.x < 417 + 241 &&
			msg.y>405 && msg.y < 405 + 46||
			msg.x > 370 && msg.x < 370 + 336 &&
			msg.y>478 && msg.y < 478 + 75)) {
			a[0] = a[1] = a[2] = 0;
		}//�ж�����Ƿ���ͼƬ������
		if (msg.message == WM_LBUTTONDOWN &&
			msg.x > 417 && msg.x < 417 + 241 &&
			msg.y>355 && msg.y < 355 + 46) {
			*just4 = 11;
		}//�ж�����Ƿ���ͼƬ��Χ�ڽ���һ���������
		else if (msg.message == WM_LBUTTONUP && *just4== 11) {
			*just4 = 1;
		}//�ж��������Ƿ��ɿ�һ��
		else if (msg.message == WM_LBUTTONDOWN &&
			msg.x > 417 && msg.x < 417 + 241 &&
			msg.y>405 && msg.y < 405 + 46) {
			*just4 = 22;
		}
		else if (msg.message == WM_LBUTTONUP && *just4 == 22) {
			*just4 = 2;
		}
		else if (msg.message == WM_LBUTTONDOWN &&
			msg.x > 370 && msg.x < 370 + 336 &&
			msg.y>478 && msg.y < 478 + 75) {
				*just4 = 33;
		}
		else if (msg.message == WM_LBUTTONUP && *just4 == 33) {
			*just4 = 3;
		}
	}


}
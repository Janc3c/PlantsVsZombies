#include "game.h"

#define WIN_WID 900
#define WIN_HIG 600

enum { PeaShooter, SunFlower, WallNut, PotatoMine, CherryBomb, CardCount };
IMAGE* plant[CardCount][20];
bool judgePlant = false;		//�ж��Ƿ����ֲ��
int curX, curY;					//��ǰֲ���ƶ������е�λ��
bool fileExist(char* name);		//�ж��ļ��Ƿ����
int index = -1;					//��ǰֲ������ֵ
IMAGE back_ground_img;
IMAGE bar_img;
IMAGE card_img[CardCount];

bool fileExist(char* name) {
	FILE* fp = fopen(name, "r");
	if (fp == NULL) {
		return false;
	}
	else {
		fclose(fp);
		return true;
	}
}

void InitGame() {
	//������Ϸ����
	loadimage(&back_ground_img, "res/map0.jpg");
	//����ֲ�￨��
	loadimage(&bar_img, "res/bar5.png");

	memset(plant, 0, sizeof(plant));
	//����ֲ�￨Ƭ
	for (int i = 0; i < CardCount; i++) {
		char name[64];
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);
		loadimage(&card_img[i], name);

		for (int j = 0; j < 20; j++) {
			sprintf_s(name,sizeof(name),"res/Plants/%d/%d.png",i,j+1);
			if (fileExist(name)) {
				plant[i][j] = new IMAGE;
				loadimage(plant[i][j], name);
			}
			else {
				break;
			}
		}
	}

	//������Ϸ����
	initgraph(WIN_WID, WIN_HIG);
}

void UpdateWindow() {
	//��ʼ����
	BeginBatchDraw();

	//����
	putimage(0, 0, &back_ground_img);
	//ֲ�￨��
	putimagePNG(250, 0, &bar_img);
	//ֲ�￨��
	for (int i = 0; i < CardCount; i++) {
		putimagePNG(338 + i * 64, 6, &card_img[i]);
	}

	//�϶������е�ֲ��
	if (index >= 0) {
		putimagePNG(curX,curY,plant[index][0]);
	}

	//��������
	EndBatchDraw();
}

void Click() {
	ExMessage msg;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN && judgePlant == false) {
			if (msg.x > 338 && msg.x < 338 + 64 * CardCount && msg.y>6 && msg.y < 96) {
				index = (msg.x - 338) / 64;
				judgePlant = true;
				printf("%d\n", index);
			}
		}
		else if (msg.message == WM_MOUSEMOVE && judgePlant == true) {
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONDOWN && judgePlant == true) {
			index = -1;
			judgePlant = false;
		}
	}
}

void GameStart() {
	while (true){
		Click();
		UpdateWindow();
		
	}
}
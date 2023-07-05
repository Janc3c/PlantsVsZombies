#include "game.h"

#define WIN_WID 1000
#define WIN_HIG 600
#define curX00 180							//��һ�е�һ�в�ƺ������Ͻ�λ�õ�xֵ����ȥ120����ԭ256��
#define curY00 95							//��һ�е�һ�в�ƺ������Ͻ�λ�õ�yֵ
#define cur_Height 100						//ÿһ����ƺ���x�ĳ���
#define cur_Width 81					    //ÿһ����ƺ���y�ĳ���

enum { PeaShooter, SunFlower, WallNut, PotatoMine, CherryBomb, CardCount };
IMAGE* Plants[CardCount][20];	//ֲ��ͼƬ
int CardNums[CardCount] = {0};	//ֲ��ͼƬ����
bool judgePlant = false;		//�ж��Ƿ����ֲ��
int curX, curY;					//��ǰֲ���ƶ������е�λ��
bool fileExist(char* name);		//�ж��ļ��Ƿ����
int index = -1;					//��ǰֲ������ֵ
IMAGE back_ground_img;			//��Ϸ����ͼƬ
IMAGE bar_img;					//ֲ�￨��ͼƬ
IMAGE card_img[CardCount];		//ֲ�￨ƬͼƬ
//ֲ��
struct plant {
	int type;					//ֲ�����࣬0����
	int frame;					//��ǰ�ǵڼ�֡
};
//ȫ��ͼֲ������
struct plant AllMap[5][9] = {0};
int timer = 0;					//����ʱ����

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

	memset(Plants, 0, sizeof(Plants));
	memset(AllMap, 0, sizeof(AllMap));
	//����ֲ�￨Ƭ
	for (int i = 0; i < CardCount; i++) {
		char name[64];
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);
		loadimage(&card_img[i], name);

		for (int j = 0; j < 20; j++) {
			sprintf_s(name,sizeof(name),"res/Plants/%d/%d.png",i,j+1);
			if (fileExist(name)) {
				Plants[i][j] = new IMAGE;
				loadimage(Plants[i][j], name);
				CardNums[i]++;
			}
			else {
				break;
			}
		}
	}



	//������Ϸ����
	initgraph(WIN_WID, WIN_HIG, EX_SHOWCONSOLE);
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
	if (index >= 1) {
		IMAGE* zhiwu = Plants[index-1][0];
		putimagePNG(curX - zhiwu->getwidth()/2, curY - zhiwu->getheight()/2, zhiwu);
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 9; j++) {
			if (AllMap[i][j].type > 0) {
				int x = curX00 + j * cur_Width;
				int y = curY00 + i * cur_Height;
				int plant_type = AllMap[i][j].type - 1;
				int page = AllMap[i][j].frame;
				putimagePNG(x, y, Plants[plant_type][page]);
			}
		}
	}

	//��������
	EndBatchDraw();
}

//����ж�
void Click() {
	ExMessage msg;
	if (peekmessage(&msg)) {
		//�״ε��ֲ�￨Ƭ
		if (msg.message == WM_LBUTTONDOWN && judgePlant == false) {
			if (msg.x > 338 && msg.x < 338 + 64 * CardCount && msg.y>6 && msg.y < 96) {
				index = (msg.x - 338) / 64 + 1;
				judgePlant = true;
				curX = msg.x;
				curY = msg.y;
			}
		}
		else if (msg.message == WM_MOUSEMOVE && judgePlant == true) {
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONDOWN && judgePlant == true) {
			if (msg.x >= curX00 && msg.x <= curX00 + cur_Width * 9 && msg.y >= curY00 && msg.y <= curY00 + cur_Height * 5) {
				int row = (msg.y - curY00) / cur_Height;
				int col = (msg.x - curX00) / cur_Width;
				
				//��ֲ
				if (AllMap[row][col].type == 0) {
					AllMap[row][col] = {
						index,
						0
					};
				}
			}
			index = 0;
			judgePlant = false;
		}
	}
}


void UpdateGame() {
	for (int i = 0; i < 5; i++)	{
		for (int j = 0; j < 9; j++) {
			int n = AllMap[i][j].type - 1;
			if (AllMap[i][j].type > 0) {
				if (AllMap[i][j].frame < CardNums[n] - 1) {
					AllMap[i][j].frame++;
				}
				else {
					AllMap[i][j].frame = 0;
				}
			}
		}
	}
}

void GameStart() {
	bool flag = false;
	while (true){
		Click();
		timer += getDelay();
		if (timer > 100) {
			flag = true;
			timer = 0;
		}
		UpdateWindow();
		if (flag) {
			flag = false;
			UpdateGame();
		}
		
	}
}
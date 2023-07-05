#include "game.h"

#define WIN_WID 1000
#define WIN_HIG 600
#define curX00 180							//��һ�е�һ�в�ƺ������Ͻ�λ�õ�xֵ����ȥ120����ԭ256��
#define curY00 95							//��һ�е�һ�в�ƺ������Ͻ�λ�õ�yֵ
#define cur_Height 100						//ÿһ����ƺ���x�ĳ���
#define cur_Width 81					    //ÿһ����ƺ���y�ĳ���
#define zmNum 10                            //��ʬ����

enum { PeaShooter, SunFlower, WallNut, PotatoMine, CherryBomb,CardCount };
IMAGE* Plants[CardCount][20];	//ֲ��ͼƬ
int CardNums[CardCount] = { 0 };	//ֲ��ͼƬ����
bool judgePlant = false;		//�ж��Ƿ����ֲ��
int curX, curY;					//��ǰֲ���ƶ������е�λ��
bool fileExist(char* name);		//�ж��ļ��Ƿ����
int index = -1;					//��ǰֲ������ֵ
IMAGE back_ground_img;			//��Ϸ����ͼƬ
IMAGE bar_img;					//ֲ�￨��ͼƬ
IMAGE card_img[CardCount];		//ֲ�￨ƬͼƬ
IMAGE sun_img[29];				//����ͼƬ
//ֲ��
struct plant {
	int type;					//ֲ�����࣬0����
	int frame;					//��ǰ�ǵڼ�֡
};
//ȫ��ͼֲ������
struct plant AllMap[5][9] = { 0 };
int timer = 0;					//����ʱ����
//����
struct SunShine {
	int x, y;					//��ǰ����
	int frame;					//����֡���
	int endY;					//Ʈ���Ŀ��y����
	bool used;					//�Ƿ�ʹ��
	int timer;					//�����ʱ��
};

//�����
struct SunShine balls[10];

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

struct zm
{
	int x, y;//�ý�ʬ���ڵ�����
	int frame;//�ý�ʬ��ǰ��ʾ��ͼƬ֡λ����
	bool used;//�Ƿ����
	int speed;//�ƶ��ٶ�
};
struct zm zms[zmNum];//��ʬ������10��
IMAGE imgZM[22];//���潩ʬÿһ֡������IMG����


void InitGame() {
	//������Ϸ����
	loadimage(&back_ground_img, "res/map0.jpg");
	//����ֲ�￨��
	loadimage(&bar_img, "res/bar5.png");
	char name[64];
	memset(Plants, 0, sizeof(Plants));
	memset(AllMap, 0, sizeof(AllMap));
	memset(balls, 0, sizeof(balls));
	//����ֲ�￨Ƭ
	for (int i = 0; i < CardCount; i++) {
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);
		loadimage(&card_img[i], name);

		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "res/Plants/%d/%d.png", i, j + 1);
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


	//��������ͼƬ
	for (int i = 0; i < 29; i++) {
		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
		loadimage(&sun_img[i], name);
	}

	//�������
	srand(time(NULL));

	//��ʼ����ʬ
	memset(zms, 0, sizeof(zms));
	for (int i = 0; i < 22; i++) {
		char name[64];
		sprintf_s(name, sizeof(name), "res/zm/0/%d.png", i + 1);
		loadimage(&imgZM[i], name);
	}


	//������Ϸ����
	initgraph(WIN_WID, WIN_HIG, EX_SHOWCONSOLE);
}

//��Ϸ����
void PutBackGround() {
	//����
	putimage(0, 0, &back_ground_img);
	//ֲ�￨��
	putimagePNG(250, 0, &bar_img);
	//ֲ�￨��
	for (int i = 0; i < CardCount; i++) {
		putimagePNG(338 + i * 64, 6, &card_img[i]);
	}
}

//��ֲ���ֲ��
void PutPlants() {
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
}

//�϶������е�ֲ��
void PutDrag() {
	if (index >= 1) {
		IMAGE* zhiwu = Plants[index - 1][0];
		putimagePNG(curX - zhiwu->getwidth() / 2, curY - zhiwu->getheight() / 2, zhiwu);
	}
}

//�����Ʈ�������
void PutSunShine1() {
	int ballnums = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < ballnums; i++) {
		if (balls[i].used) {
			int n = balls[i].frame;
			putimagePNG(balls[i].x, balls[i].y, &sun_img[n]);
		}
	}
}


//��Ϸ����
void UpdateWindow() {
	//��ʼ����
	BeginBatchDraw();

	//��Ϸ����
	PutBackGround();

	//��ֲ���ֲ��
	PutPlants();

	//�϶������е�ֲ��
	PutDrag();

	//������ɵ�����
	PutSunShine1();

	//��Ⱦ��ʬͼƬ
	for (int i = 0; i < zmNum; i++) {
		if (zms[i].used) {
			IMAGE* img = &imgZM[zms[i].frame];
			putimagePNG(zms[i].x,
				zms[i].y - (img->getheight()),
				img);
		}
	}

	//���½�ʬͼƬ֡λ��
	static int count2 = 0;
	if (count2++ == 100) {//�����������������൱��createZM���е�count
		count2 = 0;
		for (int i = 0; i < zmNum; i++) {
			if (zms[i].used) {
				if (zms[i].frame++ == 21)
					zms[i].frame = 0;
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


//����ֲ�ﶯ��֡
void PlantSwing() {
	for (int i = 0; i < 5; i++) {
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

//���������������
void CreateSunshine() {
	static int count = 0;		//��̬����
	static int fre = 100;
	count++;
	if (count >= fre) {
		fre = 100 + rand() % 100;
		count = 0;

		//�����������ȡһ������ʹ�õ�����
		int ballnums = sizeof(balls) / sizeof(balls[0]);

		int i;
		for (i = 0; i < ballnums && balls[i].used; i++);
		if (i >= ballnums)
			return;

		balls[i].used = true;
		balls[i].frame = 0;
		balls[i].x = curX00 + (rand() % (9 * cur_Width));	//�����������x��
		balls[i].endY = curY00 + ((rand() % 5) * cur_Height);//�������������յ�Y����
		balls[i].y = 60;
		balls[i].timer = 0;
		printf("%d\n", i);
	}
}

//��������״̬
void UpdateSunshine() {
	int ballnums = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < ballnums; i++) {
		if (balls[i].used) {
			balls[i].frame = (balls[i].frame + 1) % 29;

			if (balls[i].y / balls[i].endY) {
				if (balls[i].timer <= 120) {
					balls[i].timer++;
				}
				else {
					balls[i].used = false;
					balls[i].timer = 0;
				}
			}
			else {
				balls[i].y += 2;
			}
		}
	}
}


void createZM() {
	static int zmFre = 0;//������ʬ��֡�������ʼ200
	static int count = 0;//��Ϸ֡������
	if (count++ > zmFre) {//֡����������֡���ʱ�Ŵ�����ʬ�������޲���
		count = 0;//֡����������
		zmFre = rand() % 300 + 200;//֡����������

		//������ʬ
		int i;
		for (i = 0; i < zmNum && zms[i].used; i++);
		if (i < zmNum) {
			zms[i].used = true;
			zms[i].x = WIN_WID;
			zms[i].y = curY00 + cur_Height * (1 + rand() % 5);
			zms[i].speed = 1;
		}

	}

}

void updateZM() {
	static int count = 0;
	count++;
	if (count > 2) {
		count = 0;
		for (int i = 0; i < zmNum; i++) {
			//��ʬλ�ø���
			if (zms[i].used) {
				zms[i].x -= zms[i].speed;
				if (zms[i].x <= curX00)
				{
					//��Ϸʧ��
					MessageBox(NULL, "over", "over", 0);
					exit(0);
				}
			}
		}
	}
}


//������Ϸ����Ϣ
void UpdateGame() {
	PlantSwing();
	CreateSunshine();
	UpdateSunshine();

	createZM();//ÿһ֡����һ�εķ���������ʬ
	updateZM();//ÿһ֡ˢ��һ�ν�ʬ
}

//��ʼ��Ϸ
void GameStart() {
	bool flag = false;
	mciSendString("play res/audio/readysetplant.mp3", 0, 0, 0);
	mciSendString("open res/audio/grasswalk.mp3 alias bg2", 0, 0, 0);
	mciSendString("play bg2 repeat", 0, 0, 0);
	mciSendString("setaudio bg2 volume to 300", 0, 0, 0);
	while (true){
		Click();
		timer += getDelay();
		if (timer > 40) {
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
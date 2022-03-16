#if defined (_WIN64) || defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#endif 

#include <stdio.h>
#include <stdlib.h>
#include "ColorSamples.h"
#include "GLUT_Core.h"
#include "GLUT_Texture.h"
#include "GLUT_Model.h"
#include "ActiveBlock_LotateDefine.h" //アクティブブロックの回転処理に使うデータ

#include <math.h>
#include <time.h>


#define NUM 100
#define WINDOW_SIZE 500
//ウィンドウのサイズ。一応可変だけど変えるとアルファベットや数字の表示がずれるかもしれない
static double left = -WINDOW_SIZE;
static double right = WINDOW_SIZE + 260;
static double bottom = -WINDOW_SIZE - 130;
static double top = WINDOW_SIZE + 130;
//正十二角形の中心座標は(x=0,y=0)とする

static double near = -1.5;
static double far = 1.5;
static int SelectedIndex = -1;
static double gWorldMouseX = 0.;
static double gWorldMouseY = 0.;
static int gMouseDragging = 0;

static unsigned int texID = 0;
static unsigned int objID = 0;

/*** Forward declaration of Callback functions for GLUT ***/
void callback_display(void);
void callback_resize(const int w, const int h);
void callback_mouse(const int button, const int state, const int x, const int y);
void callback_motion(const int x, const int y);
void callback_keyboard(const unsigned char key, const int x, const int y);
void callback_idle(void);
void callback_initialization(void);
void callback_finalization(void);

void base_calculation();
void base_display();
void define_color();
void define_active_block();
void define_active_block2();
void active_block_display();
int move_active_block();
int ActiveBlock_Hantei_Base();//アクティブブロックの下面の判定
int ActiveBlock_Hantei_Left();//アクティブブロックの左面の判定
int ActiveBlock_Hantei_Right();//アクティブブロックの右面の判定
int ActiveBlock_Hantei_Rotate();//アクティブブロック自体が左・右に回転したときに、
								//回転した移動先にベースブロックがあるかないかの判定。無ければ1を返す

void base_rotation(int);//ベースの回転
void base_down(); //ブロックが横一列(つまり一周)並んだ時の処理
void ActiveBlock_Lotate();//アクティブブロックの回転処理。下のActiveBlock_LotateStateとセット

void Gameover_Hantei();//ベースブロックがActiveBlock_NUM段目まで積み重なったら判定内で
					   //Gameover_Display()を実行する
void Gameover_Display(); //GAMEOVERと表示する。
void Start();//初期画面。ここでSを入力すると実際のゲームが開始される
void Retry(); // ゲームオーバー後に再スタートするための処理
void Score_Display(); //スコアを表示するための処理
void NextBlock_Display(); //次以降に降って来るアクティブブロックの表示。4つ先まで見られる
void TETRIS_Display(); //4段同時消しによる「TETRIS!」表示用

double angle = 0;
static time_t autoDrop_CurrentTime, autoDrop_PastTime; //アクティブブロックの自動降下用
static int active_Random;	//現在降下しているアクティブブロック用の乱数
static int next_Random[4];		//ネクストブロック用の乱数

static int ActiveBlock_LotateState; //アクティブブロックの回転状態。0～3の整数。
									//0は定義直後（デフォルト）
									//1は0から左回りに90°、2は180°、3は270°となる。
static int ActiveBlock_LotateDirection;//アクティブブロックの回転方向。
									   //0は反時計(左)回り、1は時計(右)回り
static int score; //プレイヤーが獲得した点数。消した列数×100点。
double move_y[4][4][2];//アクティブブロックの変化量
static double ActiveBlock_DropSpeed = 0.8;//アクティブブロックの降下速度。あんまり意味ないかも…
										  //C言語の仕様上、時間を秒単位までしか取得できないのでSpeedは整数値じゃないと意味がない
										  //0.8なら1に、1.2なら2に…のように実質は小数点以下切り上げの動作となる

static int Window_State = 0; //0はスタート画面、1はゲーム画面、2ならゲームオーバーの画面。
							 //0でSが押されたら1にする
							 //1でゲームオーバーになったら2になる。
							 //2でRが押されたら0にする

static int TETRIS_Display_Count; //これが4になったら「TETRIS!」を表示する
static int TETRIS_Display_State; //「TETRIS!」表示のための管理用変数。
									 //上のが4になったらこれが1になり、1秒後0となる。（表示終了）
static time_t TETRIS_Display_pastTime, TETRIS_Display_currentTime; //上の「1秒」の管理用


double active_block[4][4][2];
/*
active_block[a][b][c]
active_block:操作中のブロック
a = 操作中の4個のブロックのうち何個目か
b = 左下(内側の辺を底辺とする)から反時計回りに数えて何個目の頂点か
c = 0:頂点のx座標
1:頂点のy座標
*/

#define ActiveBlock_NUM 12	//アクティブブロックの中心の段数。列は0で固定
#define a_NUM 16
//base_arrayの段数
double base_array[a_NUM][12][3];
/*
base_array[a][b][c]
base_array:背景、ブロックを置く枠一つ一つ
a = 中心からの距離（段数）
b = a段目で上から反時計回りに何個目か
c = 0:x座標
1:y座標
2:色&ブロックがあるか無いか。
[][][2]==-1:無し(白色),
[][][2]!=-1:あり(白以外、つまりブロックあり)
※ブロック自体の色は「白」以外で定義する。
*/

int active_block_index[4][2]; //4個のアクティブブロックそれぞれの左下のxy座標の、
							  //base_array上で言う（対応している）インデックス
int ActiveBlockIndex_Tmp[4][2]; //上のやつの一時保存用
double color[12][4]; //12色。変数格納の処理は下にあるが、ヘッダファイルに少し記述を加えた（12色を新たに定義した）


int main(int argc, char *argv[]) {
	GLUTCreateWindow(argc, argv, NULL);
	GLUTStartLoop();
	return 0;
}
void init() { printf("score = %d", score); }
void finit() {  }

void SwapArray(int num, double *array1, double *array2) {
	int i;
	double tmp;
	for (i = 0; i<num; ++i) {
		tmp = array1[i];
		array1[i] = array2[i];
		array2[i] = tmp;
	}
}
void Mouse2World(const int x, const int y, double *worldX, double *worldY) {
	const double off = 0.5;
	const WindowSize win = getWindowInfo();
	const double rateX = ((double)(x)+off) / (double)(win.width - 1);
	const double rateY = 1. - (double)y / (double)win.height;
	*worldX = rateX*(right - left) + left;
	*worldY = rateY*(top - bottom) + bottom;
}
double Distance(const double x1, const double y1, const double x2, const double y2) {
	return sqrt(fmod(x2 - x1, 2.) + fmod(y2 - y1, 2.));
}

void callback_display(void) {
	char title[100];
	double org_lw = GetLineWidth();

	sprintf(title, "回転テトリス FPS: %4.2lf", getFPS());
	setWindowTitle(title);
	setColor(white);
	//activeRotation();
	SetLineWidth(3.);
	/* draw xyz-axises*/
	/*
	setColor(cyan);
	drawLine2D(left,0.,right,0.);
	drawLine2D(0.,top,0.,bottom);
	drawLine3D(0.,0.,near,0.,0.,far);
	setColor(black);
	*/
	if (Window_State == 0) {
		Start();
	}
	else if (Window_State == 1) {
		active_block_display();
		Score_Display();
		NextBlock_Display();
		time(&autoDrop_CurrentTime); time(&TETRIS_Display_currentTime);
		if ((TETRIS_Display_currentTime - TETRIS_Display_pastTime >= 0) &&
			(TETRIS_Display_currentTime - TETRIS_Display_pastTime < 2)) {
			TETRIS_Display();
		}
		base_display();
	}
	else if (Window_State == 2) {
		Gameover_Display();
		Score_Display();
		NextBlock_Display();
		if ((TETRIS_Display_currentTime - TETRIS_Display_pastTime >= 0) &&
			(TETRIS_Display_currentTime - TETRIS_Display_pastTime < 2)) {
			TETRIS_Display();
		}
		base_display();
	}
	SetLineWidth(org_lw);
}
void callback_resize(const int w, const int h) {
	//  fixedWindowSize();
	fixedWindowAspect(w, h);
	//  Window win = getWindowInfo();
	OrthogonalProjection(left, right, bottom, top, near, far);
	//  PerspectiveProjection(left, right, bottom, top, near, far);
	//  BugResize(w,h);
}
void callback_mouse(const int button, const int state,
	const int x, const int y) {
	double worldX = 0., worldY = 0.;
	if (button == 0) { /* left button */
		if (state == 0) { /* pushed */
			Mouse2World(x, y, &worldX, &worldY);
			gMouseDragging = 1;
		}
		else {
			gMouseDragging = -1;
		}
	}
}
void callback_motion(const int x, const int y) {
	double worldX = 0., worldY = 0.;
	Mouse2World(x, y, &worldX, &worldY);
	gWorldMouseX = worldX;
	gWorldMouseY = worldY;
}
void callback_keyboard(const unsigned char key, const int x, const int y) {
	static int n;
	(void)x, (void)y;

	switch (key) {
	case 'd'://ベースの右回転。なので、
			 //アクティブブロックの左側にベースブロックが左からぶつかってくる可能性があるため、
			 //その判定（アクティブブロックの左側にベースブロックがあるかどうか）を行う。
		if (ActiveBlock_Hantei_Left() == 1) {
			angle -= 2 * M_PI / 12;
			base_rotation(1);
		}
		break;
	case 'a':////ベースの左回転。なので、
			 //アクティブブロックの右側にベースブロックが右からぶつかってくる可能性があるため、
			 //その判定（アクティブブロックの右側にベースブロックがあるかどうか）を行う。
		if (ActiveBlock_Hantei_Right() == 1) {
			angle += 2 * M_PI / 12;
			base_rotation(-1);
		}
		break;
	case 's'://アクティブブロックの下降、および初期画面から実際のゲーム画面への遷移用
		if (Window_State == 0) {
			Window_State = 1;
		}
		else {
			n = move_active_block();
			if (n == 1) define_active_block();
		}
		break;
	case 'j'://アクティブブロックの左回転
		ActiveBlock_LotateDirection = 0;
		ActiveBlock_Lotate();
		break;
	case 'l'://アクティブブロックの右回転
		ActiveBlock_LotateDirection = 1;
		ActiveBlock_Lotate();
		break;
	case 'r':
		if (Window_State == 2) { //ゲームオーバー時にRキー降下した場合リスタート（最初から）
			Retry();
		}
		break;
	case '\033':
		exit(0);
		break;
	default:
		break;
	}
}
void callback_idle(void) {}

void callback_initialization(void) {
	clearColor(white);
	setWindowTitle("Hello world!!");
	printf("start of program.\n");
	init();
	base_calculation();
	define_color();
	static time_t t; time(&t); srand((int)t); //乱数生成
	static int i;
	for (i = 0; i < 4; i++)next_Random[i] = rand() % 7;

	score = 0; TETRIS_Display_State = 0;
	define_active_block();
	time(&autoDrop_PastTime); time(&autoDrop_CurrentTime);
}
void callback_finalization(void) {
	finit();
	printf("end of program.\n");
}

void base_calculation() {
	static double r;
	static int i, j;
	r = 300 / cos(M_PI / 12); //外接円の半径
	for (i = 0; i < a_NUM; i++) {
		for (j = 0; j < 12; j++) {
			base_array[i][j][0] = -sin(j*(M_PI / 6) + (M_PI / 12)) * (r*(i + 2) / 10);
			base_array[i][j][1] = cos(j*(M_PI / 6) + (M_PI / 12)) * (r*(i + 2) / 10);
			if (i == 0) {
				base_array[i][j][2] = 1;
			}
			else {
				base_array[i][j][2] = -1;
			}
		}
	}
}

void base_display() {
	static int x[4], y[4];
	int i, j, k;

	for (i = 0; i < a_NUM - 1; i++) {
		for (j = 0; j < 12; j++) {
			x[0] = (int)base_array[i][j % 12][0];
			y[0] = (int)base_array[i][j % 12][1];
			x[1] = (int)base_array[i][(j + 11) % 12][0];
			y[1] = (int)base_array[i][(j + 11) % 12][1];
			x[2] = (int)base_array[i + 1][(j + 11) % 12][0];
			y[2] = (int)base_array[i + 1][(j + 11) % 12][1];
			x[3] = (int)base_array[i + 1][j % 12][0];
			y[3] = (int)base_array[i + 1][j % 12][1];

			pushMat();
			rotateD(angle, 0, 0, 1);
			if (i == 0 && j == 0) {
				for (k = 0; k < 12; k++) {
					setColor(color[k]);
					drawTriangle2D(0, 0, base_array[0][(k + 11) % 12][0], base_array[0][(k + 11) % 12][1],
						base_array[0][k][0], base_array[0][k][1]);
				}
			}
			setColor(color[j]);
			drawLine2D(x[0], y[0], x[1], y[1]);
			drawLine2D(x[1], y[1], x[2], y[2]);
			drawLine2D(x[2], y[2], x[3], y[3]);
			drawLine2D(x[3], y[3], x[0], y[0]);
			if (i == 0) {
				drawLine2D(x[0], y[0], x[2], y[2]);
				drawLine2D(x[1], y[1], x[3], y[3]);
			}

			if (i == 14) {
				//drawLine2D(x[0], y[0], x[2], y[2]);
				//drawLine2D(x[1], y[1], x[3], y[3]);
				drawLine2D((int)((x[3] + x[0]) / 2), (int)((y[3] + y[0]) / 2),
					(int)((x[2] + x[1]) / 2), (int)((y[2] + y[1]) / 2));
				drawLine2D((int)((x[0] + ((x[3] + x[0]) / 2)) / 2), (int)((y[0] + ((y[3] + y[0]) / 2)) / 2),
					(int)((x[1] + ((x[2] + x[1]) / 2)) / 2), (int)((y[1] + ((y[2] + y[1]) / 2)) / 2));
				drawLine2D((int)((x[3] + ((x[3] + x[0]) / 2)) / 2), (int)((y[3] + ((y[3] + y[0]) / 2)) / 2),
					(int)((x[2] + ((x[2] + x[1]) / 2)) / 2), (int)((y[2] + ((y[2] + y[1]) / 2)) / 2));
			}
			popMat();

			if (base_array[i][j][2] != -1 && i>0) {
				setColor(cyan);
				drawSquare2I(x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3]);
			}
		}
	}
}

void define_color() {
	int i;
	for (i = 0; i < 12; i++) {
		color[i][0] = color_array[i][0];
		color[i][1] = color_array[i][1];
		color[i][2] = color_array[i][2];
		color[i][3] = color_array[i][3];
	}
}

void define_active_block() {
	static int j, k;
	for (j = 0; j < 4; j++) {	//左下[12][0][]を最初に決め、それから反時計回りに残り3箇所の座標を
								//定義していく
								//[12][0][]、[12][11][]、[13][11][]、[13][0][]の順
		for (k = 0; k < 2; k++) {
			active_block[0][j][k] = base_array[12 + (j / 2)][(12 - (j == 1 || j == 2)) % 12][k];
			move_y[0][j][k] = base_array[0][(12 - (j == 1 || j == 2)) % 12][k];
		}
	}
	active_block_index[0][0] = ActiveBlock_NUM;
	active_block_index[0][1] = 0;
	ActiveBlockIndex_Tmp[0][0] = active_block_index[0][0]; //一時保存用
	ActiveBlockIndex_Tmp[0][1] = active_block_index[0][1];

	ActiveBlock_LotateState = 0;//アクティブブロックの回転状態を初期のものにする
	define_active_block2();//アクティブブロック4個のうちの残り3個を定義する
}

void active_block_display() {
	int i, j;
	static int x[4], y[4];
	static int n = 0;
	if (autoDrop_CurrentTime - autoDrop_PastTime > ActiveBlock_DropSpeed) {
		n = move_active_block();
		if (n == 1)define_active_block();
		autoDrop_PastTime = autoDrop_CurrentTime;
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			x[j] = (int)active_block[i][j][0];
			y[j] = (int)active_block[i][j][1];
		}
		setColor(blue);
		drawSquare2I(x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3]);
	}
}

void define_active_block2() {
	static int i, j, k;
	active_Random = next_Random[0];
	switch (active_Random) {
	case 0:ActiveBlock_LotateDefine_00(); break; //初期段階では回転状態は0なのでこの関数で
												 //indexを定義できる
	case 1:ActiveBlock_LotateDefine_10(); break;
	case 2:ActiveBlock_LotateDefine_20(); break;
	case 3:ActiveBlock_LotateDefine_30(); break;
	case 4:ActiveBlock_LotateDefine_40(); break;
	case 5:ActiveBlock_LotateDefine_50(); break;
	case 6:ActiveBlock_LotateDefine_60(); break;
	default:break;
	}
	for (j = 1; j < 4; j++) {
		for (k = 0; k < 2; k++) {
			active_block_index[j][k] = ActiveBlockIndex_Tmp[j][k];
		}
	}//わざわざTmpを使っている理由は、
	 //のちのアクティブブロックの回転処理を実現するためである

	 //active_block_indexを利用してアクティブブロックのxy座標とy方向の移動量を定義する
	for (i = 1; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 2; k++) {
				active_block[i][j][k] = base_array[active_block_index[i][0] + (j / 2)][(12 + active_block_index[i][1] - (j == 1 || j == 2)) % 12][k];
				move_y[i][j][k] = base_array[0][(12 + active_block_index[i][1] - (j == 1 || j == 2)) % 12][k];
			}
		}
	}

	for (i = 0; i < 3; i++) {
		next_Random[i] = next_Random[i + 1];
	}
	next_Random[3] = rand() % 7;
}

int move_active_block() {
	int i, j, k;
	static int m = 0;

	m = ActiveBlock_Hantei_Base();
	if (m == 1) {
		for (i = 0; i < 4; i++) {
			base_array[active_block_index[i][0]][active_block_index[i][1]][2] = 1;
		}
		base_down();
		Gameover_Hantei();
		return 1;
	}

	for (i = 0; i < 4; i++) {
		active_block_index[i][0] -= 1;
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 2; k++) {
				active_block[i][j][k] -= (move_y[i][j][k] / 2);
			}
		}
	}

	return 0;
}

int ActiveBlock_Hantei_Base() {
	static int i;
	i = 0;
	while (i < 4) {  //1個でもアクティブブロックがベースに触れていたらwhileをi<4で抜ける
		if ((int)base_array[active_block_index[i][0] - 1][active_block_index[i][1]][2] == 1) {
			break;
		}else{
			i++;
		}
	}
	if (i < 4)return 1;
	else return 0;
}

int ActiveBlock_Hantei_Left() {
	static int i;
	i = 0;
	while ((int)base_array[active_block_index[i][0]][(active_block_index[i][1] + 1) % 12][2] != 1 && i < 4)i++;
	if (i < 4)return 0;
	else return 1;
}

int ActiveBlock_Hantei_Right() {
	static int i;
	i = 0;
	while (base_array[active_block_index[i][0]][(12 + active_block_index[i][1] - 1) % 12][2] != 1 && i < 4)i++;
	//1個でもアクティブブロックが右のベースブロックに触れていたらwhileをi<4で抜ける
	if (i < 4)return 0;
	else return 1;
}

int ActiveBlock_Hantei_Rotate() {
	static int i;
	i = 0;
	while ((int)base_array[ActiveBlockIndex_Tmp[i][0]][ActiveBlockIndex_Tmp[i][1]][2] != 1 && i < 4)i++;
	//1個でもアクティブブロックの移動先にベースブロックがあったらwhileをi<4で抜ける
	if (i < 4)return 0;
	else return 1;
}

void base_rotation(int rotate) {	//rotate=1なら時計回り、-1なら反時計回り
	static double tmp_haiti;
	int i, j;
	if (rotate == 1) {
		for (i = 0; i < a_NUM - 1; i++) {
			tmp_haiti = base_array[i][0][2];
			for (j = 0; j < 11; j++) {
				base_array[i][j][2] = base_array[i][j + 1][2];
			}
			base_array[i][11][2] = tmp_haiti;
		}
	}
	else if (rotate == -1) {
		for (i = 0; i < a_NUM - 1; i++) {
			tmp_haiti = base_array[i][11][2];
			for (j = 10; j >= 0; j--) {
				base_array[i][j + 1][2] = base_array[i][j][2];
			}
			base_array[i][0][2] = tmp_haiti;
		}
	}
}

void base_down() {
	static int i, j, k;
	TETRIS_Display_Count = 0;
	for (i = 1; i < active_block_index[0][0] + 3; i++) {
		for (;;) {
			j = 0;
			while (base_array[i][j][2] == 1 && j < 12)j++;
			if (j >= 12) {
				for (j = 0; j < 12; j++) {
					for (k = i; k < a_NUM - 2; k++) {
						base_array[k][j][2] = base_array[k + 1][j][2];
					}
				}
				score += 100; TETRIS_Display_Count += 1;
			}
			else break;
		}
	}

	if (TETRIS_Display_Count >= 4) {
		TETRIS_Display_State = 1;
		TETRIS_Display_pastTime = TETRIS_Display_currentTime;
		time(&TETRIS_Display_currentTime);
	}
}

void ActiveBlock_Lotate() {
	switch (ActiveBlock_LotateDirection * 100 + active_Random * 10 + ActiveBlock_LotateState) {
	case 0: ActiveBlock_LotateDefine_01(); break;
	case 1: ActiveBlock_LotateDefine_02(); break;
	case 2: ActiveBlock_LotateDefine_03(); break;
	case 3: ActiveBlock_LotateDefine_00(); break;
	case 10: ActiveBlock_LotateDefine_11(); break;
	case 11: ActiveBlock_LotateDefine_12(); break;
	case 12: ActiveBlock_LotateDefine_13(); break;
	case 13: ActiveBlock_LotateDefine_10(); break;
	case 20: ActiveBlock_LotateDefine_21(); break;
	case 21: ActiveBlock_LotateDefine_22(); break;
	case 22: ActiveBlock_LotateDefine_23(); break;
	case 23: ActiveBlock_LotateDefine_20(); break;
	case 30: ActiveBlock_LotateDefine_31(); break;
	case 31: ActiveBlock_LotateDefine_32(); break;
	case 32: ActiveBlock_LotateDefine_33(); break;
	case 33: ActiveBlock_LotateDefine_30(); break;
	case 40: ActiveBlock_LotateDefine_41(); break;
	case 41: ActiveBlock_LotateDefine_42(); break;
	case 42: ActiveBlock_LotateDefine_43(); break;
	case 43: ActiveBlock_LotateDefine_40(); break;
	case 50: ActiveBlock_LotateDefine_51(); break;
	case 51: ActiveBlock_LotateDefine_52(); break;
	case 52: ActiveBlock_LotateDefine_53(); break;
	case 53: ActiveBlock_LotateDefine_50(); break;
	case 60: ActiveBlock_LotateDefine_61(); break;
	case 61: ActiveBlock_LotateDefine_62(); break;
	case 62: ActiveBlock_LotateDefine_63(); break;
	case 63: ActiveBlock_LotateDefine_60(); break;

	case 100: ActiveBlock_LotateDefine_03(); break;
	case 101: ActiveBlock_LotateDefine_00(); break;
	case 102: ActiveBlock_LotateDefine_01(); break;
	case 103: ActiveBlock_LotateDefine_02(); break;
	case 110: ActiveBlock_LotateDefine_13(); break;
	case 111: ActiveBlock_LotateDefine_10(); break;
	case 112: ActiveBlock_LotateDefine_11(); break;
	case 113: ActiveBlock_LotateDefine_12(); break;
	case 120: ActiveBlock_LotateDefine_23(); break;
	case 121: ActiveBlock_LotateDefine_20(); break;
	case 122: ActiveBlock_LotateDefine_21(); break;
	case 123: ActiveBlock_LotateDefine_22(); break;
	case 130: ActiveBlock_LotateDefine_33(); break;
	case 131: ActiveBlock_LotateDefine_30(); break;
	case 132: ActiveBlock_LotateDefine_31(); break;
	case 133: ActiveBlock_LotateDefine_32(); break;
	case 140: ActiveBlock_LotateDefine_43(); break;
	case 141: ActiveBlock_LotateDefine_40(); break;
	case 142: ActiveBlock_LotateDefine_41(); break;
	case 143: ActiveBlock_LotateDefine_42(); break;
	case 150: ActiveBlock_LotateDefine_53(); break;
	case 151: ActiveBlock_LotateDefine_50(); break;
	case 152: ActiveBlock_LotateDefine_51(); break;
	case 153: ActiveBlock_LotateDefine_52(); break;
	case 160: ActiveBlock_LotateDefine_63(); break;
	case 161: ActiveBlock_LotateDefine_60(); break;
	case 162: ActiveBlock_LotateDefine_61(); break;
	case 163: ActiveBlock_LotateDefine_62(); break;

	default: break;
	}

	if (ActiveBlock_Hantei_Rotate() == 1) {
		if (ActiveBlock_LotateDirection == 0)//左回りの場合
			ActiveBlock_LotateState = (ActiveBlock_LotateState + 1) % 4;
		else if (ActiveBlock_LotateDirection == 1)//右回りの場合
			ActiveBlock_LotateState = (ActiveBlock_LotateState + 3) % 4;
		//回転方向に応じてActiveBlock_LotateStateを更新

		static int i, j, k;
		for (j = 1; j < 4; j++) {
			for (k = 0; k < 2; k++) {
				active_block_index[j][k] = ActiveBlockIndex_Tmp[j][k];
			}
		}


		for (i = 1; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				for (k = 0; k < 2; k++) {
					active_block[i][j][k] = base_array[active_block_index[i][0] + (j / 2)][(12 + active_block_index[i][1] - (j == 1 || j == 2)) % 12][k];
					move_y[i][j][k] = base_array[0][(12 + active_block_index[i][1] - (j == 1 || j == 2)) % 12][k];
				}
			}
		}
	}
}

void Gameover_Hantei() {
	if (base_array[12][0][2] == 1 || base_array[12][1][2] == 1 ||
		base_array[12][10][2] == 1 || base_array[12][11][2] == 1) {
		Window_State = 2; //GAMEOVERと表示するための処理。
						  //そこからRキーが押されたら再び最初からゲームを開始
	}
}

void Gameover_Display() {
	static int i;
	for (i = 0; i < 4; i++) {
		base_array[active_block_index[i][0]][active_block_index[i][1]][2] = 1;
	}

	//アルファベットを1文字ずつ、1画ずつ表示していく
	//1文字の大きさを90×90とする。文字と文字の間は20離れている
	//各文字において、(0,0)を中心とする。左下(-45,-45)、右下(45,-45)、右上(45,45)、左上(-45,45)
	//表示段階では、xの方に補正するなどとしてそれぞれの文字の表示場所に合わせるようにする。
	//GAMEとOVERの間は80離れている？

	//1260 = 90*8+20*6+80+(左の残りの幅)*2 //この80は、GAMEとOVERの間の80のことである
	//(左の残りの幅) = 170
	//つまり-WINDOW_SIZE+(左の残りの幅) = -330(x座標)から表示を開始すればよい。

	static t; setColor(black);
	for (i = 0; i < 8; i++) {
		t = (-330 + 40 + 110 * i) + 80 * (i>3); //文字の配置補正用
		switch (i) {
		case 0: //文字'G'の表示
			drawLine2D(45 + t, 45, -45 + t, 45);
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, -45, 45 + t, -45);
			drawLine2D(45 + t, -45, 45 + t, 0);
			drawLine2D(45 + t, 0, 0 + t, 0);
			break;
		case 1: //文字'A'の表示
			drawLine2D(-45 + t, -45, 0 + t, 45);
			drawLine2D(0 + t, 45, 45 + t, -45);
			drawLine2D(-22 + t, 0, 22 + t, 0);
			break;
		case 2: //文字'M'の表示
			drawLine2D(-45 + t, -45, -45 + t, 45);
			drawLine2D(-45 + t, 45, 0 + t, -45);
			drawLine2D(0 + t, -45, 45 + t, 45);
			drawLine2D(45 + t, 45, 45 + t, -45);
			break;
		case 3: //文字'E'の表示
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(-45 + t, 0, 45 + t, 0);
			drawLine2D(-45 + t, -45, 45 + t, -45);
			break;
		case 4: //文字'O'の表示
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, -45, 45 + t, -45);
			drawLine2D(45 + t, -45, 45 + t, 45);
			drawLine2D(45 + t, 45, -45 + t, 45);
			break;
		case 5: //文字'V'の表示
			drawLine2D(-45 + t, 45, 0 + t, -45);
			drawLine2D(0 + t, -45, 45 + t, 45);
			break;
		case 6: //文字'E'の表示
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(-45 + t, 0, 45 + t, 0);
			drawLine2D(-45 + t, -45, 45 + t, -45);
			break;
		case 7: //文字'R'の表示
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(45 + t, 45, 45 + t, 0);
			drawLine2D(45 + t, 0, -45 + t, 0);
			drawLine2D(0 + t, 0, 45 + t, -45);
			break;
		default:break;
		}
	}

	setColor(white);
	drawSquare2I(-WINDOW_SIZE, (int)(-WINDOW_SIZE / 5), WINDOW_SIZE + 260, (int)(-WINDOW_SIZE / 5),
		WINDOW_SIZE + 260, (int)(WINDOW_SIZE / 5), -WINDOW_SIZE, (int)(WINDOW_SIZE / 5));

}
void Start() {
	//1文字の大きさを90×90とする。文字と文字の間は20離れている
	//各文字において、(0,0)を中心とする。左下(-45,-45)、右下(45,-45)、右上(45,45)、左上(-45,45)
	//表示段階では、xの方に補正するなどとしてそれぞれの文字の表示場所に合わせるようにする。
	//GAMEとOVERの間は80離れている？

	//1260 = 90*5+40*4+(左の残りの幅)*2
	//(左の残りの幅) = 315
	//つまり-WINDOW_SIZE+(左の残りの幅) = -185(x座標)から表示を開始すればよい。

	static int i, t; setColor(black);
	for (i = 0; i < 8; i++) {
		t = -185 + 45 + 130 * i; //文字の配置補正用
		switch (i) {
		case 0: //文字'S'の表示
			drawLine2D(45 + t, 45, -45 + t, 45);
			drawLine2D(-45 + t, 45, -45 + t, 0);
			drawLine2D(-45 + t, 0, 45 + t, 0);
			drawLine2D(45 + t, 0, 45 + t, -45);
			drawLine2D(45 + t, -45, -45 + t, -45);
			break;
		case 1: //文字'T'の表示
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(0 + t, 45, 0 + t, -45);
			break;
		case 2: //文字'A'の表示
			drawLine2D(-45 + t, -45, 0 + t, 45);
			drawLine2D(0 + t, 45, 45 + t, -45);
			drawLine2D(-22 + t, 0, 22 + t, 0);
			break;
		case 3: //文字'R'の表示
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(45 + t, 45, 45 + t, 0);
			drawLine2D(45 + t, 0, -45 + t, 0);
			drawLine2D(0 + t, 0, 45 + t, -45);
			break;
		case 4: //文字'T'の表示
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(0 + t, 45, 0 + t, -45);
			break;
		default:break;
		}
	}
}

void Retry() {
	Window_State = 0;
	score = 0;
	static int i, j;
	for (i = 1; i < a_NUM - 1; i++) { //ベースブロックを全て消す
		for (j = 0; j < 12; j++) {
			base_array[i][j][2] = -1;
		}
	}
	define_active_block();
	time(&autoDrop_PastTime); time(&autoDrop_CurrentTime);
}

void Score_Display() {
	//無限ループ内でscoreを一度(int型の)score_Tmpなるものに保存しておき、
	//switch(score_Tmp%10)、score_Tmp /= score_Tmp;
	//とすることで1の位から順に値(score_tmp%10)を取り出しそれによってswitch文で条件分岐。
	//値によって1桁ずつ右から順にスコアを表示させる。
	//表示されるデジタル調の数字の左上の角を基準とする。（それをiを用いて右から管理していく）

	//1つの数字の大きさを縦80×横44とし、数時間の隙間は15とする。
	//最大4桁まで正常に表示されるとして、
	//250 = 4*44 + 3*15 + (右の残りの幅)*2
	//(右の残りの幅)=14であるため、		（※左の残りの幅はどうでもいい）
	//(WINDOW_SIZE-20) - (右の残りの幅) = 480-14 = 466(x座標)から表示を開始すればよい。
	//また、数字とスコアを表示する領域の枠線との幅は上下それぞれ20とする。（固定）

	static int score_Tmp, i, x, y, tx, ty;
	score_Tmp = score; i = 0;
	x = 22; y = 40;
	setColor(blue);
	for (;;) {
		tx = 466 - 22 - (59 * i);
		ty = (WINDOW_SIZE + 130 - 70); //tx,tyは文字の場所の補正用
		switch ((int)(score_Tmp % 10)) {
		case 0:
			drawLine2D(-x + tx, y + ty, -x + tx, -y + ty); //縦80×横44
			drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
			drawLine2D(x + tx, -y + ty, x + tx, y + ty);
			drawLine2D(x + tx, y + ty, -x + tx, y + ty);
			break;
		case 1:
			drawLine2D(x + tx, -y + ty, x + tx, y + ty);
			break;
		case 2:
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(x + tx, y + ty, x + tx, 0 + ty);
			drawLine2D(x + tx, 0 + ty, -x + tx, 0 + ty);
			drawLine2D(-x + tx, 0 + ty, -x + tx, -y + ty);
			drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
			break;
		case 3:
			drawLine2D(x + tx, -y + ty, x + tx, y + ty);
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(x + tx, 0 + ty, -x + tx, 0 + ty);
			drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
			break;
		case 4:
			drawLine2D(-x + tx, y + ty, -x + tx, 0 + ty);
			drawLine2D(-x + tx, 0 + ty, x + tx, 0 + ty);
			drawLine2D(x + tx, -y + ty, x + tx, y + ty);
			break;
		case 5:
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(-x + tx, y + ty, -x + tx, 0 + ty);
			drawLine2D(x + tx, 0 + ty, -x + tx, 0 + ty);
			drawLine2D(x + tx, 0 + ty, x + tx, -y + ty);
			drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
			break;
		case 6:
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(-x + tx, y + ty, -x + tx, -y + ty);
			drawLine2D(x + tx, 0 + ty, -x + tx, 0 + ty);
			drawLine2D(x + tx, 0 + ty, x + tx, -y + ty);
			drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
			break;
		case 7:
			drawLine2D(x + tx, -y + ty, x + tx, y + ty);
			drawLine2D(x + tx, y + ty, -x + tx, y + ty);
			break;
		case 8:
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(-x + tx, y + ty, -x + tx, -y + ty);
			drawLine2D(x + tx, 0 + ty, -x + tx, 0 + ty);
			drawLine2D(x + tx, y + ty, x + tx, -y + ty);
			drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
			break;
		case 9:
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(-x + tx, y + ty, -x + tx, 0 + ty);
			drawLine2D(x + tx, 0 + ty, -x + tx, 0 + ty);
			drawLine2D(x + tx, y + ty, x + tx, -y + ty);
			drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
			break;
		default:break;
		}

		score_Tmp = (int)(score_Tmp / 10);
		if (score_Tmp == 0)break;
		else i++;
	}

	setColor(white);
	drawSquare2I(WINDOW_SIZE - 270, WINDOW_SIZE + 130 - 120, WINDOW_SIZE - 20, WINDOW_SIZE + 130 - 120,
		WINDOW_SIZE - 20, WINDOW_SIZE + 130 - 20, WINDOW_SIZE - 270, WINDOW_SIZE + 130 - 20);
	setColor(red);
	drawLine2D(WINDOW_SIZE - 270, WINDOW_SIZE + 130 - 120, WINDOW_SIZE - 20, WINDOW_SIZE + 130 - 120);
	drawLine2D(WINDOW_SIZE - 20, WINDOW_SIZE + 130 - 120, WINDOW_SIZE - 20, WINDOW_SIZE + 130 - 20);
	drawLine2D(WINDOW_SIZE - 20, WINDOW_SIZE + 130 - 20, WINDOW_SIZE - 270, WINDOW_SIZE + 130 - 20);
	drawLine2D(WINDOW_SIZE - 270, WINDOW_SIZE + 130 - 20, WINDOW_SIZE - 270, WINDOW_SIZE + 130 - 120);
}

void NextBlock_Display() {
	//260 = 44*4+16*3+(左の残りの幅)*2
	//(左の残りの幅) = 18
	//つまり、WINDOW_SIZE+(左の残りの幅) = 518(x座標)から表示を開始すればよい。

	static int i, j, x, y, tx, ty; setColor(white);
	x = 22; y = 40;
	for (i = 0; i < 4; i++) {
		tx = 518 + 22 + 60 * i; //文字の配置補正用
		ty = (WINDOW_SIZE + 130) - 10 - 100;
		switch (i) {
		case 0: //文字'N'の表示
			drawLine2D(-x + tx, -y + ty, -x + tx, y + ty);
			drawLine2D(-x + tx, y + ty, x + tx, -y + ty);
			drawLine2D(x + tx, -y + ty, x + tx, y + ty);
			break;
		case 1: //文字'E'の表示
			drawLine2D(-x + tx, -y + ty, -x + tx, y + ty);
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(-x + tx, 0 + ty, x + tx, 0 + ty);
			drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
			break;
		case 2: //文字'X'の表示
			drawLine2D(-x + tx, y + ty, x + tx, -y + ty);
			drawLine2D(x + tx, y + ty, -x + tx, -y + ty);
			break;
		case 3: //文字'T'の表示
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(0 + tx, y + ty, 0 + tx, -y + ty);
			break;
		default:break;
		}
	}

	//ここにネクストブロックの表示処理を書く
	static int nextBlock_Center[4][2] = {
		{ 630,310 },{ 630,30 },{ 630,-210 },{ 630,-410 }
	};
	static int nextBlock_x[4], nextBlock_y[4];
	setColor(blue);
	for (i = 0; i < 4; i++) {
		tx = 8 * (6 - i); ty = 5 * (6 - i);
		switch (next_Random[i]) {
		case 0:	//表示用ウィンドウの中心から見て4つの各ブロックの左下の角の座標がどれだけ離れているかを書く
			nextBlock_x[0] = -tx / 2;	nextBlock_y[0] = -ty;
			nextBlock_x[1] = -tx / 2;	nextBlock_y[1] = 0;
			nextBlock_x[2] = -3 * tx / 2; nextBlock_y[2] = -ty;
			nextBlock_x[3] = tx / 2;	nextBlock_y[3] = -ty;
			break;
		case 1:
			nextBlock_x[0] = -tx / 2;	nextBlock_y[0] = -ty;
			nextBlock_x[1] = -3 * tx / 2; nextBlock_y[1] = 0;
			nextBlock_x[2] = -3 * tx / 2; nextBlock_y[2] = -ty;
			nextBlock_x[3] = tx / 2;	nextBlock_y[3] = -ty;
			break;
		case 2:
			nextBlock_x[0] = -tx / 2;	 nextBlock_y[0] = -ty;
			nextBlock_x[1] = tx / 2;	nextBlock_y[1] = 0;
			nextBlock_x[2] = -3 * tx / 2; nextBlock_y[2] = -ty;
			nextBlock_x[3] = tx / 2;	nextBlock_y[3] = -ty;
			break;
		case 3:
			nextBlock_x[0] = -tx / 2; nextBlock_y[0] = 0;
			nextBlock_x[1] = -tx / 2; nextBlock_y[1] = ty;
			nextBlock_x[2] = -tx / 2; nextBlock_y[2] = -ty;
			nextBlock_x[3] = -tx / 2; nextBlock_y[3] = -2 * ty;
			break;
		case 4:
			nextBlock_x[0] = -tx;	 nextBlock_y[0] = 0;
			nextBlock_x[1] = 0;		nextBlock_y[1] = 0;
			nextBlock_x[2] = -tx;	 nextBlock_y[2] = -ty;
			nextBlock_x[3] = 0;		 nextBlock_y[3] = -ty;
			break;
		case 5:
			nextBlock_x[0] = -tx / 2;	nextBlock_y[0] = -ty;
			nextBlock_x[1] = -3 * tx / 2; nextBlock_y[1] = 0;
			nextBlock_x[2] = -tx / 2;	nextBlock_y[2] = 0;
			nextBlock_x[3] = tx / 2;	 nextBlock_y[3] = -ty;
			break;
		case 6:
			nextBlock_x[0] = -tx / 2;	 nextBlock_y[0] = -ty;
			nextBlock_x[1] = tx / 2;	nextBlock_y[1] = 0;
			nextBlock_x[2] = -tx / 2;	nextBlock_y[2] = 0;
			nextBlock_x[3] = -3 * tx / 2; nextBlock_y[3] = -ty;
			break;
		default: break;
		}

		for (j = 0; j < 4; j++) {
			nextBlock_x[j] += nextBlock_Center[i][0];
			nextBlock_y[j] += nextBlock_Center[i][1];
			drawSquare2I(nextBlock_x[j], nextBlock_y[j], nextBlock_x[j] + tx, nextBlock_y[j],
				nextBlock_x[j] + tx, nextBlock_y[j] + ty, nextBlock_x[j], nextBlock_y[j] + ty);
		}
	}

	setColor(white);
	drawSquare2I(WINDOW_SIZE + 10 + 20 * 0, WINDOW_SIZE + 130 - 200 - 240 - 60 * 0, //1個目のネクストブロック用ウィンドウ
		WINDOW_SIZE + 260 - 10 - 20 * 0, WINDOW_SIZE + 130 - 200 - 240 - 60 * 0,	//中心は(630,310)
		WINDOW_SIZE + 260 - 10 - 20 * 0, WINDOW_SIZE + 130 - 200 - 60 * 0,
		WINDOW_SIZE + 10 + 20 * 0, WINDOW_SIZE + 130 - 200 - 60 * 0);
	drawSquare2I(WINDOW_SIZE + 10 + 20 * 1, WINDOW_SIZE + 130 - 200 - 440 - 60 * 1, //2個目のネクストブロック用ウィンドウ
		WINDOW_SIZE + 260 - 10 - 20 * 1, WINDOW_SIZE + 130 - 200 - 440 - 60 * 1,	//中心は(630,30)
		WINDOW_SIZE + 260 - 10 - 20 * 1, WINDOW_SIZE + 130 - 200 - 240 - 60 * 1,
		WINDOW_SIZE + 10 + 20 * 1, WINDOW_SIZE + 130 - 200 - 240 - 60 * 1);
	drawSquare2I(WINDOW_SIZE + 10 + 20 * 2, WINDOW_SIZE + 130 - 200 - 600 - 60 * 2, //3個目のネクストブロック用ウィンドウ
		WINDOW_SIZE + 260 - 10 - 20 * 2, WINDOW_SIZE + 130 - 200 - 600 - 60 * 2,	//中心は(630,-210)
		WINDOW_SIZE + 260 - 10 - 20 * 2, WINDOW_SIZE + 130 - 200 - 440 - 60 * 2,
		WINDOW_SIZE + 10 + 20 * 2, WINDOW_SIZE + 130 - 200 - 440 - 60 * 2);
	drawSquare2I(WINDOW_SIZE + 10 + 20 * 3, WINDOW_SIZE + 130 - 200 - 720 - 60 * 3, //4個目のネクストブロック用ウィンドウ
		WINDOW_SIZE + 260 - 10 - 20 * 3, WINDOW_SIZE + 130 - 200 - 720 - 60 * 3,	//中心は(630,-410)
		WINDOW_SIZE + 260 - 10 - 20 * 3, WINDOW_SIZE + 130 - 200 - 600 - 60 * 3,
		WINDOW_SIZE + 10 + 20 * 3, WINDOW_SIZE + 130 - 200 - 600 - 60 * 3);

	setColor(gray);
	drawSquare2I(WINDOW_SIZE, -WINDOW_SIZE - 130, WINDOW_SIZE + 260, -WINDOW_SIZE - 130,
		WINDOW_SIZE + 260, WINDOW_SIZE + 130, WINDOW_SIZE, WINDOW_SIZE + 130);
}

void TETRIS_Display() {
	static int i, x, y, tx, ty;
	time(&TETRIS_Display_currentTime);
	if (TETRIS_Display_State != 0) {
		setColor(red);
		x = 22; y = 30;
		for (i = 0; i < 7; i++) {
			//表示領域の横が500、文字の横幅が44、左右間がそれぞれ48、文字間がそれぞれ16であるため、
			tx = -WINDOW_SIZE + 48 + x + (44 + 16)*i;
			//ウィンドウの上限が500、文字の縦幅が80、上下間がそれぞれ45であるため、
			ty = (WINDOW_SIZE + 130) - 45 - y;
			switch (i) {
			case 0: case 2:
				drawLine2D(-x + tx, y + ty, x + tx, y + ty);	//Tの表示
				drawLine2D(0 + tx, y + ty, 0 + tx, -y + ty);
				break;
			case 1:
				drawLine2D(-x + tx, y + ty, -x + tx, -y + ty);
				drawLine2D(-x + tx, y + ty, x + tx, y + ty);
				drawLine2D(-x + tx, 0 + ty, x + tx, 0 + ty);
				drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
				break;
			case 3:
				drawLine2D(-x + tx, y + ty, -x + tx, -y + ty);
				drawLine2D(-x + tx, y + ty, x + tx, y + ty);
				drawLine2D(x + tx, y + ty, x + tx, 0 + ty);
				drawLine2D(-x + tx, 0 + ty, x + tx, 0 + ty);
				drawLine2D(0 + tx, 0 + ty, x + tx, -y + ty);
				break;
			case 4:
				drawLine2D(0 + tx, y + ty, 0 + tx, -y + ty);
				drawLine2D((int)(-x / 3) + tx, y + ty, (int)(x / 3) + tx, y + ty);
				drawLine2D((int)(-x / 3) + tx, -y + ty, (int)(x / 3) + tx, -y + ty);
				break;
			case 5:
				drawLine2D(-x + tx, y + ty, -x + tx, 0 + ty);
				drawLine2D(x + tx, 0 + ty, x + tx, -y + ty);
				drawLine2D(-x + tx, y + ty, x + tx, y + ty);
				drawLine2D(-x + tx, 0 + ty, x + tx, 0 + ty);
				drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
				break;
			case 6:
				drawLine2D(0 + tx, y + ty, 0 + tx, -(int)(y/5) + ty);
				drawLine2D(0 + tx, -y + (int)(y/5) + ty, 0 + tx, -y + ty);
				break;
			}
		}
	}
	else {
		TETRIS_Display_State = 0;
	}
}
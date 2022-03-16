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
#include "ActiveBlock_LotateDefine.h" //�A�N�e�B�u�u���b�N�̉�]�����Ɏg���f�[�^

#include <math.h>
#include <time.h>


#define NUM 100
#define WINDOW_SIZE 500
//�E�B���h�E�̃T�C�Y�B�ꉞ�ς����Ǖς���ƃA���t�@�x�b�g�␔���̕\��������邩������Ȃ�
static double left = -WINDOW_SIZE;
static double right = WINDOW_SIZE + 260;
static double bottom = -WINDOW_SIZE - 130;
static double top = WINDOW_SIZE + 130;
//���\��p�`�̒��S���W��(x=0,y=0)�Ƃ���

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
int ActiveBlock_Hantei_Base();//�A�N�e�B�u�u���b�N�̉��ʂ̔���
int ActiveBlock_Hantei_Left();//�A�N�e�B�u�u���b�N�̍��ʂ̔���
int ActiveBlock_Hantei_Right();//�A�N�e�B�u�u���b�N�̉E�ʂ̔���
int ActiveBlock_Hantei_Rotate();//�A�N�e�B�u�u���b�N���̂����E�E�ɉ�]�����Ƃ��ɁA
								//��]�����ړ���Ƀx�[�X�u���b�N�����邩�Ȃ����̔���B�������1��Ԃ�

void base_rotation(int);//�x�[�X�̉�]
void base_down(); //�u���b�N�������(�܂���)���񂾎��̏���
void ActiveBlock_Lotate();//�A�N�e�B�u�u���b�N�̉�]�����B����ActiveBlock_LotateState�ƃZ�b�g

void Gameover_Hantei();//�x�[�X�u���b�N��ActiveBlock_NUM�i�ڂ܂Őςݏd�Ȃ����画�����
					   //Gameover_Display()�����s����
void Gameover_Display(); //GAMEOVER�ƕ\������B
void Start();//������ʁB������S����͂���Ǝ��ۂ̃Q�[�����J�n�����
void Retry(); // �Q�[���I�[�o�[��ɍăX�^�[�g���邽�߂̏���
void Score_Display(); //�X�R�A��\�����邽�߂̏���
void NextBlock_Display(); //���ȍ~�ɍ~���ė���A�N�e�B�u�u���b�N�̕\���B4��܂Ō�����
void TETRIS_Display(); //4�i���������ɂ��uTETRIS!�v�\���p

double angle = 0;
static time_t autoDrop_CurrentTime, autoDrop_PastTime; //�A�N�e�B�u�u���b�N�̎����~���p
static int active_Random;	//���ݍ~�����Ă���A�N�e�B�u�u���b�N�p�̗���
static int next_Random[4];		//�l�N�X�g�u���b�N�p�̗���

static int ActiveBlock_LotateState; //�A�N�e�B�u�u���b�N�̉�]��ԁB0�`3�̐����B
									//0�͒�`����i�f�t�H���g�j
									//1��0���獶����90���A2��180���A3��270���ƂȂ�B
static int ActiveBlock_LotateDirection;//�A�N�e�B�u�u���b�N�̉�]�����B
									   //0�͔����v(��)���A1�͎��v(�E)���
static int score; //�v���C���[���l�������_���B�������񐔁~100�_�B
double move_y[4][4][2];//�A�N�e�B�u�u���b�N�̕ω���
static double ActiveBlock_DropSpeed = 0.8;//�A�N�e�B�u�u���b�N�̍~�����x�B����܂�Ӗ��Ȃ������c
										  //C����̎d�l��A���Ԃ�b�P�ʂ܂ł����擾�ł��Ȃ��̂�Speed�͐����l����Ȃ��ƈӖ����Ȃ�
										  //0.8�Ȃ�1�ɁA1.2�Ȃ�2�Ɂc�̂悤�Ɏ����͏����_�ȉ��؂�グ�̓���ƂȂ�

static int Window_State = 0; //0�̓X�^�[�g��ʁA1�̓Q�[����ʁA2�Ȃ�Q�[���I�[�o�[�̉�ʁB
							 //0��S�������ꂽ��1�ɂ���
							 //1�ŃQ�[���I�[�o�[�ɂȂ�����2�ɂȂ�B
							 //2��R�������ꂽ��0�ɂ���

static int TETRIS_Display_Count; //���ꂪ4�ɂȂ�����uTETRIS!�v��\������
static int TETRIS_Display_State; //�uTETRIS!�v�\���̂��߂̊Ǘ��p�ϐ��B
									 //��̂�4�ɂȂ����炱�ꂪ1�ɂȂ�A1�b��0�ƂȂ�B�i�\���I���j
static time_t TETRIS_Display_pastTime, TETRIS_Display_currentTime; //��́u1�b�v�̊Ǘ��p


double active_block[4][4][2];
/*
active_block[a][b][c]
active_block:���쒆�̃u���b�N
a = ���쒆��4�̃u���b�N�̂������ڂ�
b = ����(�����̕ӂ��ӂƂ���)���甽���v���ɐ����ĉ��ڂ̒��_��
c = 0:���_��x���W
1:���_��y���W
*/

#define ActiveBlock_NUM 12	//�A�N�e�B�u�u���b�N�̒��S�̒i���B���0�ŌŒ�
#define a_NUM 16
//base_array�̒i��
double base_array[a_NUM][12][3];
/*
base_array[a][b][c]
base_array:�w�i�A�u���b�N��u���g����
a = ���S����̋����i�i���j
b = a�i�ڂŏォ�甽���v���ɉ��ڂ�
c = 0:x���W
1:y���W
2:�F&�u���b�N�����邩�������B
[][][2]==-1:����(���F),
[][][2]!=-1:����(���ȊO�A�܂�u���b�N����)
���u���b�N���̂̐F�́u���v�ȊO�Œ�`����B
*/

int active_block_index[4][2]; //4�̃A�N�e�B�u�u���b�N���ꂼ��̍�����xy���W�́A
							  //base_array��Ō����i�Ή����Ă���j�C���f�b�N�X
int ActiveBlockIndex_Tmp[4][2]; //��̂�̈ꎞ�ۑ��p
double color[12][4]; //12�F�B�ϐ��i�[�̏����͉��ɂ��邪�A�w�b�_�t�@�C���ɏ����L�q���������i12�F��V���ɒ�`�����j


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

	sprintf(title, "��]�e�g���X FPS: %4.2lf", getFPS());
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
	case 'd'://�x�[�X�̉E��]�B�Ȃ̂ŁA
			 //�A�N�e�B�u�u���b�N�̍����Ƀx�[�X�u���b�N��������Ԃ����Ă���\�������邽�߁A
			 //���̔���i�A�N�e�B�u�u���b�N�̍����Ƀx�[�X�u���b�N�����邩�ǂ����j���s���B
		if (ActiveBlock_Hantei_Left() == 1) {
			angle -= 2 * M_PI / 12;
			base_rotation(1);
		}
		break;
	case 'a':////�x�[�X�̍���]�B�Ȃ̂ŁA
			 //�A�N�e�B�u�u���b�N�̉E���Ƀx�[�X�u���b�N���E����Ԃ����Ă���\�������邽�߁A
			 //���̔���i�A�N�e�B�u�u���b�N�̉E���Ƀx�[�X�u���b�N�����邩�ǂ����j���s���B
		if (ActiveBlock_Hantei_Right() == 1) {
			angle += 2 * M_PI / 12;
			base_rotation(-1);
		}
		break;
	case 's'://�A�N�e�B�u�u���b�N�̉��~�A����я�����ʂ�����ۂ̃Q�[����ʂւ̑J�ڗp
		if (Window_State == 0) {
			Window_State = 1;
		}
		else {
			n = move_active_block();
			if (n == 1) define_active_block();
		}
		break;
	case 'j'://�A�N�e�B�u�u���b�N�̍���]
		ActiveBlock_LotateDirection = 0;
		ActiveBlock_Lotate();
		break;
	case 'l'://�A�N�e�B�u�u���b�N�̉E��]
		ActiveBlock_LotateDirection = 1;
		ActiveBlock_Lotate();
		break;
	case 'r':
		if (Window_State == 2) { //�Q�[���I�[�o�[����R�L�[�~�������ꍇ���X�^�[�g�i�ŏ�����j
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
	static time_t t; time(&t); srand((int)t); //��������
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
	r = 300 / cos(M_PI / 12); //�O�ډ~�̔��a
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
	for (j = 0; j < 4; j++) {	//����[12][0][]���ŏ��Ɍ��߁A���ꂩ�甽���v���Ɏc��3�ӏ��̍��W��
								//��`���Ă���
								//[12][0][]�A[12][11][]�A[13][11][]�A[13][0][]�̏�
		for (k = 0; k < 2; k++) {
			active_block[0][j][k] = base_array[12 + (j / 2)][(12 - (j == 1 || j == 2)) % 12][k];
			move_y[0][j][k] = base_array[0][(12 - (j == 1 || j == 2)) % 12][k];
		}
	}
	active_block_index[0][0] = ActiveBlock_NUM;
	active_block_index[0][1] = 0;
	ActiveBlockIndex_Tmp[0][0] = active_block_index[0][0]; //�ꎞ�ۑ��p
	ActiveBlockIndex_Tmp[0][1] = active_block_index[0][1];

	ActiveBlock_LotateState = 0;//�A�N�e�B�u�u���b�N�̉�]��Ԃ������̂��̂ɂ���
	define_active_block2();//�A�N�e�B�u�u���b�N4�̂����̎c��3���`����
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
	case 0:ActiveBlock_LotateDefine_00(); break; //�����i�K�ł͉�]��Ԃ�0�Ȃ̂ł��̊֐���
												 //index���`�ł���
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
	}//�킴�킴Tmp���g���Ă��闝�R�́A
	 //�̂��̃A�N�e�B�u�u���b�N�̉�]�������������邽�߂ł���

	 //active_block_index�𗘗p���ăA�N�e�B�u�u���b�N��xy���W��y�����̈ړ��ʂ��`����
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
	while (i < 4) {  //1�ł��A�N�e�B�u�u���b�N���x�[�X�ɐG��Ă�����while��i<4�Ŕ�����
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
	//1�ł��A�N�e�B�u�u���b�N���E�̃x�[�X�u���b�N�ɐG��Ă�����while��i<4�Ŕ�����
	if (i < 4)return 0;
	else return 1;
}

int ActiveBlock_Hantei_Rotate() {
	static int i;
	i = 0;
	while ((int)base_array[ActiveBlockIndex_Tmp[i][0]][ActiveBlockIndex_Tmp[i][1]][2] != 1 && i < 4)i++;
	//1�ł��A�N�e�B�u�u���b�N�̈ړ���Ƀx�[�X�u���b�N����������while��i<4�Ŕ�����
	if (i < 4)return 0;
	else return 1;
}

void base_rotation(int rotate) {	//rotate=1�Ȃ玞�v���A-1�Ȃ甽���v���
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
		if (ActiveBlock_LotateDirection == 0)//�����̏ꍇ
			ActiveBlock_LotateState = (ActiveBlock_LotateState + 1) % 4;
		else if (ActiveBlock_LotateDirection == 1)//�E���̏ꍇ
			ActiveBlock_LotateState = (ActiveBlock_LotateState + 3) % 4;
		//��]�����ɉ�����ActiveBlock_LotateState���X�V

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
		Window_State = 2; //GAMEOVER�ƕ\�����邽�߂̏����B
						  //��������R�L�[�������ꂽ��Ăэŏ�����Q�[�����J�n
	}
}

void Gameover_Display() {
	static int i;
	for (i = 0; i < 4; i++) {
		base_array[active_block_index[i][0]][active_block_index[i][1]][2] = 1;
	}

	//�A���t�@�x�b�g��1�������A1�悸�\�����Ă���
	//1�����̑傫����90�~90�Ƃ���B�����ƕ����̊Ԃ�20����Ă���
	//�e�����ɂ����āA(0,0)�𒆐S�Ƃ���B����(-45,-45)�A�E��(45,-45)�A�E��(45,45)�A����(-45,45)
	//�\���i�K�ł́Ax�̕��ɕ␳����ȂǂƂ��Ă��ꂼ��̕����̕\���ꏊ�ɍ��킹��悤�ɂ���B
	//GAME��OVER�̊Ԃ�80����Ă���H

	//1260 = 90*8+20*6+80+(���̎c��̕�)*2 //����80�́AGAME��OVER�̊Ԃ�80�̂��Ƃł���
	//(���̎c��̕�) = 170
	//�܂�-WINDOW_SIZE+(���̎c��̕�) = -330(x���W)����\�����J�n����΂悢�B

	static t; setColor(black);
	for (i = 0; i < 8; i++) {
		t = (-330 + 40 + 110 * i) + 80 * (i>3); //�����̔z�u�␳�p
		switch (i) {
		case 0: //����'G'�̕\��
			drawLine2D(45 + t, 45, -45 + t, 45);
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, -45, 45 + t, -45);
			drawLine2D(45 + t, -45, 45 + t, 0);
			drawLine2D(45 + t, 0, 0 + t, 0);
			break;
		case 1: //����'A'�̕\��
			drawLine2D(-45 + t, -45, 0 + t, 45);
			drawLine2D(0 + t, 45, 45 + t, -45);
			drawLine2D(-22 + t, 0, 22 + t, 0);
			break;
		case 2: //����'M'�̕\��
			drawLine2D(-45 + t, -45, -45 + t, 45);
			drawLine2D(-45 + t, 45, 0 + t, -45);
			drawLine2D(0 + t, -45, 45 + t, 45);
			drawLine2D(45 + t, 45, 45 + t, -45);
			break;
		case 3: //����'E'�̕\��
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(-45 + t, 0, 45 + t, 0);
			drawLine2D(-45 + t, -45, 45 + t, -45);
			break;
		case 4: //����'O'�̕\��
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, -45, 45 + t, -45);
			drawLine2D(45 + t, -45, 45 + t, 45);
			drawLine2D(45 + t, 45, -45 + t, 45);
			break;
		case 5: //����'V'�̕\��
			drawLine2D(-45 + t, 45, 0 + t, -45);
			drawLine2D(0 + t, -45, 45 + t, 45);
			break;
		case 6: //����'E'�̕\��
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(-45 + t, 0, 45 + t, 0);
			drawLine2D(-45 + t, -45, 45 + t, -45);
			break;
		case 7: //����'R'�̕\��
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
	//1�����̑傫����90�~90�Ƃ���B�����ƕ����̊Ԃ�20����Ă���
	//�e�����ɂ����āA(0,0)�𒆐S�Ƃ���B����(-45,-45)�A�E��(45,-45)�A�E��(45,45)�A����(-45,45)
	//�\���i�K�ł́Ax�̕��ɕ␳����ȂǂƂ��Ă��ꂼ��̕����̕\���ꏊ�ɍ��킹��悤�ɂ���B
	//GAME��OVER�̊Ԃ�80����Ă���H

	//1260 = 90*5+40*4+(���̎c��̕�)*2
	//(���̎c��̕�) = 315
	//�܂�-WINDOW_SIZE+(���̎c��̕�) = -185(x���W)����\�����J�n����΂悢�B

	static int i, t; setColor(black);
	for (i = 0; i < 8; i++) {
		t = -185 + 45 + 130 * i; //�����̔z�u�␳�p
		switch (i) {
		case 0: //����'S'�̕\��
			drawLine2D(45 + t, 45, -45 + t, 45);
			drawLine2D(-45 + t, 45, -45 + t, 0);
			drawLine2D(-45 + t, 0, 45 + t, 0);
			drawLine2D(45 + t, 0, 45 + t, -45);
			drawLine2D(45 + t, -45, -45 + t, -45);
			break;
		case 1: //����'T'�̕\��
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(0 + t, 45, 0 + t, -45);
			break;
		case 2: //����'A'�̕\��
			drawLine2D(-45 + t, -45, 0 + t, 45);
			drawLine2D(0 + t, 45, 45 + t, -45);
			drawLine2D(-22 + t, 0, 22 + t, 0);
			break;
		case 3: //����'R'�̕\��
			drawLine2D(-45 + t, 45, -45 + t, -45);
			drawLine2D(-45 + t, 45, 45 + t, 45);
			drawLine2D(45 + t, 45, 45 + t, 0);
			drawLine2D(45 + t, 0, -45 + t, 0);
			drawLine2D(0 + t, 0, 45 + t, -45);
			break;
		case 4: //����'T'�̕\��
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
	for (i = 1; i < a_NUM - 1; i++) { //�x�[�X�u���b�N��S�ď���
		for (j = 0; j < 12; j++) {
			base_array[i][j][2] = -1;
		}
	}
	define_active_block();
	time(&autoDrop_PastTime); time(&autoDrop_CurrentTime);
}

void Score_Display() {
	//�������[�v����score����x(int�^��)score_Tmp�Ȃ���̂ɕۑ����Ă����A
	//switch(score_Tmp%10)�Ascore_Tmp /= score_Tmp;
	//�Ƃ��邱�Ƃ�1�̈ʂ��珇�ɒl(score_tmp%10)�����o������ɂ����switch���ŏ�������B
	//�l�ɂ����1�����E���珇�ɃX�R�A��\��������B
	//�\�������f�W�^�����̐����̍���̊p����Ƃ���B�i�����i��p���ĉE����Ǘ����Ă����j

	//1�̐����̑傫�����c80�~��44�Ƃ��A�����Ԃ̌��Ԃ�15�Ƃ���B
	//�ő�4���܂Ő���ɕ\�������Ƃ��āA
	//250 = 4*44 + 3*15 + (�E�̎c��̕�)*2
	//(�E�̎c��̕�)=14�ł��邽�߁A		�i�����̎c��̕��͂ǂ��ł������j
	//(WINDOW_SIZE-20) - (�E�̎c��̕�) = 480-14 = 466(x���W)����\�����J�n����΂悢�B
	//�܂��A�����ƃX�R�A��\������̈�̘g���Ƃ̕��͏㉺���ꂼ��20�Ƃ���B�i�Œ�j

	static int score_Tmp, i, x, y, tx, ty;
	score_Tmp = score; i = 0;
	x = 22; y = 40;
	setColor(blue);
	for (;;) {
		tx = 466 - 22 - (59 * i);
		ty = (WINDOW_SIZE + 130 - 70); //tx,ty�͕����̏ꏊ�̕␳�p
		switch ((int)(score_Tmp % 10)) {
		case 0:
			drawLine2D(-x + tx, y + ty, -x + tx, -y + ty); //�c80�~��44
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
	//260 = 44*4+16*3+(���̎c��̕�)*2
	//(���̎c��̕�) = 18
	//�܂�AWINDOW_SIZE+(���̎c��̕�) = 518(x���W)����\�����J�n����΂悢�B

	static int i, j, x, y, tx, ty; setColor(white);
	x = 22; y = 40;
	for (i = 0; i < 4; i++) {
		tx = 518 + 22 + 60 * i; //�����̔z�u�␳�p
		ty = (WINDOW_SIZE + 130) - 10 - 100;
		switch (i) {
		case 0: //����'N'�̕\��
			drawLine2D(-x + tx, -y + ty, -x + tx, y + ty);
			drawLine2D(-x + tx, y + ty, x + tx, -y + ty);
			drawLine2D(x + tx, -y + ty, x + tx, y + ty);
			break;
		case 1: //����'E'�̕\��
			drawLine2D(-x + tx, -y + ty, -x + tx, y + ty);
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(-x + tx, 0 + ty, x + tx, 0 + ty);
			drawLine2D(-x + tx, -y + ty, x + tx, -y + ty);
			break;
		case 2: //����'X'�̕\��
			drawLine2D(-x + tx, y + ty, x + tx, -y + ty);
			drawLine2D(x + tx, y + ty, -x + tx, -y + ty);
			break;
		case 3: //����'T'�̕\��
			drawLine2D(-x + tx, y + ty, x + tx, y + ty);
			drawLine2D(0 + tx, y + ty, 0 + tx, -y + ty);
			break;
		default:break;
		}
	}

	//�����Ƀl�N�X�g�u���b�N�̕\������������
	static int nextBlock_Center[4][2] = {
		{ 630,310 },{ 630,30 },{ 630,-210 },{ 630,-410 }
	};
	static int nextBlock_x[4], nextBlock_y[4];
	setColor(blue);
	for (i = 0; i < 4; i++) {
		tx = 8 * (6 - i); ty = 5 * (6 - i);
		switch (next_Random[i]) {
		case 0:	//�\���p�E�B���h�E�̒��S���猩��4�̊e�u���b�N�̍����̊p�̍��W���ǂꂾ������Ă��邩������
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
	drawSquare2I(WINDOW_SIZE + 10 + 20 * 0, WINDOW_SIZE + 130 - 200 - 240 - 60 * 0, //1�ڂ̃l�N�X�g�u���b�N�p�E�B���h�E
		WINDOW_SIZE + 260 - 10 - 20 * 0, WINDOW_SIZE + 130 - 200 - 240 - 60 * 0,	//���S��(630,310)
		WINDOW_SIZE + 260 - 10 - 20 * 0, WINDOW_SIZE + 130 - 200 - 60 * 0,
		WINDOW_SIZE + 10 + 20 * 0, WINDOW_SIZE + 130 - 200 - 60 * 0);
	drawSquare2I(WINDOW_SIZE + 10 + 20 * 1, WINDOW_SIZE + 130 - 200 - 440 - 60 * 1, //2�ڂ̃l�N�X�g�u���b�N�p�E�B���h�E
		WINDOW_SIZE + 260 - 10 - 20 * 1, WINDOW_SIZE + 130 - 200 - 440 - 60 * 1,	//���S��(630,30)
		WINDOW_SIZE + 260 - 10 - 20 * 1, WINDOW_SIZE + 130 - 200 - 240 - 60 * 1,
		WINDOW_SIZE + 10 + 20 * 1, WINDOW_SIZE + 130 - 200 - 240 - 60 * 1);
	drawSquare2I(WINDOW_SIZE + 10 + 20 * 2, WINDOW_SIZE + 130 - 200 - 600 - 60 * 2, //3�ڂ̃l�N�X�g�u���b�N�p�E�B���h�E
		WINDOW_SIZE + 260 - 10 - 20 * 2, WINDOW_SIZE + 130 - 200 - 600 - 60 * 2,	//���S��(630,-210)
		WINDOW_SIZE + 260 - 10 - 20 * 2, WINDOW_SIZE + 130 - 200 - 440 - 60 * 2,
		WINDOW_SIZE + 10 + 20 * 2, WINDOW_SIZE + 130 - 200 - 440 - 60 * 2);
	drawSquare2I(WINDOW_SIZE + 10 + 20 * 3, WINDOW_SIZE + 130 - 200 - 720 - 60 * 3, //4�ڂ̃l�N�X�g�u���b�N�p�E�B���h�E
		WINDOW_SIZE + 260 - 10 - 20 * 3, WINDOW_SIZE + 130 - 200 - 720 - 60 * 3,	//���S��(630,-410)
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
			//�\���̈�̉���500�A�����̉�����44�A���E�Ԃ����ꂼ��48�A�����Ԃ����ꂼ��16�ł��邽�߁A
			tx = -WINDOW_SIZE + 48 + x + (44 + 16)*i;
			//�E�B���h�E�̏����500�A�����̏c����80�A�㉺�Ԃ����ꂼ��45�ł��邽�߁A
			ty = (WINDOW_SIZE + 130) - 45 - y;
			switch (i) {
			case 0: case 2:
				drawLine2D(-x + tx, y + ty, x + tx, y + ty);	//T�̕\��
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
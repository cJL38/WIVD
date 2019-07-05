#include <stdio.h> // 2017.10.25 ��Ʈ���� 2���ϼ�.
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h> // ���ʽ���������
// UP �� �� ũ���� �߻��� ����� �̿ϼ�
#define TRUE 1
#define FALSE 0

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACEBAR 32
#define P 112
#define Z 122

#define WIDTH 12
#define HEIGHT 21

#define SHADOW -2
#define BLOCK_FIXED -1
#define EMPTY 0
#define BLOCK_MOVING 1 // moving block
#define WALL 2


void setCursor(int x, int y)
{
	COORD Pos;
	Pos.X = x * 2;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void removeCursor(void) // Ŀ�� ������ ����
{
	CONSOLE_CURSOR_INFO cur;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
	cur.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
}

void setColor(unsigned short text, unsigned short back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}

int map[HEIGHT][WIDTH] =
{
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,0,0,0,0,0,0,0,0,0,0,2,
	2,2,2,2,2,2,2,2,2,2,2,2
};

int map_pre[HEIGHT][WIDTH] = { 0 };

const int Blocks[7][4][4][4] = // I, L, J, O, S, Z, T
{
	{ { 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 },{ 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0 },{ 0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0 },{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 } },// I
{ { 0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0 },{ 0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0 },{ 0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0 } },// L
{ { 0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,1,0,1,0,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,0,1,0,0,0,1,0 } },// J
{ { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 } },// O
{ { 0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0 },{ 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0 } },// S
{ { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 } },// Z
{ { 0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0 },{ 0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0 },{ 0,0,1,0,0,1,1,0,0,0,1,0,0,0,0,0 } },// T
};

int cnt = 1;
int refreshRate = 50;
int point = 0;
int level = 1;
int end = 0;

int flag_levelup = 0;
int flag_level[10] = { 1, 0, };
int flag_bonus = 0;

int flag_firstdraw = 0;
int flag_newblock = 0;
int flag_preblock = -1;
int flag_keepblock = -1;
int flag_KBcnt = 3; // keep block count

int space_key = 0; // �����̽� ���� ������ ����

int coox, cooy; // coordinate X and Y
int num_prebcase;
int num_bcase;
int num_brotation;
int keepblock = 7;

void start();
void draw_Map();
void draw_preblock();
void draw_keepblock();
void draw_shadow();

void gameover();

void make_block();
void drop_block();
void move_block(int key);

int check_crush(int x, int y);
int check_Rcrush();

void pause();
void remove_line();
void remove_shadow();

void show_info();
void show_levelup();


int main()
{
	int x = 0, y = 2;

	srand((unsigned)time(NULL));
	removeCursor();

RESTART:

	system("mode con lines=14 cols=48 ");
	setColor(15, 0);
	setCursor(x, y++);	printf("����  ����  ����  ����   ����  ����");
	setCursor(x, y++);	printf("  ��    ��        ��    ��  ��     ��    ��    ");
	setCursor(x, y++);	printf("  ��    ����    ��    ���       ��    ����");
	setCursor(x, y++);	printf("  ��    ��        ��    ��  ��     ��        ��");
	setCursor(x, y++);	printf("  ��    ����    ��    ��   ��  ����  ����");

	x = 5; y = 11;
	setColor(13, 0); setCursor(x, y); printf("Press Enter to Start the game");

	char key = getch();
	if (key == 13)
	{
		system("mode con lines=22 cols=38 ");
		system("cls");
		start();
	}
}

void start()
{
	int i, j;
	int key;
	int tmp;

	while (1)
	{
		draw_Map();
		if (flag_preblock == -1) num_prebcase = rand() % 7; //ù ��
		if (flag_newblock == 0) make_block(); // 0�̸� ����, 1�̸� ��������. ������� ������ 0����.

		if (end == 1) {
			gameover();
			break;
		}

		if (flag_preblock == 1) { // ������ ����
			num_prebcase = rand() % 7;
			draw_preblock();
		}

		if (kbhit()) { // Ű��������
			key = getch();
			move_block(key);
		}

		if (space_key == 1) { // �����̽��� ������ ����
			space_key = 0;
			if (check_crush(0, 1) == TRUE) { // ũ���� ���� = ��� X = ������
				for (i = 0; i < HEIGHT; i++)
					for (j = 0; j < HEIGHT; j++)
						if (map[i][j] == 1)
							map[i][j] = BLOCK_FIXED;
				flag_newblock = 0;

			}
		}

		if (cnt % 20 == 0) // ��� ��
			drop_block();

		refreshRate = (point / 10 + 1) * 50; // ������ ���� �ӵ� ����
		level = refreshRate / 50;


		Sleep(1000 / refreshRate); // �ӵ�.

		cnt++;
		remove_line();
		draw_shadow();
		show_info();
		show_levelup();

	} // end of while



}

void draw_Map()
{
	int i, j;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (map_pre[i][j] != map[i][j]) {
				setCursor(j, i);
				switch (map[i][j]) {
				case SHADOW:
					setColor(8, 0);
					printf("��");
					break;
				case BLOCK_FIXED:
					setColor(9 + num_bcase, 0);
					//setColor(8, 0);
					printf("��");
					break;
				case EMPTY:
					printf("  ");
					break;
				case BLOCK_MOVING:
					setColor(9 + num_bcase, 0);
					printf("��");
					break;
				case WALL:
					setColor(15, 0);
					printf("��");
					break;
				}
			}
		}
	}

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			map_pre[i][j] = map[i][j];
	setColor(15, 0);
	for (int i = 0; i < 10; i++) {
		if (map[3][1 + i] == 0) {
			setCursor(1 + i, 3);
			printf(".'");
		}
	}  // ������� �׸���

	if (flag_firstdraw == 0) {
		setCursor(12, 0);
		printf("�ˢˢˢˢ�");
		setCursor(12, 1);
		printf("��������");
		for (i = 0; i < 4; i++) {
			setCursor(16, 2 + i);
			printf("��");
		}
		setCursor(12, 6);
		printf("�ˢˢˢˢ�"); // ������ �����ִ� ĭ

		setCursor(12, 7); // ����� �����ִ� ĭ
		printf("�������");
		for (i = 0; i < 4; i++) {
			setCursor(16, 8 + i);
			printf("��");
		}
		setCursor(12, 12);
		printf("�ˢˢˢˢ�");

		flag_firstdraw = 1;
	}
}
void draw_preblock()
{
	int i, j;

	flag_preblock = 0;

	for (i = 0; i<4; i++)
		for (j = 0; j < 4; j++) {
			setColor(9 + num_prebcase, 0);
			setCursor(12 + j, 2 + i);
			if (Blocks[num_prebcase][0][i][j] == 0)
				printf("  ");
			else if (Blocks[num_prebcase][0][i][j] == 1)
				printf("��");
		}
}
void draw_keepblock()
{
	int i, j;

	for (i = 0; i<4; i++)
		for (j = 0; j < 4; j++) {
			setCursor(12 + j, 8 + i);
			if (Blocks[keepblock][0][i][j] == 0)
				printf("  ");
			else if (Blocks[keepblock][0][i][j] == 1)
				printf("��");
		}
}

void draw_shadow()
{
	int i, j, k;
	int flag = 0;
	k = 1;
	while (flag_newblock == 1) { // �̰� ������ ���� �ϼ��� ���ѷ����� ����
		if (check_crush(0, k) == 1) // ũ���� �߻� ������ ��� ������ ����
			break;
		k++;
	}

	if (k>0)	k--;

	for (i = 0; i<4; i++)
		for (j = 0; j < 4; j++) {
			if (map[cooy + i + k][coox + j] == EMPTY && Blocks[num_bcase][num_brotation][i][j] == BLOCK_MOVING)
				map[cooy + i + k][coox + j] = SHADOW;
		}


}

void gameover()
{
	setColor(15, 0);
	setCursor(7, 9);
	printf("����������������������");
	setCursor(7, 10);
	printf("�������� �������ϴ� ��");
	setCursor(7, 11);
	printf("���� ���� : %4d������", point);
	setCursor(7, 12);
	printf("����������������������");
}

void make_block()
{
	if (flag_keepblock != 1)	num_bcase = num_prebcase; // �Ϲ����� makeblock
	else if (flag_keepblock == 1) num_bcase = keepblock; // ŵ�� ����� make block

	num_brotation = 0;

	int i, j;

	for (i = 0; i < 4; i++)  // ���ӿ��� �Ǵ�
		for (j = 0; j < 4; j++)
			if (map[i][WIDTH / 2 - 2 + j] == -1) {
				end = 1;
				return 0;
			}

	for (i = 0; i < 4; i++) { // �迭 ä���
		for (j = 0; j < 4; j++) {
			switch (num_bcase) {
			case 0:
				map[i][WIDTH / 2 - 2 + j] = Blocks[num_bcase][num_brotation][i][j];
				break;
			case 1:
				map[i][WIDTH / 2 - 2 + j] = Blocks[num_bcase][num_brotation][i][j];
				break;
			case 2:
				map[i][WIDTH / 2 - 2 + j] = Blocks[num_bcase][num_brotation][i][j];
				break;
			case 3:
				map[i][WIDTH / 2 - 2 + j] = Blocks[num_bcase][num_brotation][i][j];
				break;
			case 4:
				map[i][WIDTH / 2 - 2 + j] = Blocks[num_bcase][num_brotation][i][j];
				break;
			case 5:
				map[i][WIDTH / 2 - 2 + j] = Blocks[num_bcase][num_brotation][i][j];
				break;
			case 6:
				map[i][WIDTH / 2 - 2 + j] = Blocks[num_bcase][num_brotation][i][j];
				break;
			}
		}
	}

	remove_shadow();
	flag_newblock = 1;
	if (flag_keepblock != 1) flag_preblock = 1; // ����ũ���� �Ϲ����� ���(0)���� ������ �����. ŵ�� ��쿡�� �����

	coox = 4;
	cooy = 0;
}
void drop_block()
{
	int i, j;

	if (check_crush(0, 1) == TRUE) { // ũ���� ���� = ��� X = ������
		for (i = 0; i < HEIGHT; i++)
			for (j = 0; j < HEIGHT; j++) {
				if (map[i][j] == BLOCK_MOVING)
					map[i][j] = BLOCK_FIXED;
			}
		remove_shadow();
		flag_newblock = 0;
		return 0;
	}

	for (i = HEIGHT - 1; i >= 0; i--) { // ��ĭ ���߱� 
		for (j = WIDTH - 1; j >= 0; j--) {
			if (map[i][j] == 1) {
				map[i][j] = EMPTY;
				map[i + 1][j] = BLOCK_MOVING;
			}
		}
	}

	cooy++;
}

void move_block(int key)
{
	int i, j;


	switch (key) {
	case LEFT:
		if (check_crush(-1, 0) == TRUE) break; // ������ �������� ��� break
		coox--; // ��ǥ�̵�

		for (i = 0; i <HEIGHT; i++)
			for (j = 0; j < WIDTH; j++) {
				if (map[i][j] == 1) {
					map[i][j] = 0;
					map[i][j - 1] = BLOCK_MOVING; // �� �����̱�
				}
			}
		remove_shadow();
		break;

	case RIGHT:
		if (check_crush(1, 0)) break; // �������� �������� ��� break
		coox++; // ��ǥ�̵�
		for (i = HEIGHT - 1; i >= 0; i--)
			for (j = WIDTH - 1; j >= 0; j--) {
				if (map[i][j] == 1) {
					map[i][j] = 0;
					map[i][j + 1] = BLOCK_MOVING; // �� �����̱�
				}
			}
		remove_shadow();
		break;

	case UP:
		if (check_Rcrush() == TRUE) { // ȸ�� �� ũ���� �߻��� �����ֱ�,���� �ʿ�
			if (check_crush(2, 0)) {
				move_block(LEFT);
				move_block(LEFT);
			}
			else if (check_crush(1, 0))
				move_block(LEFT);
			else if (check_crush(-2, 0)) {
				move_block(RIGHT);
				move_block(RIGHT);
			}
			else if (check_crush(-1, 0))
				move_block(RIGHT);

		}

		num_brotation = (num_brotation + 1) % 4; // ���� ����(ȸ��)

		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++) {
				if (map[cooy + i][coox + j] == WALL)
					map[cooy + i][coox + j] = WALL;
				else if (map[cooy + i][coox + j] == BLOCK_FIXED)
					map[cooy + i][coox + j] = BLOCK_FIXED;
				else
					map[cooy + i][coox + j] = Blocks[num_bcase][num_brotation][i][j];

			}

		remove_shadow();
		break;
	case DOWN:
		drop_block();
		break;
	case P: //p(�ҹ���) �������� 
		pause(); //�Ͻ����� 
		break;
	case Z:
		if (flag_KBcnt > 0) { // Ȧ�� ��� ���� Ƚ���� 0�̻�
			int tmp;
			if (flag_keepblock == 0)	flag_keepblock = 1;

			for (i = 0; i < HEIGHT; i++)
				for (j = 0; j < WIDTH; j++)
					if (map[i][j] == BLOCK_MOVING)
						map[i][j] = EMPTY;

			tmp = num_bcase; // �ӽ÷� ���� ������ ����

			make_block(); // ���� ���� �����
			keepblock = tmp; // ���� ������ ŵ
			draw_keepblock(); // ŵ�� ������ ����ĭ�� �׸�
			flag_keepblock = 0;
			flag_KBcnt--;

		}
		break;
	} // end of switch

	switch (key) {

	case SPACEBAR:
		space_key = 1; // �����̽� �� �������� ���� �÷���
		while (check_crush(0, 1) == 0) // (��ĭ �� üũ) ũ������ ���� ����
			drop_block(); // ��� ���� ��ĭ�� �Ʒ���

		break;
	}
}


int check_crush(int x, int y)
{

	int i, j;
	int flag = 0;

	for (i = 0; i <4; i++)
		for (j = 0; j < 4; j++) {
			if (map[cooy + i][coox + j] == 1 && (map[cooy + i + y][coox + j + x] == -1 || map[cooy + i + y][coox + j + x] == 2))
				flag = 1;
		}
	return flag;

}

int check_Rcrush()
{
	int i, j;
	int flag = 0;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (Blocks[num_bcase][(num_brotation + 1) % 4][i][j] == 1 && (map[cooy + i][coox + j] == -1 || map[cooy + i][coox + j] == 2))
				flag = 1;

	return flag;
}

void pause(void)
{
	getch();  // �ƹ� Ű�� 1�� �Է� �ޱ�
	puts(""); // �ٹٲ�
}


void remove_line()
{
	int amount = 0;
	int oneline = 0;
	int flaga = 0;

	int i, j, t;
	for (i = 0; i < HEIGHT; i++) { // ������ �ϼ��Ǿ����� �Ǻ�
		for (j = 1; j < WIDTH - 1; j++) {
			if (map[i][j] == BLOCK_FIXED) amount++; // ������ ��� BLOCK_FIXED�ΰ�? �� �ϼ� �Ǿ��°�?
			if (amount == 10) {
				oneline++;
				point++;
				flaga = i; // ���Ϲ��� = flaga
			}
		}
		amount = 0;
	}

	if (oneline == 3) point += 1;
	if (oneline == 4) point += 2;

	if (flag_bonus == 1 && oneline >= 1)
		point += oneline;
	if (flag_bonus == 0 && oneline == 1)
		flag_bonus = 1;
	if (flag_bonus == 1 && oneline == 0)
		flag_bonus = 0;


	if (oneline > 0) { // ���� �ϼ��Ǿ��ٸ�
		for (t = 0; t < oneline; t++) // ���� �ϼ��� ���� ��ŭ
			for (i = flaga; i >= 0; i--) // �ϼ��� �� �ٷ� ������ ������ ����.
				for (j = WIDTH - 2; j > 0; j--)
					map[i][j] = map[i - 1][j]; // ��ĭ�� ������

	}
}

void remove_shadow()
{
	int i, j;
	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			if (map[i][j] == SHADOW)
				map[i][j] = EMPTY;
}

void show_info()
{
	setColor(8, 0);
	setCursor(13, 14);
	printf("Level : %2d", refreshRate / 50); // ���� ǥ��
	setCursor(13, 16);
	printf("point : %2d", point); // ����ǥ��
	setCursor(13, 18);
	printf("Hold  : %2d", flag_KBcnt); // Ȧ�� ���� Ƚ�� ǥ��
}
void show_levelup()
{
	if (level > 10)
		return;
	if (flag_level[level - 1] == 0) {
		flag_level[level - 1] = 1;
		flag_levelup = 0;

		setColor(level + 5, 0);
		setCursor(2, 10);
		printf("����������������");
		setCursor(2, 11);
		printf("�� LEVEL UP ! ��");
		setCursor(2, 12);
		printf("��PRESENT : %d ��", level);
		setCursor(2, 13);
		printf("����������������");

		Sleep(1000);

		setCursor(2, 10);
		printf("����������������");
		setCursor(2, 11);
		printf("����������������");
		setCursor(2, 12);
		printf("����������������");
		setCursor(2, 13);
		printf("����������������");

	}



}
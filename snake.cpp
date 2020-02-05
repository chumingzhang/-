/*
	1.�������
	2.ģ�黯���
	3.ʵ�ֹ���
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>      //ʳ�����
#include <conio.h>     //�������  ����
//////////////////////////////////////
//           �����궨��
#define MAPHEIGHT 25
#define MAPWIDTH 60
#define SNAKESIZE 50      //�ߵ�������
//////////////////////////////////////
//             �������
//////////////////////////////////////
//ʳ��Ľṹ��
struct 
{
	//����
	int x;
	int y;
}food;
//////////////////////////////////////
//�ߵĽṹ��
struct
{
	//�ߵ�����
	int x[SNAKESIZE];
	int y[SNAKESIZE];
	int len;    //�ߵĳ���
	int speed;    //�ߵ��ٶ�
}snake;
//////////////////////////////////////
//          ȫ�ֱ���
int key = 'w';          //�ߵĳ���
int changeFlag = 0;     //�ߵ��ƶ��ٶ�
//////////////////////////////////////
//ģ�黯��ƣ������ܻ��֣��������ĳ���
/*�����˽�ҵ���߼�����ͼ��ʳ���������...
1.����ͼ
2.ʳ��Ĳ���
3.���������������ǰ�����ʱ�򣬻ᷢ��ʲô����
4.�ߵ�״̬�������ж��Ƿ����,ײǽ��ײ�Լ�
5.��������������ƶ�
*/
///////////////////////////////////////////
//5.��������
void gotoxy(int x, int y)   //������̭ Ҫ�Լ�ʵ��
{
	//����win32 API ȥ���ÿ���̨�Ĺ��λ��
	//1.�ҵ�����̨���������
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//2.���Ľṹ��;
	COORD coord;
	//3.��������
	coord.X = x;
	coord.Y = y;
	//4.ͬ��������̨
	SetConsoleCursorPosition(handle, coord);
}
//////////////////////////////////////
//1.����ͼ��Ȧ��ȷ����Χ
void drawMap()
{
	srand((unsigned int)time(NULL));
	//2.����
//�ߵ����ԣ�ԭʼ���Ⱥ��ٶ�
	snake.len = 3;
	printf("   ��ѡ����Ϸ�Ѷȣ�\n1.��\t2.�е�\t3.����\n");
	int choice;
	scanf("%d", &choice);
	printf("---------------------------------");
	switch (choice)
	{
	case 1:
		snake.speed = 300;
		break;
	case 2:
		snake.speed = 250;
		break;
	case 3:
		snake.speed = 200;
		break;
	}
	//1.Ȧ��
	//����
	for (int i = 4;i <= MAPHEIGHT+4;i++)
	{
		gotoxy(4, i);
		printf("��");
		gotoxy(MAPWIDTH+4, i);
		printf("��");
	}
	//����
	for (int i = 4;i <= MAPWIDTH+4;i+=2)
	{
		gotoxy(i,4);
		printf("��");
		gotoxy(i, MAPHEIGHT+4);
		printf("��");
	}
	//ʳ��� �����

	//���迪ʼ��ʱ��������Ļ�м�
	snake.x[0] = (MAPWIDTH+4) / 2;
	snake.y[0] = (MAPHEIGHT+4) / 2;
	//��ͷ
	gotoxy(snake.x[0], snake.y[0]);
	printf("��");
	//����
	for (int k = 1;k < snake.len;k++)
	{
		snake.x[k] = snake.x[k - 1] + 2;
		snake.y[k] = snake.y[k - 1];
		gotoxy(snake.x[k], snake.y[k]);
		printf("��");
	}

	//3.��ʳ�ȷ�����꣬������
	do
	{
		food.x = rand() % (MAPWIDTH - 2) + 6;   //�����������أ�����һ������
	} while (food.x % 2 != 0);
	food.y = rand() % (MAPHEIGHT-1) + 5;
	gotoxy(food.x, food.y);
	printf("��");
}
//2.ʳ��Ĳ���
void createFood()
{
	//�߰�ʳ�����
	if (snake.x[0] == food.x && snake.y[0] == food.y)
	{
		srand((unsigned int)time(NULL));
		snake.len++;
		if(snake.speed>=100) snake.speed -= 10;
		//changeFlag = 1;
		//������ʳ�ﲻ�����ߵ����ϣ��������������ż��
		while (1)
		{
			int flag = 1;
			food.x = rand() % (MAPWIDTH -2) + 6;
			food.y = rand() % (MAPHEIGHT-1) + 5;
			//�ж�ʳ���Ƿ����ߵ�����
			for (int k = 0;k < snake.len;k++)
			{
				if (snake.x[k] == food.x && snake.y[0] == food.y)
				{
					flag = 0;   //��������
					break;
				}
			}
			if (flag &&food.x%2==0)
			{
				break;
			}
		}
	}
	gotoxy(food.x,food.y);
	printf("��");

}
///////////////////////////////////////////
//3.��������
void keyDown()
{
	if (_kbhit())
	{
		//�а���
		fflush(stdin);
		key = _getch();
	}
	//����ԭ����
	if (!changeFlag)
	{
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf("  ");  
	}
	//����Ľ��������������ǰ����
	for (int i = snake.len-1;i > 0;i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	//�ƶ�����Ĵ���
	switch (key)
	{
	case 'w':
	case 'W':
		snake.y[0]--;   //��
		break;
	case 's':
	case 'S':
		snake.y[0]++;  //��
		break;
	case 'a':
	case 'A':
		snake.x[0] -= 2;   //��
		break;
	case 'd':
	case 'D':
		snake.x[0] += 2;    //��
		break;
	}
	gotoxy(snake.x[0], snake.y[0]);
	printf("��");
	changeFlag = 0;
	gotoxy(MAPHEIGHT + 6, 0);   //����ƶ���������
}
////////////////////////////////////////////
//4.�ߵ�״̬
int snakeStatus()
{
	if (snake.x[0] == 4 || snake.x[0] == MAPWIDTH +4 || snake.y[0] == 4 || snake.y[0] == MAPHEIGHT +4)
	{
		return 0;
	}
	for (int k = 1;k < snake.len;k++)
	{
		if (snake.x[0] == snake.x[k] && snake.y[0] == snake.y[k])
		{
			return 0;
		}
	}
	return 1;
}

int main()
{

	drawMap();
	while (1)
	{
		createFood();
		Sleep(snake.speed);
		keyDown();
		if (!snakeStatus())
		{
			break;
		}
	} 
	gotoxy(MAPWIDTH / 2, MAPHEIGHT / 2);
	printf("Gameover!");
	system("pause");
	return 0;
}

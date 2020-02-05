/*
	1.数据设计
	2.模块化设计
	3.实现过程
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>      //食物随机
#include <conio.h>     //按键监控  蛇走
//////////////////////////////////////
//           辅助宏定义
#define MAPHEIGHT 25
#define MAPWIDTH 60
#define SNAKESIZE 50      //蛇的最大节数
//////////////////////////////////////
//             数据设计
//////////////////////////////////////
//食物的结构体
struct 
{
	//坐标
	int x;
	int y;
}food;
//////////////////////////////////////
//蛇的结构体
struct
{
	//蛇的坐标
	int x[SNAKESIZE];
	int y[SNAKESIZE];
	int len;    //蛇的长度
	int speed;    //蛇的速度
}snake;
//////////////////////////////////////
//          全局变量
int key = 'w';          //蛇的长度
int changeFlag = 0;     //蛇的移动速度
//////////////////////////////////////
//模块化设计，即功能划分，即函数的抽象
/*具体了解业务逻辑，地图，食物，按键处理...
1.画地图
2.食物的产生
3.按键操作，但我们按键的时候，会发生什么事情
4.蛇的状态，用来判断是否结束,撞墙，撞自己
5.辅助函数，光标移动
*/
///////////////////////////////////////////
//5.辅助函数
void gotoxy(int x, int y)   //现已淘汰 要自己实现
{
	//调用win32 API 去设置控制台的光标位置
	//1.找到控制台的这个窗口
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//2.光标的结构体;
	COORD coord;
	//3.设置坐标
	coord.X = x;
	coord.Y = y;
	//4.同步到控制台
	SetConsoleCursorPosition(handle, coord);
}
//////////////////////////////////////
//1.画地图，圈地确定范围
void drawMap()
{
	srand((unsigned int)time(NULL));
	//2.画蛇
//蛇的属性：原始长度和速度
	snake.len = 3;
	printf("   请选择游戏难度：\n1.简单\t2.中等\t3.困难\n");
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
	//1.圈地
	//左右
	for (int i = 4;i <= MAPHEIGHT+4;i++)
	{
		gotoxy(4, i);
		printf("■");
		gotoxy(MAPWIDTH+4, i);
		printf("■");
	}
	//上下
	for (int i = 4;i <= MAPWIDTH+4;i+=2)
	{
		gotoxy(i,4);
		printf("■");
		gotoxy(i, MAPHEIGHT+4);
		printf("■");
	}
	//食物⊙ 蛇身■

	//假设开始的时候蛇在屏幕中间
	snake.x[0] = (MAPWIDTH+4) / 2;
	snake.y[0] = (MAPHEIGHT+4) / 2;
	//蛇头
	gotoxy(snake.x[0], snake.y[0]);
	printf("■");
	//身体
	for (int k = 1;k < snake.len;k++)
	{
		snake.x[k] = snake.x[k - 1] + 2;
		snake.y[k] = snake.y[k - 1];
		gotoxy(snake.x[k], snake.y[k]);
		printf("■");
	}

	//3.画食物：确定坐标，画出来
	do
	{
		food.x = rand() % (MAPWIDTH - 2) + 6;   //左右两个像素，上下一个像素
	} while (food.x % 2 != 0);
	food.y = rand() % (MAPHEIGHT-1) + 5;
	gotoxy(food.x, food.y);
	printf("⊙");
}
//2.食物的产生
void createFood()
{
	//蛇把食物吃了
	if (snake.x[0] == food.x && snake.y[0] == food.y)
	{
		srand((unsigned int)time(NULL));
		snake.len++;
		if(snake.speed>=100) snake.speed -= 10;
		//changeFlag = 1;
		//产生的食物不能再蛇的身上，并且坐标必须是偶数
		while (1)
		{
			int flag = 1;
			food.x = rand() % (MAPWIDTH -2) + 6;
			food.y = rand() % (MAPHEIGHT-1) + 5;
			//判断食物是否在蛇的身上
			for (int k = 0;k < snake.len;k++)
			{
				if (snake.x[k] == food.x && snake.y[0] == food.y)
				{
					flag = 0;   //在它身上
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
	printf("⊙");

}
///////////////////////////////////////////
//3.按键操作
void keyDown()
{
	if (_kbhit())
	{
		//有按键
		fflush(stdin);
		key = _getch();
	}
	//擦掉原来的
	if (!changeFlag)
	{
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf("  ");  
	}
	//后面的节数，后身体替代前身体
	for (int i = snake.len-1;i > 0;i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	//移动方向的处理
	switch (key)
	{
	case 'w':
	case 'W':
		snake.y[0]--;   //上
		break;
	case 's':
	case 'S':
		snake.y[0]++;  //下
		break;
	case 'a':
	case 'A':
		snake.x[0] -= 2;   //左
		break;
	case 'd':
	case 'D':
		snake.x[0] += 2;    //右
		break;
	}
	gotoxy(snake.x[0], snake.y[0]);
	printf("■");
	changeFlag = 0;
	gotoxy(MAPHEIGHT + 6, 0);   //光标移动，看不见
}
////////////////////////////////////////////
//4.蛇的状态
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

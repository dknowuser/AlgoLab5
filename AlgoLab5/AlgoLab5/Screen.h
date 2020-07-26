//� ������ ����� ����������� ������� ������ � �������
#pragma once

#ifndef SCREEN
#define SCREEN
#endif

#include <iostream>

//������� ������
extern const int XMAX = 80;
extern const int YMAX = 40;

//�����
char screen[XMAX][YMAX];

//�����
extern enum color {black = '*', white = '.'};

//������� �������� ������
extern void screen_init()
{
	for (int y = 0; y < YMAX; y++)
		for (int x = 0; x < XMAX; x++)
			screen[x][y] = white;
};

//������� ���������� ������
extern void screen_refresh()
{
	for (int y = YMAX - 1; 0 <= y; --y)
	{
		for (int x = 0; x < XMAX; ++x)
			std::cout << screen[x][y];
		std::cout << std::endl;
	};
};

//������� ������� ������
extern void screen_clear()
{
	screen_init();
};
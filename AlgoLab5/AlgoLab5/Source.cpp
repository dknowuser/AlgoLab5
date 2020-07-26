//Главный файл
#include <conio.h>
#include <iostream>

#ifndef SCREEN
#include "Screen.h"
#endif 

#ifndef SHAPE
#include "Shape.h"
#endif 

#ifndef EXCEP
#include "Exceptions.h"
#endif 

// Дополнительная "сборная" фигура
class myshape : public Rectangle {//Моя фигура ЯВЛЯЕТСЯ прямоугольником
	Line* l_eye; // левый глаз – моя фигура СОДЕРЖИТ линию
	Line* r_eye; // правый глаз
	Line* mouth; // рот
public:
	myshape(Point, Point);
	void draw();
	void move(int, int);
};
myshape::myshape(Point a, Point b) : Rectangle(a, b)
{

	int ll = neast().x - swest().x + 1;
	int hh = neast().y - swest().y + 1;
	l_eye = new Line(Point(swest().x + 2, swest().y + hh * 3 / 4), 2);
	r_eye = new Line(Point(swest().x + ll - 4, swest().y + hh * 3 / 4), 2);
	mouth = new Line(Point(swest().x + 2, swest().y + hh / 4), ll - 4);
};

void myshape::draw()
{
	Rectangle::draw();
	int a = (swest().x + neast().x) / 2;
	int b = (swest().y + neast().y) / 2;
	Point point(a, b);
	point.put_Point();
};
void myshape::move(int a, int b)
{
	Rectangle::move(a, b);
	l_eye->move(a, b);
	r_eye->move(a, b);
	mouth->move(a, b);
};
//=================================================================================================================================================
//Прямоугольник с крестом(пользовательский класс)
class CrossRectangle :public Rectangle {
public:
	CrossRectangle(Point a, Point b) :Rectangle(a, b) {};
	void draw();
};

void CrossRectangle::draw()
{
	Rectangle::draw();
	Line l1(north(), south());
	Line l2(west(), east());

	l1.draw();
	l2.draw();
};

//========================================================================================================================================================

int main(void)
{
	setlocale(LC_ALL, "Russian");
	screen_init();

	//== 1.Объявление набора фигур ==	
	rotatable* p1 = new Rectangle(Point(0, 0), Point(80, 5));
	shape* p2 = new Line(Point(0, 15), 17);
	shape* p3 = new myshape(Point(15, 10), Point(27, 18));
	rotatable* p4 = new CrossRectangle(Point(16, 0), Point(20, 6));
	rotatable* p5 = new CrossRectangle(Point(26, 0), Point(30, 6));
	rotatable* p6 = new CrossRectangle(Point(36, 0), Point(42, 5));
	shape_refresh();
	system("pause");

	//== 2.Ориентация ==
	try
	{
		p1->rotate_right();
	}
	catch (AlreadyRotatedException& e)
	{
		std::cout << "Фигура уже была повёрнута в данном направлении." << std::endl;
	};

	try
	{
		p6->rotate_right();
	}
	catch (AlreadyRotatedException& e)
	{
		std::cout << "Фигура уже была повёрнута в данном направлении." << std::endl;
	};

	shape_refresh();
	system("pause");

	//== 3.Сборка изображения ==
	p6->move(-20, +10);

	try
	{
		up(p3, p6);
	}
	catch (NullPointerException& e)
	{
		std::cout << "Один из параметров-указателей нулевой." << std::endl;
	};

	try
	{
		up(p2, p3);
	}
	catch (NullPointerException& e)
	{
		std::cout << "Один из параметров-указателей нулевой." << std::endl;
	};

	try
	{
		up(p1, p2);
	}
	catch (NullPointerException& e)
	{
		std::cout << "Один из параметров-указателей нулевой." << std::endl;
	};

	try
	{
		Right(p4, p3);
	}
	catch (NullPointerException& e)
	{
		std::cout << "Один из параметров-указателей нулевой." << std::endl;
	};

	try
	{
		Left(p5, p3);
	}
	catch (NullPointerException& e)
	{
		std::cout << "Один из параметров-указателей нулевой." << std::endl;
	};

	shape_refresh();
	system("pause");
	return 0;
};
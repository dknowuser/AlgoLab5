//В данном файле реализованы классы фигур и примитивов
#pragma once

#ifndef SHAPE
#define SHAPE
#endif

#ifndef SCREEN
#include "Screen.h"
#endif 

#ifndef EXCEP
#include "Exceptions.h"
#endif 

//Класс точки
class Point
{
public:
	//Координаты точки на экране
	int x, y;

	Point(int a, int b) : x(a), y(b) {};
	Point() {};

	//Метод проверки, попала ли точка на экран
	void on_screen(void)
	{
		if (!((this->x >= 0) && (this->y >= 0)
			&& (this->x < XMAX) && (this->y < YMAX)))
			throw PointMissException();
	};

	//Метод вывода точки
	void put_Point(void)
	{
		try
		{
			this->on_screen();
			screen[this->x][this->y] = black;
		}
		catch (PointMissException& e)
		{
			std::cout << "Точка (" << this->x << ", " << this->y
				<< ") лежит за пределами экрана." << std::endl;

			//Возбуждаем исключение для вышележащего уровня
			throw PointMissException();
		};
	};
};

struct shape { // Виртуальный базовый класс "фигура"
	static shape* list;	// Начало списка фигур (одно на все фигуры!)
	shape* next;
	shape() { next = list; list = this; } //Фигура присоединяется к списку
	virtual Point north() const = 0;	//Точки для привязки
	virtual Point south() const = 0;
	virtual Point east() const = 0;
	virtual Point west() const = 0;
	virtual Point neast() const = 0;
	virtual Point seast() const = 0;
	virtual Point nwest() const = 0;
	virtual Point swest() const = 0;
	virtual void draw() = 0;	//Рисование
	virtual void move(int, int) = 0;	//Перемещение
};

shape * shape::list = nullptr;	//Инициализация списка фигур

//Класс линии
class Line : public shape
{
public:
	//Линию можем задать только точками
	Line(Point Begin, Point End) : x1(Begin.x), y1(Begin.y), x2(End.x), y2(End.y) {};
	Line(Point a, int L) : x2(a.x), y2(a.y), x1(a.x + L - 1), y1(a.y) {};

	//Метод вывода линии
	void put_line(void)
	{
		/*
		Рисование отрезка прямой от (x0,y0) до (x1,y1).
		Уравнение прямой: b(x-x0) + a(y-y0) = 0.
		Минимизируется величина abs(eps),
		где eps = 2*(b(x-x0)) + a(y-y0) (алгоритм Брезенхэма для прямой).
		*/

		//Совпадение координат начала и конца линии недопустимо
		if ((this->x1 == this->x2) && (this->y1 == this->y2))
			throw InvalidParameterException();

		int beginx = this->x1;
		int beginy = this->y1;

		int dx = 1;
		int a = this->x2 - this->x1;
		if (a < 0) dx = -1, a = -a;
		int dy = 1;
		int b = this->y2 - this->y1;
		if (b < 0) dy = -1, b = -b;
		int two_a = 2 * a;
		int two_b = 2 * b;
		int xcrit = -b + two_a;
		int eps = 0;

		bool Captured = false;

		for (;;) { //Формирование прямой линии по точкам
			Point point = Point(this->x1, this->y1);

			try
			{
				point.put_Point();
			}
			catch (PointMissException& e)
			{
				Captured = true;
			};

			if (this->x1 == this->x2 && this->y1 == this->y2) break;
			if (eps <= xcrit) this->x1 += dx, eps += two_b;
			if (eps >= a || a < b) this->y1 += dy, eps -= two_a;
		};
		
		if (Captured)
		{
			std::cout << "Линия (" << beginx << ", " << beginy
				<< ") - (" << this->x2 << ", " << this->y2
				<< ") выходит за пределы экрана." << std::endl;

			//Возбуждаем исключение для вышележащего уровня
			throw PointMissException();
		};
	};

	Point north() const { return Point((x1 + x2) / 2, y2 < y1 ? y1 : y2); };
	Point south() const { return Point((x1 + x2) / 2, y2 < y1 ? y2 : y1); };
	Point east() const { return Point((x1 + x2) / 2, y2 < y1 ? y2 : y1); };
	Point west() const { return Point((x1 + x2) / 2, y2 < y1 ? y2 : y1); };
	Point neast() const { return Point((x1 + x2) / 2, y2 < y1 ? y2 : y1); };
	Point seast() const { return Point((x1 + x2) / 2, y2 < y1 ? y2 : y1); };
	Point nwest() const { return Point((x1 + x2) / 2, y2 < y1 ? y2 : y1); };
	Point swest() const { return Point((x1 + x2) / 2, y2 < y1 ? y2 : y1); };

	void move(int a, int b) { x1 += a; y1 += b; x2 += a; y2 += b; };
	void draw() { put_line(); };

	//Координаты линии
	int x1;
	int y1;
	int x2;
	int y2;
};

class rotatable : public shape { //Фигуры, пригодные к повороту 
public:
	virtual void rotate_left() = 0;	//Повернуть влево
	virtual void rotate_right() = 0;	//Повернуть вправо

	//Был ли произведён поворот в данном направлении
	bool RotatedLeft;
	bool RotatedRight;
};

class reflectable : public shape { // Фигуры, пригодные
								   // к зеркальному отражению
public:
	virtual void flip_horisontally() = 0;	// Отразить горизонтально
	virtual void flip_vertically() = 0;	// Отразить вертикально

	//Было ли произведено отражение
	bool FlippedHorisontally;
	bool FlippedVertically;
};

//Класс прямоугольника
class Rectangle : public rotatable
{
/* nw------n------ne
	|		       |
	|		       |
	w	   c       e
	|		       |
	|		       |
	sw------s------se */
	Point sw, ne;
public:
	Rectangle(Point a, Point b)
	{
		RotatedLeft = false;
		RotatedRight = false;

		if (a.x <= b.x) {
			if (a.y <= b.y) sw = a, ne = b;
			else sw = Point(a.x, b.y), ne = Point(b.x, a.y);
		}
		else {
			if (a.y <= b.y) sw = Point(b.x, a.y), ne = Point(a.x, b.y);
			else sw = b, ne = a;
		}
	};

	Point north() const { return Point((sw.x + ne.x) / 2, ne.y); }
	Point south() const { return Point((sw.x + ne.x) / 2, sw.y); }
	Point east() const { return Point(sw.x, (sw.y + ne.y) / 2); }
	Point west() const { return Point(ne.x, (sw.y + ne.y) / 2); }
	Point neast() const { return ne; }
	Point seast() const { return Point(sw.x, ne.y); }
	Point nwest() const { return Point(ne.x, sw.y); }
	Point swest() const { return sw; }

	void rotate_right() // Поворот вправо относительно se
	{
		int w = ne.x - sw.x, h = ne.y - sw.y;
		sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;

		if (!RotatedRight)
		{
			RotatedRight = true;
			RotatedLeft = false;
		}
		else
			throw AlreadyRotatedException();
	};

	void rotate_left() // Поворот влево относительно sw
	{
		int w = ne.x - sw.x, h = ne.y - sw.y;
		ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;

		if (!RotatedLeft)
		{
			RotatedLeft = true;
			RotatedRight = false;
		}
		else
			throw AlreadyRotatedException();
	};

	void move(int a, int b)
	{
		sw.x += a; sw.y += b; ne.x += a; ne.y += b;
	};

	void draw(void)
	{
		Point nw(sw.x, ne.y);
		Point se(ne.x, sw.y);
		Line l1(nw, ne);
		Line l2(ne, se);
		Line l3(se, sw);
		Line l4(sw, nw);

		//Была ли обработана ошибка отображения
		bool Captured = false;

		try
		{
			l1.draw();
		}
		catch (PointMissException& e)
		{
			if (!Captured)
			{
				Captured = true;
				std::cout << "Прямоугольник (" << ne.x << ", " << ne.y
					<< ") - (" << sw.x << ", " << sw.y 
					<< ") выходит за пределы экрана." << std::endl;
			};
		};

		try
		{
			l2.draw();
		}
		catch (PointMissException& e)
		{
			if (!Captured)
			{
				Captured = true;
				std::cout << "Прямоугольник (" << ne.x << ", " << ne.y
					<< ") - (" << sw.x << ", " << sw.y
					<< ") выходит за пределы экрана." << std::endl;
			};
		};

		try
		{
			l3.draw();
		}
		catch (PointMissException& e)
		{
			if (!Captured)
			{
				Captured = true;
				std::cout << "Прямоугольник (" << ne.x << ", " << ne.y
					<< ") - (" << sw.x << ", " << sw.y
					<< ") выходит за пределы экрана." << std::endl;
			};
		};

		try
		{
			l4.draw();
		}
		catch (PointMissException& e)
		{
			if (!Captured)
			{
				Captured = true;
				std::cout << "Прямоугольник (" << ne.x << ", " << ne.y
					<< ") - (" << sw.x << ", " << sw.y
					<< ") выходит за пределы экрана." << std::endl;	
			};
		};

		if(Captured)
			//Возбуждаем исключение для вышележащего уровня
			throw PointMissException();
	};
};

void up(shape* p, const shape* q) // поместить p над q
{	//Это ОБЫЧНАЯ функция, а не член класса!

	if ((!p) || (!q))
		throw NullPointerException();

	Point n = q->north();
	Point s = p->south();
	p->move(n.x - s.x, n.y - s.y + 1);
};

void Left(shape* p, const shape* q) // поместить p слева от q
{	//Это ОБЫЧНАЯ функция, а не член класса!
	if (!p || !q)
		throw NullPointerException();

	Point e = q->east();
	Point w = p->west();
	p->move(e.x - w.x - 1, e.y - w.y);
};

void Right(shape* p, const shape* q) // поместить p справа от q
{	//Это ОБЫЧНАЯ функция, а не член класса!
	if (!p || !q)
		throw NullPointerException();

	Point w = p->west();
	Point e = q->east();
	p->move(w.x - e.x - 1, e.y - w.y);
};

void shape_refresh() // Перерисовка всех фигур
{
	screen_clear();
	unsigned int Count = 0;
	shape* Temp = shape::list;
	for (shape* p = shape::list; p; p = p->next)
	{
		try
		{
			p->draw();
		}
		catch (PointMissException& e)
		{
			std::cout << "Фигура с номером " << Count << " искажена." << std::endl;
		}
		catch (InvalidParameterException& e)
		{
			std::cout << "Фигура с номером " << Count << " имеет недопустимые параметры." << std::endl;
		};

		Count++;
	};
	shape::list = Temp;
	screen_refresh();
};
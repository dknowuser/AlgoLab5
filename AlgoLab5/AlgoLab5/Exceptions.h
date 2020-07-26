//В данном файле реализованы классы возникающих в программе исключений
#pragma once
#ifndef EXCEP
#define EXCEP
#endif

#include <exception>

//Ошибка непопадания точки на экран
class PointMissException : public std::exception
{
public:
	PointMissException(const char* _Message = "Ошибка! Точка не попадает на экран.") 
		: exception(_Message) {};
	virtual ~PointMissException() = default;
};

//Ошибка некоректных параметров при формировании фигуры
class InvalidParameterException : public std::exception
{
public:
	InvalidParameterException(const char* _Message = "Ошибка! Заданы неккоректные параметры при формировании фигуры.")
		: exception(_Message) {};

	virtual ~InvalidParameterException() = default;
};

//Ошибка повторного поворота фигуры
class AlreadyRotatedException : public std::exception
{
public:
	AlreadyRotatedException(const char* _Message = "Ошибка! Фигура уже была повёрнута в данном направлении.")
		: exception(_Message) {};

	virtual ~AlreadyRotatedException() = default;
};

//Ошибка - один из параметров-указателей нулевой
class NullPointerException : public std::exception
{
public:
	NullPointerException(const char* _Message = "Ошибка! Один из параметров-указателей нулевой.")
		: exception(_Message) {};

	virtual ~NullPointerException() = default;
};
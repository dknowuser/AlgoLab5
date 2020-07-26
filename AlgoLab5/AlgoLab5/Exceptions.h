//� ������ ����� ����������� ������ ����������� � ��������� ����������
#pragma once
#ifndef EXCEP
#define EXCEP
#endif

#include <exception>

//������ ����������� ����� �� �����
class PointMissException : public std::exception
{
public:
	PointMissException(const char* _Message = "������! ����� �� �������� �� �����.") 
		: exception(_Message) {};
	virtual ~PointMissException() = default;
};

//������ ����������� ���������� ��� ������������ ������
class InvalidParameterException : public std::exception
{
public:
	InvalidParameterException(const char* _Message = "������! ������ ������������ ��������� ��� ������������ ������.")
		: exception(_Message) {};

	virtual ~InvalidParameterException() = default;
};

//������ ���������� �������� ������
class AlreadyRotatedException : public std::exception
{
public:
	AlreadyRotatedException(const char* _Message = "������! ������ ��� ���� �������� � ������ �����������.")
		: exception(_Message) {};

	virtual ~AlreadyRotatedException() = default;
};

//������ - ���� �� ����������-���������� �������
class NullPointerException : public std::exception
{
public:
	NullPointerException(const char* _Message = "������! ���� �� ����������-���������� �������.")
		: exception(_Message) {};

	virtual ~NullPointerException() = default;
};
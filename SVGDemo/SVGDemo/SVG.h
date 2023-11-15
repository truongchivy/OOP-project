#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <iostream>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include "Point2D.h"


using namespace std;
using namespace rapidxml;
using namespace Gdiplus;


#pragma comment (lib,"Gdiplus.lib")

class Shape {
protected:
	int ArrSize;
	Shape** Arr;

public:
	Shape();

	void ReadSVG(HDC hdc);

	virtual VOID OnPaint(HDC hdc);
	virtual void fillRectangle(HDC, int);
	~Shape();
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
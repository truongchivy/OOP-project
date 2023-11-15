#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <iostream>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "Point2D.h"
#include "SVG.h"


using namespace std;
using namespace rapidxml;
using namespace Gdiplus;


#pragma comment (lib,"Gdiplus.lib")

class Text : public Shape {
protected:
	string text;
	int rgb[3];
	int font_size;
	Point2D toado;

public:
	void SetText(string text, int* rgb, int font_size, Point2D toado);
	VOID OnPaint(HDC hdc);
};
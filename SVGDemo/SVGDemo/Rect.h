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

class Rectt : public Shape {
protected:
	//HDC hdc;
	int rgb[3];
	Point2D toado;
	int height, width;
	double fill_opacity;
	int fill_rgb[3];
	double stroke_opacity;
};


class friendRect : public Rectt {
private:
	int thickness;

public:
	void SetRect(int* rgb, Point2D start, int height, int width, int thickness, int* fill_rgb, double fill_opacity, double stroke_opacity);

	void fillRectangle(HDC hdc, int);

	VOID Draw(HDC hdc);
};



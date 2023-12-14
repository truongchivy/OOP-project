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

class Ellipses : public Shape {
protected:
	//	HDC hdc;
	int rgb[3];
	int fill_rgb[3];
	int thickness;
	Point2D toado;
	int rX, rY;

	double stroke_opacity;
	double fill_opacity;
public:
	void SetEllipse(int* rgb, Point2D toado, int rX, int rY, int thickness, int* fill_rgb, double stroke_opacity, double fill_opacity);
	VOID Draw(HDC hdc);
};
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

class Line : public Shape {
protected:
	int rgb[3];
	int thickness;
	Point2D coor1, coor2;
	double stroke_opacity;
public:
	void SetLine(int* rgb, Point2D coor1, Point2D coor2, int thickness, double stroke_opacity);

	VOID Draw(HDC hdc, vector<Transform>& transform);
};
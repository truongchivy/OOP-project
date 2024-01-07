#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <iostream>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "Point2D.h"
#include "SVG.h"
#include "Line.h"


using namespace std;
using namespace rapidxml;
using namespace Gdiplus;


#pragma comment (lib,"Gdiplus.lib")

class BothPoly : public Shape {
protected:
	int rgb[3];
	int thickness;
	int NumberPoint;
	PointF* points;
	double stroke_opacity;
	double fill_opacity;
	int fill_rgb[3];
public:
	BothPoly();
	~BothPoly();

};


class PolyLine : public BothPoly {
public:
	void SetPolyLine(int* rgb, int thickness, int NumberPoint, Point2D* points, int* fill_rgb, double fill_opacity, double stroke_opacity);
	VOID Draw(HDC hdc, vector<Transform>& transform);
};


class PolyGon : public BothPoly {
public:
	void SetPolyGon(int* rgb, int* fill_rgb, int thickness, int NumOfPoint, Point2D* points, double fill_opacity, double stroke_opacity);
	VOID Draw(HDC hdc, vector<Transform>& transform);
};
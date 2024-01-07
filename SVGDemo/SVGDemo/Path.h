#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <iostream>
#include<vector>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "Point2D.h"
#include "SVG.h"


using namespace std;
using namespace rapidxml;
using namespace Gdiplus;


#pragma comment (lib,"Gdiplus.lib")
		
class Path : public Shape {
protected:
	int rgb[3];
	int fill_rgb[3];
	int thickness;
	vector<vector<Point2D>> points;
	vector<char> commands;
	double fill_opacity;
	double stroke_opacity;
public:
	void SetPath(int rgb[3], int fill_rgb[3], int thickness, vector<char> commands, vector<vector<Point2D>> points, double stroke_opacity, double fill_opacity);
	VOID Draw(HDC hdc, vector<Transform>& transform);
};
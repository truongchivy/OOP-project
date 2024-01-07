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
#include"transform.h"



using namespace std;
using namespace rapidxml;
using namespace Gdiplus;


#pragma comment (lib,"Gdiplus.lib")

class Shape {
protected:
	int rgb[3];
	int fill_rgb[3];
	int thickness;
	double fill_opacity;
	double stroke_opacity;

public:
	void ReadXML(HDC hdc, string);
	void HandleSVG(HDC hdc, xml_node<>*& root);
	void readTransform(string value, vector<Transform>& transform);
	void convertRGB(string value, int* rgb);
	void ReadSVG(HDC hdc, xml_node<>*& root, int* fill, double fill_opacity, int* stroke_fill,
		double stroke_opacity, int thickness, int font_size, vector<Transform> transform);
	friend string rgbToHex(int red, int green, int blue);
	friend string decap(string str);
	friend string NameToHex(string name);
	virtual VOID Draw(HDC hdc, vector<Transform>& transforms);
	virtual void fillRectangle(HDC, int);
	~Shape();
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
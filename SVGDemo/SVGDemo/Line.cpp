#include "stdafx.h"
#include "rapidxml.hpp"
#include "Line.h"

void Line::SetLine(int* rgb, Point2D coor1, Point2D coor2, int thickness, double stroke_opacity)
{
	for (int i = 0; i <= 2; i++)
	{
		this->rgb[i] = rgb[i];
	}
	this->coor1 = coor1;
	this->coor2 = coor2;
	this->thickness = thickness;
	this->stroke_opacity = stroke_opacity;
}

VOID Line::Draw(HDC hdc, vector<Transform>& transform) {
    Graphics graphics(hdc);

	for (int i = 0; i < transform.size(); i++) {
		if (transform[i].GetName() == "t")
			graphics.TranslateTransform(transform[i].GetTranslate()[0], transform[i].GetTranslate()[1]);
		if (transform[i].GetName() == "r")
			graphics.RotateTransform(transform[i].GetRotate()[0]);
		if (transform[i].GetName() == "s")
			graphics.ScaleTransform(transform[i].GetScale()[0], transform[i].GetScale()[1]);
	}

	int a = 255 * stroke_opacity;
	Pen      pen(Color(a, this->rgb[0], this->rgb[1], this->rgb[2]), this->thickness);
	graphics.DrawLine(&pen, coor1.GetX(), coor1.GetY(), coor2.GetX(), coor2.GetY());
}
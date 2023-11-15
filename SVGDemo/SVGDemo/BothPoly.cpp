#include "stdafx.h"
#include "rapidxml.hpp"
#include "BothPoly.h"

BothPoly::BothPoly() {
	NumberPoint = 0;
	points = NULL;
}

void PolyLine::SetPolyLine(int* rgb, int thickness, int NumberPoint, Point2D* points, int* fill_rgb, double fill_opacity, double stroke_opacity) {
	this->thickness = thickness;
	this->NumberPoint = NumberPoint;
	this->fill_opacity = fill_opacity;
	this->stroke_opacity = stroke_opacity;

	this->points = new PointF[NumberPoint];
	for (int i = 0; i < NumberPoint; ++i) {
		this->points[i].X = points[i].GetX();
		this->points[i].Y = points[i].GetY();
	}

	for (int i = 0; i <= 2; i++)
	{
		this->rgb[i] = rgb[i];
		this->fill_rgb[i] = fill_rgb[i];
	}
}

VOID PolyLine::OnPaint(HDC hdc) {
	Graphics        graphics(hdc);
	int a = 255 * stroke_opacity;
	Pen             pen(Color(a, this->rgb[0], this->rgb[1], this->rgb[2]), this->thickness);
	int opacity = fill_opacity * 255;
	SolidBrush brush(Color(opacity, this->fill_rgb[0], this->fill_rgb[1], this->fill_rgb[2]));
	graphics.FillPolygon(&brush, this->points, this->NumberPoint);
	graphics.DrawPolygon(&pen, this->points, this->NumberPoint);
	
}


void PolyGon::SetPolyGon(int* rgb, int* fill_rgb, int thickness, int NumberPoint, Point2D* points, double fill_opacity, double stroke_opacity) {
	this->thickness = thickness;
	this->NumberPoint = NumberPoint;
	this->fill_opacity = fill_opacity;
	this->stroke_opacity = stroke_opacity;
	this->points = new PointF[NumberPoint];
	for (int i = 0; i < NumberPoint; ++i) {
		this->points[i].X = points[i].GetX();
		this->points[i].Y = points[i].GetY();
	}
	for (int i = 0; i <= 2; i++)
	{
		this->rgb[i] = rgb[i];
		this->fill_rgb[i] = fill_rgb[i];
	}
}

VOID PolyGon::OnPaint(HDC hdc) {
	Graphics        graphics(hdc);
	int a = 255 * stroke_opacity;
	Pen             pen(Color(a, this->rgb[0], this->rgb[1], this->rgb[2]), thickness);
	int opacity = fill_opacity * 255;
	SolidBrush brush(Color(opacity, this->fill_rgb[0], this->fill_rgb[1], this->fill_rgb[2]));
	graphics.FillPolygon(&brush, this->points, this->NumberPoint);
	graphics.DrawPolygon(&pen, this->points, this->NumberPoint);
}

BothPoly::~BothPoly() {
	delete[] points;
}

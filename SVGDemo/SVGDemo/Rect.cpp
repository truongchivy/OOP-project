#include "stdafx.h"
#include "rapidxml.hpp"
#include "Rect.h"

void friendRect::SetRect(int* rgb, Point2D toado, int height, int width, int thickness, int* fill_rgb, double fill_opacity, double stroke_opacity) {
	this->toado = toado;
	this->height = height;
	this->width = width;
	this->thickness = thickness;
	this->stroke_opacity = stroke_opacity;
	for (int i = 0; i <= 2; i++)
	{
		this->rgb[i] = rgb[i];
		this->fill_rgb[i] = fill_rgb[i];
	}
}

void friendRect::fillRectangle(HDC hdc, int opacity) {
	Graphics graphics(hdc);
	SolidBrush fill_rect(Color(opacity, this->fill_rgb[0], this->fill_rgb[1], this->fill_rgb[2]));
	graphics.FillRectangle(&fill_rect, toado.GetX(), toado.GetY(), width, height);
}

VOID friendRect::OnPaint(HDC hdc) {
	Graphics graphics(hdc);
	Pen      pen(Color(rgb[0], rgb[1], rgb[2]), thickness);
	graphics.DrawRectangle(&pen, toado.GetX(), toado.GetY(), width, height);
}
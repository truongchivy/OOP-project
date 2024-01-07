#include "stdafx.h"
#include "rapidxml.hpp"
#include "Ellipse.h"

void Ellipses::SetEllipse(int* rgb, Point2D toado, int rX, int rY, int thickness, int* fill_rgb, double stroke_opacity, double fill_opacity) {
	this->toado = toado;
	this->rX = rX;
	this->rY = rY;
	this->thickness = thickness;
	this->stroke_opacity = stroke_opacity;
	this->fill_opacity = fill_opacity;
	for (int i = 0; i <= 2; i++)
	{
		this->rgb[i] = rgb[i];
		this->fill_rgb[i] = fill_rgb[i];
	}
}

VOID Ellipses::Draw(HDC hdc, vector<Transform>& transform) {
	Graphics graphics(hdc);

	for (int i = 0; i < transform.size(); i++) {
		if (transform[i].GetName() == "t")
			graphics.TranslateTransform(transform[i].GetTranslate()[0], transform[i].GetTranslate()[1]);
		if (transform[i].GetName() == "r")
			graphics.RotateTransform(transform[i].GetRotate()[0]);
		if (transform[i].GetName() == "s")
			graphics.ScaleTransform(transform[i].GetScale()[0], transform[i].GetScale()[1]);
	}

	int beta = 255 * fill_opacity;
	Pen      pen(Color(beta, rgb[0], this->rgb[1], this->rgb[2]), this->thickness);
	int opacity = fill_opacity * 255;
	SolidBrush fill_ellipse(Color(opacity, this->fill_rgb[0], this->fill_rgb[1], this->fill_rgb[2]));
	graphics.FillEllipse(&fill_ellipse, this->toado.GetX() - rX, this->toado.GetY() - rY, 2 * rX, 2 * rY);
	graphics.DrawEllipse(&pen, this->toado.GetX() - rX, this->toado.GetY() - rY, 2 * rX, 2 * rY);
}
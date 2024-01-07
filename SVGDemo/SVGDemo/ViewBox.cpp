#include "stdafx.h"
#include "ViewBox.h"

// Constructors
ViewBox::ViewBox() : Rectt() {}

ViewBox::ViewBox(Point2D toado, int width, int height, int rgb[3]) : Rectt() {
    SetViewBox(toado, width, height, rgb);
}

// Set the values for the ViewBox
void ViewBox::SetViewBox(Point2D toado, int width, int height, int rgb[3]) {
    this->toado = toado;
    this->width = width;
    this->height = height;
    for (int i = 0; i <= 2; i++)
    {
        this->rgb[i] = rgb[i];
    }
}
// Additional methods or modifications specific to ViewBox can be added here

VOID ViewBox::Draw(HDC hdc, vector<Transform>& transform) {
    Graphics graphics(hdc);

    Pen      pen(Color(rgb[0], rgb[1], rgb[2]), thickness);
    graphics.DrawRectangle(&pen, toado.GetX(), toado.GetY(), width, height);
}
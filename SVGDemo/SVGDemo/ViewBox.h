#pragma once
#include "Rect.h"

class ViewBox : public Rectt {
public:
    // Constructors
    ViewBox();
    ViewBox(Point2D toado, int width, int height, int rgb[3]);

    // Set the values for the ViewBox
    void SetViewBox(Point2D toado, int width, int height, int rgb[3]);

    // Additional methods or modifications specific to ViewBox can be added here

    VOID Draw(HDC hdc, vector<Transform>& transform);
};
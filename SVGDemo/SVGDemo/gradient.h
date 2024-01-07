#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include "SVG.h"
#include "Line.h"
#include "Rect.h"
#include "Ellipse.h"
#include "Text.h"
#include "BothPoly.h"
#include <string>
#include <sstream>
#include "Path.h"
#include <vector>
#include"transform.h"
#include"ViewBox.h"

class GradientStop {
public:

    float percent;
    int rgb[3];
    int opaque;

    GradientStop();
    GradientStop(float percent, int rgb[3], int opaque);
};

class GradientID {
public:
    string ID;
    float x1, x2, y1, y2;
    vector <GradientStop> grad;
    GradientID();
    GradientID(string ID, float x, float y);
    GradientID(string ID, float x1, float y1, float x2, float y2);
    void addStop(float percent, int rgb[3], int opaque);
    void rename(string a);
};

class GradientList {
private:
    vector <GradientID> list;
public:
    GradientList();
    void addID(GradientID ID);
    GradientID getID(int pos);
    GradientID getID(string ID);
};
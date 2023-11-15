#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <iostream>

const double epsilon = 0.0001;

class Point2D {
private:
	int x, y;

public:
	Point2D();
	Point2D(int x, int y);

	int GetX();
	int GetY();
	void SetPoint(int x, int y);

	~Point2D();
};
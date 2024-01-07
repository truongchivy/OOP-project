#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <string>
#include<vector>

using namespace std;

class Transform {
private:
	string name;
	int translate[2];
	double scale[2];
	int rotate[3];

public:
	void SetName(string name);
	string GetName();
    void SetTranslate(int, int);
	int* GetTranslate();
	void SetScale(double, double);
	double* GetScale();
    void SetRotate(int, int, int);
	int* GetRotate();
};

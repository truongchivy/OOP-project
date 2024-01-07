#include "stdafx.h"
#include "rapidxml.hpp"
#include "Transform.h"


void Transform::SetName(string name) {
	this->name = name;
}
string Transform::GetName() {
	return this->name;
}

void Transform::SetTranslate(int x1, int x2) {
	this->translate[0] = x1;
	this->translate[1] = x2;
}
int* Transform::GetTranslate() {
	return this->translate;
}

void Transform::SetScale(double x1, double x2) {
	this->scale[0] = x1;
	this->scale[1] = x2;
}
double* Transform::GetScale() {
	return this->scale;
}

void Transform::SetRotate(int x1, int x2, int x3) {
	this->rotate[0] = x1;
	this->rotate[1] = x2;
	this->rotate[2] = x3;
}
int* Transform::GetRotate() {
	return this->rotate;
}
#include "stdafx.h"
#include "rapidxml.hpp"
#include "Point2D.h"


Point2D::Point2D() {
	this->x = 0;
	this->y = 0;
}


Point2D::Point2D(int x, int y) {
	this->x = x;
	this->y = y;
}


int Point2D::GetX() {
	return this->x;
}


int Point2D::GetY() {
	return this->y;
}


void Point2D::SetPoint(int x, int y) {
	this->x = x;
	this->y = y;
}


Point2D:: ~Point2D() {

}
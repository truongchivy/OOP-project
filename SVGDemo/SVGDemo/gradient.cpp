#include"stdafx.h"
#include"Gradient.h"

GradientStop::GradientStop() {
    this->percent = 0;
    for (int i = 0; i <= 2; i++)
    {
        this->rgb[i] = 0;
    }
}

GradientStop::GradientStop(float percent, int rgb[3], int opaque) {
    this->percent = percent;
    for (int i = 0; i <= 2; i++)
    {
        this->rgb[i] = rgb[i];
    }
    this->opaque = opaque;
}

GradientID::GradientID() {
    this->x1 = 0;
    this->y1 = 0;
    this->x2 = 0;
    this->y2 = 0;
    GradientStop a;
    this->grad.push_back(a);
}

GradientID::GradientID(string ID, float x, float y) {
    this->ID = ID;
    this->x1 = x;
    this->y1 = y;
    this->x2 = x;
    this->y2 = y;

}

GradientID::GradientID(string ID, float x1, float y1, float x2, float y2) {
    this->ID = ID;
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

}

void GradientID::addStop(float percent, int rgb[3], int opaque) {
    if (percent < this->grad[this->grad.size() - 1].percent)
        percent = this->grad[this->grad.size() - 1].percent;

    GradientStop newStop(percent, rgb, opaque);

    this->grad.push_back(newStop);
}

void GradientID::rename(string a) {
    this->ID = a;
}

GradientList::GradientList() {
    GradientID a;
    this->list.push_back(a);
}

void GradientList::addID(GradientID ID) {
    this->list.push_back(ID);
}
GradientID GradientList::getID(int pos) {
    return this->list[pos];
}
GradientID GradientList::getID(string ID) {
    for (int i = 0; i < this->list.size(); i += 1) {
        if (this->list[i].ID == ID) {
            return this->list[i];
        }
    }
    GradientID a;
    return a;
}
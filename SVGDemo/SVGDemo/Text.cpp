#include "stdafx.h"
#include "rapidxml.hpp"
#include "Text.h"

void Text::SetText(string text, int* rgb, int size, Point2D toado) {
    this->text = text;
    for (int i = 0; i <= 2; i++) {
        this->rgb[i] = rgb[i];
    }
    this->toado = toado;
    this->font_size = size;
}

VOID Text::Draw(HDC hdc, vector<Transform>& transform) {
    wstring widestr = wstring(text.begin(), text.end());
    const wchar_t* widecstr = widestr.c_str();
    Graphics    graphics(hdc);

    for (int i = 0; i < transform.size(); i++) {
        if (transform[i].GetName() == "t")
            graphics.TranslateTransform(transform[i].GetTranslate()[0], transform[i].GetTranslate()[1]);
        if (transform[i].GetName() == "r")
            graphics.RotateTransform(transform[i].GetRotate()[0]);
        if (transform[i].GetName() == "s")
            graphics.ScaleTransform(transform[i].GetScale()[0], transform[i].GetScale()[1]);
    }

    SolidBrush  brush(Color(this->rgb[0], this->rgb[1], this->rgb[2]));
    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, font_size, FontStyleRegular, UnitPixel);
    int alpha = fontFamily.GetEmHeight(FontStyleRegular) / font.GetHeight(FontStyleRegular);
    PointF      pointF(toado.GetX(), toado.GetY() - font.GetHeight(FontStyleRegular) + fontFamily.GetCellDescent(FontStyleRegular) / alpha);
    graphics.DrawString(widecstr, -1, &font, pointF, &brush);
}


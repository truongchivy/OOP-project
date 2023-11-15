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

Shape::Shape() {
    ArrSize = 0;
    Arr = NULL;
}

void Shape::ReadSVG(HDC hdc) {

    ifstream infile("sample.svg");
    string tmp;
    if (infile.is_open()) {
        while (getline(infile, tmp))
            if (tmp.find("line") < tmp.length() || tmp.find("polygon") < tmp.length() || tmp.find("polyline") < tmp.length() || tmp.find("text") < tmp.length() || tmp.find("rect") < tmp.size() || tmp.find("circle") < tmp.length() || tmp.find("ellipse") < tmp.length())
                ArrSize++;
    }
    else {
        cout << "error opening file\n";
    }
    infile.close();

    Arr = new Shape * [ArrSize];
    int count = 0;

    ifstream in("sample.svg");

    while (getline(in, tmp, '\n'))
    {
        if (tmp.find("rect") < tmp.size())
        {
            int pos1, pos2;
            double fill_opacity;
            if (tmp.find("fill-opacity") < tmp.length())
            {
                pos1 = tmp.find("fill-opacity") + 14;
                pos2 = tmp.find("\"", pos1);
                fill_opacity = stod(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                fill_opacity = 1;
            }

            int rgb[3];
            if (tmp.find("stroke=") < tmp.length())
            {
                pos1 = tmp.find("stroke=") + 12;
                pos2 = tmp.find(",", pos1);

                rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                rgb[0] = rgb[1] = rgb[2] = 0;
            }

            int thickness;
            if (tmp.find("stroke-width=") < tmp.length())
            {
                pos1 = tmp.find("stroke-width=") + 14;
                pos2 = tmp.find("\"", pos1);
                thickness = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                thickness = 0;
            }

            Point2D coor;
            int x, y;
            if (tmp.find("x=") < tmp.length())
            {
                pos1 = tmp.find("x=") + 3;
                pos2 = tmp.find("\"", pos1);
                x = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = tmp.find(" y=") + 4;
                pos2 = tmp.find("\"", pos1);
                y = stoi(tmp.substr(pos1, pos2 - pos1));
                coor.SetPoint(x, y);
            }
            else
            {
                coor.SetPoint(0, 0);
            }

            int width;
            if (tmp.find(" width=") < tmp.length())
            {
                pos1 = tmp.find(" width=") + 8;
                pos2 = tmp.find("\"", pos1);
                width = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                width = 0;
            }

            int height;
            if (tmp.find(" height=") < tmp.length())
            {
                pos1 = tmp.find(" height=") + 9;
                pos2 = tmp.find("\"", pos1);
                height = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                height = 0;
            }

            int fill_rgb[3];
            if (tmp.find("fill=") < tmp.length())
            {
                pos1 = tmp.find("fill=") + 10;
                pos2 = tmp.find(",", pos1);

                fill_rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                fill_rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                fill_rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                fill_rgb[0] = fill_rgb[1] = fill_rgb[2] = 0;
            }



            double stroke_opacity;
            if (tmp.find("stroke-opacity=") < tmp.size())
            {
                pos1 = tmp.find("stroke-opacity=") + 16;
                pos2 = tmp.find("\"", pos1);
                stroke_opacity = stod(tmp.substr(pos1, pos2 - pos1));
            }
            else
                stroke_opacity = 1;

            friendRect r;
            r.SetRect(rgb, coor, height, width, thickness, fill_rgb, fill_opacity, stroke_opacity);
            Arr[count] = &r;
            Arr[count]->fillRectangle(hdc, fill_opacity * 255);
            Arr[count++]->OnPaint(hdc);
            //Text txt;
            //    string str;
            //    Point2D p;
            //    p.SetPoint(200, 30);
            //   // str = str + to_string(rgb[0]) + "," + to_string(rgb[1]) + "," + to_string(rgb[2]) + "," + to_string(height) + "," + to_string(width) + "," + to_string(thickness) + "," + to_string(fill_rgb[0]) + "," + to_string(fill_rgb[1]) + "," + to_string(fill_rgb[2]) + "," + to_string(fill_opacity) + "," + to_string(stroke_opacity);
            //    str = str + to_string(fill_rgb[0]) + ", " + to_string(fill_rgb[1]) + ", " + to_string(fill_rgb[2]) + ", " + to_string(fill_opacity);
            //    txt.SetText(str, rgb, 20, p);
            //    txt.OnPaint(hdc);

        }
        else if (tmp.find("circle") < tmp.length() || tmp.find("ellipse") < tmp.length())
        {
            Point2D coor;
            int pos1, pos2;
            int cx, cy;
            if (tmp.find("cx=") < tmp.length())
            {
                pos1 = tmp.find("cx=") + 4;
                pos2 = tmp.find("\"", pos1);
                cx = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = tmp.find("cy=") + 4;
                pos2 = tmp.find("\"", pos1);
                cy = stoi(tmp.substr(pos1, pos2 - pos1));
                coor.SetPoint(cx, cy);
            }
            else
            {
                coor.SetPoint(0, 0);
            }

            int rX, rY;
            if (tmp.find("r=") < tmp.length())
            {
                pos1 = tmp.find("r=") + 3;
                pos2 = tmp.find("\"", pos1);
                rX = rY = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else if (tmp.find("rx=") < tmp.length())
            {
                pos1 = tmp.find("rx=") + 4;
                pos2 = tmp.find("\"", pos1);
                rX = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = tmp.find("ry=") + 4;
                pos2 = tmp.find("\"", pos1);
                rY = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else {
                rX = rY = 0;
            }

            int rgb[3];
            if (tmp.find("stroke=") < tmp.length())
            {
                pos1 = tmp.find("stroke=") + 12;
                pos2 = tmp.find(",", pos1);

                rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                rgb[0] = rgb[1] = rgb[2] = 0;
            }

            int fill_rgb[3];
            if (tmp.find("fill=") < tmp.length())
            {
                pos1 = tmp.find("fill=") + 10;
                pos2 = tmp.find(",", pos1);

                fill_rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                fill_rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                fill_rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                fill_rgb[0] = fill_rgb[1] = fill_rgb[2] = 0;
            }

            int thickness;
            if (tmp.find("stroke-width") < tmp.length())
            {
                pos1 = tmp.find("stroke-width=") + 14;
                pos2 = tmp.find("\"", pos1);
                thickness = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                thickness = 0;
            }

            double stroke_opacity;
            if (tmp.find("stroke-opacity=") < tmp.size())
            {
                pos1 = tmp.find("stroke-opacity=") + 16;
                pos2 = tmp.find("\"", pos1);
                stroke_opacity = stod(tmp.substr(pos1, pos2 - pos1));
            }
            else {
                stroke_opacity = 1;
            }

            double fill_opacity;
            if (tmp.find("fill-opacity=") < tmp.length())
            {
                pos1 = tmp.find("fill-opacity=") + 14;
                pos2 = tmp.find("\"", pos1);
                fill_opacity = stod(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                fill_opacity = 1;
            }
            Ellipses e;
            e.SetEllipse(rgb, coor, rX, rY, thickness, fill_rgb, stroke_opacity, fill_opacity);

            Arr[count] = &e;
            Arr[count++]->OnPaint(hdc);
        }
        else if (tmp.find("text") < tmp.length())
        {
            int pos1, pos2;
            Point2D coor;
            int x, y;
            if (tmp.find("x=") < tmp.length())
            {
                pos1 = tmp.find("x=") + 3;
                pos2 = tmp.find("\"", pos1);
                x = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = tmp.find(" y=") + 4;
                pos2 = tmp.find("\"", pos1);
                y = stoi(tmp.substr(pos1, pos2 - pos1));
                coor.SetPoint(x, y);
            }
            else
            {
                coor.SetPoint(0, 0);
            }

            int rgb[3];
            if (tmp.find("fill=") < tmp.length())
            {
                pos1 = tmp.find("fill=") + 10;
                pos2 = tmp.find(",", pos1);

                rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                rgb[0] = rgb[1] = rgb[2] = 0;
            }

            int font_size;
            if (tmp.find("font-size=") < tmp.length())
            {
                pos1 = tmp.find("font-size=") + 11;
                pos2 = tmp.find("\"", pos1);
                font_size = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                font_size = 0;
            }

            string t;
            if (tmp.find(">") < tmp.length())
            {
                pos1 = tmp.find(">") + 1;
                pos2 = tmp.find("<", pos1);
                t = tmp.substr(pos1, pos2 - pos1);
            }
            else
            {
                t = "";
            }

            Text text;
            text.SetText(t, rgb, font_size, coor);
            Arr[count] = &text;
            Arr[count++]->OnPaint(hdc);

        }

        else if (tmp.find("polyline") < tmp.length())
        {

            int pos1, pos2;
            int rgb[3];
            if (tmp.find("stroke=") < tmp.length())
            {
                pos1 = tmp.find("stroke=") + 12;
                pos2 = tmp.find(",", pos1);

                rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                rgb[0] = rgb[1] = rgb[2] = 0;
            }

            int thickness;
            if (tmp.find("stroke-width=") < tmp.length())
            {
                pos1 = tmp.find("stroke-width=") + 14;
                pos2 = tmp.find("\"", pos1);
                thickness = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                thickness = 0;
            }

            double stroke_opacity;
            if (tmp.find("stroke-opacity=") < tmp.size())
            {
                pos1 = tmp.find("stroke-opacity=") + 16;
                pos2 = tmp.find("\"", pos1);
                stroke_opacity = stod(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                stroke_opacity = 1;
            }

            int fill_rgb[3];
            if (tmp.find("fill=") < tmp.length())
            {
                pos1 = tmp.find("fill=") + 10;
                pos2 = tmp.find(",", pos1);

                fill_rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                fill_rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                fill_rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                fill_rgb[0] = fill_rgb[1] = fill_rgb[2] = 0;
            }

            double fill_opacity;
            if (tmp.find("fill-opacity=") < tmp.length())
            {
                pos1 = tmp.find("fill-opacity=") + 14;
                pos2 = tmp.find("\"", pos1);
                fill_opacity = stod(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                fill_opacity = 1;
            }

            string points;
            int NumberPoint = 0;
            Point2D* pointArrays = NULL;
            vector<Point2D> pointArray;
            if (tmp.find("points=") < tmp.size())
            {
                pos1 = tmp.find("points=") + 8;
                pos2 = tmp.find("\"", pos1);
                points = tmp.substr(pos1, pos2 - pos1);
                stringstream ss(points);
                string s;

                while (getline(ss, s, ' ')) {
                    stringstream pointSS(s);
                    string coor;
                    vector<int> coors;

                    while (getline(pointSS, coor, ',')) {
                        coors.push_back(stoi(coor));
                    }

                    Point2D point(coors[0], coors[1]);
                    pointArray.push_back(point);
                }

                NumberPoint = pointArray.size();
                pointArrays = new Point2D[NumberPoint];
                for (int i = 0; i < NumberPoint; i++)
                {
                    pointArrays[i] = pointArray[i];
                }
            }
            else
            {
                NumberPoint = 0;
                Point2D* pointArrays = NULL;
            }
            PolyLine p;
            p.SetPolyLine(rgb, thickness, NumberPoint, pointArrays, fill_rgb, fill_opacity, stroke_opacity);
            Arr[count] = &p;
            Arr[count++]->OnPaint(hdc);
        }
        else if (tmp.find("polygon") < tmp.length())
        {

            int pos1, pos2;

            int fill_rgb[3];
            if (tmp.find("fill=") < tmp.length())
            {
                pos1 = tmp.find("fill=") + 10;
                pos2 = tmp.find(",", pos1);

                fill_rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                fill_rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                fill_rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                fill_rgb[0] = fill_rgb[1] = fill_rgb[2] = 0;
            }

            double fill_opacity;
            if (tmp.find("fill-opacity=") < tmp.length())
            {
                pos1 = tmp.find("fill-opacity=") + 14;
                pos2 = tmp.find("\"", pos1);
                fill_opacity = stod(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                fill_opacity = 1;
            }

            int thickness;
            if (tmp.find("stroke-width=") < tmp.length())
            {
                pos1 = tmp.find("stroke-width=") + 14;
                pos2 = tmp.find("\"", pos1);
                thickness = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                thickness = 0;
            }

            int rgb[3];
            if (tmp.find("stroke=") < tmp.length())
            {
                pos1 = tmp.find("stroke=") + 12;
                pos2 = tmp.find(",", pos1);

                rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                rgb[0] = rgb[1] = rgb[2] = 0;
            }

            double stroke_opacity;
            if (tmp.find("stroke-opacity=") < tmp.size())
            {
                pos1 = tmp.find("stroke-opacity=") + 16;
                pos2 = tmp.find("\"", pos1);
                stroke_opacity = stod(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                stroke_opacity = 1;
            }
            string points;
            Point2D* pointArrays = NULL;
            int NumberPoint = 0;
            vector<Point2D> pointArray;
            if (tmp.find("points=") < tmp.size())
            {
                pos1 = tmp.find("points=") + 8;
                pos2 = tmp.find("\"", pos1);
                points = tmp.substr(pos1, pos2 - pos1);
                stringstream ss(points);
                string s;

                while (getline(ss, s, ' ')) {
                    stringstream pointSS(s);
                    string coor;
                    vector<int> coors;

                    while (getline(pointSS, coor, ',')) {
                        coors.push_back(stoi(coor));
                    }

                    Point2D point(coors[0], coors[1]);
                    pointArray.push_back(point);
                }

                NumberPoint = pointArray.size();
                pointArrays = new Point2D[NumberPoint];
                for (int i = 0; i < NumberPoint; i++)
                {
                    pointArrays[i] = pointArray[i];
                }
            }
            else
            {
                NumberPoint = 0;
                Point2D* pointArrays = NULL;
            }
            PolyGon gon;

            gon.SetPolyGon(rgb, fill_rgb, thickness, NumberPoint, pointArrays, fill_opacity, stroke_opacity);
            Arr[count] = &gon;
            Arr[count++]->OnPaint(hdc);
            //Text txt;
            //   string str;
            //   Point2D p;
            //   p.SetPoint(200, 30);
            //  // str = str + to_string(rgb[0]) + "," + to_string(rgb[1]) + "," + to_string(rgb[2]) + "," + to_string(height) + "," + to_string(width) + "," + to_string(thickness) + "," + to_string(fill_rgb[0]) + "," + to_string(fill_rgb[1]) + "," + to_string(fill_rgb[2]) + "," + to_string(fill_opacity) + "," + to_string(stroke_opacity);
            //   str = str + to_string(fill_rgb[0]) + ", " + to_string(fill_rgb[1]) + ", " + to_string(fill_rgb[2]) + ", " + to_string(fill_opacity);
            //   txt.SetText(str, rgb, 20, p);
            //   txt.OnPaint(hdc);
        }
        else if (tmp.find("line") < tmp.length())
        {

            int pos1, pos2;

            Point2D coor1, coor2;
            int x, y;
            if (tmp.find("x1=") < tmp.length())
            {
                pos1 = tmp.find("x1=") + 4;
                pos2 = tmp.find("\"", pos1);
                x = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = tmp.find("y1=") + 4;
                pos2 = tmp.find("\"", pos1);
                y = stoi(tmp.substr(pos1, pos2 - pos1));
                coor1.SetPoint(x, y);
            }
            else
            {
                coor1.SetPoint(0, 0);
            }

            if (tmp.find("x2=") < tmp.length())
            {
                pos1 = tmp.find("x2=") + 4;
                pos2 = tmp.find("\"", pos1);
                x = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = tmp.find("y2=") + 4;
                pos2 = tmp.find("\"", pos1);
                y = stoi(tmp.substr(pos1, pos2 - pos1));
                coor2.SetPoint(x, y);
            }
            else
            {
                coor2.SetPoint(0, 0);
            }

            int rgb[3];
            if (tmp.find("stroke=") < tmp.length())
            {
                pos1 = tmp.find("stroke=") + 12;
                pos2 = tmp.find(",", pos1);

                rgb[0] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(",", pos1);
                rgb[1] = stoi(tmp.substr(pos1, pos2 - pos1));

                pos1 = pos2 + 1;
                pos2 = tmp.find(")", pos1);
                rgb[2] = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                rgb[0] = rgb[1] = rgb[2] = 0;
            }

            int thickness;
            if (tmp.find("stroke-width") < tmp.length())
            {
                pos1 = tmp.find("stroke-width=") + 14;
                pos2 = tmp.find("\"", pos1);
                thickness = stoi(tmp.substr(pos1, pos2 - pos1));
            }
            else
            {
                thickness = 0;
            }

            double stroke_opacity;
            if (tmp.find("stroke-opacity=") < tmp.size())
            {
                pos1 = tmp.find("stroke-opacity=") + 16;
                pos2 = tmp.find("\"", pos1);
                stroke_opacity = stod(tmp.substr(pos1, pos2 - pos1));
            }
            else
                stroke_opacity = 1;

            Line l;
            l.SetLine(rgb, coor1, coor2, thickness, stroke_opacity);

            Arr[count] = &l;
            Arr[count++]->OnPaint(hdc);
        }



    }
    in.close();
}

VOID Shape::OnPaint(HDC hdc) {}
void Shape::fillRectangle(HDC hdc, int opacity) {}

Shape::~Shape() {
    delete[] Arr;
}
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow) {
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;


    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("SVG Demo"),         // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);


    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Shape        shape;
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        shape.ReadSVG(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

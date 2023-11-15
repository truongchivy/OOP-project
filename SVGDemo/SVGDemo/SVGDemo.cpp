#include "stdafx.h"
#include "rapidxml.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

VOID OnPaint(HDC hdc)
{
   // Ref: https://docs.microsoft.com/en-us/windows/desktop/gdiplus/-gdiplus-getting-started-use

   Graphics graphics(hdc);

   Pen rect1(Color(200, 200, 200), 2.0f);
   SolidBrush fill_rect1(Color(2, 0, 0, 0));
   graphics.FillRectangle(&fill_rect1, 25, 25, 800, 400);
   graphics.DrawRectangle(&rect1, 25, 25, 800, 400);

   Pen rect2(Color(55, 55, 55), 2.0f);
   SolidBrush fill_rect2(Color(51, 200, 100, 150));
   graphics.FillRectangle(&fill_rect2, 20, 20, 800, 400);
   graphics.DrawRectangle(&rect2, 20, 20, 800, 400);

   FontFamily fontFamily(L"Times New Roman");

   Font font1(&fontFamily, 40, FontStyleRegular, UnitPixel);
   SolidBrush fill_text1(Color(0, 0, 0, 255));
   WCHAR text1[] = L"SVG Demo";
   graphics.DrawString(text1, -1, &font1, PointF(0, 0), &fill_text1); 

   Pen rect3(Color(255, 0, 0), 2.0f);
   graphics.DrawRectangle(&rect3, 0, 0, 200, 50);

   Font font2(&fontFamily, 30, FontStyleRegular, UnitPixel);
   SolidBrush fill_text2(Color( 255, 0, 255));
   WCHAR text2[] = L"Nguyen Van A";
   graphics.DrawString(text2, -1, &font2, PointF(373, 373), &fill_text2); 

   Pen cirle(Color(178, 0, 255, 255), 10.0f);
   SolidBrush fill_cirle(Color(127, 255, 255, 0));
   graphics.FillEllipse(&fill_cirle, 100, 200, 200, 200); 
   graphics.DrawEllipse(&cirle, 100, 200, 200, 200);

   Pen polyline1(Color(178, 255, 0, 0), 2.0f); // de y, kiem tra lai sau
   SolidBrush fill_polyline1(Color(128, 0, 255, 255));
   Point polyline1_points[] = { {5, 37}, {15, 37}, {15, 32}, {25, 32}, {25, 37}, {35, 37}, {35, 25}, {45, 25}, {45, 37}, {55, 37}, 
   {55, 17}, {65, 17}, {65, 37}, {75, 37}, {75, 10}, {85, 10}, {85, 37}, {95, 37}, {95, 2}, {105, 2}, {105, 37}, {115, 37}, {5, 37} }; // toa do cac diem, diem 24 la diem dau
   graphics.DrawPolygon(&polyline1, polyline1_points, 23); 
   graphics.FillPolygon(&fill_polyline1, polyline1_points, 24);

   Pen ellipse(Color(178, 255, 255, 0), 3.0f);
   SolidBrush fill_ellipse(Color(128, 0, 255, 0));
   graphics.FillEllipse(&fill_ellipse, 400, 50, 200, 100);
   graphics.DrawEllipse(&ellipse, 400, 50, 200, 100);

   Pen line1(Color(178, 0, 0, 255), 5.0f);
   graphics.DrawLine(&line1, 10, 30, 30, 10);

   Pen line2(Color(204, 0, 0, 255), 10.0f);
   graphics.DrawLine(&line2, 30, 30, 50, 10);

   Pen line3(Color(229, 0, 0, 255), 15.0f);
   graphics.DrawLine(&line3, 50, 30, 70, 10);

   Pen line4(Color(229, 0, 0, 255), 20.0f);
   graphics.DrawLine(&line4, 70, 30, 90, 10);

   Pen line5(Color(255, 0, 0, 255), 25.0f);
   graphics.DrawLine(&line5, 90, 30, 110, 10);

   Pen poly1(Color(178, 255, 0, 102), 10.0f);
   SolidBrush fill_poly1(Color (127, 153, 204, 255));
   Point poly1_points[] = { {850, 75}, {958, 137}, {958, 262}, {850, 325}, {742, 262}, {742, 137} };
   graphics.FillPolygon(&fill_poly1, poly1_points, 6);
   graphics.DrawPolygon(&poly1, poly1_points, 6);

   Pen poly2(Color(178, 255, 0, 0), 10.0f);
   SolidBrush fill_poly2(Color(153, 255, 255, 0));
   Point points_poly2[] = { {350, 75}, {379, 161}, {469, 161}, {397, 215}, {423, 301}, {350, 250}, {277, 301}, {303, 215}, {231, 161}, {321, 161} };
   graphics.FillPolygon(&fill_poly2, points_poly2, 10);
   graphics.DrawPolygon(&poly2, points_poly2, 10);

   SolidBrush polyline2(Color (127, 0, 0, 0));
   Point polyline2_points[] = { {0, 40}, {40, 40}, {40, 80}, {80, 80}, {80, 120}, {120, 120}, {120, 140} };
   graphics.FillPolygon(&polyline2, polyline2_points, 7);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
   HWND                hWnd;
   MSG                 msg;
   WNDCLASS            wndClass;
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;

   // Read XML
   xml_document<> doc;
   xml_node<> *rootNode;
   // Read the xml file into a vector
   ifstream file("sample.svg");
   vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
   buffer.push_back('\0');
   // Parse the buffer using the xml file parsing library into doc 
   doc.parse<0>(&buffer[0]);

   rootNode = doc.first_node();
   xml_node<> *node = rootNode->first_node();

   while (node != NULL) {
	   char *nodeName = node->name();
	   xml_attribute<> *firstAttribute = node->first_attribute();
	   char *attributeName = firstAttribute->name();
	   char *attributeValue = firstAttribute->value();
	   xml_attribute<> *secondAttribute = firstAttribute->next_attribute();
	   // Set breakpoint here to view value
	   // Ref: http://rapidxml.sourceforge.net/manual.html
	   node = node->next_sibling();
   }

   
   
   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   wndClass.style          = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc    = WndProc;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 0;
   wndClass.hInstance      = hInstance;
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = TEXT("GettingStarted");
   
   RegisterClass(&wndClass);
   
   hWnd = CreateWindow(
      TEXT("GettingStarted"),   // window class name
      TEXT("SVG Demo"),  // window caption
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
   
   while(GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   
   GdiplusShutdown(gdiplusToken);
   return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
   WPARAM wParam, LPARAM lParam)
{
   HDC          hdc;
   PAINTSTRUCT  ps;
   
   switch(message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      OnPaint(hdc);
      EndPaint(hWnd, &ps);
      return 0;
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
} // WndProc
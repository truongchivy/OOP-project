#include "stdafx.h"
#include "rapidxml.hpp"
#include "Path.h"

void Path::SetPath(int* rgb, int* fill_rgb, int thickness, vector<char> commands, vector<vector<Point2D>> points, double fill_opacity, double stroke_opacity) {

    this->thickness = thickness;
    this->stroke_opacity = stroke_opacity;
    this->fill_opacity = fill_opacity;  
    for (int i = 0; i < 3; i++)
    {
        this->rgb[i] = rgb[i];
        this->fill_rgb[i] = fill_rgb[i];
    }
    this->commands = commands;
    this->points = points;
}

VOID Path::Draw(HDC hdc, vector<Transform>& transform) {
    Graphics        graphics(hdc);
    PointF lastPoint;
    GraphicsPath    Path;
    double          alpha = 255 * fill_opacity;
    SolidBrush      solidBrush(Color(alpha, fill_rgb[0], fill_rgb[1], fill_rgb[2]));

    for (int i = 0; i < transform.size(); i++) {
        if (transform[i].GetName() == "t")
            graphics.TranslateTransform(transform[i].GetTranslate()[0], transform[i].GetTranslate()[1]);
        if (transform[i].GetName() == "r")
            graphics.RotateTransform(transform[i].GetRotate()[0]);
        if (transform[i].GetName() == "s")
            graphics.ScaleTransform(transform[i].GetScale()[0], transform[i].GetScale()[1]);
    }
 
     for (int i = 0; i < commands.size(); ++i) {
          if (commands[i] == 'C' || commands[i] == 'c') {
             Point* pts = new Point[points[i].size()];
             for (int k = 0; k < points[i].size(); ++k) {
                 pts[k].X = points[i][k].GetX();
                 pts[k].Y = points[i][k].GetY();
             }
             Path.AddBeziers(pts, points[i].size());
             delete[] pts;
         }
         else if (commands[i] == 'z' || commands[i] == 'Z') {
             Path.CloseFigure();
         }
          //else
         //if (commands[i] == 'M' || commands[i] == 'm') {
         //    Path.StartFigure();
         //    Path.AddLine(points[i][0].GetX(), points[i][0].GetY(), points[i][0].GetX(), points[i][0].GetY());
         //}
         //else if (commands[i] == 'L' || commands[i] == 'l') {
         //    Path.AddLine(points[i][0].GetX(), points[i][0].GetY(), points[i][1].GetX(), points[i][1].GetY());
         //}
         //else if (commands[i] == 'V' || commands[i] == 'v') {
         //    // Vertical line command
         //    // Xử lý vẽ đường thẳng theo chiều dọc
         //    double y = points[i][0].GetY(); // Lấy giá trị y mới
         //    Point currentPoint = Path.GetLastPoint(); // Lấy điểm cuối cùng trong đường path
         //    Path.AddLine(currentPoint.X, currentPoint.Y, currentPoint.X, y);
         //}
         //else if (commands[i] == 'H' || commands[i] == 'h') {
         //    // Horizontal line command
         //    // Xử lý vẽ đường thẳng theo chiều ngang
         //    double x = points[i][0].GetX(); // Lấy giá trị x mới
         //    Point currentPoint = Path.GetLastPoint(); // Lấy điểm cuối cùng trong đường path
         //    Path.AddLine(currentPoint.X, currentPoint.Y, x, currentPoint.Y);
         //}
         //else if (commands[i] == 'Z' || commands[i] == 'z') {
            //Path.CloseFigure();
         //}
         else {
             Path.AddLine(points[i][0].GetX(), points[i][0].GetY(),points[i][1].GetX(), points[i][1].GetY());
         }
     }
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.FillPath(&solidBrush, &Path);

    if (thickness != 0) {
        alpha = 255 * stroke_opacity;
        Pen        pen(Color(alpha, this->rgb[0], this->rgb[1], this->rgb[2]), this->thickness);
        graphics.DrawPath(&pen, &Path);
    }
}

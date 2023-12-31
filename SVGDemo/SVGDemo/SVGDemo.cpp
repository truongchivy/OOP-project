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

void Shape::ReadXML(HDC hdc, string file_name) {
    ifstream read(file_name);

    if (!read.is_open()) {
        cerr << "Không thể mở tệp " << file_name << endl;
    }
    vector<char> buffer((istreambuf_iterator<char>(read)), istreambuf_iterator<char>());
    buffer.push_back('\0'); 
    rapidxml::xml_document<> doc;

    doc.parse<0>(&buffer[0]);

    rapidxml::xml_node<>* root = doc.first_node("svg");
    read.close();
    HandleSVG(hdc, root);
}

void Shape::HandleSVG(HDC hdc, xml_node<>*& root) {
    for (rapidxml::xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {
        string name = node->name();
        if (name == "g") {
            double fill_opacity = 1, stroke_opacity = 1;
            int thickness = 0, font_size = 0;
            vector<Transform> transform;
            int fill[] = { 0, 0, 0 },
                stroke_fill[] = { 0,0,0 };
            for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
                name = attr->name();
                string value = attr->value();

                if (name == "fill-opacity")
                    fill_opacity = stod(value);

                else if (name == "stroke-opacity")
                    stroke_opacity = stod(value);

                else if (name == "fill") {
                    if (value == "none" || value == "transparent")
                        fill_opacity = 0;
                    else
                        convertRGB(value, fill);
                }

                else if (name == "stroke") {
                    if (value == "none" || value == "transparent")
                        stroke_opacity = 0;
                    else {
                        convertRGB(value, stroke_fill);
                        if (thickness == 0)
                            thickness = 1;
                    }
                }

                else if (name == "stroke-width") {
                    thickness = stoi(value);
                }

                else if (name == "font-size")
                {
                    font_size = stoi(value);
                }
                else if (name == "transform") {
                    readTransform(value, transform);
                }
            }
            for (rapidxml::xml_node<>* n = node->first_node(); n; n = n->next_sibling()) {
                ReadSVG(hdc, n, fill, fill_opacity, stroke_fill, stroke_opacity, thickness, font_size, transform);
            }

        }
        else {
            double fill_opacity = 1, stroke_opacity = 1;
            int thickness = 0, font_size = 0;
            vector<Transform> transform;
            int fill[] = { 0, 0, 0 },
                stroke_fill[] = { 0,0,0 };
            ReadSVG(hdc, node, fill, fill_opacity, stroke_fill, stroke_opacity, thickness, font_size, transform);
        }

    }
}

struct ColorName {
    string name;
    string hex;
};

const ColorName Dictionary[] = {
    {"aliceblue", "#F0F8FF"},                  //A
    {"antiquewhite", "#FAEBD7"},
    {"aqua", "#00FFFF"},
    {"aquamarine", "#7FFFD4"},
    {"azure", "#F0FFFF"},
    {"beige", "#F5F5DC"},                  //B
    {"bisque", "#FFE4C4"},
    {"black", "#000000"},
    {"blanchedalmond", "#FFEBCD"},
    {"blue", "#0000FF"},
    {"blueviolet", "#8A2BE2"},
    {"brown", "#A52A2A"},
    {"burlywood", "#DEB887"},
    {"cadetblue", "#5F9EA0"},                  //C
    {"chartreuse", "#7FFF00"},
    {"chocolate", "#D2691E"},
    {"coral", "#FF7F50"},
    {"cornflowerblue", "#6495ED"},
    {"cornsilk", "#FFF8DC"},
    {"crimson", "#DC143C"},
    {"cyan", "#00FFFF"},
    {"darkblue", "#00008B"},                  //D
    {"darkcyan", "#008B8B"},
    {"darkgoldenrod", "#B8860B"},
    {"darkgray", "#A9A9A9"},
    {"darkgreen", "#006400"},
    {"darkkhaki", "#BDB76B"},
    {"darkmagenta", "#8B008B"},
    {"darkolivegreen", "#556B2F"},
    {"darkorange", "#FF8C00"},
    {"darkorchid", "#9932CC"},
    {"darkred", "#8B0000"},
    {"darksalmon", "#E9967A"},
    {"darkseagreen", "#8FBC8F"},
    {"darkslateblue", "#483D8B"},
    {"darkslategray", "#2F4F4F"},
    {"darkturquoise", "#00CED1"},
    {"darkviolet", "#9400D3"},
    {"deeppink", "#FF1493"},
    {"deepskyblue", "#00BFFF"},
    {"dimgray", "#696969"},
    {"dodgerblue", "#1E90FF"},
    {"firebrick", "#B22222"},                  //F
    {"floralwhite", "#FFFAF0"},
    {"forestgreen", "#228B22"},
    {"fuchsia", "#FF00FF"},
    {"gainsboro", "#DCDCDC"},                  //G
    {"ghostwhite", "#F8F8FF"},
    {"gold", "#FFD700"},
    {"goldenrod", "#DAA520"},
    {"gray", "#808080"},
    {"green", "#008000"},
    {"greenyellow", "#ADFF2F"},
    {"honeydew", "#F0FFF0"},                  //H
    {"hotpink", "#FF69B4"},
    {"indianred", "#CD5C5C"},                  //I
    {"indigo", "#4B0082"},
    {"ivory", "#FFFFF0"},
    {"khaki", "#F0E68C"},                  //K
    {"lavender", "#E6E6FA"},                  //L
    {"lavenderblush", "#FFF0F5"},
    {"lawngreen", "#7CFC00"},
    {"lemonchiffon", "#FFFACD"},
    {"lightblue", "#ADD8E6"},
    {"lightcoral", "#F08080"},
    {"lightcyan", "#E0FFFF"},
    {"lightgoldenrodyellow", "#FAFAD2"},
    {"lightgray", "#D3D3D3"},
    {"lightgreen", "#90EE90"},
    {"lightpink", "#FFB6C1"},
    {"lightsalmon", "#FFA07A"},
    {"lightseagreen", "#20B2AA"},
    {"lightskyblue", "#87CEFA"},
    {"lightslategray", "#778899"},
    {"lightsteelblue", "#B0C4DE"},
    {"lightyellow", "#FFFFE0"},
    {"lime", "#00FF00"},
    {"limegreen", "#32CD32"},
    {"linen", "#FAF0E6"},
    {"magenta", "#FF00FF"},                  //M
    {"maroon", "#800000"},
    {"mediumaquamarine", "#66CDAA"},
    {"mediumblue", "#0000CD"},
    {"mediumorchid", "#BA55D3"},
    {"mediumpurple", "#9370DB"},
    {"mediumseagreen", "#3CB371"},
    {"mediumslateblue", "#7B68EE"},
    {"mediumspringgreen", "#00FA9A"},
    {"mediumturquoise", "#48D1CC"},
    {"mediumvioletred", "#C71585"},
    {"midnightblue", "#191970"},
    {"mintcream", "#F5FFFA"},
    {"mistyrose", "#FFE4E1"},
    {"moccasin", "#FFE4B5"},
    {"navajowhite", "#FFDEAD"},                  //N
    {"navy", "#000080"},
    {"oldlace", "#FDF5E6"},                  //O
    {"olive", "#808000"},
    {"olivedrab", "#6B8E23"},
    {"orange", "#FFA500"},
    {"orangered", "#FF4500"},
    {"orchid", "#DA70D6"},
    {"palegoldenrod", "#EEE8AA"},                  //P
    {"palegreen", "#98FB98"},
    {"paleturquoise", "#AFEEEE"},
    {"palevioletred", "#DB7093"},
    {"papayawhip", "#FFEFD5"},
    {"peachpuff", "#FFDAB9"},
    {"peru", "#CD853F"},
    {"pink", "#FFC0CB"},
    {"plum", "#DDA0DD"},
    {"powderblue", "#B0E0E6"},
    {"purple", "#800080"},
    {"rebeccapurple", "#663399"},                  //R
    {"red", "#FF0000"},
    {"rosybrown", "#BC8F8F"},
    {"royalblue", "#4169E1"},
    {"saddlebrown", "#8B4513"},                  //S
    {"salmon", "#FA8072"},
    {"sandybrown", "#F4A460"},
    {"seagreen", "#2E8B57"},
    {"seashell", "#FFF5EE"},
    {"sienna", "#A0522D"},
    {"silver", "#C0C0C0"},
    {"skyblue", "#87CEEB"},
    {"slateblue", "#6A5ACD"},
    {"slategray", "#708090"},
    {"snow", "#FFFAFA"},
    {"springgreen", "#00FF7F"},
    {"steelblue", "#4682B4"},
    {"tan", "#D2B48C"},                  //T
    {"teal", "#008080"},
    {"thistle", "#D8BFD8"},
    {"tomato", "#FF6347"},
    {"turquoise", "#40E0D0"},
    {"violet", "#EE82EE"},                  //V
    {"wheat", "#F5DEB3"},                  //W
    {"white", "#FFFFFF"},
    {"whitesmoke", "#F5F5F5"},
    {"yellow", "#FFFF00"},                  //Y
    {"yellowgreen", "#9ACD32"}

};

const short DictionaryEntry[] =
{ 0, 5, 13,//ABC
21, 42, 42,//D F
46, 53, 55,//GHI
58, 58, 59,// KL
79, 94, 96,//MNO
102, 113, 113, 117,//P RS
130, 135, 135,//T V
136, 139, 139, 141,//W Y
141 };

string decap(string str) {
    string s = "";
    for (int i = 0; i < str.length(); i += 1) {
        if (64 < str[i] && str[i] < 91) {
            s += (char(str[i]) + 32);
        }
        else
            s = s + str[i];
    }
    return s;
}

string NameToHex(string name) {
    name = decap(name);
    for (int i = DictionaryEntry[(name[0] - 97)]; i < DictionaryEntry[(name[0] - 96)]; i += 1) {
        if (Dictionary[i].name == name)
            return Dictionary[i].hex;
    }

    return "#FFFFFF";
}

void Shape::convertRGB(string data, int* rgb)
{
    int color;
    if (data[0] == 'r' && data[1] == 'g' && data[2] == 'b') {
        int ind1, ind2;
        for (int i = 0; i < data.size(); ++i)
            if (data[i] == '(')
                ind1 = i + 1;
        ind2 = data.find(",", ind1);
        color = stoi(data.substr(ind1, ind2 - ind1));
        rgb[0] = color > 255 ? 255 : color;

        ind1 = ind2 + 1;
        ind2 = data.find(",", ind1);
        color = stoi(data.substr(ind1, ind2 - ind1));
        rgb[1] = color > 255 ? 255 : color;

        ind1 = ind2 + 1;
        ind2 = data.find(")", ind1);
        color = stoi(data.substr(ind1, ind2 - ind1));
        rgb[2] = color > 255 ? 255 : color;
    }
    else if (data[0] == '#') {
        unsigned int hexValue;
        if (data.length() == 4) {
            char a1 = data[1], a2 = data[2], a3 = data[3];
            data = "#" + string(1, a1) + string(1, a1) + string(1, a2) + string(1, a2) + string(1, a3) + string(1, a3);
        }
        istringstream(data.substr(1)) >> hex >> hexValue;

        rgb[0] = (hexValue >> 16) & 0xFF;
        rgb[1] = (hexValue >> 8) & 0xFF;
        rgb[2] = hexValue & 0xFF;
        int alpha = (hexValue >> 24) & 0xFF;
    }
    else {
        /*int pos1, pos2;
        pos1 = data.find("fill=") + 6;
        pos2 = data.find("\"", pos1);
        string color = data.substr(pos1, pos2 - pos1);*/
        string hexColor = NameToHex(data);
        unsigned int valueHex;
        if (hexColor.length() == 4) {
            char a1 = hexColor[1], a2 = hexColor[2], a3 = hexColor[3];
            data = "#" + string(1, a1) + string(1, a1) + string(1, a2) + string(1, a2) + string(1, a3) + string(1, a3);
        }
        istringstream(hexColor.substr(1)) >> hex >> valueHex;

        rgb[0] = (valueHex >> 16) & 0xFF;
        rgb[1] = (valueHex >> 8) & 0xFF;
        rgb[2] = valueHex & 0xFF;
        int alpha = (valueHex >> 24) & 0xFF;
    }

}

void Shape::readTransform(string value, vector<Transform>& transform) {
    for (int i = 0; i < value.size(); ++i) {
        if (value[i] == '\n' || value[i] == ',')
            value[i] = ' ';
    }

    Transform trans;

    for (int i = 0; i < value.size(); ++i) {
        if (value[i] == 't') {
            int x1 = 0, x2 = 0, x3 = 0;
            trans.SetName("t");

            int ind1 = i + 10;
            int ind2;

            if (value.find(" ", ind1) < value.find(")", ind1)) {
                ind2 = value.find(" ", ind1);
                x1 = stoi(value.substr(ind1, ind2 - ind1));


                ind1 = ind2 + 1;
                ind2 = value.find(")", ind1);
                x2 = stoi(value.substr(ind1, ind2 - ind1));
            }
            else {
                ind2 = value.find(")", ind1);
                x1 = stoi(value.substr(ind1, ind2 - ind1));
                x2 = 0;
            }
            trans.SetTranslate(x1, x2);
            transform.push_back(trans);

            i = ind2 + 1;
        }

        else if (value[i] == 'r') {
            int x1 = 0, x2 = 0, x3 = 0;
            trans.SetName("r");

            int ind1 = i + 7;
            int ind2;

            if (value.find(" ", ind1) < value.find(")", ind1)) {
                ind2 = value.find(" ", ind1);
                x1 = stoi(value.substr(ind1, ind2 - ind1));
                ind1 = ind2 + 1;

                if (value.find(" ", ind1) < value.find(")", ind1)) {
                    ind2 = value.find(" ", ind1);
                    x2 = stoi(value.substr(ind1, ind2 - ind1));
                    ind1 = ind2 + 1;

                    ind2 = value.find(")", ind1);
                    x3 = stoi(value.substr(ind1, ind2 - ind1));
                }
                else {
                    ind2 = value.find(")", ind1);
                    x2 = stoi(value.substr(ind1, ind2 - ind1));
                }
            }
            else {
                ind2 = value.find(")", ind1);
                x1 = stoi(value.substr(ind1, ind2 - ind1));
                x2 = 0;
            }


            trans.SetRotate(x1, x2, x3);
            transform.push_back(trans);

            i = ind2 + 1;
        }

        else if (value[i] == 's') {
            double x1 = 0, x2 = 0;
            trans.SetName("s");

            int ind1 = i + 6;
            int ind2;

            if (value.find(" ", ind1) < value.find(")", ind1)) {
                ind2 = value.find(" ", ind1);
                x1 = stod(value.substr(ind1, ind2 - ind1));


                ind1 = ind2 + 1;
                ind2 = value.find(")", ind1);
                x2 = stod(value.substr(ind1, ind2 - ind1));
            }
            else {
                ind2 = value.find(")", ind1);
                x1 = x2 = stod(value.substr(ind1, ind2 - ind1));
            }
            trans.SetScale(x1, x2);
            transform.push_back(trans);

            i = ind2 + 1;
        }
    }
}

void Shape::ReadSVG(HDC hdc, xml_node<>*& root, int* fill, double fill_opacity, int* stroke_fill, double stroke_opacity, int thickness, int font_size, vector<Transform> transform) {
    /*ifstream infile("sample.svg");
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

    Arr = new Shape * [ArrSize];*/

    string name = root->name();
    double fill_opacity2 = fill_opacity, stroke_opacity2 = stroke_opacity;
    int thickness2 = thickness, font_size2 = font_size;
    int fill2[] = { 0, 0, 0 },
        stroke_fill2[] = { 0,0,0 };
    for (int i = 0; i < 3; ++i) {
        fill2[i] = fill[i];
        stroke_fill2[i] = stroke_fill[i];
    }
    if (name == "viewBox")
    {
        int  width, height, x = 10, y = 10;
        Point2D start;

        for (rapidxml::xml_attribute<>* node = root->first_attribute(); node; node = node->next_attribute()) {
            name = node->name();
            string value = node->value();
            if (name == "stroke") {
                if (value == "none" || value == "transparent")
                    stroke_opacity = 0;
                else {
                    convertRGB(value, stroke_fill2);
                    if (thickness == 0)
                        thickness = 1;
                }
            }
            else if (name == "x")
                x = stoi(value);

            else if (name == "y")
                y = stoi(value);

            else if (name == "width") {
                if (value[value.length() - 1] == 't')
                    value = value.substr(0, value.length() - 2);
                width = stoi(value);
            }

            else if (name == "height") {
                if (value[value.length() - 1] == 't')
                    value = value.substr(0, value.length() - 2);
                height = stoi(value);
            }
            else if (name == "stroke-width") {
                thickness = stoi(value);
            }

            ViewBox vb;
            start.SetPoint(x, y);
            vb.SetViewBox(start, width, height, rgb);
        //    r.fillRectangle(hdc, fill_opacity * 255);
            vb.Draw(hdc, transform);
        }
    }
    else
    if (name == "g") {
        vector<Transform> transform2;
        for (int i = 0; i < transform.size(); ++i)
            transform2.push_back(transform[i]);

        for (rapidxml::xml_attribute<>* attr = root->first_attribute(); attr; attr = attr->next_attribute()) {
            name = attr->name();
            string value = attr->value();
            if (name == "fill-opacity")
                fill_opacity2 = stod(value);

            else if (name == "stroke-opacity")
                stroke_opacity2 = stod(value);

            else if (name == "fill") {
                if (value == "none" || value == "transparent")
                    fill_opacity2 = 0;
                else
                    convertRGB(value, fill2);
            }

            else if (name == "stroke") {
                if (value == "none" || value == "transparent")
                    stroke_opacity2 = 0;
                else {
                    convertRGB(value, stroke_fill2);
                    if (thickness2 == 0)
                        thickness2 = 1;
                }
            }

            else if (name == "stroke-width") {
                thickness2 = stoi(value);
            }

            else if (name == "font-size")
                font_size2 = stoi(value);

            else if (name == "transform") {
                readTransform(value, transform2);
            }
        }

        for (rapidxml::xml_node<>* n = root->first_node(); n; n = n->next_sibling()) {
            ReadSVG(hdc, n, fill2, fill_opacity2, stroke_fill2, stroke_opacity2, thickness2, font_size2, transform2);
        }
    }
    else
    if (name == "rect") {
        int  width, height, x = 10, y = 10;
        Point2D start;

        for (rapidxml::xml_attribute<>* node = root->first_attribute(); node; node = node->next_attribute()) {
            name = node->name();
            string value = node->value();

            if (name == "fill-opacity")
                fill_opacity = stod(value);

            else if (name == "stroke-opacity")
                stroke_opacity = stod(value);

            else if (name == "fill") {
                if (value == "none" || value == "transparent")
                    fill_opacity = 0;
                else
                    convertRGB(value, fill2);
            }

            else if (name == "stroke") {
                if (value == "none" || value == "transparent")
                    stroke_opacity = 0;
                else {
                    convertRGB(value, stroke_fill2);
                    if (thickness == 0)
                        thickness = 1;
                }
            }

            else if (name == "stroke-width") {
                thickness = stoi(value);
            }

            else if (name == "x")
                x = stoi(value);

            else if (name == "y")
                y = stoi(value);

            else if (name == "width") {
                if (value[value.length() - 1] == 't')
                    value = value.substr(0, value.length() - 2);
                width = stoi(value);
            }

            else if (name == "height") {
                if (value[value.length() - 1] == 't')
                    value = value.substr(0, value.length() - 2);
                height = stoi(value);
            }
            else if (name == "transform") {
                readTransform(value, transform);
            }
        }

        friendRect r;
        start.SetPoint(x, y);
        r.SetRect(stroke_fill2, start, height, width, thickness, fill2, fill_opacity, stroke_opacity);
        r.fillRectangle(hdc, fill_opacity * 255);
        r.Draw(hdc, transform);
    }

    else if (name == "circle" || name == "ellipse") {
        int  x = 0, y = 0, rx, ry = 0;
        Point2D start;

        for (rapidxml::xml_attribute<>* node = root->first_attribute(); node; node = node->next_attribute()) {
            name = node->name();
            string value = node->value();

            if (name == "fill-opacity")
                fill_opacity = stod(value);

            else if (name == "stroke-opacity")
                stroke_opacity = stod(value);

            else if (name == "fill") {
                if (value == "none" || value == "transparent")
                    fill_opacity = 0;
                else
                    convertRGB(value, fill2);
            }

            else if (name == "stroke") {
                if (value == "none" || value == "transparent")
                    stroke_opacity = 0;
                else {
                    convertRGB(value, stroke_fill2);
                    if (thickness == 0)
                        thickness = 1;
                }
            }

            else if (name == "stroke-width") {
                thickness = stoi(value);
            }

            else if (name == "cx")
                x = stoi(value);

            else if (name == "cy")
                y = stoi(value);

            else if (name == "r" || name == "rx")
                rx = ry = stoi(value);

            else if (name == "ry")
                ry = stoi(value);
            else if (name == "transform") {
                readTransform(value, transform);
            }
        }

        Ellipses e;
        start.SetPoint(x, y);
        e.SetEllipse(stroke_fill2, start, rx, ry, thickness, fill2, stroke_opacity, fill_opacity);
        e.Draw(hdc, transform);
    }

    else if (name == "text") {
        int x = 0, y = 0;
        string text = root->value();

        for (rapidxml::xml_attribute<>* node = root->first_attribute(); node; node = node->next_attribute()) {
            name = node->name();
            string value = node->value();

            if (name == "fill-opacity")
                fill_opacity = stod(value);

            else if (name == "stroke-opacity")
                stroke_opacity = stod(value);

            else if (name == "fill") {
                if (value == "none" || value == "transparent")
                    fill_opacity = 0;
                else
                    convertRGB(value, fill2);
            }

            else if (name == "x")
                x = stoi(value);

            else if (name == "y")
                y = stoi(value);

            else if (name == "font-size")
                font_size = stoi(value);
            else if (name == "transform") {
                readTransform(value, transform);
            }
        }

        Text t;
        Point2D start(x, y);
        t.SetText(text, fill2, font_size, start);
        t.Draw(hdc, transform);
    }

    else if (name == "polyline") {
    int NumberPoint = 0;
        vector<Point2D> pointArray;
        Point2D* pointArrays = NULL;
        for (rapidxml::xml_attribute<>* node = root->first_attribute(); node; node = node->next_attribute()) {
            name = node->name();
            string value = node->value();

            if (name == "fill-opacity")
                fill_opacity = stod(value);

            else if (name == "stroke-opacity")
                stroke_opacity = stod(value);

            else if (name == "fill") {
                if (value == "none" || value == "transparent")
                    fill_opacity = 0;
                else
                    convertRGB(value, fill2);
            }

            else if (name == "stroke") {
                if (value == "none" || value == "transparent")
                    stroke_opacity = 0;
                else {
                    convertRGB(value, stroke_fill2);
                    if (thickness == 0)
                        thickness = 1;
                }
            }

            else if (name == "points") {
                string points;
                int pos1 = 0;
                int pos2 = value.size();
                points = value.substr(pos1, pos2 - pos1);

                stringstream ss(points);
                string point;

                while (getline(ss, point, ' ')) {
                    stringstream pointSS(point);
                    string coordinate;
                    vector<int> coordinates;

                    while (getline(pointSS, coordinate, ','))
                        coordinates.push_back(stoi(coordinate));

                    Point2D pointObj(coordinates[0], coordinates[1]);
                    pointArray.push_back(pointObj);
                }
                NumberPoint = pointArray.size();
                pointArrays = new Point2D[NumberPoint];
                for (int i = 0; i < NumberPoint; i++)
                {
                    pointArrays[i] = pointArray[i];
                }
            }
            else if (name == "transform") {
                readTransform(value, transform);
            }
        }

        PolyLine polyLine;
        polyLine.SetPolyLine(stroke_fill2, thickness, NumberPoint, pointArrays, fill2, fill_opacity, stroke_opacity);
        polyLine .Draw(hdc, transform);
    }

    else if (name == "polygon") {
        vector<Point2D> pointArray;
        Point2D* pointArrays = NULL;
        int NumberPoint = 0;
        for (rapidxml::xml_attribute<>* node = root->first_attribute(); node; node = node->next_attribute()) {
            name = node->name();
            string value = node->value();

            if (name == "fill-opacity")
                fill_opacity = stod(value);

            else if (name == "stroke-opacity")
                stroke_opacity = stod(value);

            else if (name == "fill") {
                if (value == "none" || value == "transparent")
                    fill_opacity = 0;
                else
                    convertRGB(value, fill2);
            }

            else if (name == "stroke") {
                if (value == "none" || value == "transparent")
                    stroke_opacity = 0;
                else {
                    convertRGB(value, stroke_fill2);
                    if (thickness == 0)
                        thickness = 1;
                }
            }

            else if (name == "stroke-width")
                thickness = stoi(value);

            else if (name == "points") {
                string points;
                int pos1 = 0;
                int pos2 = value.size();
                points = value.substr(pos1, pos2 - pos1);

                stringstream ss(points);
                string point;

                while (getline(ss, point, ' ')) {
                    stringstream pointSS(point);
                    string coordinate;
                    vector<int> coordinates;

                    while (getline(pointSS, coordinate, ','))
                        coordinates.push_back(stoi(coordinate));

                    Point2D pointObj(coordinates[0], coordinates[1]);
                    pointArray.push_back(pointObj);
                }
                NumberPoint = pointArray.size();
                pointArrays = new Point2D[NumberPoint];
                for (int i = 0; i < NumberPoint; i++)
                {
                    pointArrays[i] = pointArray[i];
                }
            }
            else if (name == "transform") {
                readTransform(value, transform);
            }
        }

        PolyGon polyGon;
        polyGon.SetPolyGon(stroke_fill2, fill2, thickness, NumberPoint, pointArrays, fill_opacity, stroke_opacity);
        polyGon.Draw(hdc, transform);
    }

    else if (name == "line") {
        int  x1, y1, x2, y2;
        Point2D start;

        for (rapidxml::xml_attribute<>* node = root->first_attribute(); node; node = node->next_attribute()) {
            name = node->name();
            string value = node->value();

            if (name == "stroke-opacity")
                stroke_opacity = stod(value);

            else if (name == "stroke") {
                if (value == "none" || value == "transparent")
                    stroke_opacity = 0;
                else {
                    convertRGB(value, stroke_fill2);
                    if (thickness == 0)
                        thickness = 1;
                }
            }

            else if (name == "stroke-width") {
                thickness = stoi(value);
            }

            else if (name == "x1")
                x1 = stoi(value);

            else if (name == "y1")
                y1 = stoi(value);

            else if (name == "x2")
                x2 = stoi(value);

            else if (name == "y2")
                y2 = stoi(value);
            else if (name == "transform") {
                readTransform(value, transform);
            }
        }

        Line line;
        Point2D p1(x1, y1), p2(x2, y2);
        line.SetLine(stroke_fill2, p1, p2, thickness, stroke_opacity);
        line.Draw(hdc, transform);
    }

    else if (name == "path") {
        Point2D start;

        vector<vector<vector<Point2D>>> MultiPath;
        vector<vector<Point2D>> points;

        vector<vector<char>> Commands;
        vector<char> command;

        Point2D InitialP(0, 0);
        Point2D startP(0, 0);

        for (rapidxml::xml_attribute<>* node = root->first_attribute(); node; node = node->next_attribute()) {
            name = node->name();
            string value = node->value();

            if (name == "stroke-opacity")
            {
                stroke_opacity = stod(value);
            }

            else if (name == "fill-opacity")
            {
                fill_opacity = stod(value);
            }

            else if (name == "fill") {
                if (value == "none" || value == "transparent") {
                    fill_opacity = 0;
                }
                else
                {
                    convertRGB(value, fill2);
                }
            }

            else if (name == "stroke") {
                if (value == "none" || value == "transparent")
                {
                    stroke_opacity = 0;
                }
                else {
                    convertRGB(value, stroke_fill2);
                    if (thickness == 0)
                        thickness = 1;
                }
            }

            else if (name == "stroke-width") {
                thickness = stoi(value);
            }
            else if (name == "transform") {
                readTransform(value, transform);
            }

            else if (name == "d") {
                for (int i = 0; i < value.size(); ++i) {
                    if (value[i] == '\n' || value[i] == ',' || value[i] == 9) {
                        value[i] = ' ';
                    }
                }
                cout << value << endl;
                for (int i = 0; i < value.size(); ++i) {
                    if ((value[i] >= 'A' && value[i] <= 'Z') || (value[i] >= 'a' && value[i] <= 'z')) {
                        if (i > 1 && value[i - 1] != ' ')
                            value.insert(i, " ");
                    }

                    else if (value[i] == '-') {
                        if (value[i - 1] >= '0' && value[i - 1] <= '9')
                            value.insert(i, " ");
                    }

                    else if ((i < value.size() - 1) && (value[i] == ' ' && value[i + 1] == ' ')) {
                        value.erase(value.begin() + i);
                        --i;
                    }
                }
                value += ' ';

                double x = 0, y = 0;

                for (int i = 0; i < value.size(); ++i) {

                    if (value[i] == 'm') {
                        int ind1, ind2;

                        ind1 = i + 1;
                        ind2 = value.find(" ", ind1);
                        x = startP.GetX() + stod(value.substr(ind1, ind2 - ind1));


                        ind1 = ind2 + 1;
                        ind2 = value.find(" ", ind1);
                        y = startP.GetY() + stod(value.substr(ind1, ind2 - ind1));

                        startP.SetPoint(x, y);

                        i = ind2;

                        InitialP = startP;

                        if (value[i + 1] >= '0' && value[i + 1] <= '9') {
                            command.push_back('m');

                            vector<Point2D> Parray;
                            Parray.push_back(startP);

                            ind1 = ind2 + 1;
                            ind2 = value.find(" ", ind1);
                            x = startP.GetX() + stod(value.substr(ind1, ind2 - ind1));

                            ind1 = ind2 + 1;
                            ind2 = value.find(" ", ind1);
                            y = startP.GetY() + stod(value.substr(ind1, ind2 - ind1));

                            startP.SetPoint(x, y);
                            Parray.push_back(startP);

                            points.push_back(Parray);
                            i = ind2;
                        }
                    }

                    else if (value[i] == 'M') {
                        int ind1, ind2;

                        ind1 = i + 1;
                        ind2 = value.find(" ", ind1);
                        x = stod(value.substr(ind1, ind2 - ind1));

                        ind1 = ind2 + 1;
                        ind2 = value.find(" ", ind1);
                        y = stod(value.substr(ind1, ind2 - ind1));
                        startP.SetPoint(x, y);

                        i = ind2;

                        InitialP = startP;

                        if (value[i + 1] >= '0' && value[i + 1] <= '9') {
                            command.push_back('m');

                            vector<Point2D> Parray;
                            Parray.push_back(startP);

                            ind1 = ind2 + 1;
                            ind2 = value.find(" ", ind1);
                            x = stod(value.substr(ind1, ind2 - ind1));

                            ind1 = ind2 + 1;
                            ind2 = value.find(" ", ind1);
                            y = stod(value.substr(ind1, ind2 - ind1));

                            startP.SetPoint(x, y);
                            Parray.push_back(startP);

                            points.push_back(Parray);
                            i = ind2;
                        }
                    }

                    else if (value[i] == 'c') {
                        command.push_back('c');

                        int ind1 = i + 1, ind2;
                        Point2D p;
                        vector<Point2D> Parray;
                        Parray.push_back(startP);

                        for (int j = ind1; j < value.size(); ++j) {
                            if (value[j] < 65 || (value[j] > 90 && value[j] < 97) || value[j] > 122) {
                                if (value[j] == ' ') {
                                    ind2 = j;

                                    x = startP.GetX() + stod(value.substr(ind1, ind2 - ind1));
                                    ind1 = ind2 + 1;
                                    ind2 = value.find(" ", ind1);

                                    y = startP.GetY() + stod(value.substr(ind1, ind2 - ind1));
                                    ind1 = ind2 + 1;
                                    while (value[ind1] == ' ') {
                                        ++ind1;
                                        ++ind2;
                                    }
                                    j = ind2;

                                    p.SetPoint(x, y);
                                    Parray.push_back(p);
                                }
                            }
                            else
                            {
                                break;
                            }
                        }

                        points.push_back(Parray);
                        startP = p;

                        i = ind2;
                    }

                    else if (value[i] == 'C') {
                        command.push_back('C');

                        int ind1 = i + 1, ind2;
                        Point2D p;
                        vector<Point2D> Parray;
                        Parray.push_back(startP);

                        for (int j = ind1; j < value.size(); ++j) {
                            if (value[j] < 65 || (value[j] > 90 && value[j] < 97) || value[j] > 122) {
                                if (value[j] == ' ') {
                                    ind2 = j;

                                    x = stod(value.substr(ind1, ind2 - ind1));
                                    ind1 = ind2 + 1;
                                    ind2 = value.find(" ", ind1);

                                    y = stod(value.substr(ind1, ind2 - ind1));
                                    ind1 = ind2 + 1;
                                    while (value[ind1] == ' ') {
                                        ++ind1;
                                        ++ind2;
                                    }
                                    j = ind2;

                                    p.SetPoint(x, y);
                                    Parray.push_back(p);
                                }
                            }
                            else
                            {
                                break;
                            }
                        }

                        points.push_back(Parray);
                        startP = p;
                        i = ind2;
                    }
                    else if (value[i] == 'S') {
                    command.push_back('S');

                    int ind1 = i + 1, ind2;
                    Point2D p;
                    vector<Point2D> Parray;
                    Parray.push_back(startP);

                    for (int j = ind1; j < value.size(); ++j) {
                        if (value[j] < 65 || (value[j] > 90 && value[j] < 97) || value[j] > 122) {
                            if (value[j] == ' ') {
                                ind2 = j;

                                x = stod(value.substr(ind1, ind2 - ind1));
                                ind1 = ind2 + 1;
                                ind2 = value.find(" ", ind1);

                                y = stod(value.substr(ind1, ind2 - ind1));
                                ind1 = ind2 + 1;
                                while (value[ind1] == ' ') {
                                    ++ind1;
                                    ++ind2;
                                }
                                j = ind2;

                                p.SetPoint(x, y);
                                Parray.push_back(p);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }

                    points.push_back(Parray);
                    startP = p;
                    i = ind2;
                    }

                    else if (value[i] == 's') {
                    command.push_back('s');

                    int ind1 = i + 1, ind2;
                    Point2D p;
                    vector<Point2D> Parray;
                    Parray.push_back(startP);

                    for (int j = ind1; j < value.size(); ++j) {
                        if (value[j] < 65 || (value[j] > 90 && value[j] < 97) || value[j] > 122) {
                            if (value[j] == ' ') {
                                ind2 = j;

                                x = startP.GetX() + stod(value.substr(ind1, ind2 - ind1));
                                ind1 = ind2 + 1;
                                ind2 = value.find(" ", ind1);

                                y = startP.GetY() + stod(value.substr(ind1, ind2 - ind1));
                                ind1 = ind2 + 1;
                                while (value[ind1] == ' ') {
                                    ++ind1;
                                    ++ind2;
                                }
                                j = ind2;

                                p.SetPoint(x, y);
                                Parray.push_back(p);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }

                    points.push_back(Parray);
                    startP = p;

                    i = ind2;
                    }

                    else if (value[i] == 'h') {
                        command.push_back('h');

                        int ind1, ind2;
                        vector<Point2D> Parray;
                        Parray.push_back(startP);

                        ind1 = i + 1;
                        ind2 = value.find(" ", ind1);

                        x = startP.GetX() + stod(value.substr(ind1, ind2 - ind1));
                        startP.SetPoint(x, startP.GetY());
                        Parray.push_back(startP);
                        points.push_back(Parray);

                        i = ind2;
                    }

                    else if (value[i] == 'H') {
                        command.push_back('H');

                        int ind1, ind2;
                        vector<Point2D> Parray;
                        Parray.push_back(startP);

                        ind1 = i + 1;
                        ind2 = value.find(" ", ind1);

                        x = stod(value.substr(ind1, ind2 - ind1));
                        startP.SetPoint(x, startP.GetY());
                        Parray.push_back(startP);
                        points.push_back(Parray);

                        i = ind2;
                    }

                    else if (value[i] == 'v') {
                        command.push_back('v');

                        int ind1, ind2;
                        vector<Point2D> Parray;
                        Parray.push_back(startP);

                        ind1 = i + 1;
                        ind2 = value.find(" ", ind1);

                        y = startP.GetY() + stod(value.substr(ind1, ind2 - ind1));
                        startP.SetPoint(startP.GetX(), y);
                        Parray.push_back(startP);
                        points.push_back(Parray);

                        i = ind2;
                    }

                    else if (value[i] == 'V') {
                        command.push_back('V');

                        int ind1, ind2;
                        vector<Point2D> Parray;
                        Parray.push_back(startP);

                        ind1 = i + 1;
                        ind2 = value.find(" ", ind1);

                        y = stod(value.substr(ind1, ind2 - ind1));
                        startP.SetPoint(startP.GetX(), y);
                        Parray.push_back(startP);
                        points.push_back(Parray);

                        i = ind2;
                    }


                    else if (value[i] == 'l') {
                        command.push_back('l');

                        int ind1, ind2;
                        vector<Point2D> Parray;
                        Parray.push_back(startP);

                        ind1 = i + 1;
                        ind2 = value.find(" ", ind1);
                        x = startP.GetX() + stod(value.substr(ind1, ind2 - ind1));

                        ind1 = ind2 + 1;
                        ind2 = value.find(" ", ind1);
                        y = startP.GetY() + stod(value.substr(ind1, ind2 - ind1));

                        startP.SetPoint(x, y);
                        Parray.push_back(startP);
                        points.push_back(Parray);

                        i = ind2;
                    }

                    else if (value[i] == 'L') {
                        command.push_back('L');

                        int ind1, ind2;
                        vector<Point2D> Parray;
                        Parray.push_back(startP);

                        ind1 = i + 1;
                        ind2 = value.find(" ", ind1);
                        x = stod(value.substr(ind1, ind2 - ind1));

                        ind1 = ind2 + 1;
                        ind2 = value.find(" ", ind1);
                        y = stod(value.substr(ind1, ind2 - ind1));

                        startP.SetPoint(x, y);
                        Parray.push_back(startP);
                        points.push_back(Parray);

                        i = ind2;
                    }
                   

                    else if (value[i] == 'Z' || value[i] == 'z') {
                        command.push_back('z');

                        vector<Point2D> Parray;
                        Parray.push_back(startP);
                        Parray.push_back(InitialP);
                        points.push_back(Parray);

                        startP = InitialP;
                        MultiPath.push_back(points);
                        Commands.push_back(command);
                        command.clear();
                        points.clear();
                    }
                }  
            }
             else if (name == "transform") {
             readTransform(value, transform);
            }
            }

        Path p;
        for (int i = 0; i < MultiPath.size(); ++i) {
            p.SetPath(stroke_fill2, fill2, thickness, Commands[i], MultiPath[i], fill_opacity, stroke_opacity);
            p.Draw(hdc, transform);
        }
    }
}


VOID Shape::Draw(HDC hdc, vector<Transform>& transform) {}
void Shape::fillRectangle(HDC hdc, int opacity) {}

Shape::~Shape() {}




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
        shape.ReadXML(hdc, "TikTok_logo.svg");
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
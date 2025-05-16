// #include <Windows.h>
// #include <math.h>
// #include <vector>
// #include <algorithm>
// #include <climits>
// #include <iostream>

// using namespace std;

// int roundLocal(double x) {
//     return (int)(x + 0.5);
// }

// struct Point {
//     int x;
//     int y;

//     Point() {}

//     Point(int x, int y) : x(x), y(y) {}
// };

// // Bresenham’s Efficient DDA (for all 8 zones)
// void BresenhamsEfficientDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
//     int dx = abs(x2 - x1);
//     int dy = abs(y2 - y1);
//     int x = x1;
//     int y = y1;

//     double m = (double)dy / dx;

//     int d1 = dx - 2 * dy;
//     int d1c1 = -2 * dy;
//     int d1c2 = 2 * (dx - dy);

//     int d2 = 2 * dx - dy;
//     int d2c1 = 2 * dx;
//     int d2c2 = 2 * (dx - dy);

//     SetPixel(hdc, x, y, c);

//     if (m <= 1) {
//         if (x2 >= x1 && y2 >= y1) {
//             while (x < x2) {
//                 if (d1 > 0) {
//                     x++;
//                     d1 += d1c1;
//                 } else {
//                     x++;
//                     y++;
//                     d1 += d1c2;
//                 }
//                 SetPixel(hdc, x, y, c);
//             }
//         } else if (x2 < x1 && y2 > y1) {
//             while (x > x2) {
//                 if (d1 > 0) {
//                     x--;
//                     d1 += d1c1;
//                 } else {
//                     x--;
//                     y++;
//                     d1 += d1c2;
//                 }
//                 SetPixel(hdc, x, y, c);
//             }
//         } else if (x2 > x1 && y2 < y1) {
//             while (x < x2) {
//                 if (d1 > 0) {
//                     x++;
//                     d1 += d1c1;
//                 } else {
//                     x++;
//                     y--;
//                     d1 += d1c2;
//                 }
//                 SetPixel(hdc, x, y, c);
//             }
//         } else if (x2 < x1 && y2 < y1) {
//             while (x > x2) {
//                 if (d1 > 0) {
//                     x--;
//                     d1 += d1c1;
//                 } else {
//                     x--;
//                     y--;
//                     d1 += d1c2;
//                 }
//                 SetPixel(hdc, x, y, c);
//             }
//         }
//     } else {
//         if (x2 >= x1 && y2 >= y1) {
//             while (y < y2) {
//                 if (d2 < 0) {
//                     y++;
//                     d2 += d2c1;
//                 } else {
//                     x++;
//                     y++;
//                     d2 += d2c2;
//                 }
//                 SetPixel(hdc, x, y, c);
//             }
//         } else if (x2 < x1 && y2 > y1) {
//             while (y < y2) {
//                 if (d2 < 0) {
//                     y++;
//                     d2 += d2c1;
//                 } else {
//                     x--;
//                     y++;
//                     d2 += d2c2;
//                 }
//                 SetPixel(hdc, x, y, c);
//             }
//         } else if (x2 > x1 && y2 < y1) {
//             while (y > y2) {
//                 if (d2 < 0) {
//                     y--;
//                     d2 += d2c1;
//                 } else {
//                     x++;
//                     y--;
//                     d2 += d2c2;
//                 }
//                 SetPixel(hdc, x, y, c);
//             }
//         } else if (x2 <= x1 && y2 <= y1) {
//             while (y > y2) {
//                 if (d2 < 0) {
//                     y--;
//                     d2 += d2c1;
//                 } else {
//                     x--;
//                     y--;
//                     d2 += d2c2;
//                 }
//                 SetPixel(hdc, x, y, c);
//             }
//         }
//     }
// }

// Point vIntersect(Point p1, Point p2, int x_edge) {
//     Point result;
//     result.x = x_edge;
//     result.y = p1.y + (x_edge - p1.x) * ((double)(p2.y - p1.y) / (p2.x - p1.x));
//     return result;
// }

// Point hIntersect(Point p1, Point p2, int y_edge) {
//     Point result;
//     result.y = y_edge;
//     result.x = p1.x + (y_edge - p1.y) * ((double)(p2.x - p1.x) / (p2.y - p1.y));
//     return result;
// }

// vector<Point> polygonClip(vector<Point> p, int xl, int xr, int yb, int yt) {
//     int n = p.size();
//     Point p1 = p[n - 1];
//     vector<Point> output1, output2, output3, output4;
//     bool in1 = p1.x >= xl;

//     for (int i = 0; i < n; i++) {
//         Point p2 = p[i];
//         bool in2 = p2.x >= xl;

//         if (in1 && in2) {
//             output1.push_back(p2);
//         } else if (in2) {
//             output1.push_back(vIntersect(p1, p2, xl));
//             output1.push_back(p2);
//         } else if (in1) {
//             output1.push_back(vIntersect(p1, p2, xl));
//         }

//         p1 = p2;
//         in1 = in2;
//     }

//     p1 = output1.back();
//     in1 = p1.x <= xr;
//     for (int i = 0; i < output1.size(); i++) {
//         Point p2 = output1[i];
//         bool in2 = p2.x <= xr;

//         if (in1 && in2) {
//             output2.push_back(p2);
//         } else if (in2) {
//             output2.push_back(vIntersect(p1, p2, xr));
//             output2.push_back(p2);
//         } else if (in1) {
//             output2.push_back(vIntersect(p1, p2, xr));
//         }

//         p1 = p2;
//         in1 = in2;
//     }

//     p1 = output2.back();
//     in1 = p1.y >= yb;
//     for (int i = 0; i < output2.size(); i++) {
//         Point p2 = output2[i];
//         bool in2 = p2.y >= yb;

//         if (in1 && in2) {
//             output3.push_back(p2);
//         } else if (in2) {
//             output3.push_back(hIntersect(p1, p2, yb));
//             output3.push_back(p2);
//         } else if (in1) {
//             output3.push_back(hIntersect(p1, p2, yb));
//         }

//         p1 = p2;
//         in1 = in2;
//     }

//     p1 = output3.back();
//     in1 = p1.y <= yt;
//     for (int i = 0; i < output3.size(); i++) {
//         Point p2 = output3[i];
//         bool in2 = p2.y <= yt;

//         if (in1 && in2) {
//             output4.push_back(p2);
//         } else if (in2) {
//             output4.push_back(hIntersect(p1, p2, yt));
//             output4.push_back(p2);
//         } else if (in1) {
//             output4.push_back(hIntersect(p1, p2, yt));
//         }

//         p1 = p2;
//         in1 = in2;
//     }

//     return output4;
// }

// // Windows callback
// LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
//     HDC hdc;
//     static vector<Point> polygon;
//     static vector<Point> clippedPolygon;
//     static int xl, xr, yb, yt;

//     switch (msg) {
//         case WM_LBUTTONDOWN:
//             hdc = GetDC(hwnd);

//             // Original polygon
//             polygon = {
//                 {110, 110},
//                 {200, 200},
//                 {200, 230},
//                 {170, 200},
//                 {130, 170}
//             };

//             // Clipping rectangle bounds
//             xl = 150, xr = 200, yb = 150, yt = 200;

//             // Draw original polygon in red
//             for (size_t i = 0; i < polygon.size(); i++) {
//                 Point a = polygon[i];
//                 Point b = polygon[(i + 1) % polygon.size()];
//                 BresenhamsEfficientDDA(hdc, a.x, a.y, b.x, b.y, RGB(255, 0, 0));
//             }

//             // Draw clipping rectangle in green
//             BresenhamsEfficientDDA(hdc, xl, yb, xl, yt, RGB(0, 255, 0));
//             BresenhamsEfficientDDA(hdc, xl, yt, xr, yt, RGB(0, 255, 0));
//             BresenhamsEfficientDDA(hdc, xr, yt, xr, yb, RGB(0, 255, 0));
//             BresenhamsEfficientDDA(hdc, xr, yb, xl, yb, RGB(0, 255, 0));

//             // Clip and draw the polygon
//             clippedPolygon = polygonClip(polygon, xl, xr, yb, yt);
//             for (size_t i = 0; i < clippedPolygon.size(); i++) {
//                 Point a = clippedPolygon[i];
//                 Point b = clippedPolygon[(i + 1) % clippedPolygon.size()];
//                 BresenhamsEfficientDDA(hdc, a.x, a.y, b.x, b.y, RGB(0, 0, 255));
//             }

//             ReleaseDC(hwnd, hdc);
//             break;

//         case WM_DESTROY:
//             PostQuitMessage(0);
//             break;
//     }

//     return DefWindowProc(hwnd, msg, wp, lp);
// }

// // WinMain
// int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR cmd, int nsh) {
//     WNDCLASS wc;
//     wc.cbClsExtra = 0;
//     wc.cbWndExtra = 0;
//     wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
//     wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Fixed incorrect cursor
//     wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
//     wc.lpszClassName = TEXT("MyClass"); // Use TEXT() for compatibility
//     wc.lpszMenuName = NULL;
//     wc.lpfnWndProc = WndProc;
//     wc.style = CS_HREDRAW | CS_VREDRAW;
//     wc.hInstance = hi;
    
//     RegisterClass(&wc);

//     HWND hwnd = CreateWindow(TEXT("MyClass"), TEXT("Hello World!"), WS_OVERLAPPEDWINDOW, 
//                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
//                              NULL, NULL, hi, NULL); // Use TEXT()

//     ShowWindow(hwnd, nsh);
//     UpdateWindow(hwnd);

//     MSG msg;
//     while (GetMessage(&msg, NULL, 0, 0) > 0) {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }
// }

#include <Windows.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <climits>
#include <List>
#include <iostream>
// #include "1_bresenhamDDALine.cpp"

using namespace std;

int roundLocal(double x) {
    return (int)(x + 0.5);
}

struct Point {
    int x;
    int y;

    Point() {};
    
    Point(int x, int y): x(x), y(y) {}
};


void BresenhamsEfficientDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int x = x1;
	int y = y1;

    double m = (double)dy / dx;

	//int d1 = (dx * (2 * y + 1 - 2 * y1)) - (2 * dy * (x + 1 - x1));
    int d1 = dx - 2*dy;
	int d1c1 = -2 * dy;
	int d1c2 = 2 * (dx - dy);

    int d2 = 2*dx - dy;
    int d2c1 = 2*dx;
    int d2c2 = 2 * (dx - dy);

	SetPixel(hdc, x, y, c);

	if(m <= 1) {
        if (x2 >= x1 && y2 >= y1) {
            while(x < x2) {
                if(d1 > 0) {
                    x++;
                    d1 += d1c1;
                } else {
                    x++;
                    y++;
                    d1 += d1c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if(x2 < x1 && y2 > y1) {
            while(x > x2) {
                if(d1 > 0) {
                    x--;
                    d1 += d1c1;
                } else {
                    x--;
                    y++;
                    d1 += d1c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if (x2 > x1 && y2 < y1) {
            while(x < x2) {
                if(d1 > 0) {
                    x++;
                    d1 += d1c1;
                } else {
                    x++;
                    y--;
                    d1 += d1c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if (x2 < x1 && y2 < y1) {
            while(x > x2) {
                if(d1 > 0) {
                    x--;
                    d1 += d1c1;
                } else {
                    x--;
                    y--;
                    d1 += d1c2;
                }
                SetPixel(hdc, x, y, c);
            }
        }

    } else {
        if (x2 >= x1 && y2 >= y1) {
            while(y < y2) {
                if(d2 < 0) {
                    y++;
                    d2 += d2c1;
                } else {
                    x++;
                    y++;
                    d2 += d2c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if(x2 < x1 && y2 > y1) {
            while(y < y2) {
                if(d2 < 0) {
                    y++;
                    d2 += d2c1;
                } else {
                    x--;
                    y++;
                    d2 += d2c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if (x2 > x1 && y2 < y1) {
            while(y > y2) {
                if(d2 < 0) {
                    y--;
                    d2 += d2c1;
                } else {
                    x++;
                    y--;
                    d2 += d2c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if (x2 <= x1 && y2 <= y1) {
            while(y > y2) {
                if(d2 < 0) {
                    y--;
                    d2 += d2c1;
                } else {
                    x--;
                    y--;
                    d2 += d2c2;
                }
                SetPixel(hdc, x, y, c);
            }
        }
    }
}

Point vIntersect(Point p1, Point p2, int x_edge) {
    Point result;

    result.x = x_edge;
    result.y = p1.y + (x_edge - p1.x) * ((double)(p2.y-p1.y)/(p2.x-p1.x));

    return result;

}

Point hIntersect(Point p1, Point p2, int y_edge) {
    Point result;

    result.y = y_edge;
    result.x = p1.x + (y_edge - p1.y) * ((double)(p2.x-p1.x)/(p2.y-p1.y));

    return result;

}


vector<Point> polygonClip(vector<Point> p, int xl, int xr, int yb, int yt) {

    int n = p.size();
    
    Point p1 = p[n-1];
    vector<Point> output1, output2, output3, output4;
    
    bool in1 = p1.x >= xl;
    
    for (int i = 0; i < n; i++) {
        
        Point p2 = p[i];
        // cout << "in test: " << p2.x << " " << p2.y << endl;
        bool in2 = p2.x > xl;
        
        if(in1 && in2) {
            output1.push_back(p2);
            
        } else if(in2) {
            Point newp = vIntersect(p1, p2, xl);
            output1.push_back(newp);
            output1.push_back(p2);
            
        } else if (in1) {
            Point newp = vIntersect(p1, p2, xl);
            output1.push_back(newp);
            
        }
        
        p1 = p2;
        in1 = in2;
        
    }

    in1 = p1.x <= xr;
    
    n = output1.size();
    p1 = output1[n-1];
    for (int i = 0; i < n; i++) {
        
        Point p2 = output1[i];
        // cout << "in test: " << p2.x << " " << p2.y << endl;
        bool in2 = p2.x <= xr;
        
        if(in1 && in2) {
            output2.push_back(p2);
            
        } else if(in2) {
            Point newp = vIntersect(p1, p2, xr);
            output2.push_back(newp);
            output2.push_back(p2);
            
        } else if (in1) {
            Point newp = vIntersect(p1, p2, xr);
            output2.push_back(newp);
            
        }
        
        p1 = p2;
        in1 = in2;
        
    }
    
    in1 = p1.y >= yb;
    
    n = output2.size();
    p1 = output2[n-1];
    for (int i = 0; i < n; i++) {
        
        Point p2 = output2[i];
        // cout << "in test: " << p2.x << " " << p2.y << endl;
        bool in2 = p2.y >= yb;
        
        if(in1 && in2) {
            output3.push_back(p2);
            
        } else if(in2) {
            Point newp = hIntersect(p1, p2, yb);
            output3.push_back(newp);
            output3.push_back(p2);
            
        } else if (in1) {
            Point newp = hIntersect(p1, p2, yb);
            output3.push_back(newp);
            
        }
        
        p1 = p2;
        in1 = in2;
        
    }
    
    in1 = p1.y <= yt;
    
    n = output3.size();
    p1 = output3[n-1];
    for (int i = 0; i < n; i++) {
        
        Point p2 = output3[i];
        // cout << "in test: " << p2.x << " " << p2.y << endl;
        bool in2 = p2.y <= yt;
        
        if(in1 && in2) {
            output4.push_back(p2);
            
        } else if(in2) {
            Point newp = hIntersect(p1, p2, yt);
            output4.push_back(newp);
            output4.push_back(p2);
            
        } else if (in1) {
            Point newp = hIntersect(p1, p2, yt);
            output4.push_back(newp);

        }
        
        p1 = p2;
        in1 = in2;

    }
    
    return output4;
    
    
}


// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
    HDC hdc;
    static Point pp, p1, p2, p3, p4, p5;
    static vector<Point> p;
    static int c = 0, x, y, max_y = 0, min_y = 999999, xl, xr, yb, yt;
    static vector<Point> resultPoly;

    switch (m) {
        case WM_LBUTTONDOWN:
            hdc = GetDC(hwnd);

            cout << "im here" << endl;

            p1 = Point(110, 110);
            p2 = Point(200, 200);
            p3 = Point(200, 230);
            p4 = Point(170, 200);
            p5 = Point(130, 170);

            p.push_back(p1);
            p.push_back(p2);
            p.push_back(p3);
            p.push_back(p4);
            p.push_back(p5);

            xl = 150;
            xr = 200;
            yb = 150;
            yt = 200;

            BresenhamsEfficientDDA(hdc, p1.x, p1.y, p2.x, p2.y, RGB(255, 0, 0));
            BresenhamsEfficientDDA(hdc, p2.x, p2.y, p3.x, p3.y, RGB(255, 0, 0));
            BresenhamsEfficientDDA(hdc, p3.x, p3.y, p4.x, p4.y, RGB(255, 0, 0));
            BresenhamsEfficientDDA(hdc, p4.x, p4.y, p5.x, p5.y, RGB(255, 0, 0));
            BresenhamsEfficientDDA(hdc, p5.x, p5.y, p1.x, p1.y, RGB(255, 0, 0));

            BresenhamsEfficientDDA(hdc, xl, xl, yb, yt, RGB(0, 0, 255));
            BresenhamsEfficientDDA(hdc, xl, xr, yt, yt, RGB(0, 0, 255));
            BresenhamsEfficientDDA(hdc, xl, xl, yt, yb, RGB(0, 0, 255));
            BresenhamsEfficientDDA(hdc, xr, xl, yt, yt, RGB(0, 0, 255));

            resultPoly = polygonClip(p, xl, xr, yb, yt);

            cout << "p1: " << resultPoly[0].x << " " << resultPoly[0].y << endl;
            cout << "p1: " << resultPoly[1].x << " " << resultPoly[1].y << endl;
            cout << "p1: " << resultPoly[2].x << " " << resultPoly[2].y << endl;
            cout << "p1: " << resultPoly[3].x << " " << resultPoly[3].y << endl;
            cout << "p1: " << resultPoly[4].x << " " << resultPoly[4].y << endl;



        

            BresenhamsEfficientDDA(hdc, resultPoly[0].x, resultPoly[0].y, resultPoly[1].x, resultPoly[1].y, RGB(0, 0, 0));
            BresenhamsEfficientDDA(hdc, resultPoly[1].x, resultPoly[1].y, resultPoly[2].x, resultPoly[2].y, RGB(0, 0, 0));
            BresenhamsEfficientDDA(hdc, resultPoly[2].x, resultPoly[2].y, resultPoly[3].x, resultPoly[3].y, RGB(0, 0, 0));
            BresenhamsEfficientDDA(hdc, resultPoly[3].x, resultPoly[3].y, resultPoly[4].x, resultPoly[4].y, RGB(0, 0, 0));
            BresenhamsEfficientDDA(hdc, resultPoly[4].x, resultPoly[4].y, resultPoly[0].x, resultPoly[0].y, RGB(0, 0, 0));


            ReleaseDC(hwnd, hdc);
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, m, wp, lp);
    }

    return 0;
}

// WinMain
int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR cmd, int nsh) {
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Fixed incorrect cursor
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.lpszClassName = TEXT("MyClass"); // Use TEXT() for compatibility
    wc.lpszMenuName = NULL;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hi;
    
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(TEXT("MyClass"), TEXT("Hello World!"), WS_OVERLAPPEDWINDOW, 
                             CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                             NULL, NULL, hi, NULL); // Use TEXT()

    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
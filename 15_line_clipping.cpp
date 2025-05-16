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

union outcode {
    struct {
        unsigned left:1, right:1, bottom:1, top:1;
    };

    unsigned all:4;
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

outcode getOutCode(Point p, int xl, int xr, int yb, int yt) {
    outcode result;

    result.all = 0;

    if(p.x < xl) {
        result.left = 1;

    }
    
    if (p.x > xr) {
        result.right = 1;
        
    }
    
    if (p.y < yb) {
        result.bottom = 1;
        
    }
    
    if (p.y > yt) {
        result.top = 1;
        
    }

    return result;
}

Point vIntersect(Point p1, Point p2, int x_edge) {
    Point result;

    result.x = x_edge;
    result.y = (x_edge - p1.x) * ((double)(p2.y-p1.y)/(p2.x-p1.x));

    return result;

}

Point hIntersect(Point p1, Point p2, int y_edge) {
    Point result;

    result.y = y_edge;
    result.x = (y_edge - p1.y) * ((double)(p2.x-p1.x)/(p2.y-p1.y));

    return result;

}

pair<Point, Point> cohinLineClipping(Point p1, Point p2, int xl, int xr, int yb, int yt) {
    outcode out1 = getOutCode(p1, xl, xr, yb, yt);
    outcode out2 = getOutCode(p2, xl, xr, yb, yt);

    while(true) {

        if(out1.all == 0 && out2.all == 0) {
            cout << p1.x << " " << p2.y << endl;
            cout << p1.x << " " << p2.y << endl;
            return pair<Point, Point>(p1, p2);
            

        }

        if((out1.all & out2.all) != 0) {
            return pair<Point, Point>(Point(-1,-1), Point(-1,-1));
        }

        if(out1.all) {
            if(out1.left) {
                p1 = vIntersect(p1, p2, xl);

            }
            
            if (out1.right) {
                p1 = vIntersect(p1, p2, xr);
                
            }
            
            if (out1.bottom) {
                p1 = hIntersect(p1, p2, yb);
                
            }
            
            if (out1.top) {
                p1 = hIntersect(p1, p2, yt);
                
            }
            
            out1 = getOutCode(p1, xl, xr, yb, yt);
            cout << "out1: " <<  out1.left << " " <<  out1.right << " " <<  out1.bottom << " " <<  out1.top << " " << endl;
            cout << "out2: " <<  out2.left << " " <<  out2.right << " " <<  out2.bottom << " " <<  out2.top << " " << endl;
            
        } 
        
        if(out2.all) {
            if(out2.left) {
                p2 = vIntersect(p1, p2, xl);
                
            } 
            
            if (out2.right) {
                p2 = vIntersect(p1, p2, xr);
                
            }
            
            if (out2.bottom) {
                p2 = hIntersect(p1, p2, yb);
                
            }
            
            if (out2.top) {
                p2 = hIntersect(p1, p2, yt);
                
            }
            
            out2 = getOutCode(p2, xl, xr, yb, yt);
            cout << "out1: " <<  out1.left << " " <<  out1.right << " " <<  out1.bottom << " " <<  out1.top << " " << endl;
            cout << "out2: " <<  out2.left << " " <<  out2.right << " " <<  out2.bottom << " " <<  out2.top << " " << endl;
            // cout << out2.all << endl;
        }
        
    }
    
    return pair<Point, Point>(p1, p2);
}




// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
    HDC hdc;
    static Point p[30], pp, p1, p2;
    static int c = 0, x, y, max_y = 0, min_y = 999999, xl, xr, yb, yt;
    static pair<Point, Point> resultLine;

    switch (m) {
        case WM_LBUTTONDOWN:
            hdc = GetDC(hwnd);

            p1 = Point(1, 1);
            p2 = Point(200, 200);

            xl = 0;
            xr = 150;
            yb = 0;
            yt = 150;

            resultLine = cohinLineClipping(p1, p2, xl, xr, yb, yt);

            cout << resultLine.first.x << " " << resultLine.first.y << endl;
            cout << resultLine.second.x << " " << resultLine.second.y << endl;


            // x = LOWORD(lp);
            // y = HIWORD(lp);
            
            // y > max_y ? max_y = y : max_y = max_y;
            // y < min_y ? min_y = y : min_y = min_y;

            // pp = Point(x, y);

            // p[c] = pp;
            // c++;
            // c = c%7;

            // if(c%7 == 0) {
            //     // lineFill(hdc, p, 7, RGB(255, 255, 0), RGB(0, 0, 255), max_y - min_y);

            // }

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
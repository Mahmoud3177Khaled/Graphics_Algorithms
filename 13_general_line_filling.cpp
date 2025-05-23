#include <Windows.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <climits>
#include <List>
// #include "1_bresenhamDDALine.cpp"

using namespace std;

int roundLocal(double x) {
    return (int)(x + 0.5);
}


struct node {
    double x;
    double m;
    int y_max;
    
    node(double x, double m, int y_max) {
        this->x = x;
        this->m = m;
        this->y_max = y_max;
    }
    
    node() {}
};

typedef struct {
    vector<node> nodes;

} EdgeTable[800];

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

void edge2table(Point p1, Point p2, EdgeTable table) {
    if(p1.y == p2.y) {
        return;
    }

    if(p1.y > p2.y) {
        swap(p1, p2);
    }

    double x = p1.x;
    int y = p1.y;

    double m = (double)(p2.y-p1.y)/(p2.x-p1.x);

    table[p1.y].nodes.push_back(node(x, 1/m, p2.y));

}

void polygon2table(Point p[], int n, EdgeTable table) {
    Point p1 = p[n-1];

    for (int i = 0; i < n; i++) {
        Point p2 = p[i];
        edge2table(p1, p2, table);
        p1 = p[i];
    }
    
}

void table2screen(HDC hdc, EdgeTable table, COLORREF c) {
    vector<node> active;

    int y = 0;

    while(y < 800) {

        // add nodes from table[y] to active if any
        for (node n : table[y].nodes) {
            active.push_back(n);
        }

        // remove edges that end at current y
        active.erase(remove_if(active.begin(), active.end(), [y](const node& n) {
            return n.y_max == y;
        }), active.end());

        // if active is empty or somehow has a single node just go to next scanline 
        if (active.size() < 2) {
            y++;
            continue;
        }

        // sort active edges by x to draw
        sort(active.begin(), active.end(), [](const node& a, const node& b) {
            return a.x < b.x;
        });

        // fill between pairs of intersections 2 nodes at a time
        for (int i = 0; i + 1 < active.size(); i += 2) {
            int x1 = roundLocal(active[i].x);
            int x2 = roundLocal(active[i + 1].x);
            BresenhamsEfficientDDA(hdc, x1, y, x2, y, c);

        }

        // increment x values for next scanline with 1/its slope
        for (auto& n : active) {
            n.x += n.m;
        }

        y++;

    }
}

void lineFill(HDC hdc, Point p[], int n, COLORREF c) {
    EdgeTable table;

    polygon2table(p, n, table);

    table2screen(hdc, table, c);
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
    HDC hdc;
    static Point p[30], pp;
    static int c = 0, x, y;

    switch (m) {
        case WM_LBUTTONDOWN:
            hdc = GetDC(hwnd);

            x = LOWORD(lp);
            y = HIWORD(lp);
            
            pp = Point(x, y);

            p[c] = pp;
            c++;
            c = c%7;

            if(c%7 == 0) {
                lineFill(hdc, p, 7, RGB(0, 0, 0));

            }

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
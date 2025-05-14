#include <Windows.h>
#include <math.h>
#include <algorithm>

using namespace std;

int roundLocal(double x) {
    return (int)(x + 0.5);
}

struct Point {
    double x;
    double y;

    Point() {};
    
    Point(double x, double y): x(x), y(y) {}
    
    Point operator* (double c) const {
        return  {static_cast<double>(c * x), static_cast<double>(c * y)};
    }
    
    Point operator+(const Point& other) const {
        return {x + other.x, y + other.y};
    }

    friend Point operator*(double c, const Point& p) {
        return p * c;
    }
};


void BezierRec2(HDC hdc, Point p1, Point p2, Point p3, Point p4, COLORREF c1, COLORREF c2, COLORREF c3, COLORREF c4) {

    if(abs(p1.x-p4.x) <= 1 && abs(p1.y-p4.y) <= 1) {
        return;
    } 

    Point q1 = Point((p1.x+p2.x)/2, (p1.y+p2.y)/2);
    Point q2 = Point((p2.x+p3.x)/2, (p2.y+p3.y)/2);
    Point q3 = Point((p3.x+p4.x)/2, (p3.y+p4.y)/2);
    
    COLORREF cq1 = RGB((GetRValue(c1)+GetRValue(c2))/2, (GetGValue(c1)+GetGValue(c2))/2, (GetBValue(c1)+GetBValue(c2))/2);
    COLORREF cq2 = RGB((GetRValue(c2)+GetRValue(c3))/2, (GetGValue(c2)+GetGValue(c3))/2, (GetBValue(c2)+GetBValue(c3))/2);
    COLORREF cq3 = RGB((GetRValue(c3)+GetRValue(c4))/2, (GetGValue(c3)+GetGValue(c4))/2, (GetBValue(c3)+GetBValue(c4))/2);
    
    
    Point r1 = Point((q1.x+q2.x)/2, (q1.y+q2.y)/2);
    Point r2 = Point((q2.x+q3.x)/2, (q2.y+q3.y)/2);
    
    COLORREF cr1 = RGB((GetRValue(cq1)+GetRValue(cq2))/2, (GetGValue(cq1)+GetGValue(cq2))/2, (GetBValue(cq1)+GetBValue(cq2))/2);
    COLORREF cr2 = RGB((GetRValue(cq2)+GetRValue(cq3))/2, (GetGValue(cq2)+GetGValue(cq3))/2, (GetBValue(cq2)+GetBValue(cq3))/2);
    

    Point m = Point((r1.x+r2.x)/2, (r1.y+r2.y)/2);
    
    COLORREF cm = RGB((GetRValue(cr1)+GetRValue(cr2))/2, (GetGValue(cr1)+GetGValue(cr2))/2, (GetBValue(cr1)+GetBValue(cr2))/2);

    SetPixel(hdc, m.x, m.y, cm);

    BezierRec2(hdc, p1, q1, r1, m, c1, cq1, cr1, cm);
    BezierRec2(hdc, m, r2, q3, p4, cm, cr2, cq3, c4);

}


// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
    HDC hdc;
    static Point p[30], pp;
    static int x, y, c = 0;

    switch (m) {
        case WM_LBUTTONDOWN:
            hdc = GetDC(hwnd);

            x = LOWORD(lp);
            y = HIWORD(lp);

            pp = Point(x, y);

            p[c] = pp;
            c++;
            c = c % 4;

            if(c % 4 == 0) {
                BezierRec2(hdc, p[0], p[1], p[2], p[3], RGB(0, 255, 0), RGB(255, 255, 0), RGB(255, 0, 0), RGB(0, 0, 255));

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
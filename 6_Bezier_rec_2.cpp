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


void BezierRec2(HDC hdc, Point p1, Point p2, Point p3, Point p4, COLORREF c) {

    if(abs(p1.x-p4.x) <= 1 && abs(p1.y-p4.y) <= 1) {
        return;
    } 

    Point q1 = Point((p1.x+p2.x)/2, (p1.y+p2.y)/2);
    Point q2 = Point((p2.x+p3.x)/2, (p2.y+p3.y)/2);
    Point q3 = Point((p3.x+p4.x)/2, (p3.y+p4.y)/2);
    
    Point r1 = Point((q1.x+q2.x)/2, (q1.y+q2.y)/2);
    Point r2 = Point((q2.x+q3.x)/2, (q2.y+q3.y)/2);

    Point m = Point((r1.x+r2.x)/2, (r1.y+r2.y)/2);

    SetPixel(hdc, m.x, m.y, c);

    BezierRec2(hdc, p1, q1, r1, m, c);
    BezierRec2(hdc, m, r2, q3, p4, c);

}


// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
    HDC hdc;

    switch (m) {
        case WM_LBUTTONDOWN:
            hdc = GetDC(hwnd);

            BezierRec2(hdc, Point(100, 100), Point(150, 150), Point(200, 150), Point(250, 100), RGB(0, 0, 0));

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
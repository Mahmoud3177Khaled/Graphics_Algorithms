#include<Windows.h>
#include<math.h>
#include<algorithm>

using namespace std;

int roundLocal(double x) {
	return (int)(x + 0.5);
}


struct Point {
    double x;
    double y;
    
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

Point arr[3] = {Point(100, 100), Point(150, 150), Point(200, 100)};

Point BezierRec1(HDC hdc, double t, int i, int j, COLORREF c) {
    if(i == j) {
        SetPixel(hdc, round(arr[i].x), round(arr[i].y), c);
        return arr[i];
    }
    
    // SetPixel(hdc, arr[i].x, arr[i].y, c);
    return (1-t)*BezierRec1(hdc, t, i, j-1, c) + t*BezierRec1(hdc, t, i+1, j, c);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
	HDC hdc;
	static int x1, y1;

	static int x2, y2, oldx2, oldy2;

	static bool start = false;

	switch (m) {
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);

        x1 = LOWORD(lp);
        y1 = HIWORD(lp);

        for (double t = 0; t <= 1; t+=0.01) {
            BezierRec1(hdc, t, 0, 2, RGB(0, 0, 0));
        }
        

		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);

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
		break;
	}

	return 0;
}

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
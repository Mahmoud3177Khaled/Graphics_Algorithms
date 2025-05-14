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

    Point() {}

    friend Point operator*(double c, const Point& p) {
        return p * c;
    }
};

// Point arr[3] = {Point(100, 100), Point(150, 150), Point(200, 100)};

Point BezierRec1(Point* arr, double t, int i, int j) {
    if (i == j) return arr[i];
    return (1 - t) * BezierRec1(arr, t, i, j - 1) + t * BezierRec1(arr, t, i + 1, j);
}

int RBezierRec1(int* arrR, double t, int i, int j) {
    if (i == j) return arrR[i];
    return (1 - t) * RBezierRec1(arrR, t, i, j - 1) + t * RBezierRec1(arrR, t, i + 1, j);
}
int GBezierRec1(int* arrG, double t, int i, int j) {
    if (i == j) return arrG[i];
    return (1 - t) * GBezierRec1(arrG, t, i, j - 1) + t * GBezierRec1(arrG, t, i + 1, j);
}
int BBezierRec1(int* arrB, double t, int i, int j) {
    if (i == j) return arrB[i];
    return (1 - t) * BBezierRec1(arrB, t, i, j - 1) + t * BBezierRec1(arrB, t, i + 1, j);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
	HDC hdc;
    static Point p[30], pp;
    static int x, y, c = 0, r, g, b;
    static int arrR[3] = {0, 0, 255}, arrG[3] = {0, 255, 0}, arrB[3] = {255, 0, 0};

	switch (m) {
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);

        x = LOWORD(lp);
        y = HIWORD(lp);

        pp = Point(x, y);

        p[c] = pp;
        c++;
        c = c % 3;

        if(c % 3 == 0) {
            for (double t = 0; t <= 1; t += 0.001) {
                Point pResult = BezierRec1(p, t, 0, 2);
                r = RBezierRec1(arrR, t, 0, 2);
                g = GBezierRec1(arrG, t, 0, 2);
                b = BBezierRec1(arrB, t, 0, 2);

                SetPixel(hdc, round(pResult.x), round(pResult.y), RGB(r, g, b));
            }

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
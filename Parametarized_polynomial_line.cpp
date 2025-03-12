#include<Windows.h>
#include<math.h>
// #include <windef.h>
#include <math.h>
#include <windef.h>

using namespace std;

int roundLocal(double x) {
	return (int)(x + 0.5);
}

int max(double x, double y) {
    if(x >= y) {
        return x;
    } else {
        return y;
    }
}

void ParametarizedPolynomialLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c1, COLORREF c2) {
    int alfa1 = x2 - x1;
    int alfa2 = y2 - y1;
    int beta1 = x1;
    int beta2 = y1;

    int alfaR = GetRValue(c2) - GetRValue(c1);
    int alfaG = GetGValue(c2) - GetGValue(c1);
    int alfaB = GetBValue(c2) - GetBValue(c1);
    int betaR = GetRValue(c1);
    int betaG = GetGValue(c1);
    int betaB = GetBValue(c1);

    double step = 1.0/max(abs(alfa1), abs(alfa2));
    // double stepR = 1.0/abs(alfaR);
    // double stepG = 1.0/abs(alfaG);
    // double stepB = 1.0/abs(alfaB);


    // if(alfa1 == 0 || alfa2 == 0) {
    //     return;
    // }
    
    for (double t = 0; t <= 1; t += step) {
        double x = alfa1 * t + beta1;
        double y = alfa2 * t + beta2;

        double r = alfaR * t + betaR;
        double g = alfaG * t + betaG;
        double b = alfaB * t + betaB;

        // alfaR += stepR;
        // alfaG += stepG;
        // alfaB += stepB;
        
        SetPixel(hdc, round(x), round(y), RGB(round(r), round(g), round(b)));
    }
    
    
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
	HDC hdc;
	static int x1, y1;

	static int x2, y2, oldx2, oldy2;

	static bool start = false;

    static COLORREF c1 = RGB(255, 0, 0);
    static COLORREF c2 = RGB(0, 0, 255);
	switch (m) {
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);

        x1 = LOWORD(lp);
        y1 = HIWORD(lp);

        start = true;

		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);

        start = false;
        oldx2 = 0;
        oldy2 = 0;

        x1 = 0;
        y1 = 0;


		ReleaseDC(hwnd, hdc);
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);

        if(start) {
            oldx2 = x2;
            oldy2 = y2;

            x2 = LOWORD(lp);
            y2 = HIWORD(lp);

            ParametarizedPolynomialLine(hdc, x1, y1, oldx2, oldy2, RGB(192, 192, 192), RGB(192, 192, 192));
            ParametarizedPolynomialLine(hdc, x1, y1, x2, y2, c1, c2);
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
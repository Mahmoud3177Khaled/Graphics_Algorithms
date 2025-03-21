#include<Windows.h>
#include<math.h>

int roundLocal(double x) {
	return (int)(x + 0.5);
}

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {

	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	SetPixel(hdc, xc - x, yc - y, c);

	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc - y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc - x, c);

}

void CircleCartesienNaive(HDC hdc, int xc, int yc, int r, COLORREF c) {
	int x = 0;
    int y = r;

    Draw8Points(hdc, xc, yc, x, y, c);

    while(x < y) {
        x++;
        y = sqrt(r*r - x*x);

        Draw8Points(hdc, xc, yc, x, y, c);
    }

}

void CirclePolarNaive(HDC hdc, int xc, int yc, int r, COLORREF c) {

    int x = r;
    int y = 0;
    double theta = 0;
    double dTheta = 1.0/r;

    Draw8Points(hdc, xc, yc, x, y, c);
    
    while(x > y) {
        theta += dTheta;
        x = r * cos(theta);
        y = r * sin(theta);
        
        Draw8Points(hdc, xc, yc, round(x), round(y), c);
        
    }


}

void CirclePolarEff(HDC hdc, int xc, int yc, int r, COLORREF c) {

    double x = r;
    double y = 0;
    double dTheta = 1.0/r;

    Draw8Points(hdc, xc, yc, x, y, c);

    while (x > y) {
        int xtemp = x;
        x = x * cos(dTheta) - y * sin(dTheta);
        y = xtemp * sin(dTheta) + y * cos(dTheta);

        Draw8Points(hdc, xc, yc, round(x), round(y), c);
    }


}



LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
	HDC hdc;

	switch (m) {
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);

        CircleCartesienNaive(hdc, 100, 100, 50, RGB(0, 0, 0));
        CirclePolarNaive(hdc, 200, 200, 50, RGB(0, 0, 0));
        CirclePolarEff(hdc, 300, 300, 50, RGB(0, 0, 0));

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
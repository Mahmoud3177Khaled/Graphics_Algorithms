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

void HermiteParamCubeCurve(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int points, COLORREF c) {
	double alpha1 =-1*x1 + 3*x2 - 3*x3 + 1*x4;
	double beta1 = 3*x1 - 6*x2 + 3*x3 + 0*x4;
	double omega1 = -3*x1 + 3*x2;
	double delta1 = x1;

	double alpha2 =-1*y1 + 3*y2 - 3*y3 + 1*y4;
	double beta2 = 3*y1 - 6*y2 + 3*y3 + 0*y4;
	double omega2 = -3*y1 + 3*y2;
	double delta2 = y1;
	
	double step = 1.0/points;

	for (double t = 0; t <= 1; t += step)
	{
		double x = alpha1*t*t*t + beta1*t*t + omega1*t + delta1;
		double y = alpha2*t*t*t + beta2*t*t + omega2*t + delta2;

		SetPixel(hdc, round(x), round(y), c);
	}
	



}



LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
	HDC hdc;

	switch (m) {
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);

		HermiteParamCubeCurve(hdc, 100, 100, 150, 150, 200, 100, 300, 150, 500, RGB(0, 0, 0));

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
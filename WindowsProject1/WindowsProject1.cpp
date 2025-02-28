#include<Windows.h>

int round(double x) {
	return (int)(x + 0.5);
}

void BresenhamsEfficientDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int x = x1;
	int y = y1;

	int d = (dx * (2 * y + 1 - 2 * y1)) - (2 * dy * (x + 1 - x1));
	int d1 = -2 * dy;
	int d2 = 2 * (dx - dy);

	SetPixel(hdc, x, y, c);

	if (x2 >= x1 && y2 >= y1) {

		while (x < x2) {

			if (d > 0) {
				d += d1;
				x++;
				SetPixel(hdc, x, y, c);
			}
			else {
				d += d2;
				x++;
				y++;
				SetPixel(hdc, x, y, c);
			}
		}

	} else if ((x2 < x1 && y2 >= y1) || (y2 < y1 && x2 >= x1)) {

		while (x > x2) {

			if (d > 0) {
				d += d1;
				x--;
				SetPixel(hdc, x, y, c);
			}
			else {
				d += d2;
				x--;
				y++;
				SetPixel(hdc, x, y, c);
			}
		}

	} else {
		while (x > x2) {

			if (d > 0) {
				d += d1;
				x--;
				SetPixel(hdc, x, y, c);
			}
			else {
				d += d2;
				x--;
				y--;
				SetPixel(hdc, x, y, c);
			}
		}
	}
}


void DraftbresenhamsDDALine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int x = x1;
	int y = y1;

	SetPixel(hdc, x, y, c);


	while (x < x2) {

		int d = (dx * (2*y+1 - 2*y1)) - (2 * dy * (x+1 - x1));
		if (d > 0) {
			x++;
			SetPixel(hdc, x, y, c);
		} else {
			x++;
			y++;
			SetPixel(hdc, x, y, c);
		}
	}
}

void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	SetPixel(hdc, x1, y1, c);

	if (abs(dx) >= abs(dy)) {
		double m = (double)dy / dx;

		int x = x1;
		double y = y1;

		while (x < x2) {
			x++;
			y += m;
			SetPixel(hdc, x, round(y), c);
		}

	} else {
		double m = (double)dx / dy;

		double x = x1;
		int y = y1;

		while (y < y2) {
			y++;
			x += m;
			SetPixel(hdc, round(x), y, c);
		}
	}

}

LRESULT WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
	HDC hdc;
	switch (m) {
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		//DraftbresenhamsDDALine(hdc, 200, 200, 400, 100, RGB(255, 0, 0));
		BresenhamsEfficientDDA(hdc, 100, 100, 200, 200, RGB(255, 0, 0));
		BresenhamsEfficientDDA(hdc, 100, 100, 200, 50, RGB(255, 0, 0));
		BresenhamsEfficientDDA(hdc, 300, 300, 50, 200, RGB(255, 0, 0));
		BresenhamsEfficientDDA(hdc, 300, 300, 50, 50, RGB(255, 0, 0));
		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);
		//DraftbresenhamsDDALine(hdc, 200, 200, 400, 100, RGB(0, 0, 255));
		BresenhamsEfficientDDA(hdc, 100, 100, 200, 200, RGB(0, 0, 0));
		BresenhamsEfficientDDA(hdc, 100, 100, 200, 50, RGB(0, 0, 0));
		BresenhamsEfficientDDA(hdc, 300, 300, 50, 200, RGB(0, 0, 0));
		BresenhamsEfficientDDA(hdc, 300, 300, 50, 50, RGB(0, 0, 0));
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
	wc.hCursor = LoadCursor(NULL, IDI_WINLOGO);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hi;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(L"MyClass", L"Hello World!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hi, 0);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}	
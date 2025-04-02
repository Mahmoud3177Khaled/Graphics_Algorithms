#include<Windows.h>
#include<math.h>

int roundLocal(double x) {
	return (int)(x + 0.5);
}

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

void CircleBresenham(HDC hdc, int xc, int yc, int r, COLORREF c) {
	int x = 0;
	int y = r;

	int d = 1 - r;
	Draw8Points(hdc, xc, yc, x, y, c);

	while (x<y) {
		if (d < 0) {
			d += 2 * x + 3;
			x++;
		}
		else {
			d += 2 * (x - y) + 5;
			x++;
			y--;
		}
		Draw8Points(hdc, xc, yc, x, y, c);
	}


}

void CircleBresenhamEff(HDC hdc, int xc, int yc, int r, COLORREF c) {
	int x = 0;
	int y = r;

	int d = 1 - r;
	int d1 = 3;
	int d2 = 5 - 2 * r;
	Draw8Points(hdc, xc, yc, x, y, c);

	while (x < y) {
		if (d < 0) {
			d += d1;
			d1 += 2;
			d2 += 2;
			x++;
		}
		else {
			d += d2;
			d1 += 2;
			d2 += 4;
			x++;
			y--;
		}
		Draw8Points(hdc, xc, yc, x, y, c);
	}


}

LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
	HDC hdc;
	static int xc1, yc1, x2, y2;

	static int xc2, yc2, x, y, r, oldr, oldx2, oldy2;

	static bool start = false;

	switch (m) {
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);

		start = true;
		r = 0;
		xc1 = LOWORD(lp);
		yc1 = HIWORD(lp);

		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);
		
		start = false;

		xc2 = LOWORD(lp);
		yc2 = HIWORD(lp);

		x = abs(xc2 - xc1);
		y = abs(yc2 - yc1);

		r = roundLocal(sqrt(x * x + y * y));

		BresenhamsEfficientDDA(hdc, xc1, yc1, x2, y2, RGB(192, 192, 192));
		CircleBresenhamEff(hdc, xc1, yc1, r, RGB(192, 192, 192));
		CircleBresenhamEff(hdc, xc1, yc1, r, RGB(0, 0, 0));

		ReleaseDC(hwnd, hdc);
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);

		if (start) {
			oldx2 = x2;
            oldy2 = y2;

            x2 = LOWORD(lp);
            y2 = HIWORD(lp);

            BresenhamsEfficientDDA(hdc, xc1, yc1, oldx2, oldy2, RGB(192, 192, 192));
            BresenhamsEfficientDDA(hdc, xc1, yc1, x2, y2, RGB(255, 0, 0));

			xc2 = LOWORD(lp);
			yc2 = HIWORD(lp);

			x = abs(xc2 - xc1);
			y = abs(yc2 - yc1);

			oldr = r;
			r = roundLocal(sqrt(x * x + y * y));

			CircleBresenhamEff(hdc, xc1, yc1, oldr, RGB(192, 192, 192));
			CircleBresenhamEff(hdc, xc1, yc1, r, RGB(255, 0, 0));
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
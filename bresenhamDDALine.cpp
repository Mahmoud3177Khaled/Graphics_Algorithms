#include<Windows.h>
#include<math.h>

using namespace std;

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
                    y--;
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
                    y--;
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
                    y++;
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
                    y++;
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

        } else if (x2 < x1 && y2 < y1) {
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

void parabolaDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int oldx = x1;
	int oldy = y1;
	int diff = 0;

	int x = x1;
	int y = x1 * x1;
	int dy = 2 * x1 + 1;
	int d2y = 2;

	SetPixel(hdc, x1, y1, c);

	while (x < x2) {
		oldx = x;
		oldy = y;
		diff = abs(x-x1);
		x++;
		y += dy;
		dy += d2y;
		
		//SetPixel(hdc, 100+x, 100+y, c);
		BresenhamsEfficientDDA(hdc, 100+oldx, 100+oldy, 100+x, 100+y, c);
		BresenhamsEfficientDDA(hdc, 100+oldx-2*diff, 100+oldy, 100+x-2*diff, 100+y, c);

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


LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
	HDC hdc;
	static int xc1, yc1;

	static int xc2, yc2, x, y, r, oldr;

	static bool start = false;
	static bool finished = false;

	switch (m) {
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);

        BresenhamsEfficientDDA(hdc, 100, 100, 200, 150, RGB(0, 0, 0));
        BresenhamsEfficientDDA(hdc, 100, 100, 0, 150, RGB(0, 0, 0));
        BresenhamsEfficientDDA(hdc, 100, 100, 200, 50, RGB(0, 0, 0));
        BresenhamsEfficientDDA(hdc, 100, 100, 0, 50, RGB(0, 0, 0));

        BresenhamsEfficientDDA(hdc, 100, 100, 150, 200, RGB(0, 0, 0));
        BresenhamsEfficientDDA(hdc, 100, 100, 150, 0, RGB(0, 0, 0));
        BresenhamsEfficientDDA(hdc, 100, 100, 50, 200, RGB(0, 0, 0));
        BresenhamsEfficientDDA(hdc, 100, 100, 50, 0, RGB(0, 0, 0));



		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);

		ReleaseDC(hwnd, hdc);
		break;

	case WM_MOUSEMOVE:
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
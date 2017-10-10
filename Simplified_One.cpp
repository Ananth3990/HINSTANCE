#include<Windows.h>

#pragma comment(lib,"User32.lib")
#pragma comment (lib,"gdi32.lib")


HWND ghwnd = NULL;
HINSTANCE ghInstance = NULL;

DWORD dwStyle; 
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

WNDCLASSEX ac;
HWND hwnd;
MSG msg;
TCHAR szClassName[] = TEXT("Parent Window");

HDC hdc, hdcMem;

TCHAR *Processor[3] = {TEXT("Intel Core i3"),TEXT("Intel Core i5"),TEXT("Intel Core i7")};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	void uninitialize(void);
	LRESULT CALLBACK AcCallBack(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	MSG msg;

	ghInstance = hInstance;

	ac.cbSize = sizeof(WNDCLASSEX);
	ac.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	ac.cbClsExtra = 0;
	ac.cbWndExtra = 0;
	ac.hInstance = hInstance;
	ac.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	ac.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	ac.hCursor = LoadCursor(NULL, IDC_ARROW);
	ac.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	ac.lpfnWndProc = AcCallBack;
	ac.lpszClassName = szClassName;
	ac.lpszMenuName = NULL;


	RegisterClassEx(&ac);

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szClassName,
		TEXT("Shree Ganesha"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("Parent Window creation failed"), TEXT("Error"), MB_OK);
		exit(0);
	}

	ghwnd = hwnd;

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	uninitialize();
	return ((int)msg.wParam);
}


void uninitialize(void)
{
	dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
	SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
	SetWindowPlacement(ghwnd, &wpPrev);
	SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
	ShowCursor(TRUE);
	DestroyWindow(ghwnd);
}

void MakeFullScreen(HWND hwnd)
{
	BOOL isMonitorInfo = FALSE;
	HMONITOR hMonitor = NULL;
	MONITORINFO mi;

	//Get the style of current window
	dwStyle = GetWindowLong(hwnd, GWL_STYLE);

	/* WS_OVERLAPPEDWINDOW is combination of following styles 
	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
	we have specified this in our style in wndclasses structure.

	For fullscreen i dont need any of these flags
	*/

	if (dwStyle & WS_OVERLAPPEDWINDOW) //check if currently obtained style matches with we have given
	{

		//Get information about primary monitor which comes into picture when we have multiple monitors
		//But currently we have only one
		hMonitor = MonitorFromWindow(hwnd, MONITORINFOF_PRIMARY);
		mi = { sizeof(MONITORINFO) };

		//Takes information (in our case out of many info we need is left,top,right and bottom) about our monitor ands saves it in parameter mi 
		isMonitorInfo = GetMonitorInfo(hMonitor, &mi);

		if (isMonitorInfo == TRUE) //if above function goes well

		{
			//Now we go for full screen

			//Takes current style and removes WS_OVERLAPPEDWINDOW from its style
			SetWindowLong(hwnd, GWL_STYLE, dwStyle&~WS_OVERLAPPEDWINDOW);

			//Here we specify x,y width and height of our screen from monitor which is there in mi variable and tell that my window should be on top of all windows and say my window to become fullscreen.
			SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
		}
	}
	ShowCursor(TRUE); //In sirs code this was FALSE, but i have kept it TRUE for getting mouse cursor
}

LRESULT CALLBACK AcCallBack(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	void uninitialize(void);
	void MakeFullScreen(HWND hwnd);
	
	switch (iMsg)
	{
	case WM_CREATE:
		MakeFullScreen(hwnd);
		break;
		
	case WM_CLOSE:
		uninitialize();
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
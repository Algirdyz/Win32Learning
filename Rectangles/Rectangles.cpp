#include "stdafx.h"
#include "Rectangles.h"
#include "MainContainer.h"
#include <fstream>
#include <string>
#include "Utils.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#define MAX_LOADSTRING 100
#define IDC_GENERATE_BUTTON 101
#define IDC_WIDTH_EDIT 102
#define IDC_HEIGHT_EDIT 103
#define IDC_NUMBER_EDIT 103

HWND m_hwndHeight;
HWND m_hwndWidth;
HWND m_hwndGenerate;
HWND hWndButton;


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


MainContainer dataContainer = MainContainer(0, 0, 0);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Rectangles::Rectangles() :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL)
{
}

Rectangles::~Rectangles()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);

}


void Rectangles::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT Rectangles::Initialize()
{
	HRESULT hr;

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Rectangles::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"Rectangles";

		RegisterClassEx(&wcex);

		WNDCLASSEX wcex2 = { sizeof(WNDCLASSEX) };
		wcex2.style = CS_HREDRAW | CS_VREDRAW;
		wcex2.lpfnWndProc = Rectangles::WndProc;
		wcex2.cbClsExtra = 0;
		wcex2.cbWndExtra = sizeof(LONG_PTR);
		wcex2.hInstance = HINST_THISCOMPONENT;
		wcex2.hbrBackground = NULL;
		wcex2.lpszMenuName = NULL;
		wcex2.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex2.lpszClassName = L"Graphics";

		RegisterClassEx(&wcex2);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);


		// Create the window.
		m_hwnd = CreateWindow(
			L"Rectangles",
			L"Rectangles Algorithm",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(1028.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(800.f * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);

		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
		HFONT hFont = ::CreateFontIndirect(&ncm.lfMessageFont);
		::SendMessage(m_hwnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));


		//Create graphics window
		m_hwndDirect2d = CreateWindow(
			L"Graphics",
			L"Graphics screen",
			WS_CHILD | WS_VISIBLE,
			0,
			100,
			static_cast<UINT>(ceil(1028.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(700.f * dpiY / 96.f)),
			m_hwnd,
			NULL,
			HINST_THISCOMPONENT,
			this
		);
		
		m_hwndHeight = CreateWindowEx(WS_EX_STATICEDGE, TEXT("Edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 140,
			20, m_hwnd, NULL, NULL, NULL);

		m_hwndWidth = CreateWindowEx(WS_EX_STATICEDGE, TEXT("Edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 10, 140,
			20, m_hwnd, NULL, NULL, NULL);

		m_hwndGenerate = CreateWindowEx(WS_EX_STATICEDGE, TEXT("Edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER, 310, 10, 140,
			20, m_hwnd, NULL, NULL, NULL);

		hWndButton = CreateWindowEx(NULL,
			L"BUTTON",
			L"OK",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			460,
			10,
			100,
			20,
			m_hwnd,
			(HMENU)IDC_GENERATE_BUTTON,
			GetModuleHandle(NULL),
			NULL);


		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}

		hr = m_hwndDirect2d ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwndDirect2d, SW_SHOWNORMAL);
			UpdateWindow(m_hwndDirect2d);
		}
	}

	return hr;
}

int WINAPI WinMain(
	HINSTANCE /* hInstance */,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
)
{
	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			Rectangles app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

HRESULT Rectangles::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}

HRESULT Rectangles::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwndDirect2d, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwndDirect2d, size),
			&m_pRenderTarget
		);


		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush
			);
		}
		if (SUCCEEDED(hr))
		{
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush
			);
		}
	}

	return hr;
}

void Rectangles::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK Rectangles::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Rectangles *pDemoApp = (Rectangles *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pDemoApp)
		);

		

		result = 1;
	}
	else if(message == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
			case IDC_GENERATE_BUTTON:
			{
				std::vector<char> height(256);
				GetWindowTextA(m_hwndHeight, &height[0], height.size());

				std::vector<char> width(256);
				GetWindowTextA(m_hwndWidth, &width[0], width.size());

				std::vector<char> numberOfRectangles(256);
				GetWindowTextA(m_hwndGenerate, &numberOfRectangles[0], numberOfRectangles.size());

				std::string heightS = std::string(height.begin(), height.end());
				std::string widthS = std::string(width.begin(), width.end());
				std::string numberToGenerateS = std::string(numberOfRectangles.begin(), numberOfRectangles.end());

				auto a = Utils::is_number(heightS);
				if (!(Utils::is_number(heightS) && Utils::is_number(widthS) && Utils::is_number(numberToGenerateS))) {
					break;
				}

				float heightF = stof(heightS);
				float widthF = stof(widthS);
				float numberToGenerateF = stof(numberToGenerateS);

				dataContainer = MainContainer(heightF, widthF, numberToGenerateF);
				Rectangles *pDemoApp = reinterpret_cast<Rectangles *>(static_cast<LONG_PTR>(
					::GetWindowLongPtrW(
						hwnd,
						GWLP_USERDATA
					)));
				pDemoApp->OnRender();
			}
			break;
		}
	}
	else
	{
		Rectangles *pDemoApp = reinterpret_cast<Rectangles *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));
		
		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pDemoApp->OnRender();
				ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

HRESULT Rectangles::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		

		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		for (int x = 0; x < width; x += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				m_pLightSlateGrayBrush,
				0.5f
			);
		}

		for (int y = 0; y < height; y += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				m_pLightSlateGrayBrush,
				0.5f
			);
		}

		float topMargin = dataContainer.DrawRectangles(m_pRenderTarget, m_pLightSlateGrayBrush);

		dataContainer.DrawSolvedRectangles(m_pRenderTarget, m_pLightSlateGrayBrush, topMargin, 10);
		
		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

void Rectangles::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

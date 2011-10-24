#include "WindowBase.h"

namespace {
	static LPCTSTR lpszClassName = _T("MainWnd");
}

// コンストラクタ
WindowBase::WindowBase( HINSTANCE hInstance ):
	m_hInstance(hInstance),
	m_Active(true),
	m_FullScreen(false),
	m_Width(0),
	m_Height(0)
{
}

// GetWindowLongにthisポインタを設定する
void WindowBase::SetPointer( HWND hWnd ){
	SetWindowLong( hWnd, GWL_USERDATA, reinterpret_cast<LONG>(this) );
}

// ウインドウプロシージャを呼び出す
LRESULT CALLBACK WindowBase::LocalWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ){
	// プロパティリストからthisポインタを取得する
	WindowBase* thisPtr = reinterpret_cast<WindowBase*>(::GetWindowLong( hWnd, GWL_USERDATA ));

	// thisポインタが取得出来なかった場合
	if( !thisPtr ){
		if( message == WM_CREATE ){
			thisPtr = reinterpret_cast<WindowBase*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams );
		}
		if( thisPtr ){
			thisPtr->SetPointer( hWnd );
		}
	}

	// thisポインタが取得できた場合
	if( thisPtr ){
		LRESULT lResult = thisPtr->WindowProc( hWnd, message, wParam, lParam );
		return lResult;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

// グローバルなウインドウプロシージャ
LRESULT WindowBase::WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ){
	switch( message ){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE ){
				PostQuitMessage(0);
			}
			return 0;

		case WM_ACTIVATE:
			if( wParam == WA_INACTIVE){
				m_Active = true;
			}
			return 0;
	}

	m_Active = false;
	return DefWindowProc( hWnd, message, wParam, lParam );
}

// ウインドウの初期化処理
HRESULT WindowBase::InitializeWindow(
	int width, int height, bool fullScreen,
	LPCTSTR windowName, WindowBase* cWnd
){
	// ウインドウが既に起動しているかどうか
	HWND hWnd = FindWindow( lpszClassName, windowName ); 
	if( hWnd != NULL ){
		// ウインドウを最前面に持ってくる
		SetForegroundWindow( hWnd );
		// ウインドウをアクティブにする
		SetActiveWindow( hWnd );

		return E_FAIL;
	}

	WNDCLASSEX wcex = {
		sizeof WNDCLASSEX, 
		CS_HREDRAW | CS_VREDRAW,
		WindowBase::LocalWindowProc,
		0,
		0,
		m_hInstance,
		NULL,
		NULL,
        (HBRUSH)(COLOR_WINDOW+1),
		NULL,
		lpszClassName,
		NULL
	};
   
	if( !RegisterClassEx(&wcex) ){ return E_FAIL; }

	// 正確に指定されたウインドウサイズに調整する
	RECT rc;
	SetRect( &rc, 0, 0, width, height );
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, false );
    
	hWnd = CreateWindowEx(
		NULL,
		wcex.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		wcex.hInstance,
		reinterpret_cast<LPVOID>(cWnd)
	);

	if( hWnd == NULL ){ return E_FAIL; }

	m_FullScreen = fullScreen;
	m_hWnd		 = hWnd;
	m_Width		 = width;
	m_Height	 = height;

	return S_OK;
}

// HWNDの取得
HWND WindowBase::GetHWND() const {
	return m_hWnd;
}

// HINSNTANCEの取得
HINSTANCE WindowBase::GetHINSTANCE() const{
	return m_hInstance;
}

// フルスクリーンの有無の取得
bool WindowBase::GetFullScreen() const {
	return m_FullScreen;
}

// 縦サイズの取得
int WindowBase::GetWidth() const {
	return m_Width;
}

// 横サイズの取得
int WindowBase::GetHeight() const {
	return m_Height;
}
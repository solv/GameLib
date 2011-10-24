#include "WindowBase.h"

namespace {
	static LPCTSTR lpszClassName = _T("MainWnd");
}

// �R���X�g���N�^
WindowBase::WindowBase( HINSTANCE hInstance ):
	m_hInstance(hInstance),
	m_Active(true),
	m_FullScreen(false),
	m_Width(0),
	m_Height(0)
{
}

// GetWindowLong��this�|�C���^��ݒ肷��
void WindowBase::SetPointer( HWND hWnd ){
	SetWindowLong( hWnd, GWL_USERDATA, reinterpret_cast<LONG>(this) );
}

// �E�C���h�E�v���V�[�W�����Ăяo��
LRESULT CALLBACK WindowBase::LocalWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ){
	// �v���p�e�B���X�g����this�|�C���^���擾����
	WindowBase* thisPtr = reinterpret_cast<WindowBase*>(::GetWindowLong( hWnd, GWL_USERDATA ));

	// this�|�C���^���擾�o���Ȃ������ꍇ
	if( !thisPtr ){
		if( message == WM_CREATE ){
			thisPtr = reinterpret_cast<WindowBase*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams );
		}
		if( thisPtr ){
			thisPtr->SetPointer( hWnd );
		}
	}

	// this�|�C���^���擾�ł����ꍇ
	if( thisPtr ){
		LRESULT lResult = thisPtr->WindowProc( hWnd, message, wParam, lParam );
		return lResult;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

// �O���[�o���ȃE�C���h�E�v���V�[�W��
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

// �E�C���h�E�̏���������
HRESULT WindowBase::InitializeWindow(
	int width, int height, bool fullScreen,
	LPCTSTR windowName, WindowBase* cWnd
){
	// �E�C���h�E�����ɋN�����Ă��邩�ǂ���
	HWND hWnd = FindWindow( lpszClassName, windowName ); 
	if( hWnd != NULL ){
		// �E�C���h�E���őO�ʂɎ����Ă���
		SetForegroundWindow( hWnd );
		// �E�C���h�E���A�N�e�B�u�ɂ���
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

	// ���m�Ɏw�肳�ꂽ�E�C���h�E�T�C�Y�ɒ�������
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

// HWND�̎擾
HWND WindowBase::GetHWND() const {
	return m_hWnd;
}

// HINSNTANCE�̎擾
HINSTANCE WindowBase::GetHINSTANCE() const{
	return m_hInstance;
}

// �t���X�N���[���̗L���̎擾
bool WindowBase::GetFullScreen() const {
	return m_FullScreen;
}

// �c�T�C�Y�̎擾
int WindowBase::GetWidth() const {
	return m_Width;
}

// ���T�C�Y�̎擾
int WindowBase::GetHeight() const {
	return m_Height;
}
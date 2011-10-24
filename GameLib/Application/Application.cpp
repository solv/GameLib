#include "Application.h"
#include "../Input/Input.h"
#include "../Utility/Utility.h"

// コンストラクタ
Application::Application( HINSTANCE hInstance ):
	m_spWindowBase( boost::make_shared<WindowBase>( hInstance ) )
{
}

// 初期化
HRESULT Application::Initialize( const int width, const int height, const bool fullScreen, const LPCTSTR windowName ){
	// ウインドウの生成
	if( FAILED( m_spWindowBase->InitializeWindow( width, height, fullScreen, windowName, m_spWindowBase.get() ) ) ){
		return E_FAIL;
	}

	// Inputの初期化
	if( FAILED( Input::Instance().Initialize( m_spWindowBase->GetHWND(), m_spWindowBase->GetHINSTANCE() ) ) ){
		return E_FAIL;
	}

	// ウインドウの表示
	ShowWindow( m_spWindowBase->GetHWND(), SW_SHOWDEFAULT );

	return S_OK;
}

// メッセージループ
WPARAM Application::Run(){
	// メッセージループ
	MSG msg;
	do{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			DispatchMessage( &msg );
		}else{
			// ここにシーンの更新処理を書く
		}
	}while( msg.message != WM_QUIT );

	return msg.wParam;
}
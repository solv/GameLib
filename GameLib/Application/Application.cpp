#include "Application.h"
#include "../Input/Input.h"
#include "../Utility/Utility.h"

// �R���X�g���N�^
Application::Application( HINSTANCE hInstance ):
	m_spWindowBase( boost::make_shared<WindowBase>( hInstance ) )
{
}

// ������
HRESULT Application::Initialize( const int width, const int height, const bool fullScreen, const LPCTSTR windowName ){
	// �E�C���h�E�̐���
	if( FAILED( m_spWindowBase->InitializeWindow( width, height, fullScreen, windowName, m_spWindowBase.get() ) ) ){
		return E_FAIL;
	}

	// Input�̏�����
	if( FAILED( Input::Instance().Initialize( m_spWindowBase->GetHWND(), m_spWindowBase->GetHINSTANCE() ) ) ){
		return E_FAIL;
	}

	// �E�C���h�E�̕\��
	ShowWindow( m_spWindowBase->GetHWND(), SW_SHOWDEFAULT );

	return S_OK;
}

// ���b�Z�[�W���[�v
WPARAM Application::Run(){
	// ���b�Z�[�W���[�v
	MSG msg;
	do{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			DispatchMessage( &msg );
		}else{
			// �����ɃV�[���̍X�V����������
		}
	}while( msg.message != WM_QUIT );

	return msg.wParam;
}
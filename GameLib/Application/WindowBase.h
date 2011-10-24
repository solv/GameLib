#pragma once

#include "../stdafx.h"

//
// �E�C���h�E���Ǘ�����N���X
//
class WindowBase : private boost::noncopyable
{
private:
	HWND		m_hWnd;			// �E�C���h�E�n���h��		
	HINSTANCE	m_hInstance;	// �A�v���P�[�V�����̃n���h��
	bool m_FullScreen;			// �t���X�N���[�����[�h�̗L��
	bool m_Active;				// �E�C���h�E�̃A�N�e�B�u�̗L��
	int m_Width;				// �E�C���h�E���T�C�Y
	int m_Height;				// �E�C���h�E�c�T�C�Y

public:
	// �R���X�g���N�^
	WindowBase( HINSTANCE );

	// GetWindowLong��this�|�C���^��ݒ肷��
	void SetPointer( HWND );

	// �E�C���h�E�v���V�[�W�����Ăяo��
	static LRESULT CALLBACK LocalWindowProc( HWND, UINT, WPARAM, LPARAM );

	// �E�C���h�E�v���V�[�W���̖{��
	LRESULT WindowProc( HWND, UINT, WPARAM, LPARAM );

	// �E�C���h�E�̏�����
	HRESULT InitializeWindow( int, int, bool, LPCTSTR, WindowBase* );

	// HWND�̎擾
	HWND GetHWND() const;

	// HINSNTANCE�̎擾
	HINSTANCE GetHINSTANCE() const;
	
	//  �t���X�N���[���̗L���̎擾
	bool GetFullScreen() const;

	// �c�T�C�Y�̎擾
	int GetWidth() const;

	// ���T�C�Y�̎擾
	int GetHeight() const;
};
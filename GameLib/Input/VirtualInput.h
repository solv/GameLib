#pragma once

#include "Input.h"
#include "VirtualButtonArray.h"
#include <boost/noncopyable.hpp>

class VirtualInput : private boost::noncopyable
{
private:
	// �R���X�g���N�^
	VirtualInput();
	// �f�X�g���N�^
	~VirtualInput();

public:
	static const int KeyBoard = 0;
	static const int JoyPad	  = 1;
	static const int VIRTUAL_BUTTON_MAX = 8;
	UINT m_Button[VIRTUAL_BUTTON_MAX];

public:
	// �C���X�^���X�̎擾
	static VirtualInput& Instance();

	// �f�o�C�X�̏�������Input�N���X�ɔC����
	HRESULT Initialize( HWND hWnd, HINSTANCE hInstance );

	// ���͏�Ԃ̍X�V
	HRESULT Update();

	// ���̓{�^���̎擾
	int GetKey( const VButtonArray::Button button ) const;

	// �{�^���̕ύX
	void ChangeButton( VButtonArray::Button oldButton, const KeyArray::Key newKey );
	void ChangeButton( VButtonArray::Button oldButton, const ButtonArray::Button newButton );

	// ���ݓ��͂���Ă���{�^���ԍ�
	int GetInputButton() const;
};
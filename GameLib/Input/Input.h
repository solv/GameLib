#pragma once

#include "../stdafx.h"
#include "KeyArray.h"
#include "ButtonArray.h"

// �O���錾
class Keyboard;
class JoyPad;

//
// ���͊Ǘ��N���X
//
class Input : private boost::noncopyable
{
private:
	boost::intrusive_ptr<IDirectInput8> m_pDirectInput;	// DirectInput�f�o�C�X
	boost::shared_ptr<Keyboard> m_Keyboard;				// �L�[�{�[�h���Ǘ�����f�o�C�X
	boost::shared_ptr<JoyPad> m_JoyPad;					// �W���C�p�b�h���Ǘ�����f�o�C�X

private:
	// �R���X�g���N�^
	Input();
	// �f�X�g���N�^
	~Input();

public:
	// �C���X�^���X�̎擾
	static Input& Instance();

	// DirectInput�̏�����
	HRESULT Initialize( const HWND, const HINSTANCE );

	// �L�[�{�[�h�f�o�C�X�̎擾
	boost::shared_ptr<Keyboard> GetKeyboard() const;

	// �L�[�{�[�h�f�o�C�X�̎擾
	boost::shared_ptr<JoyPad> GetJoyPad() const;
};


//
// �L�[�{�[�h�f�o�C�X�N���X
//
class Keyboard : private boost::noncopyable
{
private:
	static const int KEYBOARD_BUFFER_NUM = 256;					// �L�[�{�[�h�̃L�[�z��̌�
	boost::intrusive_ptr<IDirectInputDevice8> m_KeyboardDevice;	// �L�[�{�[�h�f�o�C�X
	boost::array<int, KEYBOARD_BUFFER_NUM> m_KeyInputLevel;		// ���͏󋵂��Ǘ�����z��

public:
	// �R���X�g���N�^
	Keyboard();

	// �f�X�g���N�^
	~Keyboard();

	// �L�[�{�[�h�f�o�C�X�̏�����
	HRESULT Initialize( const boost::intrusive_ptr<IDirectInput8>, const HWND );

	// ���͏�Ԃ̍X�V
	HRESULT Update();

	// ���͂��ꂽ�L�[�̎擾
	int GetKey( const KeyArray::Key ) const;
};


//
// �W���C�p�b�h�f�o�C�X�N���X
//
class JoyPad : private boost::noncopyable
{
private:
	static const int JOYPAD_BUFFER_NUM = 32;										// �W���C�p�b�h�̃L�[�z��̌�

	boost::intrusive_ptr<IDirectInput8>						m_pDirectInput;			// DirectInput�f�o�C�X
	std::vector<boost::array<int, JOYPAD_BUFFER_NUM>>		m_ButtonInputLevel;		// ���͏󋵂��Ǘ�����z��
	std::vector<boost::intrusive_ptr<IDirectInputDevice8>>	m_JoyPadDeviceVector;	// �W���C�p�b�h�f�o�C�X(�ϒ�)
	
private:
	// �W���C�p�b�h�����o���邽�߂̃R�[���o�b�N�֐�
	static BOOL CALLBACK EnumJoyPadCallback( const DIDEVICEINSTANCE* , LPVOID );
	static BOOL CALLBACK EnumSizesCallback( LPCDIDEVICEOBJECTINSTANCE, LPVOID );

public:
	// �R���X�g���N�^
	JoyPad();

	// �f�X�g���N�^
	~JoyPad();

	// �W���C�p�b�h�f�o�C�X�̏�����
	HRESULT Initialize( const boost::intrusive_ptr<IDirectInput8>, const HWND );

	// ���͏�Ԃ̍X�V
	HRESULT Update();

	// ���͂��ꂽ�L�[�̎擾
	int GetKey( const DWORD padNum, const ButtonArray::Button ) const;
};
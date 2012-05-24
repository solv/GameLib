#pragma once

#define DIRECTINPUT_VERSION		DIRECTINPUT_HEADER_VERSION
#include <dinput.h>
#include <boost/shared_ptr.hpp>
#include "KeyArray.h"
#include "ButtonArray.h"

// �O���錾
class KeyBoard;
class JoyPad;

class Input
{
private:
	LPDIRECTINPUT8 m_pDirectInput;

private:
	// �R���X�g���N�^
	Input();
	// �R�s�[�R���X�g���N�^
	Input( const Input& );
	// �f�X�g���N�^
	~Input();

private:
	boost::shared_ptr<KeyBoard> m_KeyBoard;
	boost::shared_ptr<JoyPad> m_JoyPad;
	//boost::Shared_ptr<Mouse> m_Mouse;

public:
	// �C���X�^���X�̎擾
	static Input& Instance();

	// DirectInput�̏�����
	HRESULT Initialize( HWND hWnd, HINSTANCE hInstance );

	// �L�[�{�[�h�f�o�C�X�̎擾
	KeyBoard* GetKeyBoard() const;

	// �W���C�p�b�h�f�o�C�X�̎擾
	JoyPad* GetJoyPad() const;
};

class KeyBoard
{
public:
	static const int KEY_NUM = 256;// �L�[�{�[�h�̃L�[��

private:
	static const int DIMGR_BUFFER_SIZE = 256; // �o�b�t�@�[�T�C�Y

	bool m_KeyInputOnOffBuffer[KEY_NUM]; // �L�[�̓��̗͂L��
	int m_KeyInputLevelBuffer[KEY_NUM];	 // �L�[�̓��̓��x��

	LPDIRECTINPUTDEVICE8	m_KeyBoardDevice; // �L�[�{�[�h�p�̃f�o�C�X
	DIDEVICEOBJECTDATA		m_DeviceObject[DIMGR_BUFFER_SIZE];

public:
	// �R���X�g���N�^
	KeyBoard();

	// �f�X�g���N�^
	~KeyBoard();

	// �L�[�{�[�h�̏�����
	HRESULT Initialize( LPDIRECTINPUT8 pDirectInput, HWND hWnd  );

	// �X�V
	HRESULT Update();

	// ���̓L�[�̎擾
	int GetKey( const KeyArray::Key key) const;
};

class JoyPad
{
public:
	static const int JOYPAD_BUTTON_NUM	= 16;	// �W���C�p�b�h�̃{�^���̐�

private:
	static const int DIMGR_BUFFER_SIZE = 256;	// �o�b�t�@�[�T�C�Y
	static int m_JoyPadNum; // �ڑ�����Ă���W���C�p�b�h�̌�

	bool m_KeyInputOnOffBuffer[JOYPAD_BUTTON_NUM]; // �L�[�̓��̗͂L��
	int  m_KeyInputLevelBuffer[JOYPAD_BUTTON_NUM];	 // �L�[�̓��̓��x��

	LPDIRECTINPUTDEVICE8	m_JoyPadDevice; // �W���C�p�b�h�p�̃f�o�C�X
	DIDEVICEOBJECTDATA		m_DeviceObject[JOYPAD_BUTTON_NUM];
	DIDEVCAPS				m_DeviceCaps;
	LPDIRECTINPUT8			m_pDirectInput; // DirectInput�̃f�o�C�X

private:
	// �W���C�p�b�h���o�̃R�[���o�b�N�֐�
	static BOOL CALLBACK EnumSizesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef );
	static BOOL CALLBACK EnumJoyPadCallback( const DIDEVICEINSTANCE* lpddi, LPVOID lpContext );

public:
	// �R���X�g���N�^
	JoyPad();

	// �f�X�g���N�^
	~JoyPad();

	// �W���C�p�b�h�̏�����
	HRESULT Initialize( LPDIRECTINPUT8 pDirectInput, HWND hWnd );

	// �X�V
	HRESULT Update();

	// ���̓{�^���̎擾
	int GetKey( const ButtonArray::Button button ) const;

	// �ڑ�����Ă���W���C�p�b�h�̌�
	static int GetJoyPadNum();
};
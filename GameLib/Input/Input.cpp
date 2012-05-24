#include "Input.h"
#include "../Common.h"

//
// �C���v�b�g�N���X
//
// �R���X�g���N�^
Input::Input():
	m_pDirectInput(NULL),
	m_KeyBoard( new KeyBoard() ),
	m_JoyPad( new JoyPad() )
{
}

// �R�s�[�R���X�g���N�^
Input::Input( const Input& obj ){
}

// �f�X�g���N�^
Input::~Input(){
	SAFE_RELEASE(m_pDirectInput);
}

// �C���X�^���X�̎擾
Input& Input::Instance(){
	static Input instance;
	return instance;
}

// DirectInput�̏�����
HRESULT Input::Initialize( HWND hWnd, HINSTANCE hInstance ){
	// �f�o�C�X�̎擾
	if( FAILED( ::DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_pDirectInput), NULL ) ) ){
		return E_FAIL;
	}

	// �L�[�{�[�h�̏�����
	if( FAILED( m_KeyBoard->Initialize( m_pDirectInput, hWnd ) ) ){
		return E_FAIL;
	}
	if( FAILED( m_JoyPad->Initialize( m_pDirectInput, hWnd ) ) ){
		return E_FAIL;
	}
	return S_OK;
}

// �L�[�{�[�h�f�o�C�X�̎擾
KeyBoard* Input::GetKeyBoard() const {
	return m_KeyBoard.get();
}

// �W���C�p�b�h�f�o�C�X�̎擾
JoyPad* Input::GetJoyPad() const {
	return m_JoyPad.get();
}

//
// �L�[�{�[�h�N���X
//
// �R���X�g���N�^
KeyBoard::KeyBoard():
	m_KeyBoardDevice(NULL)
{
	for( int i = 0; i < KEY_NUM; ++i ){
		m_KeyInputOnOffBuffer[i] = false;
		m_KeyInputLevelBuffer[i] = 0;
	}
}

// �f�X�g���N�^
KeyBoard::~KeyBoard(){
	if( m_KeyBoardDevice ){
		m_KeyBoardDevice->Unacquire();
	}
	SAFE_RELEASE(m_KeyBoardDevice);
}

// �L�[�{�[�h�f�o�C�X�̏�����
HRESULT KeyBoard::Initialize( LPDIRECTINPUT8 pDirectInput, HWND hWnd){
	// �L�[�{�[�h�f�o�C�X�̍쐬
	if( FAILED( pDirectInput->CreateDevice( GUID_SysKeyboard,&m_KeyBoardDevice, NULL ) ) ){
		return E_FAIL;
	}

	// �擾�f�[�^�t�H�[�}�b�g�̐ݒ�
	if( FAILED( m_KeyBoardDevice->SetDataFormat(&c_dfDIKeyboard) ) ){
		return E_FAIL;
	}

	// �������x���̐ݒ�
	if( FAILED( m_KeyBoardDevice->SetCooperativeLevel(
		hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
	) ) ){
		return E_FAIL;
	}

	// �o�b�t�@�T�C�Y�̎w��
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize		= sizeof DIPROPDWORD;
	dipdw.diph.dwHeaderSize	= sizeof DIPROPHEADER;
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= DIMGR_BUFFER_SIZE;

	if( FAILED( m_KeyBoardDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) ){
		return E_FAIL;
	}

	// �L�[�{�[�h�̓��͂�L���ɂ���
	m_KeyBoardDevice->Acquire();

	return S_OK;
}

// �L�[���͂̍X�V
HRESULT KeyBoard::Update(){
	// �L�[�{�[�h�f�o�C�X�������ꍇ�͏I��
	if( m_KeyBoardDevice == NULL ){
		return S_OK;
	}

	// �f�o�C�X���̎擾
	DWORD elements = KEY_NUM;
	HRESULT hr = m_KeyBoardDevice->GetDeviceData( sizeof DIDEVICEOBJECTDATA, m_DeviceObject, &elements, 0 );
	if( hr != DI_OK ){
		if( m_KeyBoardDevice->Acquire() != DI_OK ){
			return E_FAIL;
		}
		m_KeyBoardDevice->GetDeviceData( sizeof DIDEVICEOBJECTDATA, m_DeviceObject, &elements, 0 );
	}

	// ���͏󋵂̎擾
	for( DWORD i = 0; i < elements; ++i ){
		m_KeyInputOnOffBuffer[m_DeviceObject[i].dwOfs] = ( (m_DeviceObject[i].dwData & 0x80) >> 7 ) == 0 ? false : true;
	}

	for( DWORD i = 0; i < KEY_NUM; ++i ){
		m_KeyInputOnOffBuffer[i] == true ? ++m_KeyInputLevelBuffer[i] : m_KeyInputLevelBuffer[i] = 0;
	}

	return S_OK;
}

// �L�[�̓��͏󋵂̎擾
int KeyBoard::GetKey( const KeyArray::Key key ) const {
	return m_KeyInputLevelBuffer[key];
}

//
// �W���C�p�b�h�N���X
//

int JoyPad::m_JoyPadNum(0);

// �R���X�g���N�^
JoyPad::JoyPad():
	m_JoyPadDevice(NULL)
{
	for( int i = 0; i < JOYPAD_BUTTON_NUM; ++i ){
		m_KeyInputOnOffBuffer[i] = false;
		m_KeyInputLevelBuffer[i] = 0;
	}
}

// �f�X�g���N�^
JoyPad::~JoyPad(){
	if( m_JoyPadDevice ){
		m_JoyPadDevice->Unacquire();
	}
	SAFE_RELEASE(m_JoyPadDevice);
}

// �W���C�p�b�h�f�o�C�X�̏�����
HRESULT JoyPad::Initialize( LPDIRECTINPUT8 pDirectInput, HWND hWnd ){
	// �f�o�C�X��n��
	m_pDirectInput = pDirectInput;

	// �W���C�p�b�h�f�o�C�X�̍쐬
	if( FAILED( pDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoyPadCallback, static_cast<void*>(this), DIEDFL_ATTACHEDONLY ) ) ) {
		return E_FAIL;
	}

	// �W���C�p�b�h�͌��������H
	if( m_JoyPadNum == 0 ){
		return S_OK;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof diprop;
	diprop.diph.dwHeaderSize = sizeof diprop.diph;
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_ABS;

	if( FAILED( m_JoyPadDevice->SetDataFormat( &c_dfDIJoystick ) ) ){
		return E_FAIL;
	}
	if( FAILED( m_JoyPadDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) &&
		FAILED( m_JoyPadDevice->EnumObjects( EnumSizesCallback, static_cast<void*>( m_JoyPadDevice ), DIDFT_AXIS ) ) &&
		FAILED( m_JoyPadDevice->SetProperty( DIPROP_AXISMODE, &diprop.diph ) )
	){
		return E_FAIL;
	}

	diprop.dwData = DIMGR_BUFFER_SIZE;
	if( FAILED( m_JoyPadDevice->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph ) ) ){
		return E_FAIL;
	}
	m_JoyPadDevice->Acquire();

	return S_OK;
}

// �{�^�����͂̍X�V
HRESULT JoyPad::Update(){
	if( m_JoyPadDevice == NULL ){
		return S_OK;
	}
	
	if( FAILED( m_JoyPadDevice->Poll() ) ){
		if( m_JoyPadDevice->Acquire() != DI_OK ){
			return E_FAIL;
		}
		m_JoyPadDevice->Poll();
	}

	DIJOYSTATE dijs;
	if( m_JoyPadDevice->GetDeviceState( sizeof DIJOYSTATE, &dijs ) == DI_OK ){
		for( int i = 0; i < JOYPAD_BUTTON_NUM; ++i ){
			if( dijs.rgbButtons[i] & 0x80 ){
				m_KeyInputOnOffBuffer[i] = true;
			}else{
				m_KeyInputOnOffBuffer[i] = false;
			}
		}
	}else{
		if( m_JoyPadDevice->Acquire() != DI_OK ){
			return E_FAIL;
		}
	}

	for( DWORD i = 0; i < JOYPAD_BUTTON_NUM; ++i ){
		m_KeyInputOnOffBuffer[i] == true ? ++m_KeyInputLevelBuffer[i] : m_KeyInputLevelBuffer[i] = 0;
	}

	return S_OK;
}

// �W���C�p�b�h�̃{�^���̓��͏󋵂̎擾
int JoyPad::GetKey( const ButtonArray::Button button ) const {
	return m_KeyInputLevelBuffer[button];
}

// �W���C�p�b�h�����o����R�[���o�b�N�֐�
BOOL CALLBACK JoyPad::EnumSizesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef ){
	// �f�o�C�X�̐ݒ�
	DIPROPRANGE diprg;
	ZeroMemory( &diprg, sizeof diprg );
	diprg.diph.dwSize = sizeof diprg;
	diprg.diph.dwHeaderSize = sizeof diprg.diph;
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	LPDIRECTINPUTDEVICE8 lpJoy = reinterpret_cast<LPDIRECTINPUTDEVICE8>( lpvRef );
	if( FAILED( lpJoy->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ){
		return E_FAIL;
	}
	return DIENUM_STOP;
}

BOOL CALLBACK JoyPad::EnumJoyPadCallback( const DIDEVICEINSTANCE* lpddi, LPVOID lpContext ){
	// �W���C�p�b�h�f�o�C�X�̍쐬
	JoyPad* joypad = reinterpret_cast<JoyPad*>( lpContext );
	joypad->m_DeviceCaps.dwSize = sizeof DIDEVCAPS;
	joypad->m_pDirectInput->CreateDevice( lpddi->guidInstance, &joypad->m_JoyPadDevice, NULL );
	joypad->m_JoyPadDevice->GetCapabilities( &joypad->m_DeviceCaps );
	++m_JoyPadNum;

	return DIENUM_STOP;
}

// �ڑ�����Ă���W���C�p�b�h�̌�
int JoyPad::GetJoyPadNum(){
	return m_JoyPadNum;
}
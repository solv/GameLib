#include "../stdafx.h"
#include "Input.h"
#include "../Utility/Utility.h"
#include <algorithm>

//
// ���͊Ǘ��N���X
//

// �R���X�g���N�^
Input::Input():
	m_pDirectInput(NULL),
	m_Keyboard( boost::make_shared<Keyboard>() ),
	m_JoyPad( boost::make_shared<JoyPad>() )
{
}

// �f�X�g���N�^
Input::~Input(){
}

// �C���X�^���X�̎擾
Input& Input::Instance(){
	static Input instance;
	return instance;
}

// DirectInput�̏�����
HRESULT Input::Initialize( const HWND hWnd, const HINSTANCE hInstance ){
	ASSERT(hWnd);
	ASSERT(hInstance);

	// �G���[�����p
	HRESULT hr;

	// DirectInput�f�o�C�X�̎擾
	hr = DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_pDirectInput), NULL );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// �L�[�{�[�h�f�o�C�X�̏�����
	hr = m_Keyboard->Initialize( m_pDirectInput, hWnd );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// �W���C�p�b�h�f�o�C�X�̏�����
	hr = m_JoyPad->Initialize( m_pDirectInput, hWnd );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	return S_OK;
}

// �L�[�{�[�h�f�o�C�X�̎擾
boost::shared_ptr<Keyboard> Input::GetKeyboard() const {
	return m_Keyboard;
}

// �L�[�{�[�h�f�o�C�X�̎擾
boost::shared_ptr<JoyPad> Input::GetJoyPad() const {
	return m_JoyPad;
}



//
// �L�[�{�[�h�f�o�C�X�N���X
//

// �R���X�g���N�^
Keyboard::Keyboard():
	m_KeyboardDevice(NULL)
{
	std::fill( m_KeyInputLevel.begin(), m_KeyInputLevel.end(), 0 );
}

// �f�X�g���N�^
Keyboard::~Keyboard(){
	if( m_KeyboardDevice ){
		m_KeyboardDevice->Unacquire();
	}
}

// �L�[�{�[�h�f�o�C�X�̏�����
HRESULT Keyboard::Initialize( const boost::intrusive_ptr<IDirectInput8> pDirectInput, const HWND hWnd ){
	ASSERT(pDirectInput);
	ASSERT(hWnd);

	// �G���[�����p
	HRESULT hr;

	// �L�[�{�[�h�f�o�C�X�̍쐬
	// intrusive_ptr���_�u���|�C���^�����Ȃ�����temp�I�u�W�F�N�g���g�p
	IDirectInputDevice8* tempKeyboardDevice = m_KeyboardDevice.get();
	hr = pDirectInput->CreateDevice( GUID_SysKeyboard, &tempKeyboardDevice, NULL );
	if( FAILED( hr ) ){
		return E_FAIL;
	}
	ASSERT(tempKeyboardDevice);
	boost::intrusive_ptr<IDirectInputDevice8> temp(tempKeyboardDevice);
	m_KeyboardDevice.swap(temp);

	// �擾�f�[�^�t�H�[�}�b�g�̐ݒ�
	hr = m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// �������x���̐ݒ�
	hr = m_KeyboardDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// �L�[�{�[�h�f�o�C�X��L���ɂ���
	m_KeyboardDevice->Acquire();

	return S_OK;
}

// ���͏�Ԃ̍X�V
HRESULT Keyboard::Update(){
	// �f�o�C�X��������Ȃ����͋A��
	if( m_KeyboardDevice == NULL ){
		return S_OK;
	}
	ASSERT(m_KeyboardDevice);

	// �f�o�C�X�̏�Ԃ̏����擾
	HRESULT hr;
	boost::array<BYTE, KEYBOARD_BUFFER_NUM> buffer = {};

	// ���݂̏�Ԃ��擾����
	hr = m_KeyboardDevice->GetDeviceState( buffer.size(), buffer.c_array() );
	if( hr != DI_OK ){ // �f�o�C�X�����X�g���Ă�����
		if( m_KeyboardDevice->Acquire() != DI_OK ){ // �Ď擾�����݂�
			return E_FAIL;
		}
		hr = m_KeyboardDevice->GetDeviceState( buffer.size(), buffer.c_array() );
	}

	// ���͏󋵂��X�V����
	for( int i = 0; i < KEYBOARD_BUFFER_NUM; ++i ){
		buffer[i] & 0x80 ? ++m_KeyInputLevel[i] :  m_KeyInputLevel[i] = 0;
	}
	
	return S_OK;
}

// ���͂��ꂽ�L�[�̎擾
int Keyboard::GetKey( const KeyArray::Key key ) const {
	if( m_KeyInputLevel.empty() ){ return 0; }
	return m_KeyInputLevel[key];
}



//
// �W���C�p�b�h�f�o�C�X�N���X
//

// �R���X�g���N�^
JoyPad::JoyPad():
	m_pDirectInput(NULL)
{
	m_JoyPadDeviceVector.clear();
	m_ButtonInputLevel.clear();
}

// �f�X�g���N�^
JoyPad::~JoyPad(){
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		if(*i){
			(*i)->Unacquire();
		}
	}
}

// �W���C�p�b�h�����o���邽�߂̃R�[���o�b�N�֐�
BOOL CALLBACK JoyPad::EnumJoyPadCallback( const DIDEVICEINSTANCE* lpddi, LPVOID lpContext ){
	// �G���[�����p
	HRESULT hr;

	// this�|�C���^���󂯂Ƃ�
	JoyPad* joypad = reinterpret_cast<JoyPad*>( lpContext );
	
	// �W���C�p�b�h�f�o�C�X���쐬����
	IDirectInputDevice8* tempJoyPadDevice;
	hr = joypad->m_pDirectInput->CreateDevice( lpddi->guidInstance, &tempJoyPadDevice, NULL );
	if( FAILED( hr ) ){
		return DIENUM_CONTINUE;
	}

	ASSERT(tempJoyPadDevice);

	// �쐬�����f�o�C�X��z��ɒǉ�����
	boost::intrusive_ptr<IDirectInputDevice8> temp( tempJoyPadDevice );
	joypad->m_JoyPadDeviceVector.push_back(temp);

	// ���͏󋵂��Ǘ�����z���ǉ�����
	joypad->m_ButtonInputLevel.push_back( boost::array<int, JOYPAD_BUFFER_NUM>() );

	return DIENUM_CONTINUE;
}

BOOL CALLBACK JoyPad::EnumSizesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef ){
	// ���݂��Ă���W���C�p�b�h�̃C�e���[�^
	JoyPad* joypad = reinterpret_cast<JoyPad*>( lpvRef );
	static std::vector<boost::intrusive_ptr<IDirectInputDevice8>>::iterator itr_pos = joypad->m_JoyPadDeviceVector.begin();
		
	
	// �G���[�����p
	HRESULT hr;

	// ���͈̔͂̏����i�[����\����
	DIPROPRANGE diprg		= {};
	diprg.diph.dwSize		= sizeof DIPROPRANGE;
	diprg.diph.dwHeaderSize = sizeof DIPROPHEADER;
	diprg.diph.dwHow		= DIPH_BYID;
	diprg.diph.dwObj		= lpddoi->dwType;
	diprg.lMin				= -1000;
	diprg.lMax				= +1000;

	// �ݒ肷��
	hr = (*itr_pos)->SetProperty( DIPROP_RANGE, &diprg.diph );
	if( FAILED( hr ) ){ // ���s������
		return DIENUM_STOP;
	}
	
	if( (itr_pos - joypad->m_JoyPadDeviceVector.begin()) < (static_cast<int>(joypad->m_JoyPadDeviceVector.size()) - 1) ){
		++itr_pos; // ���̃f�o�C�X�̃|�C���^�Ɉړ�����
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

// �W���C�p�b�h�f�o�C�X�̏�����
HRESULT JoyPad::Initialize( const boost::intrusive_ptr<IDirectInput8> pDirectInput, const HWND hWnd ){
	ASSERT(pDirectInput);
	ASSERT(hWnd);
	
	// �G���[�����p
	HRESULT hr;

	// �f�o�C�X�������o�ϐ��ɓn��
	m_pDirectInput = pDirectInput;

	ASSERT(m_pDirectInput);

	// �W���C�p�b�h�f�o�C�X��񋓂��쐬����
	hr = m_pDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoyPadCallback, static_cast<LPVOID>(this), DIEDFL_ATTACHEDONLY );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// �W���C�p�b�h��������Ȃ��ꍇ�̓L�[�{�[�h�݂̂ƂȂ�̂ł����ŏI������
	if( m_JoyPadDeviceVector.empty() ){
		return S_OK;
	}

	// �f�[�^�`���̐ݒ�
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		hr = (*i)->SetDataFormat( &c_dfDIJoystick );
		if( FAILED( hr ) ){
			return E_FAIL;
		}
	}

	// �������x���̐ݒ�
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		hr = (*i)->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND );
		if( FAILED( hr ) ){
			return E_FAIL;
		}
	}

	// ���͈̔͂̐ݒ�
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		hr = (*i)->EnumObjects( EnumSizesCallback, static_cast<void*>(this), DIDFT_AXIS );
		if( FAILED( hr ) ){
			return E_FAIL;
		}
	}

	// �����[�h�̐ݒ�
	DIPROPDWORD diprop		 = {};
	diprop.diph.dwSize		 = sizeof diprop;
	diprop.diph.dwHeaderSize = sizeof diprop.diph;
	diprop.diph.dwObj		 = 0;
	diprop.diph.dwHow		 = DIPH_DEVICE;
	diprop.dwData			 = DIPROPAXISMODE_ABS; // ��Βl���[�h

	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		hr = (*i)->SetProperty( DIPROP_AXISMODE, &diprop.diph );
		if( FAILED( hr ) ){
			return E_FAIL;
		}
	}

	// �W���C�p�b�h��L���ɂ���
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		(*i)->Acquire();
	}

	return S_OK;
}

// ���͏�Ԃ̍X�V
HRESULT JoyPad::Update(){
	// �G���[�����p
	HRESULT hr;

	// �f�o�C�X�̌����擾����
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		// �f�o�C�X�����݂��Ȃ��ꍇ�͋A��
		if( m_JoyPadDeviceVector.empty() ){
			return S_OK;
		}

		// �f�o�C�X���|�[�����O����
		hr = (*i)->Poll();
		if( FAILED( hr ) ){
			hr = (*i)->Acquire();
			while( hr == DIERR_INPUTLOST ){
				hr = (*i)->Acquire();
			}
		}

		// ���݂̓��͏�Ԃ��擾����
		DIJOYSTATE dijs;
		
		hr = (*i)->GetDeviceState( sizeof DIJOYSTATE, &dijs );
		if( hr == DI_OK ){
			// ���ꂼ��̃{�^���̏�Ԃ��擾����
			for( DWORD button = 0; button < JOYPAD_BUFFER_NUM; ++button ){
				dijs.rgbButtons[button] & 0x80 ?
					++m_ButtonInputLevel[i - m_JoyPadDeviceVector.begin()][button] :
					m_ButtonInputLevel[i - m_JoyPadDeviceVector.begin()][button] = 0;
			}
		}
	}

	return S_OK;
}

// ���͂��ꂽ�L�[�̎擾
int JoyPad::GetKey( const DWORD padNum, const ButtonArray::Button button ) const {
	if( m_ButtonInputLevel.empty() ){ return 0; }
	return m_ButtonInputLevel[padNum][button];
}
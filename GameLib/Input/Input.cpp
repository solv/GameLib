#include "Input.h"
#include "../Common.h"

//
// インプットクラス
//
// コンストラクタ
Input::Input():
	m_pDirectInput(NULL),
	m_KeyBoard( new KeyBoard() ),
	m_JoyPad( new JoyPad() )
{
}

// コピーコンストラクタ
Input::Input( const Input& obj ){
}

// デストラクタ
Input::~Input(){
	SAFE_RELEASE(m_pDirectInput);
}

// インスタンスの取得
Input& Input::Instance(){
	static Input instance;
	return instance;
}

// DirectInputの初期化
HRESULT Input::Initialize( HWND hWnd, HINSTANCE hInstance ){
	// デバイスの取得
	if( FAILED( ::DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_pDirectInput), NULL ) ) ){
		return E_FAIL;
	}

	// キーボードの初期化
	if( FAILED( m_KeyBoard->Initialize( m_pDirectInput, hWnd ) ) ){
		return E_FAIL;
	}
	if( FAILED( m_JoyPad->Initialize( m_pDirectInput, hWnd ) ) ){
		return E_FAIL;
	}
	return S_OK;
}

// キーボードデバイスの取得
KeyBoard* Input::GetKeyBoard() const {
	return m_KeyBoard.get();
}

// ジョイパッドデバイスの取得
JoyPad* Input::GetJoyPad() const {
	return m_JoyPad.get();
}

//
// キーボードクラス
//
// コンストラクタ
KeyBoard::KeyBoard():
	m_KeyBoardDevice(NULL)
{
	for( int i = 0; i < KEY_NUM; ++i ){
		m_KeyInputOnOffBuffer[i] = false;
		m_KeyInputLevelBuffer[i] = 0;
	}
}

// デストラクタ
KeyBoard::~KeyBoard(){
	if( m_KeyBoardDevice ){
		m_KeyBoardDevice->Unacquire();
	}
	SAFE_RELEASE(m_KeyBoardDevice);
}

// キーボードデバイスの初期化
HRESULT KeyBoard::Initialize( LPDIRECTINPUT8 pDirectInput, HWND hWnd){
	// キーボードデバイスの作成
	if( FAILED( pDirectInput->CreateDevice( GUID_SysKeyboard,&m_KeyBoardDevice, NULL ) ) ){
		return E_FAIL;
	}

	// 取得データフォーマットの設定
	if( FAILED( m_KeyBoardDevice->SetDataFormat(&c_dfDIKeyboard) ) ){
		return E_FAIL;
	}

	// 協調レベルの設定
	if( FAILED( m_KeyBoardDevice->SetCooperativeLevel(
		hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
	) ) ){
		return E_FAIL;
	}

	// バッファサイズの指定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize		= sizeof DIPROPDWORD;
	dipdw.diph.dwHeaderSize	= sizeof DIPROPHEADER;
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= DIMGR_BUFFER_SIZE;

	if( FAILED( m_KeyBoardDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) ){
		return E_FAIL;
	}

	// キーボードの入力を有効にする
	m_KeyBoardDevice->Acquire();

	return S_OK;
}

// キー入力の更新
HRESULT KeyBoard::Update(){
	// キーボードデバイスが無い場合は終了
	if( m_KeyBoardDevice == NULL ){
		return S_OK;
	}

	// デバイス情報の取得
	DWORD elements = KEY_NUM;
	HRESULT hr = m_KeyBoardDevice->GetDeviceData( sizeof DIDEVICEOBJECTDATA, m_DeviceObject, &elements, 0 );
	if( hr != DI_OK ){
		if( m_KeyBoardDevice->Acquire() != DI_OK ){
			return E_FAIL;
		}
		m_KeyBoardDevice->GetDeviceData( sizeof DIDEVICEOBJECTDATA, m_DeviceObject, &elements, 0 );
	}

	// 入力状況の取得
	for( DWORD i = 0; i < elements; ++i ){
		m_KeyInputOnOffBuffer[m_DeviceObject[i].dwOfs] = ( (m_DeviceObject[i].dwData & 0x80) >> 7 ) == 0 ? false : true;
	}

	for( DWORD i = 0; i < KEY_NUM; ++i ){
		m_KeyInputOnOffBuffer[i] == true ? ++m_KeyInputLevelBuffer[i] : m_KeyInputLevelBuffer[i] = 0;
	}

	return S_OK;
}

// キーの入力状況の取得
int KeyBoard::GetKey( const KeyArray::Key key ) const {
	return m_KeyInputLevelBuffer[key];
}

//
// ジョイパッドクラス
//

int JoyPad::m_JoyPadNum(0);

// コンストラクタ
JoyPad::JoyPad():
	m_JoyPadDevice(NULL)
{
	for( int i = 0; i < JOYPAD_BUTTON_NUM; ++i ){
		m_KeyInputOnOffBuffer[i] = false;
		m_KeyInputLevelBuffer[i] = 0;
	}
}

// デストラクタ
JoyPad::~JoyPad(){
	if( m_JoyPadDevice ){
		m_JoyPadDevice->Unacquire();
	}
	SAFE_RELEASE(m_JoyPadDevice);
}

// ジョイパッドデバイスの初期化
HRESULT JoyPad::Initialize( LPDIRECTINPUT8 pDirectInput, HWND hWnd ){
	// デバイスを渡す
	m_pDirectInput = pDirectInput;

	// ジョイパッドデバイスの作成
	if( FAILED( pDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoyPadCallback, static_cast<void*>(this), DIEDFL_ATTACHEDONLY ) ) ) {
		return E_FAIL;
	}

	// ジョイパッドは見つかった？
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

// ボタン入力の更新
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

// ジョイパッドのボタンの入力状況の取得
int JoyPad::GetKey( const ButtonArray::Button button ) const {
	return m_KeyInputLevelBuffer[button];
}

// ジョイパッドを検出するコールバック関数
BOOL CALLBACK JoyPad::EnumSizesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef ){
	// デバイスの設定
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
	// ジョイパッドデバイスの作成
	JoyPad* joypad = reinterpret_cast<JoyPad*>( lpContext );
	joypad->m_DeviceCaps.dwSize = sizeof DIDEVCAPS;
	joypad->m_pDirectInput->CreateDevice( lpddi->guidInstance, &joypad->m_JoyPadDevice, NULL );
	joypad->m_JoyPadDevice->GetCapabilities( &joypad->m_DeviceCaps );
	++m_JoyPadNum;

	return DIENUM_STOP;
}

// 接続されているジョイパッドの個数
int JoyPad::GetJoyPadNum(){
	return m_JoyPadNum;
}
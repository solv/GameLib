#include "../stdafx.h"
#include "Input.h"
#include "../Utility/Utility.h"
#include <algorithm>

//
// 入力管理クラス
//

// コンストラクタ
Input::Input():
	m_pDirectInput(NULL),
	m_Keyboard( boost::make_shared<Keyboard>() ),
	m_JoyPad( boost::make_shared<JoyPad>() )
{
}

// デストラクタ
Input::~Input(){
}

// インスタンスの取得
Input& Input::Instance(){
	static Input instance;
	return instance;
}

// DirectInputの初期化
HRESULT Input::Initialize( const HWND hWnd, const HINSTANCE hInstance ){
	ASSERT(hWnd);
	ASSERT(hInstance);

	// エラー処理用
	HRESULT hr;

	// DirectInputデバイスの取得
	hr = DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_pDirectInput), NULL );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// キーボードデバイスの初期化
	hr = m_Keyboard->Initialize( m_pDirectInput, hWnd );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// ジョイパッドデバイスの初期化
	hr = m_JoyPad->Initialize( m_pDirectInput, hWnd );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	return S_OK;
}

// キーボードデバイスの取得
boost::shared_ptr<Keyboard> Input::GetKeyboard() const {
	return m_Keyboard;
}

// キーボードデバイスの取得
boost::shared_ptr<JoyPad> Input::GetJoyPad() const {
	return m_JoyPad;
}



//
// キーボードデバイスクラス
//

// コンストラクタ
Keyboard::Keyboard():
	m_KeyboardDevice(NULL)
{
	std::fill( m_KeyInputLevel.begin(), m_KeyInputLevel.end(), 0 );
}

// デストラクタ
Keyboard::~Keyboard(){
	if( m_KeyboardDevice ){
		m_KeyboardDevice->Unacquire();
	}
}

// キーボードデバイスの初期化
HRESULT Keyboard::Initialize( const boost::intrusive_ptr<IDirectInput8> pDirectInput, const HWND hWnd ){
	ASSERT(pDirectInput);
	ASSERT(hWnd);

	// エラー処理用
	HRESULT hr;

	// キーボードデバイスの作成
	// intrusive_ptrがダブルポインタを取れないためtempオブジェクトを使用
	IDirectInputDevice8* tempKeyboardDevice = m_KeyboardDevice.get();
	hr = pDirectInput->CreateDevice( GUID_SysKeyboard, &tempKeyboardDevice, NULL );
	if( FAILED( hr ) ){
		return E_FAIL;
	}
	ASSERT(tempKeyboardDevice);
	boost::intrusive_ptr<IDirectInputDevice8> temp(tempKeyboardDevice);
	m_KeyboardDevice.swap(temp);

	// 取得データフォーマットの設定
	hr = m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// 協調レベルの設定
	hr = m_KeyboardDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// キーボードデバイスを有効にする
	m_KeyboardDevice->Acquire();

	return S_OK;
}

// 入力状態の更新
HRESULT Keyboard::Update(){
	// デバイスが見つからない時は帰る
	if( m_KeyboardDevice == NULL ){
		return S_OK;
	}
	ASSERT(m_KeyboardDevice);

	// デバイスの状態の情報を取得
	HRESULT hr;
	boost::array<BYTE, KEYBOARD_BUFFER_NUM> buffer = {};

	// 現在の状態を取得する
	hr = m_KeyboardDevice->GetDeviceState( buffer.size(), buffer.c_array() );
	if( hr != DI_OK ){ // デバイスがロストしていたら
		if( m_KeyboardDevice->Acquire() != DI_OK ){ // 再取得を試みる
			return E_FAIL;
		}
		hr = m_KeyboardDevice->GetDeviceState( buffer.size(), buffer.c_array() );
	}

	// 入力状況を更新する
	for( int i = 0; i < KEYBOARD_BUFFER_NUM; ++i ){
		buffer[i] & 0x80 ? ++m_KeyInputLevel[i] :  m_KeyInputLevel[i] = 0;
	}
	
	return S_OK;
}

// 入力されたキーの取得
int Keyboard::GetKey( const KeyArray::Key key ) const {
	if( m_KeyInputLevel.empty() ){ return 0; }
	return m_KeyInputLevel[key];
}



//
// ジョイパッドデバイスクラス
//

// コンストラクタ
JoyPad::JoyPad():
	m_pDirectInput(NULL)
{
	m_JoyPadDeviceVector.clear();
	m_ButtonInputLevel.clear();
}

// デストラクタ
JoyPad::~JoyPad(){
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		if(*i){
			(*i)->Unacquire();
		}
	}
}

// ジョイパッドを検出するためのコールバック関数
BOOL CALLBACK JoyPad::EnumJoyPadCallback( const DIDEVICEINSTANCE* lpddi, LPVOID lpContext ){
	// エラー処理用
	HRESULT hr;

	// thisポインタを受けとる
	JoyPad* joypad = reinterpret_cast<JoyPad*>( lpContext );
	
	// ジョイパッドデバイスを作成する
	IDirectInputDevice8* tempJoyPadDevice;
	hr = joypad->m_pDirectInput->CreateDevice( lpddi->guidInstance, &tempJoyPadDevice, NULL );
	if( FAILED( hr ) ){
		return DIENUM_CONTINUE;
	}

	ASSERT(tempJoyPadDevice);

	// 作成したデバイスを配列に追加する
	boost::intrusive_ptr<IDirectInputDevice8> temp( tempJoyPadDevice );
	joypad->m_JoyPadDeviceVector.push_back(temp);

	// 入力状況を管理する配列を追加する
	joypad->m_ButtonInputLevel.push_back( boost::array<int, JOYPAD_BUFFER_NUM>() );

	return DIENUM_CONTINUE;
}

BOOL CALLBACK JoyPad::EnumSizesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef ){
	// 現在しているジョイパッドのイテレータ
	JoyPad* joypad = reinterpret_cast<JoyPad*>( lpvRef );
	static std::vector<boost::intrusive_ptr<IDirectInputDevice8>>::iterator itr_pos = joypad->m_JoyPadDeviceVector.begin();
		
	
	// エラー処理用
	HRESULT hr;

	// 軸の範囲の情報を格納する構造体
	DIPROPRANGE diprg		= {};
	diprg.diph.dwSize		= sizeof DIPROPRANGE;
	diprg.diph.dwHeaderSize = sizeof DIPROPHEADER;
	diprg.diph.dwHow		= DIPH_BYID;
	diprg.diph.dwObj		= lpddoi->dwType;
	diprg.lMin				= -1000;
	diprg.lMax				= +1000;

	// 設定する
	hr = (*itr_pos)->SetProperty( DIPROP_RANGE, &diprg.diph );
	if( FAILED( hr ) ){ // 失敗したら
		return DIENUM_STOP;
	}
	
	if( (itr_pos - joypad->m_JoyPadDeviceVector.begin()) < (static_cast<int>(joypad->m_JoyPadDeviceVector.size()) - 1) ){
		++itr_pos; // 次のデバイスのポインタに移動する
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

// ジョイパッドデバイスの初期化
HRESULT JoyPad::Initialize( const boost::intrusive_ptr<IDirectInput8> pDirectInput, const HWND hWnd ){
	ASSERT(pDirectInput);
	ASSERT(hWnd);
	
	// エラー処理用
	HRESULT hr;

	// デバイスをメンバ変数に渡す
	m_pDirectInput = pDirectInput;

	ASSERT(m_pDirectInput);

	// ジョイパッドデバイスを列挙し作成する
	hr = m_pDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoyPadCallback, static_cast<LPVOID>(this), DIEDFL_ATTACHEDONLY );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// ジョイパッドが見つからない場合はキーボードのみとなるのでここで終了する
	if( m_JoyPadDeviceVector.empty() ){
		return S_OK;
	}

	// データ形式の設定
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		hr = (*i)->SetDataFormat( &c_dfDIJoystick );
		if( FAILED( hr ) ){
			return E_FAIL;
		}
	}

	// 協調レベルの設定
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		hr = (*i)->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND );
		if( FAILED( hr ) ){
			return E_FAIL;
		}
	}

	// 軸の範囲の設定
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		hr = (*i)->EnumObjects( EnumSizesCallback, static_cast<void*>(this), DIDFT_AXIS );
		if( FAILED( hr ) ){
			return E_FAIL;
		}
	}

	// 軸モードの設定
	DIPROPDWORD diprop		 = {};
	diprop.diph.dwSize		 = sizeof diprop;
	diprop.diph.dwHeaderSize = sizeof diprop.diph;
	diprop.diph.dwObj		 = 0;
	diprop.diph.dwHow		 = DIPH_DEVICE;
	diprop.dwData			 = DIPROPAXISMODE_ABS; // 絶対値モード

	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		hr = (*i)->SetProperty( DIPROP_AXISMODE, &diprop.diph );
		if( FAILED( hr ) ){
			return E_FAIL;
		}
	}

	// ジョイパッドを有効にする
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		(*i)->Acquire();
	}

	return S_OK;
}

// 入力状態の更新
HRESULT JoyPad::Update(){
	// エラー処理用
	HRESULT hr;

	// デバイスの個数分取得する
	for( auto i = m_JoyPadDeviceVector.begin(); i != m_JoyPadDeviceVector.end(); ++i ){
		// デバイスが存在しない場合は帰る
		if( m_JoyPadDeviceVector.empty() ){
			return S_OK;
		}

		// デバイスをポーリングする
		hr = (*i)->Poll();
		if( FAILED( hr ) ){
			hr = (*i)->Acquire();
			while( hr == DIERR_INPUTLOST ){
				hr = (*i)->Acquire();
			}
		}

		// 現在の入力状態を取得する
		DIJOYSTATE dijs;
		
		hr = (*i)->GetDeviceState( sizeof DIJOYSTATE, &dijs );
		if( hr == DI_OK ){
			// それぞれのボタンの状態を取得する
			for( DWORD button = 0; button < JOYPAD_BUFFER_NUM; ++button ){
				dijs.rgbButtons[button] & 0x80 ?
					++m_ButtonInputLevel[i - m_JoyPadDeviceVector.begin()][button] :
					m_ButtonInputLevel[i - m_JoyPadDeviceVector.begin()][button] = 0;
			}
		}
	}

	return S_OK;
}

// 入力されたキーの取得
int JoyPad::GetKey( const DWORD padNum, const ButtonArray::Button button ) const {
	if( m_ButtonInputLevel.empty() ){ return 0; }
	return m_ButtonInputLevel[padNum][button];
}
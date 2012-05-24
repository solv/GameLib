#include "VirtualInput.h"

// コンストラクタ
VirtualInput::VirtualInput(){
	m_Button[VButtonArray::UP]		= KeyArray::UP		| (KeyBoard << 8);
	m_Button[VButtonArray::DOWN]	= KeyArray::DOWN	| (KeyBoard << 8);
	m_Button[VButtonArray::LEFT]	= KeyArray::LEFT	| (KeyBoard << 8);
	m_Button[VButtonArray::RIGHT]	= KeyArray::RIGHT	| (KeyBoard << 8);
	m_Button[VButtonArray::BUTTON1] = KeyArray::Z		| (KeyBoard << 8);
	m_Button[VButtonArray::BUTTON2] = KeyArray::X		| (KeyBoard << 8);
	m_Button[VButtonArray::BUTTON3] = KeyArray::C		| (KeyBoard << 8);
	m_Button[VButtonArray::BUTTON4] = KeyArray::ENTER	| (KeyBoard << 8);

	//m_Button[VButtonArray::UP]	= ButtonArray::BUTTON13	| (JoyPad << 8);
	//m_Button[VButtonArray::DOWN]	= ButtonArray::BUTTON15	| (JoyPad << 8);
	//m_Button[VButtonArray::LEFT]	= ButtonArray::BUTTON16	| (JoyPad << 8);
	//m_Button[VButtonArray::RIGHT]	= ButtonArray::BUTTON14	| (JoyPad << 8);
	//m_Button[VButtonArray::BUTTON1] = ButtonArray::BUTTON6| (JoyPad << 8);
	//m_Button[VButtonArray::BUTTON2] = ButtonArray::BUTTON3| (JoyPad << 8);
	//m_Button[VButtonArray::BUTTON3] =ButtonArray::BUTTON2	| (JoyPad << 8);
}


// デストラクタ
VirtualInput::~VirtualInput(){
}

// インスタンスの取得
VirtualInput& VirtualInput::Instance(){
	static VirtualInput instance;
	return instance;
}
// デバイスの初期化をInputクラスに任せる
HRESULT VirtualInput::Initialize( HWND hWnd, HINSTANCE hInstance ){
	return Input::Instance().Initialize( hWnd, hInstance );
}

// 入力状態の更新
HRESULT VirtualInput::Update(){
	if( FAILED( Input::Instance().GetKeyBoard()->Update() ) ){
		return E_FAIL;
	}
	if( FAILED( Input::Instance().GetJoyPad()->Update() ) ){
		return E_FAIL;
	}

	return S_OK;
}

// 入力ボタンの取得
int VirtualInput::GetKey( const VButtonArray::Button button ) const {
	// キーボードかジョイパッドの分岐とボタン番号
	UINT inputFlag	= m_Button[button] >> 8;
	UINT value		= m_Button[button] & 0xFF;

	if( inputFlag == 0 ){
		return Input::Instance().GetKeyBoard()->GetKey( static_cast<KeyArray::Key>(value) );
	}else if( inputFlag == 1 ){
		return Input::Instance().GetJoyPad()->GetKey( static_cast<ButtonArray::Button>(value) );
	}

	return -1;
}

// 入力ボタンの変更
void VirtualInput::ChangeButton( VButtonArray::Button oldButton, const KeyArray::Key newKey ){
	m_Button[oldButton] = newKey | (KeyBoard << 8);
}
void VirtualInput::ChangeButton( VButtonArray::Button oldButton, const ButtonArray::Button newButton ){
	m_Button[oldButton] = newButton | (JoyPad << 8);
}

// 現在入力されているボタン番号
int VirtualInput::GetInputButton() const {
	for( int i = 0; i < KeyBoard::KEY_NUM; ++i ){
		if( Input::Instance().GetKeyBoard()->GetKey( static_cast<KeyArray::Key>(i) ) == 1 ){
			return i | (KeyBoard << 8);
		}
	}
	for( int i = 0; i < JoyPad::JOYPAD_BUTTON_NUM; ++i ){
		if( Input::Instance().GetJoyPad()->GetKey( static_cast<ButtonArray::Button>(i) ) == 1 ){
			return i | (JoyPad << 8);
		}
	}
	return -1;
}
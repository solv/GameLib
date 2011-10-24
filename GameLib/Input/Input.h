#pragma once

#include "../stdafx.h"
#include "KeyArray.h"
#include "ButtonArray.h"

// 前方宣言
class Keyboard;
class JoyPad;

//
// 入力管理クラス
//
class Input : private boost::noncopyable
{
private:
	boost::intrusive_ptr<IDirectInput8> m_pDirectInput;	// DirectInputデバイス
	boost::shared_ptr<Keyboard> m_Keyboard;				// キーボードを管理するデバイス
	boost::shared_ptr<JoyPad> m_JoyPad;					// ジョイパッドを管理するデバイス

private:
	// コンストラクタ
	Input();
	// デストラクタ
	~Input();

public:
	// インスタンスの取得
	static Input& Instance();

	// DirectInputの初期化
	HRESULT Initialize( const HWND, const HINSTANCE );

	// キーボードデバイスの取得
	boost::shared_ptr<Keyboard> GetKeyboard() const;

	// キーボードデバイスの取得
	boost::shared_ptr<JoyPad> GetJoyPad() const;
};


//
// キーボードデバイスクラス
//
class Keyboard : private boost::noncopyable
{
private:
	static const int KEYBOARD_BUFFER_NUM = 256;					// キーボードのキー配列の個数
	boost::intrusive_ptr<IDirectInputDevice8> m_KeyboardDevice;	// キーボードデバイス
	boost::array<int, KEYBOARD_BUFFER_NUM> m_KeyInputLevel;		// 入力状況を管理する配列

public:
	// コンストラクタ
	Keyboard();

	// デストラクタ
	~Keyboard();

	// キーボードデバイスの初期化
	HRESULT Initialize( const boost::intrusive_ptr<IDirectInput8>, const HWND );

	// 入力状態の更新
	HRESULT Update();

	// 入力されたキーの取得
	int GetKey( const KeyArray::Key ) const;
};


//
// ジョイパッドデバイスクラス
//
class JoyPad : private boost::noncopyable
{
private:
	static const int JOYPAD_BUFFER_NUM = 32;										// ジョイパッドのキー配列の個数

	boost::intrusive_ptr<IDirectInput8>						m_pDirectInput;			// DirectInputデバイス
	std::vector<boost::array<int, JOYPAD_BUFFER_NUM>>		m_ButtonInputLevel;		// 入力状況を管理する配列
	std::vector<boost::intrusive_ptr<IDirectInputDevice8>>	m_JoyPadDeviceVector;	// ジョイパッドデバイス(可変長)
	
private:
	// ジョイパッドを検出するためのコールバック関数
	static BOOL CALLBACK EnumJoyPadCallback( const DIDEVICEINSTANCE* , LPVOID );
	static BOOL CALLBACK EnumSizesCallback( LPCDIDEVICEOBJECTINSTANCE, LPVOID );

public:
	// コンストラクタ
	JoyPad();

	// デストラクタ
	~JoyPad();

	// ジョイパッドデバイスの初期化
	HRESULT Initialize( const boost::intrusive_ptr<IDirectInput8>, const HWND );

	// 入力状態の更新
	HRESULT Update();

	// 入力されたキーの取得
	int GetKey( const DWORD padNum, const ButtonArray::Button ) const;
};
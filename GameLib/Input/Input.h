#pragma once

#define DIRECTINPUT_VERSION		DIRECTINPUT_HEADER_VERSION
#include <dinput.h>
#include <boost/shared_ptr.hpp>
#include "KeyArray.h"
#include "ButtonArray.h"

// 前方宣言
class KeyBoard;
class JoyPad;

class Input
{
private:
	LPDIRECTINPUT8 m_pDirectInput;

private:
	// コンストラクタ
	Input();
	// コピーコンストラクタ
	Input( const Input& );
	// デストラクタ
	~Input();

private:
	boost::shared_ptr<KeyBoard> m_KeyBoard;
	boost::shared_ptr<JoyPad> m_JoyPad;
	//boost::Shared_ptr<Mouse> m_Mouse;

public:
	// インスタンスの取得
	static Input& Instance();

	// DirectInputの初期化
	HRESULT Initialize( HWND hWnd, HINSTANCE hInstance );

	// キーボードデバイスの取得
	KeyBoard* GetKeyBoard() const;

	// ジョイパッドデバイスの取得
	JoyPad* GetJoyPad() const;
};

class KeyBoard
{
public:
	static const int KEY_NUM = 256;// キーボードのキー数

private:
	static const int DIMGR_BUFFER_SIZE = 256; // バッファーサイズ

	bool m_KeyInputOnOffBuffer[KEY_NUM]; // キーの入力の有無
	int m_KeyInputLevelBuffer[KEY_NUM];	 // キーの入力レベル

	LPDIRECTINPUTDEVICE8	m_KeyBoardDevice; // キーボード用のデバイス
	DIDEVICEOBJECTDATA		m_DeviceObject[DIMGR_BUFFER_SIZE];

public:
	// コンストラクタ
	KeyBoard();

	// デストラクタ
	~KeyBoard();

	// キーボードの初期化
	HRESULT Initialize( LPDIRECTINPUT8 pDirectInput, HWND hWnd  );

	// 更新
	HRESULT Update();

	// 入力キーの取得
	int GetKey( const KeyArray::Key key) const;
};

class JoyPad
{
public:
	static const int JOYPAD_BUTTON_NUM	= 16;	// ジョイパッドのボタンの数

private:
	static const int DIMGR_BUFFER_SIZE = 256;	// バッファーサイズ
	static int m_JoyPadNum; // 接続されているジョイパッドの個数

	bool m_KeyInputOnOffBuffer[JOYPAD_BUTTON_NUM]; // キーの入力の有無
	int  m_KeyInputLevelBuffer[JOYPAD_BUTTON_NUM];	 // キーの入力レベル

	LPDIRECTINPUTDEVICE8	m_JoyPadDevice; // ジョイパッド用のデバイス
	DIDEVICEOBJECTDATA		m_DeviceObject[JOYPAD_BUTTON_NUM];
	DIDEVCAPS				m_DeviceCaps;
	LPDIRECTINPUT8			m_pDirectInput; // DirectInputのデバイス

private:
	// ジョイパッド検出のコールバック関数
	static BOOL CALLBACK EnumSizesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef );
	static BOOL CALLBACK EnumJoyPadCallback( const DIDEVICEINSTANCE* lpddi, LPVOID lpContext );

public:
	// コンストラクタ
	JoyPad();

	// デストラクタ
	~JoyPad();

	// ジョイパッドの初期化
	HRESULT Initialize( LPDIRECTINPUT8 pDirectInput, HWND hWnd );

	// 更新
	HRESULT Update();

	// 入力ボタンの取得
	int GetKey( const ButtonArray::Button button ) const;

	// 接続されているジョイパッドの個数
	static int GetJoyPadNum();
};
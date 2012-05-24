#pragma once

#include "Input.h"
#include "VirtualButtonArray.h"
#include <boost/noncopyable.hpp>

class VirtualInput : private boost::noncopyable
{
private:
	// コンストラクタ
	VirtualInput();
	// デストラクタ
	~VirtualInput();

public:
	static const int KeyBoard = 0;
	static const int JoyPad	  = 1;
	static const int VIRTUAL_BUTTON_MAX = 8;
	UINT m_Button[VIRTUAL_BUTTON_MAX];

public:
	// インスタンスの取得
	static VirtualInput& Instance();

	// デバイスの初期化をInputクラスに任せる
	HRESULT Initialize( HWND hWnd, HINSTANCE hInstance );

	// 入力状態の更新
	HRESULT Update();

	// 入力ボタンの取得
	int GetKey( const VButtonArray::Button button ) const;

	// ボタンの変更
	void ChangeButton( VButtonArray::Button oldButton, const KeyArray::Key newKey );
	void ChangeButton( VButtonArray::Button oldButton, const ButtonArray::Button newButton );

	// 現在入力されているボタン番号
	int GetInputButton() const;
};
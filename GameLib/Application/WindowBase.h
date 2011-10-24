#pragma once

#include "../stdafx.h"

//
// ウインドウを管理するクラス
//
class WindowBase : private boost::noncopyable
{
private:
	HWND		m_hWnd;			// ウインドウハンドル		
	HINSTANCE	m_hInstance;	// アプリケーションのハンドル
	bool m_FullScreen;			// フルスクリーンモードの有無
	bool m_Active;				// ウインドウのアクティブの有無
	int m_Width;				// ウインドウ横サイズ
	int m_Height;				// ウインドウ縦サイズ

public:
	// コンストラクタ
	WindowBase( HINSTANCE );

	// GetWindowLongにthisポインタを設定する
	void SetPointer( HWND );

	// ウインドウプロシージャを呼び出す
	static LRESULT CALLBACK LocalWindowProc( HWND, UINT, WPARAM, LPARAM );

	// ウインドウプロシージャの本体
	LRESULT WindowProc( HWND, UINT, WPARAM, LPARAM );

	// ウインドウの初期化
	HRESULT InitializeWindow( int, int, bool, LPCTSTR, WindowBase* );

	// HWNDの取得
	HWND GetHWND() const;

	// HINSNTANCEの取得
	HINSTANCE GetHINSTANCE() const;
	
	//  フルスクリーンの有無の取得
	bool GetFullScreen() const;

	// 縦サイズの取得
	int GetWidth() const;

	// 横サイズの取得
	int GetHeight() const;
};
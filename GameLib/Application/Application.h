#pragma once

#include "../stdafx.h"
#include "WindowBase.h"

//
// アプリケーションを管理するクラス
//
class Application : private boost::noncopyable
{
private:
	// ウインドウクラスを管理するクラス
	boost::shared_ptr<WindowBase> m_spWindowBase;

public:
	// コンストラクタ
	Application( const HINSTANCE );

	// 初期化
	HRESULT Initialize( const int, const int, const bool, const LPCTSTR );

	// メッセージループ
	WPARAM Run();
};
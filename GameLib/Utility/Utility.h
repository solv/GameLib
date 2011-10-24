//
// ユーティリティファイル
//

#pragma once

#include "../stdafx.h"
#include <crtdbg.h>

// 手を加えたSAFE_DELETE系マクロ
#define SAFE_DELETE(p)			{ delete (p); (p) = NULL; }
#define SAFE_DELETE_ARRAY(p)	{ delete [] (p); (p) = NULL; }
#define SAFE_RELEASE(p)			{ if((p)){ p->Release(); } }

// デバッグコンソールに文字列を出力します(可変長引数で指定可)
#ifdef _DEBUG
	#define MyOutputDebugString( str, ... ) \
			{ \
				TCHAR c[1024]; \
				_stprintf_s( c, _countof(c), str, __VA_ARGS__ ); \
				OutputDebugString( c ); \
			}
#else
	#define MyOutputDebugString( str, ... ) // 空実装
#endif

// メモリリークを検知するnew演算子
#ifndef _DEBUG
	#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define NEW  new
#endif

// メモリリークを検出する
// プログラムの一番最後に追加してください
#ifdef _DEBUG
	#define MemoryLeaksChecker _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#else
	#define MemoryLeaksChecker
#endif

// ASSERTマクロ
// ユーザーメッセージ無しと有りの2種類
#ifdef _DEBUG
	#define ASSERT(expr) \
			(void) ((!!(expr)) || \
			(1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, NULL)) || \
			(_CrtDbgBreak(), 0))

	#define ASSERT_MSG(expr,msg) \
			(void) ((!!(expr)) || \
			(1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, L##msg)) || \
			(_CrtDbgBreak(), 0))
#else
	#define ASSERT(expr)
	#define ASSERT_MSG(expr,msg)
#endif

// boost::intrusive_ptrのためのインライン関数
// 参照カウントを増加させる
inline void intrusive_ptr_add_ref( IUnknown* p ){
	ASSERT(p);
	p->AddRef();
}

// 参照カウントを減少させる
inline void intrusive_ptr_release( IUnknown* p ){
	ASSERT(p);
	p->Release();
}
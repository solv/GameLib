#include "GameLib/Library.h"

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// メモリリークのチェック
	MemoryLeaksChecker;
	
	// アプリケーションの初期化
	CApplication ca( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
	if( FAILED( ca.Initialize() ) ){
		return -1;
	}

	// アプリケーショの実行
	ca.Run();

	return 0;
}
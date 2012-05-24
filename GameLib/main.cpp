#include "GameLib.h"

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// メモリリークのチェック
	MemoryLeaksChecker;
	
	// アプリケーションの初期化
	Application ca( hInstance );
	if( FAILED( ca.Initialize(640,480,false,_T("GameLib")) ) ){
		return -1;
	}

	// アプリケーショの実行
	ca.Run();

	return 0;
}
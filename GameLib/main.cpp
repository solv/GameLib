#include "GameLib.h"

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// ���������[�N�̃`�F�b�N
	MemoryLeaksChecker;
	
	// �A�v���P�[�V�����̏�����
	Application ca( hInstance );
	if( FAILED( ca.Initialize(640,480,false,_T("GameLib")) ) ){
		return -1;
	}

	// �A�v���P�[�V���̎��s
	ca.Run();

	return 0;
}
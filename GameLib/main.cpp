#include "GameLib/Library.h"

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// ���������[�N�̃`�F�b�N
	MemoryLeaksChecker;
	
	// �A�v���P�[�V�����̏�����
	CApplication ca( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
	if( FAILED( ca.Initialize() ) ){
		return -1;
	}

	// �A�v���P�[�V���̎��s
	ca.Run();

	return 0;
}
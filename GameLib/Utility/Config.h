//---------------------------------------------------------
// SolvLib�̃f�o�b�O�@�\�̃R���t�B�O���j���[
//---------------------------------------------------------

#pragma once 

//�S�Ẵf�o�b�O���ڂ𖳌��ɂ���
//�S�Ă̍��ڂ��D�悳���
//#define _DEBUG_NOTHING_

//�S�Ẵf�o�b�O���ڂ�L���ɂ���
//_DEBUG_NOTHING_�̎��ɗD�悳���
#define _DEBUG_ALL_

//���������[�N���oNEW
#define _CRTDBG_MAP_ALLOC_

//���������[�N���o��
#define _MEMORY_LEAKS_CHECKER_

//�S�Ẵf�o�b�O���ڂ�L���ɂ���
#ifdef _DEBUG_ALL_
	#define _CRTDBG_MAP_ALLOC_
	#define _MEMORY_LEAKS_CHECKER_
#endif /* _DEBUG_ALL */

//�S�Ẵf�o�b�O���ڂ𖳌��ɂ���
#ifdef _DEBUG_NOTHING_
	#undef _CRTDBG_MAP_ALLOC_
	#undef _MEMORY_LEAKS_CHECKER_
#endif /* _DEBUG_NOTHING_ */

//�f�o�b�J�ǂݍ���
#include "Utility.h"
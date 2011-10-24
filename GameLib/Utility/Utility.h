//
// ���[�e�B���e�B�t�@�C��
//

#pragma once

#include "stdafx.h"
#include <crtdbg.h>

// ���������SAFE_DELETE�n�}�N��
#define SAFE_DELETE(p)			{ delete (p); (p) = NULL; }
#define SAFE_DELETE_ARRAY(p)	{ delete [] (p); (p) = NULL; }
#define SAFE_RELEASE(p)			{ if((p)){ p->Release(); } }

// �f�o�b�O�R���\�[���ɕ�������o�͂��܂�(�ϒ������Ŏw���)
#ifdef _DEBUG
	#define MyOutputDebugString( str, ... ) \
			{ \
				TCHAR c[1024]; \
				_stprintf_s( c, _countof(c), str, __VA_ARGS__ ); \
				OutputDebugString( c ); \
			}
#else
	#define MyOutputDebugString( str, ... ) // �����
#endif

// ���������[�N�����m����new���Z�q
#ifndef _DEBUG
	#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define NEW  new
#endif

// ���������[�N�����o����
// �v���O�����̈�ԍŌ�ɒǉ����Ă�������
#ifdef _DEBUG
	#define MemoryLeaksChecker _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#else
	#define MemoryLeaksChecker
#endif

// ASSERT�}�N��
// ���[�U�[���b�Z�[�W�����ƗL���2���
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

// boost::intrusive_ptr�̂��߂̃C�����C���֐�
// �Q�ƃJ�E���g�𑝉�������
inline void intrusive_ptr_add_ref( IUnknown* p ){
	ASSERT(p);
	p->AddRef();
}

// �Q�ƃJ�E���g������������
inline void intrusive_ptr_release( IUnknown* p ){
	ASSERT(p);
	p->Release();
}
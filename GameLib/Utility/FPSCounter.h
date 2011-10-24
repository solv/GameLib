#pragma once

#include "../stdafx.h"
#include <boost/circular_buffer.hpp>
//
// FPS����N���X
//
class FPSCounter
{
private:
	enum {
		FPSCOUNTER_QUERY_PER_COUNTER = 0,	// QueryPerformanceCounter
		FPSCOUNTER_TIMEGETTIME		 = 1	// timeGetTime
	};

	int m_CounterFlag;					// �g�p����v���֐��̃t���O
	LARGE_INTEGER m_Counter;			// �N���b�N�J�E���g��
	double m_Freq;						// 1�b������̃N���b�N�J�E���g��
	LONGLONG m_OldCount;				// �ȑO�̃N���b�N�J�E���g��
	DWORD m_TGTOldCount;				// �ȑO�̎���(�~���b)
	boost::circular_buffer<double> m_DefTimeList;	// ���ԃ��X�g
	UINT m_Num;							// �ړ����όv�Z�̃f�[�^��
	double m_SumTimes;					// ���ʕ����̍��v

private:
	// ���݂̎����̎擾
	double GetCurDefTime();
	// FPS�l�̍X�V
	double UpdateFPS( const double def );

public:
	// �R���X�g���N�^(�f�t�H���g�T���v������10)
	FPSCounter( const UINT sample = 10 );
	// �f�X�g���N�^
	~FPSCounter();

	// FPS�l�̎擾
	double GetFPS();

	// �T���v�����̐ݒ�
	void SetSampleNum( const UINT sample = 10 );
};

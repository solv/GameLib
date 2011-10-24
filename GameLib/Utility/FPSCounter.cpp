#include "FPSCounter.h"

// �R���X�g���N�^(�f�t�H���g�T���v������10)
FPSCounter::FPSCounter( const UINT sample ){
	// �T���v�����̐ݒ�
	SetSampleNum(sample);

	// �v���֐��̐ݒ�
	if( QueryPerformanceCounter( &m_Counter ) != 0 ){
		m_CounterFlag = FPSCOUNTER_QUERY_PER_COUNTER;	// QueryPerformanceCounter���g�p����
		m_OldCount = m_Counter.QuadPart;				// �������̎���(�N���b�N���̎擾)
		LARGE_INTEGER Freq;
		QueryPerformanceFrequency( &Freq );				// 1�b������̃N���b�N���̎擾
		m_Freq = static_cast<double>( Freq.QuadPart );
	}else{
		m_CounterFlag = FPSCOUNTER_TIMEGETTIME;	// timeGetTime���g�p����
		timeBeginPeriod(1);						// ���x���グ��
		m_TGTOldCount = timeGetTime();			// �������̎���(�~���b�̎擾)
	}

	// ����
	GetFPS();
}

// �f�X�g���N�^
FPSCounter::~FPSCounter(){
	if( m_CounterFlag == FPSCOUNTER_TIMEGETTIME ){
		timeEndPeriod(1);	// ���x��߂�
	}
}

// FPS�l�̎擾
double FPSCounter::GetFPS(){
	double def = GetCurDefTime();
	if( def == 0 ){
		return 0;
	}

	return UpdateFPS(def);
}

// ���݂̎����̎擾
double FPSCounter::GetCurDefTime(){
	if( m_CounterFlag == FPSCOUNTER_QUERY_PER_COUNTER ){
		QueryPerformanceCounter( &m_Counter );				// ���ݎ����̎擾
		LONGLONG longDef = m_Counter.QuadPart - m_OldCount;	// ���������̎Z�o
		double def = static_cast<double>( longDef );		// �{���x���������_�ɕϊ�
		m_OldCount = m_Counter.QuadPart;					// �ȑO�̎����̕ێ�
		return def * 1000 / m_Freq;
	}else{
		// timeGetTime�֐��ɂ��v��
		DWORD curTime = timeGetTime();
		DWORD curDef = curTime - m_TGTOldCount;	// ���������̎Z�o
		m_TGTOldCount = curTime;				// ���ݎ����̕ێ�
		return curDef;
	}
}

// FPS�l�̍X�V
double FPSCounter::UpdateFPS( const double def ){
	m_DefTimeList.push_back( def );	// �V�����f�[�^���擾

	// FPS�̌v�Z
	double FPS = 0.0;
	double aveDef = ( m_SumTimes + def ) / m_Num;
	if( aveDef != 0.0 ){
		FPS = 1000.0 / aveDef;
	}

	// ���ʉ��Z�����̍X�V
	m_SumTimes += def - *m_DefTimeList.begin();

	return FPS;
}

// �T���v�����̐ݒ�
void FPSCounter::SetSampleNum( const UINT sample ){
	m_Num = sample;						// ���ς��v�Z�����
	m_DefTimeList.resize( m_Num, 0.0 );	// ���X�g�̏�����
	m_SumTimes = 0;
}
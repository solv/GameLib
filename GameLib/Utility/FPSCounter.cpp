#include "FPSCounter.h"

// コンストラクタ(デフォルトサンプル数は10)
FPSCounter::FPSCounter( const UINT sample ){
	// サンプル数の設定
	SetSampleNum(sample);

	// 計測関数の設定
	if( QueryPerformanceCounter( &m_Counter ) != 0 ){
		m_CounterFlag = FPSCOUNTER_QUERY_PER_COUNTER;	// QueryPerformanceCounterを使用する
		m_OldCount = m_Counter.QuadPart;				// 生成時の時刻(クロック数の取得)
		LARGE_INTEGER Freq;
		QueryPerformanceFrequency( &Freq );				// 1秒あたりのクロック数の取得
		m_Freq = static_cast<double>( Freq.QuadPart );
	}else{
		m_CounterFlag = FPSCOUNTER_TIMEGETTIME;	// timeGetTimeを使用する
		timeBeginPeriod(1);						// 精度を上げる
		m_TGTOldCount = timeGetTime();			// 生成時の時刻(ミリ秒の取得)
	}

	// 測定
	GetFPS();
}

// デストラクタ
FPSCounter::~FPSCounter(){
	if( m_CounterFlag == FPSCOUNTER_TIMEGETTIME ){
		timeEndPeriod(1);	// 精度を戻す
	}
}

// FPS値の取得
double FPSCounter::GetFPS(){
	double def = GetCurDefTime();
	if( def == 0 ){
		return 0;
	}

	return UpdateFPS(def);
}

// 現在の時刻の取得
double FPSCounter::GetCurDefTime(){
	if( m_CounterFlag == FPSCOUNTER_QUERY_PER_COUNTER ){
		QueryPerformanceCounter( &m_Counter );				// 現在時刻の取得
		LONGLONG longDef = m_Counter.QuadPart - m_OldCount;	// 差分時刻の算出
		double def = static_cast<double>( longDef );		// 倍精度浮動小数点に変換
		m_OldCount = m_Counter.QuadPart;					// 以前の時刻の保持
		return def * 1000 / m_Freq;
	}else{
		// timeGetTime関数による計測
		DWORD curTime = timeGetTime();
		DWORD curDef = curTime - m_TGTOldCount;	// 差分時刻の算出
		m_TGTOldCount = curTime;				// 現在時刻の保持
		return curDef;
	}
}

// FPS値の更新
double FPSCounter::UpdateFPS( const double def ){
	m_DefTimeList.push_back( def );	// 新しいデータを取得

	// FPSの計算
	double FPS = 0.0;
	double aveDef = ( m_SumTimes + def ) / m_Num;
	if( aveDef != 0.0 ){
		FPS = 1000.0 / aveDef;
	}

	// 共通加算部分の更新
	m_SumTimes += def - *m_DefTimeList.begin();

	return FPS;
}

// サンプル数の設定
void FPSCounter::SetSampleNum( const UINT sample ){
	m_Num = sample;						// 平均を計算する個数
	m_DefTimeList.resize( m_Num, 0.0 );	// リストの初期化
	m_SumTimes = 0;
}
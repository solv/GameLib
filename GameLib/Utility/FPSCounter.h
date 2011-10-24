#pragma once

#include "../stdafx.h"
#include <boost/circular_buffer.hpp>
//
// FPS測定クラス
//
class FPSCounter
{
private:
	enum {
		FPSCOUNTER_QUERY_PER_COUNTER = 0,	// QueryPerformanceCounter
		FPSCOUNTER_TIMEGETTIME		 = 1	// timeGetTime
	};

	int m_CounterFlag;					// 使用する計測関数のフラグ
	LARGE_INTEGER m_Counter;			// クロックカウント数
	double m_Freq;						// 1秒あたりのクロックカウント数
	LONGLONG m_OldCount;				// 以前のクロックカウント数
	DWORD m_TGTOldCount;				// 以前の時刻(ミリ秒)
	boost::circular_buffer<double> m_DefTimeList;	// 時間リスト
	UINT m_Num;							// 移動平均計算のデータ数
	double m_SumTimes;					// 共通部分の合計

private:
	// 現在の時刻の取得
	double GetCurDefTime();
	// FPS値の更新
	double UpdateFPS( const double def );

public:
	// コンストラクタ(デフォルトサンプル数は10)
	FPSCounter( const UINT sample = 10 );
	// デストラクタ
	~FPSCounter();

	// FPS値の取得
	double GetFPS();

	// サンプル数の設定
	void SetSampleNum( const UINT sample = 10 );
};

#include "Audio.h"

//
// XAudio2のデバイスを管理するクラス
//

// コンストラクタ
IXAudioDevice::IXAudioDevice():
	m_pXAudio2(NULL),
	m_pMasteringVoice(NULL)
{
}

// デストラクタ
IXAudioDevice::~IXAudioDevice(){
	// COMポインタ類の後始末
	if( m_pMasteringVoice ){
		m_pMasteringVoice->DestroyVoice();
	}
	if( m_pXAudio2 ){
		m_pXAudio2->Release();
	}
	CoUninitialize();
}

// インスタンスの取得
IXAudioDevice& IXAudioDevice::Instance(){
	static IXAudioDevice instance;
	return instance;
}

// 初期化
HRESULT IXAudioDevice::Initialize(){
	// エラー処理用
	HRESULT hr;

	// COMの初期化
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	// IXAudio2の初期化
	UINT32 flags = 0;
	
	#ifdef _DEBUG
		flags |= XAUDIO2_DEBUG_ENGINE;
	#endif

	hr = XAudio2Create( &m_pXAudio2, flags );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// IXAudio2MasteringVoiceの初期化
	hr = m_pXAudio2->CreateMasteringVoice( &m_pMasteringVoice );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	return S_OK;
}

// IXAudio2の取得
IXAudio2*  IXAudioDevice::GetXAudio2(){
	return m_pXAudio2;
}

// IXAudio2MasteringVoiceの取得
IXAudio2MasteringVoice* IXAudioDevice::GetMasteringVoice(){
	return m_pMasteringVoice;
}


//
// BGMを管理するクラス
//

// コンストラクタ
Audio::Audio():
	m_pSourceVoice(NULL)
{
}

// デストラクタ
Audio::~Audio(){
	// 停止させてから解放する
	m_pSourceVoice->Stop();
	m_pSourceVoice->DestroyVoice();
}

// BGMの読み込み(ogg)
HRESULT Audio::LoadBGM( LPSTR fileName ){
	// oggファイル
	OggVorbis_File ovf;

	// エラー処理用
	HRESULT hr;

	// ファイルを開く
	if( ov_fopen( fileName, &ovf ) != 0 ){
		return E_FAIL;
	}

	// oggファイルの音声フォーマットを取得
	vorbis_info* oggInfo = ov_info(&ovf, -1);

	// Waveファイルの読み込み
	WAVEFORMATEX format		= {};
	format.wFormatTag		= WAVE_FORMAT_PCM;
	format.nChannels		= static_cast<WORD>(oggInfo->channels);
	format.wBitsPerSample	= 16;
	format.nSamplesPerSec	= oggInfo->rate;
	format.nBlockAlign		= format.wBitsPerSample / 8 * format.nChannels;
    format.nAvgBytesPerSec	= format.nSamplesPerSec * format.nBlockAlign;

	// IXAudio2SourceVoiceの初期化
	hr = IXAudioDevice::Instance().GetXAudio2()->CreateSourceVoice( &m_pSourceVoice, &format );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// ファイルの終端まで読み込む
	std::vector<char> tempm_WaveData(4096,0);
	int readSize = 0;
	int comSize = 0;

	while(1){
		// PCMデータ読み込み
		readSize = ov_read( &ovf, &tempm_WaveData[0], tempm_WaveData.size(), 0, 2, 1, NULL );
	
		// 読み込めなかった
		if( readSize == OV_HOLE || readSize == OV_EBADLINK ){
			ov_clear( &ovf );
			return E_FAIL;
		}
		// 全て読み込み終わった
		if( readSize == 0 ){
			break;
		}
		// 音声ファイルデータを拡張 
		m_WaveData.resize( m_WaveData.size() + readSize );
		// メモリのコピー
		memcpy( &m_WaveData[0] + comSize, &tempm_WaveData[0] ,readSize );
		// 読み込みバイト数変更
		comSize += readSize;
	}

	// 読み込んだファイルをバッファに送る
	const XAUDIO2_BUFFER buffer = {
		XAUDIO2_END_OF_STREAM,
		m_WaveData.size(),
		&m_WaveData[0],
		0,
        0,
        XAUDIO2_NO_LOOP_REGION,
        0,
        XAUDIO2_LOOP_INFINITE,
        nullptr
    };
	hr = m_pSourceVoice->SubmitSourceBuffer( &buffer );
	if( FAILED( hr ) ){
		// ファイルの解放
		ov_clear(&ovf);
		return E_FAIL;
	}

	// ファイルの解放
	ov_clear(&ovf);

	return S_OK;
}

// 再生する
HRESULT Audio::Play(){
	return m_pSourceVoice->Start();
}
HRESULT Audio::SePlay(){
	m_pSourceVoice->FlushSourceBuffers();
	return m_pSourceVoice->Start();
}

// 停止する
HRESULT Audio::Stop(){
	return m_pSourceVoice->Stop();
}

// 再生中かどうか
bool Audio::PlayState(){
	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice->GetState( &state );
    return (state.BuffersQueued > 0) != 0;
}
#pragma once 

#include <vorbis/vorbisfile.h>
#include <xaudio2.h>
#include <vector>
#include <windows.h>
#include <boost/noncopyable.hpp>

//
// XAudio2のデバイスを管理するクラス
//
class IXAudioDevice : private boost::noncopyable
{
private:
	IXAudio2* m_pXAudio2;						// XAudio2のデバイス(エンジン)
	IXAudio2MasteringVoice* m_pMasteringVoice;	// XAudio2のプライマリバッファ

private:
	// コンストラクタ
	IXAudioDevice();

	// デストラクタ
	~IXAudioDevice();

public:
	// インスタンスの取得
	static IXAudioDevice& Instance();

	// 初期化
	HRESULT Initialize();

	// IXAudio2の取得
	IXAudio2* GetXAudio2();

	// IXAudio2MasteringVoiceの取得
	IXAudio2MasteringVoice* GetMasteringVoice();
};

//
// BGMを管理するクラス
//
class Audio
{
private:
	IXAudio2SourceVoice* m_pSourceVoice;	// 音源ソース
	std::vector<BYTE> m_WaveData;			// 音源データ

public:
	// コンストラクタ
	Audio();
	// デストラクタ
	~Audio();

	// BGMを読み込む
	HRESULT LoadBGM( LPSTR );

	// 再生する
	HRESULT Play();
	HRESULT SePlay();

	// 停止する
	HRESULT Stop();
	
	// 再生中かどうか
	bool PlayState();
};
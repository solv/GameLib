#pragma once 

#include <vorbis/vorbisfile.h>
#include <xaudio2.h>
#include <vector>
#include <windows.h>
#include <boost/noncopyable.hpp>

//
// XAudio2�̃f�o�C�X���Ǘ�����N���X
//
class IXAudioDevice : private boost::noncopyable
{
private:
	IXAudio2* m_pXAudio2;						// XAudio2�̃f�o�C�X(�G���W��)
	IXAudio2MasteringVoice* m_pMasteringVoice;	// XAudio2�̃v���C�}���o�b�t�@

private:
	// �R���X�g���N�^
	IXAudioDevice();

	// �f�X�g���N�^
	~IXAudioDevice();

public:
	// �C���X�^���X�̎擾
	static IXAudioDevice& Instance();

	// ������
	HRESULT Initialize();

	// IXAudio2�̎擾
	IXAudio2* GetXAudio2();

	// IXAudio2MasteringVoice�̎擾
	IXAudio2MasteringVoice* GetMasteringVoice();
};

//
// BGM���Ǘ�����N���X
//
class Audio
{
private:
	IXAudio2SourceVoice* m_pSourceVoice;	// �����\�[�X
	std::vector<BYTE> m_WaveData;			// �����f�[�^

public:
	// �R���X�g���N�^
	Audio();
	// �f�X�g���N�^
	~Audio();

	// BGM��ǂݍ���
	HRESULT LoadBGM( LPSTR );

	// �Đ�����
	HRESULT Play();
	HRESULT SePlay();

	// ��~����
	HRESULT Stop();
	
	// �Đ������ǂ���
	bool PlayState();
};
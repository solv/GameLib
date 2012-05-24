#include "Audio.h"

//
// XAudio2�̃f�o�C�X���Ǘ�����N���X
//

// �R���X�g���N�^
IXAudioDevice::IXAudioDevice():
	m_pXAudio2(NULL),
	m_pMasteringVoice(NULL)
{
}

// �f�X�g���N�^
IXAudioDevice::~IXAudioDevice(){
	// COM�|�C���^�ނ̌�n��
	if( m_pMasteringVoice ){
		m_pMasteringVoice->DestroyVoice();
	}
	if( m_pXAudio2 ){
		m_pXAudio2->Release();
	}
	CoUninitialize();
}

// �C���X�^���X�̎擾
IXAudioDevice& IXAudioDevice::Instance(){
	static IXAudioDevice instance;
	return instance;
}

// ������
HRESULT IXAudioDevice::Initialize(){
	// �G���[�����p
	HRESULT hr;

	// COM�̏�����
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	// IXAudio2�̏�����
	UINT32 flags = 0;
	
	#ifdef _DEBUG
		flags |= XAUDIO2_DEBUG_ENGINE;
	#endif

	hr = XAudio2Create( &m_pXAudio2, flags );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// IXAudio2MasteringVoice�̏�����
	hr = m_pXAudio2->CreateMasteringVoice( &m_pMasteringVoice );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	return S_OK;
}

// IXAudio2�̎擾
IXAudio2*  IXAudioDevice::GetXAudio2(){
	return m_pXAudio2;
}

// IXAudio2MasteringVoice�̎擾
IXAudio2MasteringVoice* IXAudioDevice::GetMasteringVoice(){
	return m_pMasteringVoice;
}


//
// BGM���Ǘ�����N���X
//

// �R���X�g���N�^
Audio::Audio():
	m_pSourceVoice(NULL)
{
}

// �f�X�g���N�^
Audio::~Audio(){
	// ��~�����Ă���������
	m_pSourceVoice->Stop();
	m_pSourceVoice->DestroyVoice();
}

// BGM�̓ǂݍ���(ogg)
HRESULT Audio::LoadBGM( LPSTR fileName ){
	// ogg�t�@�C��
	OggVorbis_File ovf;

	// �G���[�����p
	HRESULT hr;

	// �t�@�C�����J��
	if( ov_fopen( fileName, &ovf ) != 0 ){
		return E_FAIL;
	}

	// ogg�t�@�C���̉����t�H�[�}�b�g���擾
	vorbis_info* oggInfo = ov_info(&ovf, -1);

	// Wave�t�@�C���̓ǂݍ���
	WAVEFORMATEX format		= {};
	format.wFormatTag		= WAVE_FORMAT_PCM;
	format.nChannels		= static_cast<WORD>(oggInfo->channels);
	format.wBitsPerSample	= 16;
	format.nSamplesPerSec	= oggInfo->rate;
	format.nBlockAlign		= format.wBitsPerSample / 8 * format.nChannels;
    format.nAvgBytesPerSec	= format.nSamplesPerSec * format.nBlockAlign;

	// IXAudio2SourceVoice�̏�����
	hr = IXAudioDevice::Instance().GetXAudio2()->CreateSourceVoice( &m_pSourceVoice, &format );
	if( FAILED( hr ) ){
		return E_FAIL;
	}

	// �t�@�C���̏I�[�܂œǂݍ���
	std::vector<char> tempm_WaveData(4096,0);
	int readSize = 0;
	int comSize = 0;

	while(1){
		// PCM�f�[�^�ǂݍ���
		readSize = ov_read( &ovf, &tempm_WaveData[0], tempm_WaveData.size(), 0, 2, 1, NULL );
	
		// �ǂݍ��߂Ȃ�����
		if( readSize == OV_HOLE || readSize == OV_EBADLINK ){
			ov_clear( &ovf );
			return E_FAIL;
		}
		// �S�ēǂݍ��ݏI�����
		if( readSize == 0 ){
			break;
		}
		// �����t�@�C���f�[�^���g�� 
		m_WaveData.resize( m_WaveData.size() + readSize );
		// �������̃R�s�[
		memcpy( &m_WaveData[0] + comSize, &tempm_WaveData[0] ,readSize );
		// �ǂݍ��݃o�C�g���ύX
		comSize += readSize;
	}

	// �ǂݍ��񂾃t�@�C�����o�b�t�@�ɑ���
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
		// �t�@�C���̉��
		ov_clear(&ovf);
		return E_FAIL;
	}

	// �t�@�C���̉��
	ov_clear(&ovf);

	return S_OK;
}

// �Đ�����
HRESULT Audio::Play(){
	return m_pSourceVoice->Start();
}
HRESULT Audio::SePlay(){
	m_pSourceVoice->FlushSourceBuffers();
	return m_pSourceVoice->Start();
}

// ��~����
HRESULT Audio::Stop(){
	return m_pSourceVoice->Stop();
}

// �Đ������ǂ���
bool Audio::PlayState(){
	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice->GetState( &state );
    return (state.BuffersQueued > 0) != 0;
}
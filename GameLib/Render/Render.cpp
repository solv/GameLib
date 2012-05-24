#include "Render.h"
#include "../Common.h"

// �R���X�g���N�^
Render::Render():
	m_pD3D(NULL),
	m_pd3dDevice(NULL)
{
}

// �R�s�[�R���X�g���N�^
Render::Render( const Render& obj ){
}

// �f�X�g���N�^
Render::~Render(){
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pd3dDevice);
}

// �C���X�^���X�̎擾
Render& Render::Instance(){
	static Render instance;
	return instance;
}

// DirectX�̏�����
HRESULT Render::Initialize( HWND hWnd, bool fullScreen ){
	// IDirect3D9�R���|�[�l���g�̎擾
	if( ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL ){
		return E_FAIL;
	}

	// D3D�f�o�C�X�𐶐����邽�߂̍\���̂�ݒ肷��BZ�o�b�t�@��
	ZeroMemory( &m_d3dpp, sizeof(D3DPRESENT_PARAMETERS) );
	
	// �v���[���e�[�V�����p�����[�^�̐ݒ�
	if( fullScreen ){
		// ���݂̃v���C�}���f�B�X�v���C�A�_�v�^�̃��[�h�����Ƃ�����
		D3DDISPLAYMODE aDisplayMode;		// �f�B�X�v���C�̃��[�h�̏������߂邽��
		if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &aDisplayMode ) ) ){
			return E_FAIL;
		}

		m_Width = aDisplayMode.Width;
		m_Height = aDisplayMode.Height;
		m_d3dpp.BackBufferWidth		= aDisplayMode.Width;		// �o�b�N�o�b�t�@�̕�
		m_d3dpp.BackBufferHeight	= aDisplayMode.Height;		// �o�b�N�o�b�t�@�̍���
		
		m_d3dpp.BackBufferFormat	= aDisplayMode.Format;		// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
		m_d3dpp.Windowed			= false;					// �t���X�N���[�����[�h
	}else{
		m_Width = 640;
		m_Height = 480;
		m_d3dpp.BackBufferFormat	= D3DFMT_UNKNOWN;			// �t�H�[�}�b�g�͍��̉�ʃ��[�h�ɏ]��
		m_d3dpp.Windowed			= true;						// �E�B���h�E���[�h
	}

	//���ʃp�����[�^
	m_d3dpp.BackBufferCount				= 1;						// �o�b�N�o�b�t�@�̐�
	m_d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;		// �}���`�T���v�����O�͍s��Ȃ�
	m_d3dpp.MultiSampleQuality			= 0;						// �}���`�T���v�����O�͍s��Ȃ��̂�0
	m_d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// Direct3D�ɃX���b�v�G�t�F�N�g��C����
	m_d3dpp.EnableAutoDepthStencil		= true;						//Direct3D�̐[�x�o�b�t�@�̊Ǘ���C����
	m_d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;				//�[�x�o�b�t�@�̃t�H�[�}�b�g
	m_d3dpp.hDeviceWindow				= hWnd;						//�J�o�[�E�C���h�E=�A�v���P�[�V�����̃E�C���h�E
	m_d3dpp.Flags						= 0;						//�t���O�͎g��Ȃ�
	m_d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		//���̃��t���b�V�����[�g�����̂܂�
	m_d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	//���j�^�̐�����A��҂�
	
	// CreateDevice��Direct3D������������Ƃ��AHAL�AHEL�̑g�ݍ��킹�̓v���O���}�����R�Ɍ��߂邱�Ƃ��ł��邪
	// ���ʂ͂��̑g�ݍ��킹�ŏ[��

	// D3D�f�o�C�X�̍쐬
	// HAL�~HARDWARE
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
								D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice ) ) )
	{
		// HAL�~SOFTWARE
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice ) ) )
		{
			// RED�~HARDWARE
			if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,
										D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_d3dpp, &m_pd3dDevice) ) )
			{
				// REF�~SOFTWARE
				if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp, &m_pd3dDevice) ) )
				{
					//����ł��R�P���炨���܂��B
					return E_FAIL;
				}
			}
		}
	}

	//��ŕύX���Ȃ������_�����O�X�e�[�g�̐ݒ�
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );			// Z�o�b�t�@��L���ɂ���B
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );		// �|���S���̗��͕\������B
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );				// ���C�g��L����

	m_pd3dDevice->SetRenderState( D3DRS_AMBIENT , 0x00808080);			// ����
	m_pd3dDevice->LightEnable( 0 , true );								// ���C�g�̐ݒ�A���C�g��L����	

	// �V�F�[�f�B���O���[�h�̕ύX�B
	m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );

	//�e�N�X�`���X�e�[�g�̐ݒ�
	//m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1 , D3DTA_DIFFUSE );		// �e�N�X�`���̐F���g�p
	//m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );		// ���_�̐F���g�p
	//m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP , D3DTOP_MODULATE);		// ��Z����

	// �t�B���^�̐ݒ�AD3DTEXF_POINT��D3DTEXF_LINEAR�ɕύX���ăe�N�X�`���̕ω��̎d��������Ƃ�����Ɗy����
	m_pd3dDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_POINT );
	m_pd3dDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_POINT );
	m_pd3dDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_POINT );

	// WM_PAINT���Ă΂�Ȃ��悤�ɂ���
	::ValidateRect( hWnd, 0 );

	return S_OK;
}

// Device�|�C���^�̎擾
LPDIRECT3DDEVICE9 Render::GetDevice() const {
	return m_pd3dDevice;
}

HRESULT Render::BeginScene(){
	if( FAILED(m_pd3dDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 128, 128, 255 ),
		1.0f, 0
		) )
	){
		return E_FAIL;
	}

	if( FAILED( m_pd3dDevice->BeginScene() ) ){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Render::EndScene(){
	m_pd3dDevice->EndScene();
	if( m_pd3dDevice->Present( 0, 0, 0, 0 ) == D3DERR_DEVICELOST ){
		if( FAILED( m_pd3dDevice->Reset(&m_d3dpp) ) ){
			return E_FAIL;
		}
	}

	return S_OK;
}


void Render::GetDisplaySize( int& w, int& h ){
	w = m_Width;
	h = m_Height;
}
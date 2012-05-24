#include "Render.h"
#include "../Common.h"

// コンストラクタ
Render::Render():
	m_pD3D(NULL),
	m_pd3dDevice(NULL)
{
}

// コピーコンストラクタ
Render::Render( const Render& obj ){
}

// デストラクタ
Render::~Render(){
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pd3dDevice);
}

// インスタンスの取得
Render& Render::Instance(){
	static Render instance;
	return instance;
}

// DirectXの初期化
HRESULT Render::Initialize( HWND hWnd, bool fullScreen ){
	// IDirect3D9コンポーネントの取得
	if( ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL ){
		return E_FAIL;
	}

	// D3Dデバイスを生成するための構造体を設定する。Zバッファつき
	ZeroMemory( &m_d3dpp, sizeof(D3DPRESENT_PARAMETERS) );
	
	// プレゼンテーションパラメータの設定
	if( fullScreen ){
		// 現在のプライマリディスプレイアダプタのモードをすとくする
		D3DDISPLAYMODE aDisplayMode;		// ディスプレイのモードの情報を収めるため
		if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &aDisplayMode ) ) ){
			return E_FAIL;
		}

		m_Width = aDisplayMode.Width;
		m_Height = aDisplayMode.Height;
		m_d3dpp.BackBufferWidth		= aDisplayMode.Width;		// バックバッファの幅
		m_d3dpp.BackBufferHeight	= aDisplayMode.Height;		// バックバッファの高さ
		
		m_d3dpp.BackBufferFormat	= aDisplayMode.Format;		// バックバッファのフォーマット
		m_d3dpp.Windowed			= false;					// フルスクリーンモード
	}else{
		m_Width = 640;
		m_Height = 480;
		m_d3dpp.BackBufferFormat	= D3DFMT_UNKNOWN;			// フォーマットは今の画面モードに従う
		m_d3dpp.Windowed			= true;						// ウィンドウモード
	}

	//共通パラメータ
	m_d3dpp.BackBufferCount				= 1;						// バックバッファの数
	m_d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;		// マルチサンプリングは行わない
	m_d3dpp.MultiSampleQuality			= 0;						// マルチサンプリングは行わないので0
	m_d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// Direct3Dにスワップエフェクトを任せる
	m_d3dpp.EnableAutoDepthStencil		= true;						//Direct3Dの深度バッファの管理を任せる
	m_d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;				//深度バッファのフォーマット
	m_d3dpp.hDeviceWindow				= hWnd;						//カバーウインドウ=アプリケーションのウインドウ
	m_d3dpp.Flags						= 0;						//フラグは使わない
	m_d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		//今のリフレッシュレートをそのまま
	m_d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	//モニタの垂直回帰を待つ
	
	// CreateDeviceでDirect3Dを初期化するとき、HAL、HELの組み合わせはプログラマが自由に決めることができるが
	// 普通はこの組み合わせで充分

	// D3Dデバイスの作成
	// HAL×HARDWARE
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
								D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice ) ) )
	{
		// HAL×SOFTWARE
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice ) ) )
		{
			// RED×HARDWARE
			if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,
										D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_d3dpp, &m_pd3dDevice) ) )
			{
				// REF×SOFTWARE
				if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp, &m_pd3dDevice) ) )
				{
					//それでもコケたらおしまい。
					return E_FAIL;
				}
			}
		}
	}

	//後で変更しないレンダリングステートの設定
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );			// Zバッファを有効にする。
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );		// ポリゴンの裏は表示する。
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );				// ライトを有効に

	m_pd3dDevice->SetRenderState( D3DRS_AMBIENT , 0x00808080);			// 環境光
	m_pd3dDevice->LightEnable( 0 , true );								// ライトの設定、ライトを有効に	

	// シェーディングモードの変更。
	m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );

	//テクスチャステートの設定
	//m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1 , D3DTA_DIFFUSE );		// テクスチャの色を使用
	//m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );		// 頂点の色を使用
	//m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP , D3DTOP_MODULATE);		// 乗算する

	// フィルタの設定、D3DTEXF_POINTをD3DTEXF_LINEARに変更してテクスチャの変化の仕方を見るとかすると楽しい
	m_pd3dDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_POINT );
	m_pd3dDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_POINT );
	m_pd3dDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_POINT );

	// WM_PAINTが呼ばれないようにする
	::ValidateRect( hWnd, 0 );

	return S_OK;
}

// Deviceポインタの取得
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
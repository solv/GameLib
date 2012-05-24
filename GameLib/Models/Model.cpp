#include "Model.h"
#include "d3dx9tex.h"
#include <string>
#include "../../Common.h"

LPDIRECT3DDEVICE9 Model::m_pd3dDevice = NULL;

// コンストラクタ
Model::Model(){
	m_Mesh = NULL;
	m_MeshMaterials = NULL;
	m_MeshTextures = NULL;
	m_MaterialsNum = 0;
	m_Pitch = 0.0f;
	m_Roll = 0.0f;
	m_Yaw = 0.0f;
	m_Scr = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
}

Model::~Model(){
	SAFE_DELETE_ARRAY(m_MeshMaterials);
	if( m_MeshTextures ){
		for( DWORD i = 0; i < m_MaterialsNum; ++i ){
			SAFE_RELEASE(m_MeshTextures[i]);
		}
	}
	SAFE_DELETE_ARRAY(m_MeshTextures);
	SAFE_RELEASE(m_Mesh);
}

HRESULT Model::Initialize( LPDIRECT3DDEVICE9 pd3dDevice ){
	m_pd3dDevice = pd3dDevice;
	return S_OK;
}

HRESULT Model::LoadModelFromFile( LPCTSTR fileName ){
    LPD3DXBUFFER pD3DXMtrlBuffer; //一時バッファ用

	// Xファイルの読み込み。
    if( FAILED( D3DXLoadMeshFromX(	fileName, D3DXMESH_SYSTEMMEM , m_pd3dDevice, NULL, 
									&pD3DXMtrlBuffer, NULL,
									&m_MaterialsNum, &m_Mesh ) )
	){
		return E_FAIL;
	}

	// pD3DXMtrlBufferから、マテリアルやテクスチャの名称などの情報を取得する。
	D3DXMATERIAL* d3dxMaterials	= reinterpret_cast<D3DXMATERIAL*>( pD3DXMtrlBuffer->GetBufferPointer() );
	m_MeshMaterials	= new D3DMATERIAL9[m_MaterialsNum];
	m_MeshTextures	= new LPDIRECT3DTEXTURE9[m_MaterialsNum];

	for( DWORD i = 0; i < m_MaterialsNum; ++i ){
		// 質感（material）のコピー
		m_MeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// 質感（material）に環境光を設定する（D3DXが何もしないため）
		m_MeshMaterials[i].Ambient = m_MeshMaterials[i].Diffuse;
		m_MeshTextures[i] = NULL;

		if( d3dxMaterials[i].pTextureFilename != NULL && lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 ){
			char textureName[1024];
			sprintf_s( textureName, _countof(textureName), "Data/model/%s", d3dxMaterials[i].pTextureFilename );	
			if( FAILED( D3DXCreateTextureFromFileA( m_pd3dDevice, textureName, &m_MeshTextures[i] ) ) ){
				return E_FAIL;
			}
		}
	}

	// 質感（material）バッファの開放。
	SAFE_RELEASE(pD3DXMtrlBuffer);
	
	return S_OK;
}

void Model::SetupModelMatrix(){
	// 自機のための行列
	D3DXMATRIX	ScrMatrix;		// 拡大用行列
	D3DXMATRIX	PosMatrix;		// 位置用行列
	D3DXMATRIX	RotMatrix;		// 回転用行列
	D3DXMATRIX	ResultMatrix;	// 結果の行列

	//単位行列を収める（念のため）
	D3DXMatrixIdentity( &ScrMatrix );
	D3DXMatrixIdentity( &PosMatrix );
	D3DXMatrixIdentity( &RotMatrix );
	D3DXMatrixIdentity( &ResultMatrix );

	// 平行移動と回転の行列を設定
	D3DXMatrixScaling( &ScrMatrix, m_Scr.x, m_Scr.y, m_Scr.z );
	D3DXMatrixRotationYawPitchRoll( &RotMatrix ,  m_Yaw , m_Roll , m_Pitch );
	D3DXMatrixTranslation( &PosMatrix, m_Pos.GetPosX() , m_Pos.GetPosY() , m_Pos.GetPosZ() );

	// 行列の合成
	ResultMatrix = ScrMatrix * RotMatrix * PosMatrix;

	// 描画デバイスへ適用
	m_pd3dDevice->SetTransform( D3DTS_WORLD , &ResultMatrix );
}

void Model::Draw(){
	// 行列の適用
	SetupModelMatrix();
	// マテリアルの数だけループ
	for( DWORD i = 0; i < m_MaterialsNum; ++i ){
		// サブセットにマテリアルとテクスチャを設定。
		m_pd3dDevice->SetMaterial( &m_MeshMaterials[i] );
		m_pd3dDevice->SetTexture( 0, m_MeshTextures[i] );
		// メッシュ・サブセットの描画。
		m_Mesh->DrawSubset(i);
	}
}

// 位置の設定
Position& Model::GetPosition(){
	return m_Pos;
}

// 拡大率の設定
void Model::SetScr( const float x_, const float y_, const float z_ ){
	m_Scr.x = x_;
	m_Scr.y = y_;
	m_Scr.z = z_;
}
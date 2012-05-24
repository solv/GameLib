#pragma once

#include <d3d9.h>
#include <d3dx9mesh.h>
#include "Position.h"

class Model
{
protected:
	Position	m_Pos;		// オブジェクトの座標と方向
	D3DXVECTOR3 m_Scr;		// オブジェクトの拡大率
	float		m_Yaw;		// ヨー
	float		m_Roll;		// ロール
	float		m_Pitch;	// ピッチ

	LPD3DXMESH				m_Mesh;			// システム中のメッシュオブジェクト
	D3DMATERIAL9*			m_MeshMaterials;// メッシュの質感(materials)
	LPDIRECT3DTEXTURE9*		m_MeshTextures;	// メッシュのテクスチャ(texture)
	DWORD					m_MaterialsNum;	// メッシュ質感の数
	
	static LPDIRECT3DDEVICE9 m_pd3dDevice; // デバイスのポインタ

public:
	// コンストラクタ
	Model();

	// デストラクタ
	~Model();

	// デバイスポインタの初期化
	static HRESULT Initialize( LPDIRECT3DDEVICE9 pd3dDevice );

	// Xファイルからメッシュデータを読み込む
	HRESULT LoadModelFromFile( LPCTSTR fileName );

	// 行列の設定
	void SetupModelMatrix();

	// 描画
	virtual void Draw();

	// 位置の設定
	Position& GetPosition();

	// 拡大率の設定
	void SetScr( const float, const float, const float );
};

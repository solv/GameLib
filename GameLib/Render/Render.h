#pragma once

#include <d3d9.h>

// DirectXの描写デバイスを担当します
class Render
{
private:
	LPDIRECT3D9	m_pD3D;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	D3DPRESENT_PARAMETERS m_d3dpp;
	int m_Width;
	int m_Height;

private:
	// コンストラクタ
	Render();
	// コピーコンストラクタ
	Render( const Render& );
	// デストラクタ
	~Render();

public:
	// インスタンスの取得
	static Render& Instance();

	// DirectXの初期化
	HRESULT Initialize( HWND hWnd, bool fullScreen );

	// Deviceポインタの取得
	LPDIRECT3DDEVICE9 GetDevice() const;

	HRESULT BeginScene();
	HRESULT EndScene();

	// モニタのサイズの取得
	void GetDisplaySize( int& w, int& h );
};
#pragma once

#include <d3d9.h>

// DirectX�̕`�ʃf�o�C�X��S�����܂�
class Render
{
private:
	LPDIRECT3D9	m_pD3D;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	D3DPRESENT_PARAMETERS m_d3dpp;
	int m_Width;
	int m_Height;

private:
	// �R���X�g���N�^
	Render();
	// �R�s�[�R���X�g���N�^
	Render( const Render& );
	// �f�X�g���N�^
	~Render();

public:
	// �C���X�^���X�̎擾
	static Render& Instance();

	// DirectX�̏�����
	HRESULT Initialize( HWND hWnd, bool fullScreen );

	// Device�|�C���^�̎擾
	LPDIRECT3DDEVICE9 GetDevice() const;

	HRESULT BeginScene();
	HRESULT EndScene();

	// ���j�^�̃T�C�Y�̎擾
	void GetDisplaySize( int& w, int& h );
};
#pragma once

#include <d3d9.h>
#include <d3dx9mesh.h>
#include "Position.h"

class Model
{
protected:
	Position	m_Pos;		// �I�u�W�F�N�g�̍��W�ƕ���
	D3DXVECTOR3 m_Scr;		// �I�u�W�F�N�g�̊g�嗦
	float		m_Yaw;		// ���[
	float		m_Roll;		// ���[��
	float		m_Pitch;	// �s�b�`

	LPD3DXMESH				m_Mesh;			// �V�X�e�����̃��b�V���I�u�W�F�N�g
	D3DMATERIAL9*			m_MeshMaterials;// ���b�V���̎���(materials)
	LPDIRECT3DTEXTURE9*		m_MeshTextures;	// ���b�V���̃e�N�X�`��(texture)
	DWORD					m_MaterialsNum;	// ���b�V�������̐�
	
	static LPDIRECT3DDEVICE9 m_pd3dDevice; // �f�o�C�X�̃|�C���^

public:
	// �R���X�g���N�^
	Model();

	// �f�X�g���N�^
	~Model();

	// �f�o�C�X�|�C���^�̏�����
	static HRESULT Initialize( LPDIRECT3DDEVICE9 pd3dDevice );

	// X�t�@�C�����烁�b�V���f�[�^��ǂݍ���
	HRESULT LoadModelFromFile( LPCTSTR fileName );

	// �s��̐ݒ�
	void SetupModelMatrix();

	// �`��
	virtual void Draw();

	// �ʒu�̐ݒ�
	Position& GetPosition();

	// �g�嗦�̐ݒ�
	void SetScr( const float, const float, const float );
};

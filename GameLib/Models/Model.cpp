#include "Model.h"
#include "d3dx9tex.h"
#include <string>
#include "../../Common.h"

LPDIRECT3DDEVICE9 Model::m_pd3dDevice = NULL;

// �R���X�g���N�^
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
    LPD3DXBUFFER pD3DXMtrlBuffer; //�ꎞ�o�b�t�@�p

	// X�t�@�C���̓ǂݍ��݁B
    if( FAILED( D3DXLoadMeshFromX(	fileName, D3DXMESH_SYSTEMMEM , m_pd3dDevice, NULL, 
									&pD3DXMtrlBuffer, NULL,
									&m_MaterialsNum, &m_Mesh ) )
	){
		return E_FAIL;
	}

	// pD3DXMtrlBuffer����A�}�e���A����e�N�X�`���̖��̂Ȃǂ̏����擾����B
	D3DXMATERIAL* d3dxMaterials	= reinterpret_cast<D3DXMATERIAL*>( pD3DXMtrlBuffer->GetBufferPointer() );
	m_MeshMaterials	= new D3DMATERIAL9[m_MaterialsNum];
	m_MeshTextures	= new LPDIRECT3DTEXTURE9[m_MaterialsNum];

	for( DWORD i = 0; i < m_MaterialsNum; ++i ){
		// �����imaterial�j�̃R�s�[
		m_MeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// �����imaterial�j�Ɋ�����ݒ肷��iD3DX���������Ȃ����߁j
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

	// �����imaterial�j�o�b�t�@�̊J���B
	SAFE_RELEASE(pD3DXMtrlBuffer);
	
	return S_OK;
}

void Model::SetupModelMatrix(){
	// ���@�̂��߂̍s��
	D3DXMATRIX	ScrMatrix;		// �g��p�s��
	D3DXMATRIX	PosMatrix;		// �ʒu�p�s��
	D3DXMATRIX	RotMatrix;		// ��]�p�s��
	D3DXMATRIX	ResultMatrix;	// ���ʂ̍s��

	//�P�ʍs������߂�i�O�̂��߁j
	D3DXMatrixIdentity( &ScrMatrix );
	D3DXMatrixIdentity( &PosMatrix );
	D3DXMatrixIdentity( &RotMatrix );
	D3DXMatrixIdentity( &ResultMatrix );

	// ���s�ړ��Ɖ�]�̍s���ݒ�
	D3DXMatrixScaling( &ScrMatrix, m_Scr.x, m_Scr.y, m_Scr.z );
	D3DXMatrixRotationYawPitchRoll( &RotMatrix ,  m_Yaw , m_Roll , m_Pitch );
	D3DXMatrixTranslation( &PosMatrix, m_Pos.GetPosX() , m_Pos.GetPosY() , m_Pos.GetPosZ() );

	// �s��̍���
	ResultMatrix = ScrMatrix * RotMatrix * PosMatrix;

	// �`��f�o�C�X�֓K�p
	m_pd3dDevice->SetTransform( D3DTS_WORLD , &ResultMatrix );
}

void Model::Draw(){
	// �s��̓K�p
	SetupModelMatrix();
	// �}�e���A���̐��������[�v
	for( DWORD i = 0; i < m_MaterialsNum; ++i ){
		// �T�u�Z�b�g�Ƀ}�e���A���ƃe�N�X�`����ݒ�B
		m_pd3dDevice->SetMaterial( &m_MeshMaterials[i] );
		m_pd3dDevice->SetTexture( 0, m_MeshTextures[i] );
		// ���b�V���E�T�u�Z�b�g�̕`��B
		m_Mesh->DrawSubset(i);
	}
}

// �ʒu�̐ݒ�
Position& Model::GetPosition(){
	return m_Pos;
}

// �g�嗦�̐ݒ�
void Model::SetScr( const float x_, const float y_, const float z_ ){
	m_Scr.x = x_;
	m_Scr.y = y_;
	m_Scr.z = z_;
}
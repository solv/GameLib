#include "ParentScene.h"
#include "GameScene.h"
#include "../Utility/Utility.h"

// �R���X�g���N�^
ParentScene::ParentScene():
	m_Scene( NULL )
{
}

// �f�X�g���N�^
ParentScene::~ParentScene(){
	SAFE_DELETE(m_Scene);
}

// �C���X�^���X�̎擾
ParentScene& ParentScene::Instance(){
	static ParentScene instance;
	return instance;
}

// �V�[���̍X�V
void ParentScene::Update(){
	// �V�[�����X�V����
	SceneBase* next = m_Scene->Update();

	// ���݂̃V�[���Ɩ߂�l�Ƃ��ẴV�[�����r����
	// ��v���Ȃ��ꍇ�͌��݃V�[���ƍ����ւ���
	if( next != m_Scene ){
		SAFE_DELETE(m_Scene);
		m_Scene = next;
	}
}
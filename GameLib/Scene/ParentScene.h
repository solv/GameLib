#pragma once

#include "../stdafx.h"

// �O���錾
class SceneBase;

//
// �V�[���Ǘ��̐e�N���X
//
class ParentScene : private boost::noncopyable
{
private:
	// �R���X�g���N�^
	ParentScene();

	// �f�X�g���N�^
	~ParentScene();

private:
	SceneBase* m_Scene; // ���݃V�[���̃|�C���^

public:
	// �C���X�^���X�̎擾
	static ParentScene& Instance();

	// �V�[���̍X�V
	void Update();
};
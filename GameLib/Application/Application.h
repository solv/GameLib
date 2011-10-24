#pragma once

#include "../stdafx.h"
#include "WindowBase.h"

//
// �A�v���P�[�V�������Ǘ�����N���X
//
class Application : private boost::noncopyable
{
private:
	// �E�C���h�E�N���X���Ǘ�����N���X
	boost::shared_ptr<WindowBase> m_spWindowBase;

public:
	// �R���X�g���N�^
	Application( const HINSTANCE );

	// ������
	HRESULT Initialize( const int, const int, const bool, const LPCTSTR );

	// ���b�Z�[�W���[�v
	WPARAM Run();
};
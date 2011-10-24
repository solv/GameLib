#pragma once

#include <boost/noncopyable.hpp>

//
// �V�[���̊��N���X
//
class SceneBase : private boost::noncopyable
{
public:
	virtual ~SceneBase(){}
	virtual SceneBase* Update() = 0;
};
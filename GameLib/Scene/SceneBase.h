#pragma once

#include <boost/noncopyable.hpp>

//
// シーンの基底クラス
//
class SceneBase : private boost::noncopyable
{
public:
	virtual ~SceneBase(){}
	virtual SceneBase* Update() = 0;
};
#pragma once

#include "../stdafx.h"

// 前方宣言
class SceneBase;

//
// シーン管理の親クラス
//
class ParentScene : private boost::noncopyable
{
private:
	// コンストラクタ
	ParentScene();

	// デストラクタ
	~ParentScene();

private:
	SceneBase* m_Scene; // 現在シーンのポインタ

public:
	// インスタンスの取得
	static ParentScene& Instance();

	// シーンの更新
	void Update();
};
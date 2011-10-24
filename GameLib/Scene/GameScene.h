#pragma once

#include "SceneBase.h"

//
// ゲームシーンクラス
//
class GameScene : public SceneBase
{
public:
	// コンストラクタ
	GameScene(){}
	
	// デストラクタ
	~GameScene(){}

	// シーンのアップデート
	SceneBase* Update();
};
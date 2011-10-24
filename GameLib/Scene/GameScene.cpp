#include "GameScene.h"

// シーンのアップデート
SceneBase* GameScene::Update(){
	SceneBase* scene = this;	// 自分自身のポインタを格納する
	return scene;				// 遷移するシーンのポインタを返す(通常は自分自身)
}
#include "ParentScene.h"
#include "GameScene.h"
#include "../Utility/Utility.h"

// コンストラクタ
ParentScene::ParentScene():
	m_Scene( new GameScene )
{
}

// デストラクタ
ParentScene::~ParentScene(){
	SAFE_DELETE(m_Scene);
}

// インスタンスの取得
ParentScene& ParentScene::Instance(){
	static ParentScene instance;
	return instance;
}

// シーンの更新
void ParentScene::Update(){
	// シーンを更新する
	SceneBase* next = m_Scene->Update();

	// 現在のシーンと戻り値としてのシーンを比較して
	// 一致しない場合は現在シーンと差し替える
	if( next != m_Scene ){
		SAFE_DELETE(m_Scene);
		m_Scene = next;
	}
}
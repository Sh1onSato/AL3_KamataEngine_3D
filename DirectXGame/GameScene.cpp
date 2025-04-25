#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() { 
	delete model_;
	delete player_;
}

void GameScene::Initialize() {
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	// 3Dモデルデータの生成
	model_ = Model::Create();
	// カメラの初期化
	camera_.Initialize();

	//自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_,textureHandle_,&camera_);
}

void GameScene::Update() { 
	player_->Update();

}

void GameScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	player_->Draw();

	Model::PostDraw();
}

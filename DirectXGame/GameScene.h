#pragma once
#include"Player.h"
#include<vector>

// ゲームシーン
class GameScene {
public:
	~GameScene();
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	std::vector<std::vector<KamataEngine::WorldTransform*>>  worldTransformBlocks_;

private:
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;

	//カメラ
	KamataEngine::Camera camera_;
	// デバックカメラ有効
	bool debugCameraEnabled_ = false;
	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

};

#pragma once
#include"Player.h"
#include<vector>
#include"Skydome.h"
#include"MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"

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

	void GenerateBlocks();

	void CheckAllCollisions();

	bool IsFinished() const { return finished_; }

private:
	enum class Phase {
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
	};
	Phase phase_;

	void ChangePhase();

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
	// プレイヤーモデル
	KamataEngine::Model* player_model_ = nullptr;

	// ブロックモデル
	KamataEngine::Model* block_model_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;


	// スカイドーム
	Skydome* skydome_ = nullptr;

	// 3Dモデル
	KamataEngine:: Model* modelSkydome_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	CameraController* CController_ = nullptr;

	Enemy* enemy_ = nullptr;

	KamataEngine::Model* enemy_model_ = nullptr;

	std::list<Enemy*> enemies_;

	DeathParticles* deathParticles_ = nullptr;

	Model* deathParticle_model_ = nullptr;

	bool finished_ = false;
};

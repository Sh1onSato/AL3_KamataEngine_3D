#include "GameScene.h"
#include"Math.h"

using namespace KamataEngine;
GameScene::~GameScene() { 
	delete model_;
	delete player_;
	delete debugCamera_;
	delete skydome_;
	delete mapChipField_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {
	// 3Dモデルデータの生成
	model_ = Model::Create();
	// カメラの初期化
	camera_.Initialize();

	//自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_model_ = Model::CreateFromOBJ("player");
	player_->Initialize(player_model_, textureHandle_, &camera_);

	//スカイドームの生成
	skydome_ = new Skydome();
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// スカイドームの初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	// ブロックモデル
	block_model_ = Model::CreateFromOBJ("block");

	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {

		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Update() { 
	player_->Update();

	skydome_->Update();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}
	// デバックカメラの更新
	debugCamera_->Update();
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		debugCameraEnabled_ = !debugCameraEnabled_;
	}

#endif // _DEBUG
	if (debugCameraEnabled_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
	} else {
		camera_.UpdateMatrix();
	}
}

void GameScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// スカイドームの描画
	skydome_->Draw();

	player_->Draw();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			block_model_->Draw(*worldTransformBlock, camera_);
		}
	}

	Model::PostDraw();
}

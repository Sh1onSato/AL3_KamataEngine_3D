#include "Player.h"
#include <cassert>
#include "Math.h"


void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle,KamataEngine::Camera* camera) { 
	// NULLポインタチェック
	assert(model);
	
	//引数の内容をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 2.0f;

}

void Player::Update() { 
	// 行列を定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}



void Player::Draw() { 


	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_, textureHandle_); 


}

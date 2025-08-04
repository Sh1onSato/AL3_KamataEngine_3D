#define NOMINMAX

#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include "Math.h"


float Player::EaseInOut(float t, float start, float end) {

	// 0.0 <= t <= 1.0 の範囲を強制
	t = std::min(std::max(t, 0.0f), 1.0f);

	float t_eased = t * t * (3.0f - 2.0f * t);

	// 線形補間(lerp)と組み合わせて最終値を計算
	return start + (end - start) * t_eased;
}

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) { 
	// NULLポインタチェック
	assert(model);
	
	//引数の内容をメンバ変数に記録
	model_ = model;
	
	camera_ = camera;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() { 

	// 移動入力
	if (onGround_) {
		// キーボード入力を取得
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			// 加速/減速
			velocity_.x += acceleration.x;
			velocity_.y += acceleration.y;
			velocity_.z += acceleration.z;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_.y += kJumpAcceleration / 60.0f;
		}

	} else {
		// 落下速度
		velocity_.y += -kGravityAcceleration / 60.0f;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	bool landing = false;

	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			worldTransform_.translation_.y = 1.0f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}
	// 旋回制御
	if (turnTimer_ > 0.0f) {
		// タイマーを進める
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	// 行列を定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}



void Player::Draw() { 


	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_, textureHandle_); 


}

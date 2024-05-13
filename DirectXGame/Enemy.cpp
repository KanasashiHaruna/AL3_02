#include "Enemy.h"
#include "assert.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& leaveVelocity) {
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("UFO.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	ApprochVelocity_ = velocity;
	LeaveVelocity_ = leaveVelocity;

}

void Enemy::Update() {

	
	switch (phase_) { 
		case Phase::Approach:
	default:
		    worldTransform_.translation_.x += ApprochVelocity_.x;
		    worldTransform_.translation_.y += ApprochVelocity_.y;
		    worldTransform_.translation_.z += ApprochVelocity_.z;
		    

			if (worldTransform_.translation_.z < 0.0f) {
			    phase_ = Phase::Leave;
			}
		    break;
	case Phase::Leave:
		  
		   worldTransform_.translation_.x += LeaveVelocity_.x;
		   worldTransform_.translation_.y += LeaveVelocity_.y;
		   worldTransform_.translation_.z += LeaveVelocity_.z;
		 
		    break;
	}
	worldTransform_.UpdateMatrix();
	// 座標の画面表示-------------------

	ImGui::Begin("Enemy");
	ImGui::Text(
	    "Enemy %f %f %f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	
}
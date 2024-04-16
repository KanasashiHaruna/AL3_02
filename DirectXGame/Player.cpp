#include "Player.h"
#include "cassert"

void Player::Initialize(Model* model, uint32_t textureJandle) { 
	assert(model); 
	model_ = model;
	textureHandle_ = textureJandle;
	worldTransform_.Initialize();
}

void Player::Update() { 
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& vewProjection) {
	model_->Draw(worldTransform_, vewProjection, textureHandle_);
}
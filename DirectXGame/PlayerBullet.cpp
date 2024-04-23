#include "PlayerBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "WorldTransform.h"


void PlayerBullet::Initialize(Model* model, const Vector3& position) { 
	assert(model);
	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("bomb.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

}

void PlayerBullet::Update() { 
	worldTransform_.UpdateMatrix(); 

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
#include "PlayerBox.h"
#include "cassert"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "imgui.h"

void PlayerBox::Initialize(Model* model, const Vector3& position) { 
	assert(model);
	model_ = model;

	//テクスチャ読み取り
	//textureHandle_ = TextureManager::Load("rock2.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	isAttack = false;
}

Vector3 PlayerBox::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBox::Update() {
	
	
		switch (phase_) {
		case Phase::Appearance:
		default:
			if (isAttack == true) {
				phase_ = Phase::Move;
			}
			break;

		case Phase::Move:
			worldTransform_.translation_.z += velocity_.z;
		}
		worldTransform_.UpdateMatrix();

		if (--deathTimer_ <= 0) {
		    isDead_ = true;
		}

		//-----------------------------------
		ImGui::Begin("Box");
		ImGui::Text(
		    "Box.translation{%f,%f,%f}", worldTransform_.translation_.x,
		    worldTransform_.translation_.y, worldTransform_.translation_.z);

		ImGui::End();
	
}

void PlayerBox::Draw(const ViewProjection& viewProjection) {
	
		model_->Draw(worldTransform_, viewProjection, texturehandle_);
	
}

//当たったら動く関数--------
void PlayerBox::OnCollision() { isAttack = true; }
void PlayerBox::CollisionDeath() { isDead_ = true; }

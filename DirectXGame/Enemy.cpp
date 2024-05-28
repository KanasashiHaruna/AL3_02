#include "Enemy.h"
#include "assert.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include "Player.h"
#include "mathFunction.h"

// TransformNormal-----------------------------------
Vector3 TransformNomal2(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};

	return result;
}
void Enemy::Approach() { fireTimer = 60; }

// 正規化Normalize---------------
Vector3 Normalize(const Vector3& v) {
	float answer = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	Vector3 result;
	result = {v.x / answer, v.y / answer, v.z / answer};
	return result;
}
// 減算Subtract--------------------
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	return result;
}

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

	//Fire();
	Approach();
}


void Enemy::Update() {

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}

		return false;
	});
	//------------------------------------------------
	switch (phase_) { 
		case Phase::Approach:
	default:
		  // worldTransform_.translation_.x += ApprochVelocity_.x;
		  // worldTransform_.translation_.y += ApprochVelocity_.y;
		  // worldTransform_.translation_.z += ApprochVelocity_.z;
		    

			if (worldTransform_.translation_.z < 0.0f) {
			    phase_ = Phase::Leave;
			}

			fireTimer--;
		    if (fireTimer <= 0) {
			    Fire();
			    fireTimer = kFireInterval;
			}
		    break;
	case Phase::Leave:
		  
		  worldTransform_.translation_.x += LeaveVelocity_.x;
		  worldTransform_.translation_.y += LeaveVelocity_.y;
		  worldTransform_.translation_.z += LeaveVelocity_.z;
		 
		    break;
	}
	worldTransform_.UpdateMatrix();
	//Fire();

	for (EnemyBullet* bullet : bullets_) {
		    bullet->Update();
	}
	// 座標の画面表示-------------------

	ImGui::Begin("Enemy");
	ImGui::Text(
	    "Enemy %f %f %f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度
	float kBulletSpeed = 0.1f;
	Vector3 velocity(0, 0, kBulletSpeed);

	Vector3 p = player_->GetWorldPosition();
	Vector3 e = GetWorldPosition();

	Vector3 distance = Subtract(player_->GetWorldPosition(), GetWorldPosition());
	Vector3 distanceNolm = Normalize(distance);
	velocity = Multiply(kBulletSpeed, distanceNolm);
	// 速度のベクトルを自機の向きに合わせて回転させる
	velocity = TransformNomal2(velocity, worldTransform_.matWorld_);

	//GetWorldPosition();
	//player_->GetWorldPosition();


	


	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);

}

Vector3 Enemy::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		    bullet->Draw(viewProjection);
	}
}

void Enemy::OnCollision() {}


Enemy::~Enemy() {

	for (EnemyBullet* bullet : bullets_) {
		    delete bullet;
	}
}
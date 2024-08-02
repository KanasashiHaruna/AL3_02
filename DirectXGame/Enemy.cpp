#include "Enemy.h"
#include "assert.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include "Player.h"
#include "mathFunction.h"

#pragma region 関数
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

#pragma endregion

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& leaveVelocity) {
	assert(model);
	model_ = model;
	

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("boss.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	ApprochVelocity_ = velocity;
	LeaveVelocity_ = leaveVelocity;

	//EnemyBullet* newBullet = new EnemyBullet();
	//newBullet->Initialize(model, position, velocity);
	
	worldTransform_.scale_ = {20, 20, 20};
	//Fire();
	Approach();
}

void Enemy::SetParent(const WorldTransform* parent) {
	worldTransform_.parent_ = parent; 
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
		case Phase::Move:
	default:
		    isStop = false;
		    worldTransform_.translation_.z += -0.1f;
	

			//fireTimer--;
		    //if (fireTimer <= 0) {
			//    Fire();
			//    fireTimer = kFireInterval;
			//}
		    attackTime++;
		    if (attackTime >= 150) {
			    isAction = true;
			}

			if (isAction == true) {
			    phase_ = Phase::Attack;
			    attackTime = 0;
			}

		    break;
	case Phase::Attack:
		    isAction = false;
		    worldTransform_.translation_.z += LeaveVelocity_.z;

			if (isBoxAttack == true) {
			    phase_ = Phase::Stop;
			}

		    break;
	case Phase::Stop:
		    isBoxAttack = false;
		    worldTransform_.translation_.z += 0.2f;
		    stopTime++;

			if (stopTime >= 150) {
			    isStop = true;
			}

			if (isStop == true) {
			    phase_ = Phase::Move;
			    stopTime = 0;
			}
		    break;
	}
	worldTransform_.UpdateMatrix();
	

	

	// 座標の画面表示-------------------

	ImGui::Begin("Enemy");
	ImGui::Text(
	    "Enemy %f %f %f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::Text("Enemy Hp: %f", Hp);

	ImGui::Text("Enemy phase:%d", phase_);
	ImGui::Text("AttackTime: %d", attackTime);
	ImGui::Text("StopTime: %d", stopTime);
	
	ImGui::End();
}


void Enemy::Fire() {
	
	// 弾の速度
	float kBulletSpeed = 0.5f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度のベクトルを自機の向きに合わせて回転させる
	velocity = TransformNomal2(velocity, worldTransform_.matWorld_);

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


#pragma region 描画
void Enemy::Draw(const ViewProjection& viewProjection) {
	if (isDead_ == false) {

		 model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
}
#pragma endregion


#pragma region 当たり判定
void Enemy::OnCollision() { 
	
	//isDead_ = true; 
	Hp = Hp - 1;

	if (Hp <= 0) {

		 isDead_ = true;
	
	}
}

void Enemy::BoxCollision() { 
	
	//Hp = Hp - 5;
	//
	//if (Hp <= 0) {
	//
	//	 isDead_ = true;
	//}
	isBoxAttack = true;

}
#pragma endregion


Enemy::~Enemy() {

	for (EnemyBullet* bullet : bullets_) {
		    delete bullet;
	}
}
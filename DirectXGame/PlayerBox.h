#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "Input.h"

class PlayerBox {
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void OnCollision();
	void CollisionDeath();
	Vector3 GetWorldPosition();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t texturehandle_ = 0u;
	//速度
	Vector3 velocity_ = {0.0f, 0.0f, 0.6f};

	//当たったかのフラグ
	bool isAttack = false;

	enum class Phase { 
		Appearance, //出現
		Move,       //移動
	};

	Phase phase_ = Phase::Appearance;

	// デスフラグ
	bool isDead_ = false;
	static const int32_t kLifeTime = 60;
	int32_t deathTimer_ = kLifeTime;

public:
	bool IsDead() const { return isDead_; }
};

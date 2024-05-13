#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "Input.h"

class Enemy {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& leaveVelocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 ApprochVelocity_;
	Vector3 LeaveVelocity_;

	enum class Phase {
		Approach,    //接近
		Leave,       //離脱
	};

	Phase phase_ = Phase::Approach;
	
	
};

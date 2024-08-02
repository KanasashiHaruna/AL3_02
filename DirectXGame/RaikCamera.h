#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

class RaikCamera {
public:
	void Initialize(const Vector3& position, const Vector3& rotation);
	void Update();
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldPosition() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	Vector3 velocity_ = {0.0f, 0.0f,0.1f};
	Vector3 rotationVelocity_ = {0.0f, 0.01f, 0.0f};
};

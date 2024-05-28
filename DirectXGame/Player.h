#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	void Initialize(Model* model, uint32_t textureJandle);
	void Update();
	void Draw(ViewProjection& vewProjection);
	Vector3 Add(const Vector3& v1, const Vector3& v2);
	void Attack();
	Vector3 GetWorldPosition();
	void OnCollision();   //当たり判定

	//デストラクタ
	~Player();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キー入力
	Input* input_ = nullptr;

	// 弾
	//PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	public:
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	
};

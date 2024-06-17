#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Sprite.h"

class Player {
public:
	void Initialize(Model* model, uint32_t textureJandle, Vector3& position);
	void Update();
	void Draw(ViewProjection& vewProjection);
	Vector3 Add(const Vector3& v1, const Vector3& v2);
	void Attack();
	Vector3 GetWorldPosition();
	Vector3 GetWorldPosition1();
	void OnCollision();   //当たり判定
	void DrawUI();
	void SetParent(const WorldTransform* parent);

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

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	public:
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	
};

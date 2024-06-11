#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <list>
#include "mathFunction.h"

class Player;
class GameScene;

class Enemy {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& leaveVelocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Fire();
	void Approach();
	
	
	Vector3 GetWorldPosition();
	void OnCollision();  //当たり判定

	~Enemy();

	Player* player_ = nullptr;
	void SetPlayer(Player* player) { player_ = player; }

	GameScene* gameScene_ = nullptr;
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	
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
	
	//弾
	//std::list<EnemyBullet*> bullets_;
	
	public:
	static const int kFireInterval = 60;

	private:
	int32_t fireTimer = 0;

	public:
	    //const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	    
		// デスフラグ
	    bool isDead_ = false;

	public:
	    bool IsDead() const { return isDead_; }
};


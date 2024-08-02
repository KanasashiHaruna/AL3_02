#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "EnemyBullet.h"
#include "mathFunction.h"


class Enemy {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, const Vector3& leaveVelocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Fire();
	void Approach();
	void SetParent(const WorldTransform* parent);
	
	
	Vector3 GetWorldPosition();
	void OnCollision();  //当たり判定
	void BoxCollision();

	~Enemy();
	
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
		Move,
		Attack,
		Stop,
	};

	Phase phase_ = Phase::Move;
	
	//弾
	std::list<EnemyBullet*> bullets_;
	
	public:
	static const int kFireInterval = 60;
	    float Hp = 100;

	private:
	int32_t fireTimer = 0;
	int32_t attackTime = 0;
	int32_t stopTime = 0;

	public:
	    const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	    
	//フラグ
	    bool isDead_ = false;
	    bool isAction = false;
	    bool isStop = false;
	    bool isBoxAttack = false;
	

	public:
	    bool IsDead() const { return isDead_; }
};


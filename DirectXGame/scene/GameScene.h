#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Skydome.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	float playerRadius_ = 2.0f;
	float enemyRadius_ = 2.0f;

	Vector3 position_ = {10, 0, 10};
	Vector3 velocity_ = {0, 0, -0.1f};

	Vector3 velocity2_ = {-0.2f, 0.2f, -0.2f};


	//Skydome
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	

	// スプライト
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool debaugflug_ = false;

	
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

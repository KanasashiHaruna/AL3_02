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
#include "RaikCamera.h"
#include "sstream"


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

	/// <summary>
	///敵発生データの読み込み
	/// </summary>
	void loadEnemyPopDate();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();
	///敵を発生させる関数
	void EnemyOccurrence(Vector3 position);

	void CheckAllCollisions();

	//敵の弾を追加する
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	//敵を追加する
	void AddEnemy(Enemy* enemy);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;


	// プレイヤー
	Player* player_ = nullptr;
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemys_;
	float playerRadius_ = 2.0f;


	Vector3 playerPosition = {0.0f, 0.0f, -10.0f};
	Vector3 position_ = {10, 0, 10};
	Vector3 velocity_ = {0, 0, -0.1f};

	Vector3 velocity2_ = {-0.2f, 0.2f, -0.2f};

	std::list<EnemyBullet*> enemyBullets_;
	const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }
	float enemyRadius_ = 2.0f;

	//敵発生コマンド
	std::stringstream enemyPopCommands;
	bool isWait = false;
	uint32_t waitTimer = 0;

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

	//レールカメラ
	RaikCamera* railCamera_ = nullptr;
	Vector3 railCameraPosition = {0, 0, -100};
	Vector3 railCameraRotate = {0, 0, 0};
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

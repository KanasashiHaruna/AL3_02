#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Enemy.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}



void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	#pragma region  テクスチャとモデル関連
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("space_cat.png");

	// 3Dモデルの生成と解放
	model_ = Model::Create();
	#pragma endregion

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	#pragma region プレイヤー
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_,textureHandle_,playerPosition);

	Vector3 playerposition(0, 0, 50);
	player_->Initialize(model_, textureHandle_, playerposition);
    #pragma endregion

	#pragma region Skydome
	//skydome------------------------------------------------
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_,textureHandle_);
	#pragma endregion

	#pragma region 敵キャラ
	//敵キャラの生成-------------------------------------------
	enemy_ = new Enemy();

    // 敵キャラに自キャラのアドレスを渡す
	//enemy_->SetPlayer(player_);
	enemy_->Initialize(model_, position_, velocity_, velocity2_);
    #pragma endregion
	
	// デバッグカメラの生成-----------------------------------
	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// レールカメラ-----------------------------------------
	railCamera_ = new RaikCamera();
	railCamera_->Initialize(railCameraPosition, railCameraRotate);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();	
	player_->SetParent(&railCamera_->GetWorldPosition());
}

void GameScene::Update() {
	player_->Update(viewProjection_);
	enemy_->Update();
	
#pragma region デバッグカメラの処理
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		debaugflug_ = true;
	}
#endif

	if (debaugflug_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	} else {

		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	}
#pragma endregion
	
	CheckAllCollisions();
}



void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skydome_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	player_->DrawUI();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
	
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB, posC, posD, posE;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<PlayerBox*>& playerBoxs = player_->GetBoxs();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets(); 


//自キャラの座標
	posA = player_->GetWorldPosition();

	#pragma region 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();

		float Length = ((posB.x - posA.x) * (posB.x - posA.x)) +
		               ((posB.y - posA.y) * (posB.y - posA.y)) +
		               ((posB.z - posA.z) * (posB.z - posA.z));

		float radius = (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_);
		if (Length <= radius) {
			player_->OnCollision();
			bullet->OnCollision();
		}
		
	}
    #pragma endregion

// 　敵弾と自弾	
	
	for (EnemyBullet* bullet : enemyBullets) {
		for (PlayerBullet* playerBullet : playerBullets) {
			posC = playerBullet->GetWorldPosition();
			posB = bullet->GetWorldPosition();
			
			float Length = ((posB.x - posC.x) * (posB.x - posC.x)) +
			               ((posB.y - posC.y) * (posB.y - posC.y)) +
			               ((posB.z - posC.z) * (posB.z - posC.z));

			float radius = (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_);
			if (Length <= radius) {
				playerBullet->OnCollision();
				bullet->OnCollision();
			}

		}
	}


#pragma region 自弾と敵キャラ
	
	posD = enemy_->GetWorldPosition();
	for (PlayerBullet* playerBullet : playerBullets) {
		posC = playerBullet->GetWorldPosition();

		float Length = ((posC.x - posD.x) * (posC.x - posD.x)) +
		               ((posC.y - posD.y) * (posC.y - posD.y)) +
		               ((posC.z - posD.z) * (posC.z - posD.z));

		float radius = (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_);
		if (Length <= radius) {
			playerBullet->OnCollision();
         	enemy_->OnCollision();
		}
	}
#pragma endregion

	#pragma region 自弾と箱
	for (PlayerBullet* playerBullet : playerBullets) {
		for (PlayerBox* playerBox : playerBoxs) {
			posC = playerBullet->GetWorldPosition();
			posE = playerBox->GetWorldPosition();

			float Length = ((posC.x - posE.x) * (posC.x - posE.x)) +
			               ((posC.y - posE.y) * (posC.y - posE.y)) +
			               ((posC.z - posE.z) * (posC.z - posE.z));

			float radius = (playerRadius_ + playerBoxradius_)*(playerRadius_ + playerBoxradius_);
			if (Length <= radius) {
				playerBullet->OnCollision();
				playerBox->OnCollision();
			}
		}
	}
	#pragma endregion

	#pragma region Boxと敵
	for (PlayerBox* playerBox : playerBoxs) {
		posE = playerBox->GetWorldPosition();
	  
		float Length = ((posE.x - posD.x) * (posE.x - posD.x)) +
		               ((posE.y - posD.y) * (posE.y - posD.y)) +
		               ((posE.z - posD.z) * (posE.z - posD.z));

		float radius = (playerBoxradius_ + enemyRadius_) * (playerBoxradius_ + enemyRadius_);
		if (Length <= radius) {
			playerBox->CollisionDeath();
			enemy_->BoxCollision();
		}
	}
}


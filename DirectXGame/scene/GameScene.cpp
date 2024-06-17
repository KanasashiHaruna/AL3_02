#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Enemy.h"
#include <fstream>

GameScene::GameScene() {}

//GameScene::~GameScene() {}



void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("space_cat.png");

	// 3Dモデルの生成と解放
	model_ = Model::Create();
	

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	//player_->Initialize(model_,textureHandle_,playerPosition);
	Vector3 playerposition(0, 0, 50);
	player_->Initialize(model_, textureHandle_, playerposition);

	//skydome
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_,textureHandle_);




	// 敵キャラに自キャラのアドレスを渡す
	
	
	//enemy_->Initialize(model_, position_, velocity_,velocity2_);
	
	

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// レールカメラ
	railCamera_ = new RaikCamera();
	railCamera_->Initialize(railCameraPosition, railCameraRotate);
	// ワールドトランスフォームの初期設定


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();	
	player_->SetParent(&railCamera_->GetWorldPosition());
}

//敵を発生させる
void GameScene::EnemyOccurrence(Vector3 position) { 
	
	// 敵キャラの生成
	// enemys_ = new Enemy();
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model_, position, velocity_, velocity2_);
	AddEnemy(newEnemy);
	newEnemy->SetPlayer(player_);
	newEnemy->SetGameScene(this);
}

void GameScene::loadEnemyPopDate() {
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());   //条件が満たしていないときにエラー


	//ファイルの内容を文字列ストリーミングにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();

}

//敵発生コマンドの更新
void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
		   //待機完了
			isWait = false;
		}
		return;
	}

	//一行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
       //一行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を収得
		getline(line_stream, word,',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行っを飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
		//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

		//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

		//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

		//敵を発生させる
			EnemyOccurrence(Vector3(x, y, z));
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待機時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}

//敵の弾の追加
void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	///リストに登録する
	enemyBullets_.push_back(enemyBullet);

}

//敵の追加
void GameScene::AddEnemy(Enemy* enemy) {
	//リストに登録する
	enemys_.push_back(enemy);
}

void GameScene::Update() {
	//loadEnemyPopDate();
	//UpdateEnemyPopCommands();

	player_->Update();
	//enemys_->Update();
	loadEnemyPopDate();
	UpdateEnemyPopCommands();
	// 敵の更新処理
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			//インスタンスの解放とリストからの除外
			delete bullet;
			return true;
		}
		return false;
	});

	//弾の更新処理
	for (EnemyBullet* bullet : enemyBullets_) {
		
		bullet->Update();
	}
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			// インスタンスの解放とリストからの除外
			delete enemy;
			return true;
		}
		return false;
	});
	
/// デバッグカメラの処理
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
	//enemy_->Draw(viewProjection_);

	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
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
	Vector3 posA, posB,posC,posD;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = GetBullets();    /////

#pragma region  

//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();

		//if (posB.z <= 0.0f) {
		//	static int a = 0;
		//	a++;
		//}

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

//#pragma region 　	
	//敵弾と自弾
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
#pragma endregion

#pragma region
	//自弾と敵キャラ
	
	for (PlayerBullet* playerBullet : playerBullets) {
		for (Enemy* enemy : enemys_) {

			posD = enemy->GetWorldPosition();
			posC = playerBullet->GetWorldPosition();

			float Length = ((posC.x - posD.x) * (posC.x - posD.x)) +
			               ((posC.y - posD.y) * (posC.y - posD.y)) +
			               ((posC.z - posD.z) * (posC.z - posD.z));

			float radius = (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_);
			if (Length <= radius) {
				playerBullet->OnCollision();
			    enemy->OnCollision();
			}
		}
	}
#pragma endregion
}

GameScene::~GameScene() {
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}
#include "Player.h"
#include "cassert"
#include "ImGuiManager.h"
#include "WorldTransform.h"
#include "MathFunction.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "WinApp.h"
#include "ViewProjection.h"

void Player::Initialize(Model* model, uint32_t textureJandle,Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureJandle;
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	//レティクル用テクスチャ
	uint32_t textureReticle = TextureManager::Load("rethikuru.png");
	//スプライト
	sprite2DReticle_ = Sprite::Create(textureReticle, {0, 0}, {1, 1, 1, 1}, {0.5f,0.5f});

	input_ = Input::GetInstance();
}

Vector3 Player::Add(const Vector3& v1, const Vector3& v2) {
	Vector3 answer;
	answer.x = v1.x + v2.x;
	answer.y = v1.y + v2.y;
	answer.z = v1.z + v2.z;

	return answer;
}

// 行列の積-----------------------------------------
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;

			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

// 平行移動行列------------------------------------------
Matrix4x4 MakeTranslateMAtrix(const Vector3& translate) {
	Matrix4x4 result;

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}

////スケール行列-----------------------------------
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.x;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

//// xyz軸回転行列---------------------------------
Matrix4x4 MakeRotateXMatrix(const Vector3& rotate) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = std::cos(rotate.x);
	result.m[1][2] = std::sin(rotate.x);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = -std::sin(rotate.x);
	result.m[2][2] = std::cos(rotate.x);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}
Matrix4x4 MakeRotateYMatrix(const Vector3& rotate) {
	Matrix4x4 result;

	result.m[0][0] = std::cos(rotate.y);
	result.m[0][1] = 0.0f;
	result.m[0][2] = -std::sin(rotate.y);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = std::sin(rotate.y);
	result.m[2][1] = 0.0f;
	result.m[2][2] = std::cos(rotate.y);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}
Matrix4x4 MakeRotateZMatrix(const Vector3& rotate) {
	Matrix4x4 result;

	result.m[0][0] = std::cos(rotate.z);
	result.m[0][1] = std::sin(rotate.z);
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = -std::sin(rotate.z);
	result.m[1][1] = std::cos(rotate.z);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

    //--------------------------------------------------

void Player::SetParent(const WorldTransform* parent) {
	worldTransform_.parent_=parent;
}

void Player::Update(
    const ViewProjection& viewProjection) {
	//-----------------------------------------------
	// 
	XINPUT_STATE joyState;

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへのオフセット(ｚ+向き)
	Vector3 offset = {0, 0, 1.0f};
	//自機のワールド行列の回転を反映
	offset = TransformNomal(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = Normalize1(offset);
	offset = Multiply(kDistancePlayerTo3DReticle, offset);
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(offset, GetWorldPosition());
	

	//--------------------------------------------------------------
	
	//3Dレティクルのワールド座標から2Dレティクルのスクリーンを座標計算
	Vector3 positionReticle = GetWorldPosition1();

	//ビューポート行列
	//Matrix4x4 matViewport =
	   // MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	
	#pragma region 2D標準
	//2D標準-----------------------------------------------------------------
	
	
	POINT mousePosition;
	//
	////マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	////クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	
	////マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));
	
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;

		sprite2DReticle_->SetPosition(spritePosition);
	}
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	////ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV =
	    MultiplyEx(viewProjection.matView, MultiplyEx(viewProjection.matProjection, matViewport));
	////合成行列の逆行列を計算
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	//
	////スクリーン座標
	Vector3 posNear = Vector3((float)spritePosition.x, (float)spritePosition.y, 0);
	Vector3 posFar = Vector3((float)spritePosition.x, (float)spritePosition.y, 1);
	//
	////スクリーン座標からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);
	//
	////マウスレイ方向
	Vector3 mouseDirection = Subtract3(posFar, posNear);
	mouseDirection = Normalize1(mouseDirection);
	//
	////カメラから標準オブジェクトの距離
	const float kDistanceTestObject =70.0f;
	worldTransform3DReticle_.translation_ = Add(posNear, Multiply(kDistanceTestObject, mouseDirection));

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();
	
	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));


	//ゲームパッドの状態を得る変数

	
	#pragma endregion

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}

		return false;
	});

	worldTransform_.TransferMatrix();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	
	

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	
	//if (input_->PushKey(DIK_LEFT)) {
	//	move.x -= kCharacterSpeed;
	//} else if (input_->PushKey(DIK_RIGHT)) {
	//	move.x += kCharacterSpeed;
	//}
	//
	//// 押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_DOWN)) {
	//	move.y -= kCharacterSpeed;
	//} else if (input_->PushKey(DIK_UP)) {
	//	move.y += kCharacterSpeed;
	//}
	//
	//// ゲームパッド状態取得
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	move.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kCharacterSpeed;
	//	move.y -= (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kCharacterSpeed;
	//}

	//
	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 座標の画面表示-------------------

	ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f)", positionReticle.x, positionReticle.y);
	//ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	//ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();

	// 移動制限------------------------
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 20.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// Rotate--------------------------
	const float kRotSpeed = 0.02f; /// 回転速さ[radian]

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y = worldTransform_.rotation_.y - kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y = worldTransform_.rotation_.y + kRotSpeed;
	}
	Attack();

	

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	worldTransform_.UpdateMatrix();
	worldTransform3DReticle_.UpdateMatrix();
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	Matrix4x4 matrix;
	matrix = worldTransform_.matWorld_;

	worldPos.x = matrix.m[3][0];
	worldPos.y = matrix.m[3][1];
	worldPos.z = matrix.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorldPosition1() {
	Vector3 worldPos;
	Matrix4x4 matrix;
	matrix = worldTransform3DReticle_.matWorld_;

	worldPos.x = matrix.m[3][0];
	worldPos.y = matrix.m[3][1];
	worldPos.z = matrix.m[3][2];

	return worldPos;
}

void Player::Attack() {
	//if (input_->PushKey(DIK_W)) {
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		 return;
	}
	  //弾の速度
	  //  const float kBulletSpeed = 1.0f;
	Vector3 velocity;
	// 自機から照準オブジェクトへのベクトル
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		velocity = Subtract3(worldTransform3DReticle_.translation_, GetWorldPosition());
		velocity = Normalize1(velocity);
		velocity = Multiply(1.0f, velocity);

		////速度のベクトルを自機の向きに合わせて回転させる
		velocity = TransformNomal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);
		//}
	}
	
	
}


void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//model_->Draw(worldTransform_, viewProjection);

	//if (bullet_) {
	//	bullet_->Draw(viewProjection);
	//}
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	//model_->Draw(worldTransform3DReticle_, viewProjection);
}
void Player::DrawUI() {

	sprite2DReticle_->Draw();

}
void Player::OnCollision() {

}
	Player::~Player() { 
	delete sprite2DReticle_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet; 
	}
}
#include "Player.h"
#include "cassert"
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureJandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureJandle;
	worldTransform_.Initialize();

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

//平行移動行列------------------------------------------
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

void Player::Update() {   //------------------------------------------------
	worldTransform_.TransferMatrix();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	//ワールド行列の更新
	Matrix4x4 scaleMatrix =MakeScaleMatrix(worldTransform_.scale_);   ///スケーリング行列の作成
	Matrix4x4 rotateMatrixX =MakeRotateXMatrix(worldTransform_.rotation_);
	Matrix4x4 rotateMatrixY = MakeRotateYMatrix(worldTransform_.rotation_);
	Matrix4x4 rotateMatrixZ = MakeRotateXMatrix(worldTransform_.rotation_); /// 回転行列
	Matrix4x4 rotateMatrixXYZ = Multiply(rotateMatrixX, Multiply(rotateMatrixY, rotateMatrixZ));   //回転行列の合成
	Matrix4x4 translateMatrix = MakeTranslateMAtrix(worldTransform_.translation_);//平行移動行列

	worldTransform_.matWorld_ = Multiply(scaleMatrix, Multiply(rotateMatrixXYZ, translateMatrix));

	worldTransform_.TransferMatrix();


	//座標の画面表示-------------------
	
	ImGui::Begin("player");
	ImGui::Text(
	    "Player %f %f %f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();

	
	//移動制限------------------------
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 20.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}
void Player::Draw(ViewProjection& vewProjection) {
	model_->Draw(worldTransform_, vewProjection, textureHandle_);
}
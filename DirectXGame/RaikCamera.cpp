#include "RaikCamera.h"
#include "MathFunction.h"
#include "WorldTransform.h"
//#include "imGui.h"
#include "mathFunction.h"

void RaikCamera::Initialize(const Vector3& position, const Vector3& rotation) { 

	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	
	viewProjection_.farZ = 2000.0f;
	//ビュープロジェクション
	viewProjection_.Initialize();

	worldTransform_.Initialize();
}



void RaikCamera::Update() { 
    worldTransform_.translation_ = Subtract3(worldTransform_.translation_, velocity_);
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotationVelocity_);
   
	worldTransform_.UpdateMatrix();
   
	///カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
   
	//カメラの座標を画面表示する処理(デバッグ表示)
	//ImGui::Begin("Camera");
	//ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	//ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	//ImGui::End();

}



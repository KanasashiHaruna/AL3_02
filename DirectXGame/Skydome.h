#pragma once
#include "model.h"
#include "WorldTransform.h"

class Skydome {
public:
	void Initialize(Model* model, uint32_t textureJandle);
	void Update();
	void Draw(ViewProjection& viewProjection);


private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
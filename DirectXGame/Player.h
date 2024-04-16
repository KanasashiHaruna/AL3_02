#pragma once
#include "model.h"
#include "WorldTransform.h"
class Player {
public:
	void Initialize(Model* model, uint32_t textureJandle);
	void Update();
	void Draw(ViewProjection& vewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};

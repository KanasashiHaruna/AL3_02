#include "WorldTransform.h"
#include "model.h"
#include"Input.h"
class Player {
public:
	void Initialize(Model* model, uint32_t textureJandle);
	void Update();
	void Draw(ViewProjection& vewProjection);
	Vector3 Add(const Vector3& v1,const Vector3& v2);

	

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キー入力
	Input* input_ = nullptr;

};
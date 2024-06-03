#include "Skydome.h"
#include "cassert"

void Skydome::Initialize(Model* model, uint32_t textureJandle) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureJandle;
	worldTransform_.Initialize();
}

void Skydome::Update() {


}

void Skydome::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_,viewProjection);

}

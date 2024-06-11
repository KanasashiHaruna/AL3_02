#include "WorldTransform.h"
#include <cmath>
#include "mathFunction.h"


void WorldTransform::UpdateMatrix() {

	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	//親があれば親のワールド行列をかける
	if (parent_) {
		matWorld_ = MultiplyEx(matWorld_, parent_->matWorld_);
	}
	TransferMatrix();
}
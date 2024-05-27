#include "mathFunction.h"

// スカラー倍Multiply-------------

Vector3 Multiply(float scalar, const Vector3& v){
	Vector3 result;
	result = {scalar * v.x, scalar * v.y, scalar * v.z};
	return result;
}


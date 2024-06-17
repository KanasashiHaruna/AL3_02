#pragma once
#include "WorldTransform.h"

Vector3 Multiply(float scalar, const Vector3& v);
Matrix4x4 MultiplyEx(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 TransformNomal(const Vector3& v, const Matrix4x4& m);

//加算Add
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算Subtract--------------------
Vector3 Subtract3(const Vector3& v1, const Vector3& v2);
// 行列の減算Subtract
Matrix4x4 Subtract4(const Matrix4x4& m1, const Matrix4x4& m2);

// 正規化Normalize---------------
Vector3 Normalize1(const Vector3& v);

    // 逆行列Inverse
Matrix4x4 Inverse(const Matrix4x4& m);


// ビューポート変換行列-----------------------------
Matrix4x4 MakeViewportMatrix(
float left, float top, float width, float height, float minDepth, float maxDepth);
Matrix4x4 MakeRotateYMatrixEx(const Vector3& rotate);
Matrix4x4 MakeRotateZMatrixEx(const Vector3& rotate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
#pragma once
#include "WorldTransform.h"

Vector3 Multiply(float scalar, const Vector3& v);
Matrix4x4 MultiplyEx(const Matrix4x4& m1, const Matrix4x4& m2);

//加算Add
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 逆行列Inverse
Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 MakeRotateXMatrixEx(const Vector3& rotate);
Matrix4x4 MakeRotateYMatrixEx(const Vector3& rotate);
Matrix4x4 MakeRotateZMatrixEx(const Vector3& rotate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
#pragma once
#include"KamataEngine.h"
using namespace KamataEngine;
Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b);
Matrix4x4 MakeScaleMatrix(const Vector3& Scale);
Matrix4x4 MakeTranslationMatrix(const Vector3& Translate);
Matrix4x4 MakeRotationXMatrix(float radian);
Matrix4x4 MakeRotationYMatrix(float radian);
Matrix4x4 MakeRotationZMatrix(float radian);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

void WorldTransformUpdate(KamataEngine::WorldTransform& worldTransform);



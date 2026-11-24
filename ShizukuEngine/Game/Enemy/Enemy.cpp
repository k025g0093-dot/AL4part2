#include "Enemy.h"
#include <algorithm>
#include <cmath>
#include "../../ShizukuEngine/ShizukuEngine.h"

void Enemy::Initialize(
	int model, int animationModel, 
	const Vector3& position,
	Type enemyType
) {
	mModel = model;
	mAnimation = animationModel;
	mPosition = position;

	//タイプに応じて速度変更
	if (mType == Type::kChase) {
		mSpeed = 4.0f;
	}
	else {
		mSpeed = 2.0f;
	}
}

void Enemy::Update(const Vector3& playerPosition, float dt) {
	mDt = dt;

	switch (mType)
	{
	case Enemy::Type::kChase:
		UpdateChase(playerPosition, dt);
		break;
	case Enemy::Type::kRanged:
		UpdateRanged(playerPosition, dt);
		break;
	default:
		break;
	}
}

void Enemy::UpdateChase(const Vector3& playerPosition, float dt) {

	float dx = playerPosition.x - mPosition.x;
	float dz = playerPosition.z - mPosition.z;

	float distance = std::sqrt(dx * dx + dz * dz);

	if (distance > 1.0f) {
		float dirX = dx / distance;
		float dirZ = dz / distance;
	
		mPosition.x += dirX * mSpeed * dt;
		mPosition.z += dirZ * mSpeed * dt;

		mRotationY = std::atan2(dirX, dirZ);
	}
}

void Enemy::UpdateRanged(const Vector3& playerPosition, float dt) {

	float dx = playerPosition.x - mPosition.x;
	float dz = playerPosition.z - mPosition.z;

	float distance = std::sqrt(dx * dx + dz * dz);

	if (distance > 1.0f) {
		float dirX = dx / distance;
		float dirZ = dz / distance;
	
		mPosition.x += dirX * mSpeed * dt;
		mPosition.z += dirZ * mSpeed * dt;

		mRotationY = std::atan2(dirX, dirZ);
	}
}

void Enemy::Draw(ShizukuEngine*engine) {
	engine->DrawModel(
		mModel,
		mPosition,
		{ 0.0f, mRotationY, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }
	);
}


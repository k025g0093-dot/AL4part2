#include "Enemy.h"
#include <algorithm>
#include <cmath>
#include "../../ShizukuEngine/ShizukuEngine.h"

void Enemy::Initialize(int model, int animationModel, const Vector3& position) {
	mModel = model;
	mAnimation = animationModel;
	mPosition = position;
}

void Enemy::Update(const EnemyCommand& cmd, float dt) {
	mDt = dt;

	mPosition += cmd.move * (mSpeed * dt);
	mPosition.x = std::clamp(mPosition.x, -kFieldHalf, kFieldHalf);
	mPosition.z = std::clamp(mPosition.z, -kFieldHalf, kFieldHalf);
}

void Enemy::SpwanEnemy(){
	if (mFireTimer > 0) {
		mFireTimer--;
		return;
	}
	else {
		mIsSpwan = true;
		mFireTimer = mDt;
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


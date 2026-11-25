#include "MiniBoss.h"
#include <algorithm>
#include <cmath>
#include "../../ShizukuEngine/ShizukuEngine.h"

void MiniBoss::Initialize(
	int model, int animationModel, 
	const Vector3& position
) {
	mModel = model;
	mAnimation = animationModel;
	mPosition = position;
	mHp = mMaxHp;
	mSpeed = 4;
}

void MiniBoss::Update(const Vector3& playerPosition, float dt) {
	mDt = dt;
	UpdateChase(playerPosition, dt);
}

void MiniBoss::UpdateChase(const Vector3& playerPosition, float dt) {

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

void MiniBoss::UpDateDamage() {
	mHp -= damage;
}


void MiniBoss::Draw(ShizukuEngine*engine) {
	if (mHp > 0) {
		engine->DrawModel(
			mModel,
			mPosition,
			{ 0.0f, mRotationY, 0.0f },
			{ 2.5f, 2.5f, 2.5f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}
}


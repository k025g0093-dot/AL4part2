#include "Player.h"
#include "../../ShizukuEngine/ShizukuEngine.h"
#include <algorithm>
#include <cmath>

void Player::Initialize(int model, int animationModel,const Vector3& position) {

	mModel = model;
	mAnimation = animationModel;
	mPosition = position;
	mHp = mMaxHp;
}

void Player::Update(const PlayerCommand& cmd, float dt) {

	mShotThisFrame = false;

	mPosition += cmd.move * (mSpeed * dt);
	mPosition.x = std::clamp(mPosition.x, -kFieldHalf, kFieldHalf);
	mPosition.z = std::clamp(mPosition.z, -kFieldHalf, kFieldHalf);

	if (mFireTimer > 0) {
		mFireTimer--;
	}

	if (cmd.isFire) {
		mShotThisFrame = true;
		mFireTimer = dt;
	}

}

void Player::UpDateDamage() {
	mHp -= damage;
}

void Player::Draw(ShizukuEngine* engine) {
		engine->DrawSkinnedModel(
			mModel,
			mAnimation,
			mPosition ,
			{ 0.0f, mRotationY, 0.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
}

#include "Player.h"
#include <algorithm>
#include <cmath>

void Player::Initialize(int model, const Vector3& position) {

	mModel = model;
	mPosition = position;
}

void Player::Update(const PlayerCommand& cmd, float dt) {

	mPosition += cmd.move * (mSpeed * dt);
	mPosition.x = std::clamp(mPosition.x, -kFieldHalf, kFieldHalf);
	mPosition.z = std::clamp(mPosition.z, -kFieldHalf, kFieldHalf);

}

